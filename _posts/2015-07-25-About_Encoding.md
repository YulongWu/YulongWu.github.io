---
title:      Unicode字符编码与Python中的应用
category:   Algorithm
tag:        [unicode, utf8, ascii, python]
lastupdate: "2015-08-28"
---

今天写Python时又遇到了编码问题，考虑到前段时间面试时又被问到了字符编码的问题，so 比较彻底了了解了一下字符编码问题。记录如下：

首先，开发者可能经常遇到这些编码规则：

* ASCii
* ANSI
* GB2132
* GBK
* Unicode
* UTF-8
* UTF-16

## 按从简到繁的顺序进行介绍：

首先就是计算机专业同学都学过的ASC2编码，20世纪60年代起源于美国。它是这些编码里面最老也最简单的编码方法。每个编码占用1个字节（8个bit），理论上可以存储256个字符。但是考虑到扩展性，最高位bit没有用，因此只收录了128个字符，由于起源于美国，所以收录的大多为常用英文字符（键盘上的那些等）。这当然对于非英语的人是不够用的，所以有了下面这些编码方法。

### Unicode:

后来各国（主要欧洲）基于ASC2编码空余的128个位置演化出了对于自己语言的各种编码方式，相互不能兼容。所以Unicode (unique code?)就被设计出来，Unicode定义了一组统一的字符字符编码方式，理解Unicode的关键点在于“Unicode作为一个编码系统分为编码方式和实现方式两个层次”。所谓编码方式，即一个字符对应于一个什么样的编码；所谓实现方式，就是这个编码在计算机中应该如何表示（如何存储）。所以下面就分编码方式和实现方式分别讲解一下：

> #### Unicode 之 编码方式：

> 所谓编码方式，就是将字符对应为一组统一排列的码。起初的Unicode只定义了16位的编码空间，以为足够表示人类各种语言的符号。但后来发现并不够，于是将编码空间扩展到了32位。但是现在的编码空间只用到了0x10ffff，即在0x0000-0xffff的基本多文种平面上加了16个辅助平面。不过人类实际上常用的字符并不到216（65536）个（都定义在基本多文种平面内），所以目前常用的Unicode字符集编码方式为UCS-2，其对所有字符统一使用2字节定长编码，最多可表示216(65536)个字符（汉字占20901个，包括了所有简体字，繁体字等），即所有基本多文种平面中的字符。涵盖16个辅助平面的编码方式为UCS-4，对所有字符使用4字节进行编码，同时向下兼容UCS-2（即UCS-2中的编码0x????对应到UCS-4中为0x0000????）。这里需要注意的是常用的UCS-2编码只能编码216个字符，所以并不是所有Unicode字符都能在UCS-2找到对应编码的。一般系统支持的字符集也只限基本多文种平面。

> #### Unicode 之 实现方式：

> 所谓Unicode的实现方式，就是如何定义编码的存储方式，使其面向计算机友好（就是计算机能够高效的辨识，并且能够尽量节省存储空间）。目前常用的Unicode实现方式为：UTF-8, UTF-16 Big Endian，UTF-16 Small Endian。其中的UTF其实就是Unicode实现方式的意思，全称为Unicode Transformation Format。计算机系统或者软件中使用的都是Unicode的实现方式。

### UTF-8:

一句话概括：UTF-8是对Unicode编码的一种变长前缀编码实现方式。UTF-8根据字符使用频率采用变长编码对Unicode字符集的抽象编码进行编码。编码空间为264，编码从1字节长到4字节长不等。对于汉字，大部分常用字都以3字节编码（21000+），少见字以4字节编码（50000+）。值得一提的是，UTF-8中的8是代表码元的长度，即编码的长度是以字节为单位的。

### UTF-16:

UTF-16和UTF-8的不同之处在于其编码的码元长度为16 bits，也就是说，UTF-16编码出的字符只有2字节长和4字节长两种（即对应1个16 bits码元和2个16 bits码元）。
另外，值得一提的是，UTF-16的编码因为是以两个字节为码元，所以存在两种存储方式，`Big Endian`和`Small Endian`，中文叫大端序或者小端序。假设一个2字节的编码为`0x1234`，那么大端序就是将`0x12`存在`0x34`前面，如果按照内存从低到高读就是`0x1234`；而小端序就是将`0x43`存在`0x12`前面，如果按照内存从低到高读就是`0x3412`。再举个栗子，关于`Big Endian`和`Small Endian`: 汉字“严”对于的Unicode编码为 4E 25，如果采用`Big Endian`，其存储形式就是4E 25，如果采用`Small Endian`，其存储形式就是25 4E。便于记忆的话，大端序就是高位在前，小端序就是低位在前。人们平时的书写数字可以认为是大端序。那么，计算机如何识别文本使用的是大端序还是小端序呢？这就需要用到BOM文件头技术，关于BOM头，继续读下去就知道了。;)

### GB2312:

