# wos
纯粹是想做着玩的，搞了这么多年的内核开发，没有一个自己的OS，感觉不太像话。参考文档
[http://wiki.osdev.org/Getting_Started](http://wiki.osdev.org/Getting_Started)。
##搭建cross compiler##
第一步搭建cross compiler，又称为交叉编译器，搞过嵌入式的人都懂，之前我做过arm cross compiler，也做过mips cross compiler。所谓的交叉编译器，就是在host平台编译出target平台上可执行的程序。例如我们一般在x86平台上运行arm cross compiler，得到可以在ARM开发板上可以执行的程序。
我要做的这个os kernel是基于32位x86体系的，所以要做一个x86的的cross compiler. 

我们一般使用GCC编译器，开源的，方便使用(我爱GNU）。构建交叉编译器的过程，简单来说，就是用主机的编译器（host-GCC)编译GCC源码，得到一个新版本的GCC编译器，也就是我们需要的target-GCC。参考[http://wiki.osdev.org/GCC_Cross-Compiler](http://wiki.osdev.org/GCC_Cross-Compiler).

##最简单的内核##
可能还不能称之为内核，这仅是一段从启动到打印出"hello world"的小程序，但这是一个好的开头，我们可以在这个基础上加入更多的内容，例如内存管理，进城管理，驱动程序等。

