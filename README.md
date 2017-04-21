# wos
一方面受到[lowlevelprogramming-university](https://github.com/gurugio/lowlevelprogramming-university)启发，另一方面搞了这么多年的内核开发，没有一个自己的OS，感觉不太像话。


## 搭建cross compiler


第一步搭建cross compiler，又称为交叉编译器，搞过嵌入式的人都懂，之前我做过arm cross compiler，也做过mips cross compiler。所谓的交叉编译器，就是在host平台编译出target平台上可执行的程序。例如我们一般在x86平台上运行arm cross compiler，得到可以在ARM开发板上可以执行的程序。
我要做的这个os kernel是基于32位x86体系的，所以要做一个x86的的cross compiler. 

我们一般使用GCC编译器，开源的，方便使用(我爱GNU）。构建交叉编译器的过程，简单来说，就是用主机的编译器（host-GCC)编译GCC源码，得到一个新版本的GCC编译器，也就是我们需要的target-GCC。参考[http://wiki.osdev.org/GCC_Cross-Compiler](http://wiki.osdev.org/GCC_Cross-Compiler).


## 最简单的内核

可能还不能称之为内核，这仅是一段从启动到打印出"hello world"的小程序，但这是一个好的开头，我们可以在这个基础上加入更多的内容，例如内存管理，进城管理，驱动程序等。

_在从简单内核进入到内存管理的过程中，我有点搞不清楚怎么做硬件初始化,正在阅读 [i386 manual](http://css.csail.mit.edu/6.858/2015/readings/i386.pdf) 和 [operating system from 0 to 1](https://tuhdo.github.io/os01/)，希望能找到答案_

## x86 Manual-内存管理

### x86 段式存储管理
#### 地址转换

![](http://ooaecudja.bkt.clouddn.com/seg_tran.png)

#### segment descriptor

segment descriptor是由编译器/连接器/加载器/OS创建的，不是由应用程序员创建。结构如下：

![](http://ooaecudja.bkt.clouddn.com/descriptor.png)
#### descriptor table
x86 处理器能识别两种Descriptor table：
* global descriptor table（GDT）, 其地址保存在GDTR 寄存器;指令LGDT/SGDT用来访问GDTR
* local descriptor table（LDT)，其地址保存在LDTR寄存器（每个进程有自己的LDT），指令LLDT/SLDT用来访问LDTR
所谓的descriptor table， 是内存中的一块区域，用来存放segment descriptor.

#### selector
在x86的段式存储管理下，一个逻辑地址(应用程序员看到的地址）包含两个部分，16位的segment selector和32位的偏移地址。selector一般由编译器/连接器指定，对应用程序员不可见。selector的格式：

15--------------4--3----0
|-----index-----|T/L|RPL|

T:table indicator，表示使用GDT(0）还是LDT(1)
RPL: requestor's priviledge level

selector用于在descriptor table中获取descriptor ，descriptor中有段的base address以及一些权限控制位。

#### segment register
segment register 存储selector以及对应的segment descriptor ，避免每次都需要去内存读取descriptor。
操作segment register的指令有：
* mov， pop，lds，lss，lgs，lfs
* call/jmp

### page管理
前面段式存储管理将应用程序的逻辑地址转换为线性地址，那么PAGE管理将线性地址转换为物理地址；PAGE内存管理模式是以PAGE方式划分虚拟内存的内存管理系统的基本功能。这个功能对于x86是可选的，仅当CR0寄存器的PG位设置之后才会生效。PG位一般由OS在系统初始化过程中设置。

#### 线性地址
线性地址是间接访问物理地址的一种地址，它有页目录项表，页表项，偏移三部分构成。

31------------2221---------------1211------------0
|----DIR-------||-----PAGE--------||-----OFFSET--|

当前使用的页目录项存储在CR3寄存器，也被称之为页目录基址寄存器（PDBR）；内存管理软件可以选择为所有进程使用一个页目录表，或者为不同的进程使用不同的页目录表，或者结合两者。

### 结合page管理和段式管理

x86可以“关闭”段式管理模式；x86没有直接的命令用来关闭段式寄存器，但是可以这么来实现：让所有的segment descriptor指向整个32位的线性地址空间。

### 8086实模式寻址(real mode)
最早期的8086CPU只有一种工作方式就是real mode，数据总线为16位，地址总线为20位，real mode下所有寄存器都是16位，从80286开始有了保护模式，从80386开始CPU数据总线和地址总线都是32位，而且寄存器也是32位，但80386为了保持向前兼容都保留了real mode。现代的操作系统加电时首先工作在实模式下，然后再切换到保护模式下。
在real mode下寻址比较简单，cs/ds等段寄存器中存放的是基址，真实的物理地址采用 *基址<<4 + 偏移地址* 得到；
而在保护模式下，cs/ds中存放的段选择符；段选择符(selector)中的index需要去GDT/LDT索引段描述符， 段描述符有记录该段的基址/长度等信息。因此在进入保护模式之前，需要先设置好全局描述符表（GDT），，然后用lgdt指令加载描述符基址，然后设置CPU的保护标志进入保护模式。