GB2132编码是中国大陆对简体汉字所指定的一种2字节定长（ASC2字符例外）编码方法，除过ASCII字符为1字节编码，其余每个字符对应一个2字节编码。包括6763个汉字，同时收录了包括拉丁字母、希腊字母、日文平假名及片假名字母、俄语西里尔字母在内的682个字符，不包括一些生僻字和繁体字。
GB2312并不属于Unicode编码的实现方式，它属于另一种叫做ANSI的编码类型。

### GBK:

针对GB2312表示的汉字数量不够的问题，由微软提出的了对于中文的GBK编码。每个编码也对应2个字节，收录了20,902个字符，包括中国大陆、台湾、日本及韩国通用字符集的汉字。

### ANSI:

ANSI并不是一种具体的编码方法，而是很多种编码方法的总称，ANSI编码就是指在低编码空间兼容ASCII码，又在高编码空间加入了自己语言的编码。在Unicode出现之前，各个国家制定了很多基于各自语言的ANSI编码，比如中国大陆的GB2312和台湾的Big5等。ANSI包括了中文的很多种编码（GB2312, GBK, Big5），日文编码（Shift_JIS）。ANSI是美国国家标准学会的简称，她是ASCII码的制定方。之所以统称ANSI，推测是因为他们都保留了0x00-0x7f的空间给ASC2字符，ASCII字符只占1字节，和ASCII码本身一致。

### 编码文件头BOM (Byte Order Mark):

对于一份文本文件，计算机如何确定其编码方式呢？文件编码头被设计来解决这个问题。文件编码头，就是用一种存储于文件头的特殊编码，表示该文件的编码方式，以便于软件进行辨认。Unicode的几种实现方式对应的BOM信息如下：

| Unicode | BOM文件头 |
|:------- |----------|
| UTF-8 | EF BB BF |
| UTF-16 Big Endian | FE FF |
| UTF-16 Small Endian	 | FF FE |
| UTF-32 Big Endian	| 00 00 FE FF |
| UTF-32 Small Endian	| FF FE 00 00 |

编辑器可以根据文件头识别Unicode字符文件编码。但是该规定并不是强制的，不能保证所有文件都存储文件头。对于没有存储文件头的编码方式未知文件，还有最后一招就是根据编码范围基于概率统计去猜其编码，准确率据说还不错不过不是100%。

# Python中的应用：

下面的说一下我接触到的Python中关于字符编码的知识，不够全面也不够透彻，只是顺带让大家有个直观的理解。（这里的Python限于Python2，Python3开始默认使用了utf8编码，所以下面的问题很多不存在了）
要完全理解上面程序的逻辑，我们需要先了解几个概念：
首先Python中用于存储字符串的类型主要有两种：`str` 和 `unicode`，`unicode`可以简单理解为一种封装了（即对用户透明）`unicode`编码实现方式的`str`。任何Unicode编码的字符串都可以以一种编码实现方式转化为`str`进行存储。而且在已知`str`的编码实现方法的情况下，也可以将`str`字符串解码为`unicode`字符串。两者通过`encode`和`decode`方法互相转化。

** <font color="red">encode: unicode -> str, decode: str -> unicode。</font> **

在Python中如果要对str进行转码（比如将utf8编码的字符串转为gbk），必须以unicode作为桥梁。即`s.decode('encoding_name_from').encode('encoding_name_to')`。
下面用一些例子让大家对编码有一个更直观的理解：
先来第一个例子：
例1:

{% highlight python %}
>>> s = u"一点资讯"
>>> s
u'\u4e00\u70b9\u8d44\u8baf'
>>> print s
一点资讯
{% endhighlight %}
讲解：Python中以u开头的字符串即表示unicode类型的字符串，Python会根据环境变量拿到当前编码实现方式，然后以这种编码方式构造`unicode`类型。假设我的终端编码方式为utf8，则`s=u'一点资讯'`等价于`s=unicode('一点资讯','utf8')`。`unicode`类型是python的自建(built-in)类型。将其直接打印的话（即`repr()`的输出），python会调用一种叫做unicode-escape的编码实现方法，对于unicode类型，其将非ASCII的字符转化为`\uxxxx`的形式并以ASCII字符形式进行存储，其中的xxxx就对应了UCS-2编码。即`repr(uni_string)`等价于`repr(uni_string.encode('unicode-escape'))`。那为何print可以在终端直接打印出内容呢？因为print默默地对其后的unicode字符串做了encode的工作，encode_name取自当前的环境变量。print s相当于`print s.encode(env_encoding)`

我们可以调用`unicode.encode()`方法将`unicode`转化以指定编码实现方式转化为`str`类型：
对于以Unicode编码实现方式存储的`str`类型，`repr()`方法会将非ASCII字符直接按字节转化为`\x??`的形式，其中`??`即该字节存储值的16进制表示。而`print`方法，会对后面的字符串默默地调用`encode(env_encoding)`。

