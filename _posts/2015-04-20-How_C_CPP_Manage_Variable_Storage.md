---
title:          C/C++如何管理变量存储空间
category:       Programming_Languages
tag:            [C, C++, Language]
lastupdate:     "2015-04-20"
---

本篇接上一篇blog[C/C++字符串字面量解析](/C_CPP_String_Literal.html)最后的示例程序，进一步探讨C/C++对于变量存储空间的管理。

翠花，先上程序：

程序输出：
{% highlight Bash shell scripts %}
                    --------Code Area--------
&                                string_literal = 0x100224e16
$                        another_string_literal = 0x100224e5f
$another_string_literal_with_same_value(pooled) = 0x100224e5f
                    --------Global Area--------
$                         const_static_variable = 0x100224f98
$                               static_variable = 0x10022502c
$                        global_static_variable = 0x100225030
$                               global_variable = 0x100225028
                    --------Heap Area--------
$                               malloc_variable = 0x7ff130403770
$                                  new_variable = 0x7ff130403780
                    --------Stack Area--------
$                                const_variable = 0x7fff5f9dbb2c
$                                local_variable = 0x7fff5f9dbb28
{% endhighlight %}

程序源码：
{% highlight c++ %}
#include <stdio.h>
#include <stdlib.h>

static int gv21 = 1;
int gv22 = 1;
int main() {
int max_width = 46;
char* v1 = "a";
printf("                  --------Code Area--------\n");
printf("&%*s = %p\n", max_width, "string_literal", v1);
char *v11 = "b";
printf("$%*s = %p\n", max_width, "another_string_literal", v11);
char *v12 = "b";
printf("$%*s = %p\n", max_width, "another_string_literal_with_same_value(pooled)", v12);
printf("                  --------Global Area--------\n");
const static int v31 = 1;
printf("$%*s = %p\n", max_width, "const_static_variable", &v31);
static int v2 = 1;
printf("$%*s = %p\n", max_width, "static_variable", &v2);
printf("$%*s = %p\n", max_width, "global_static_variable", &gv21);
printf("$%*s = %p\n", max_width, "global_variable", &gv22);
printf("                  --------Heap Area--------\n");
int *v4 = (int *)malloc(sizeof(int));
printf("$%*s = %p\n", max_width, "malloc_variable", v4);
int *v5 = new int(1);
printf("$%*s = %p\n", max_width, "new_variable", v5);
printf("                  --------Stack Area--------\n");
const int v3 = 1;
printf("$%*s = %p\n", max_width, "const_variable", &v3);
int v6 = 1;
printf("$%*s = %p\n", max_width, "local_variable", &v6);
}
{% endhighlight %}

C/C++将变量存储在四个区域中，分别是：

1. 代码区(code area)
2. 全局变量区(global variable area)
3. 堆区(heap area)
4. 栈区(stack area)

下面逐一讲解：

## 代码区 ##
该区为只读，处于程序的地址空间最小端，用来**存储代码和字面量(literal variables)**。

## 全局变量区 ##
该区将**存储static和global两种类型的变量**及其各种组合类型，如：global static, const static, const global等。

### static与global的比较 ###
这里大家可能会产生一个疑问，global变量和static变量既然都是存储在全局变量区，那么这两者有什么不同呢？

其实，在C/C++看来，这两者主要在以下两点不同：

|  | static variable | global variable|
|--| --------------- | ---------------|
|作用域 | 限制在声明其的函数或类内 | 全局访问|
|生命周期 | 定义的函数或类被首次使用 至 程序结束 | 程序开始运行 至 程序结束|

1. 作用域

   **static变量的作用域被限制在声明其的函数或类内，而global变量没有这种限制**（要不就不用叫global了）。但是，static变量的生命周期并不会随着函数的退出而结束，它会一直驻留在全局变量区中。我们可以通过指向static变量的指针在其作用域外访问static数据。

2. 生命周期

   **static变量的生命周期从所属函数或类第一次被使用开始，所以其不适用于多线程编程。所以我们不应该在多线程环境下使用static变量**。global变量则不存在这个问题。

## 堆区 ##
这个区的地址空间在栈区之前，用来存储用户动态开辟的变量，即使用`new`，`malloc`等函数申请的空间。从申请的空间地址我们可以看出，**堆区所申请的空间地址是按照从小到大分配的**（区别于栈区）。

## 栈区 ##
我想大家可能都比较熟悉[函数调用栈](https://en.wikipedia.org/wiki/Call_stack)的概念了，栈区就是用来存储函数调用栈的。假设main函数调用了函数A，函数A的相关信息（栈帧，局部变量，返回地址）就会压入该栈，当函数A结束返回时就会将这些信息出栈以达到释放空间的目的。另外我们可以看出，**栈区所申请的地址空间是按照从大到小分配的**。即`v3`的地址是大于`v6`的。
那么问题又来了，为什么编译器要设计成这种堆区和栈区相反的地址分配顺序呢？相信聪明的同学也许已经想到了。堆区和栈区实际上共享了同一块内存空间，他们被设计成从这个空间的两端开始，下图描述了这种结构：

![堆区和栈区空间分配示意图](/public/post_imgs/How_C_CPP_Manage_Variable_Storage_1.svg)

这样做的好处是显而易见的，即实现了堆区和栈区共享存储空间的目的。

另外值得一提的是，每个线程只拥有一个独立的栈区，其他三个区均共享父进程。

最后，将这四个区的地址空间展示如下图：
![C/C++存储空间示意图](/public/post_imgs/How_C_CPP_Manage_Variable_Storage_2.svg)

我个人理解有限，也难免有些许出错之处，欢迎批评指正！