{% highlight python %}
>>> s_utf8 = s.encode('utf8')
>>> s_utf8
'\xe4\xb8\x80\xe7\x82\xb9\xe8\xb5\x84\xe8\xae\xaf'
>>> print s_utf8
一点资讯
>>> s_gbk = s.encode('gbk')
>>> s_gbk
'\xd2\xbb\xb5\xe3\xd7\xca\xd1\xb6'
>>> print s_gbk
һ????Ѷ
{% endhighlight %}

注：由于我的命令行默认是用utf-8编码的，所以 `>>> print s_utf8` 可以输出正确内容，但是 `>>> print s_gbk` 就会输出乱码。这是因为print函数会默默地对字符串以默认编码方式进行编码，所以 `>>> print s_gbk`等价于 `>>> print s_gbk.encode('utf8')`。从utf-8和gbk的编码上我们也可以发现，utf-8对于常用中文一般都以3字节编码（示例中s_utf8的编码占了3*4=12字节），gbk对于中文以2字节编码。所以对于中文文章，使用gbk编码要比使用utf-8编码节省1/3以上的空间，不过前者的通用性当然差一些。
当然，在Python中，我们还可以对gbk, utf-8等编码进行解码：

{% highlight python %}
>>> s=s_utf8.decode('utf8')
>>> s
u'\u4e00\u70b9\u8d44\u8baf'
>>> print s
一点资讯
>>> s=s_gbk.decode('gbk')
>>> s
u'\u4e00\u70b9\u8d44\u8baf'
>>> print s
一点资讯
{% endhighlight %}

另外，关于u作为前缀修饰字符串所带来的变化，下面这个例子可以直观的说明：
{% highlight python %}
>>> s="一点资讯"
>>> s
'\xe4\xb8\x80\xe7\x82\xb9\xe8\xb5\x84\xe8\xae\xaf'
>>> s=u"一点资讯"
>>> s
u'\u4e00\u70b9\u8d44\u8baf'
{% endhighlight %}
即，不带u修饰的字符串会以当前默认编码方式进行编码（本机为utf-8），而带u修饰的字符串会自动根据当前系统编码方式封装为unicode类型。

关于`encode()`与`decode()`方法，有一个很容易让大家疑惑的地方这里说一下。我们知道unicode类型encode以后变为str类型，str类型decode以后变为unicode类型。可是为何unicode也有decode方法，而且str也有encode方法呢？其实这里python悄悄地做了点手脚，如下：

  `unicode.decode(coding)` <=> `unicode.encode(default_encoding).decode(coding)`

  `str.encode(coding)` <=> `str.decode(default_encoding).encode(coding)`

这下就看明白了吧，python自动在中间default_encoding加入了一个过渡。

最后，谈谈两种Python中大家可能经常会遇到的关于编码的操作：
看起来下 面两者都是在设定编码方式，那么他们的作用一样吗？不一样的话各是做什么的呢？
首先，他们虽然都是在设定编码方式，不过作用当然是不一样的。

> 1. ***文件头部定义：`# -*-coding: utf-8 -*-` 或者 `# coding: latin-1`***
这种方式基本只会出现在python脚本内部，用来定义编译器解析该脚本时所采用的编码方式。最直观的作用是，如果你需要在脚本中定义一个包含中文的字符串，就必须在脚本头部使用如上定义，否则默认使用ascii编码，遇到中文就会报错。错误信息如下：`SyntaxError: Non-ASCII character '\xe4' in file caller.py on line 7, but no encoding declared; see http://www.python.org/peps/pep-0263.html for details`

> 2. ***文件头部写：`import sys; reload(sys); sys.setdefaultencoding('utf8')`***
而如上三句，作用和第一点完全不同，它是在设定python的系统默认编码。Python会在所有需要编码方式参数却为指明的情况下默认使用系统默认编码。其既可以写在脚本里，也可以写在交互式命令行中。如果不设定，默认的编码方式也是ascii。

最后的最后，请大家记住关于字符编码的一个关键点：Unicode本身只是一个规定了字符极其对应编码数值对照表，而UTF-8, UTF-16等均是将数值对应为具体存储在计算机中的编码的实现方法。做个比喻的话，Unicode相当于RFC制定的一套规范，而UTF-8, UTF-16等都是对这套规范的不同版本的实现，当然支持的程度（可编码字符的多少）也各不相同。

## Reference:
[1] : [http://www.ruanyifeng.com/blog/2007/10/ascii_unicode_and_utf-8.html](http://www.ruanyifeng.com/blog/2007/10/ascii_unicode_and_utf-8.html)

[2] : [http://blog.csdn.net/softman11/article/details/6124345](http://blog.csdn.net/softman11/article/details/6124345)

[3] : [http://blog.csdn.net/trochiluses/article/details/16825269](http://blog.csdn.net/softman11/article/details/6124345)
