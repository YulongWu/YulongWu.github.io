---
title:      求两个有序数组的中位数及其扩展
category:   Algorithm
tag:        [algorithm, binary search, leetcode]
lastupdate: "2015-04-22"
---

今天刷leetcode时遇到了下面这个问题:

**在两个排序的数组中求中位数。其中，偶数个元素需要返回中间两个数的平均值。**
方法的接口为：
{% highlight c++ %}
double findMedianSortedArrays_old(vector<int>& nums1, vector<int>& nums2);
{% endhighlight %}

首先，请认真思考该题五分钟（有纸笔写写画画就再好不过了）。

下面我将由浅入深讲解三种方法，并利用这三种方法试图向大家阐述分析问题时**由简到繁的解题模式**。即先找到和该问题相似的更简单的问题，然后试图从简化问题的解法中寻找灵感，看能否做一些添枝加叶的工作来得出这个复杂问题的解。

首先，先公示一下三种解法的时间复杂度（len1, len2为两个数组的长度），如下表所示：

| 解法 | 时间复杂度 |
|:-----	|-------------------------  |
| 解法1 	| O(len1 + len2)          	|
| 解法2 	| O(log(min(len1, len2)))   |
| 解法3 	| O(log(min(len1, len2))) 	|

### 解法一 ###
拿到这个问题，我们能想到什么和它类似的更简单的问题呢？我们来考虑将两个数组简化为一个数组来思考，并且基于这一个数组的最直观的解题方法，即：
> *思路1.0*：对于一个排好序的数组求中位数，即求第`(len+1)/2`小的数，那么就从小到大依次扫描元素，直到扫描到第`(len+1)/2`个元素为止。
(有木有很简单？！什么，你说还有平均数的情况？那属于枝叶，咱先不考虑。)
那么，这个简单问题的解法带给我们了什么灵感呢？也许你已经想到了，**我们明明也可以在两个数组上从小到大进行元素的依次扫描嘛！**这类似于归并排序的思想。好了，至此解法1就基本诞生了！下来加上求平均数情况的枝叶便好（不详述，详见代码）。

代码如下：
{% highlight c++ %}
double findMedianSortedArrays_old(vector<int>& nums1, vector<int>& nums2) {
    auto iter1 = nums1.begin(), iter2 = nums2.begin(), end1 = nums1.end(), end2 = nums2.end(), cur = nums1.begin(), prev = cur;
    int median_index = ((nums1.size() + nums2.size()) >> 1) + 1;
    for(int i=0; i < median_index; ++i) {
        prev = cur;
        if(iter1 != end1 && iter2 != end2) {
            if(*iter1 < *iter2) {
                cur = iter1;
                ++iter1;
            }   
            else {
                cur = iter2;
                ++iter2;
            }   
        }   
        else if(iter1 != end1) {
            cur = iter1;
            ++iter1;
        }   
        else {
            cur = iter2;
            ++iter2;
        }   
    }   
    return (nums1.size() + nums2.size()) & 1 ? *cur : ((*cur + *prev) / 2.); 
}
{% endhighlight %}

但是，我们仔细想想的话，因为其相对应的简化问题（即一个数组的情况）是可以继续优化的，解法一的代码应该还可以更优化。我们知道在题目给定了数组有序时，应该首先想到**二分法**，但是我们的解法还没有利用二分法，怎么可以不考虑**二分法**这一利器（将复杂度从O(n)降O(logn)）呢？！所以下面的两个解法，我们考虑一下二分法。

### 解法二 ###

拿到题目之后，我们还可以把它化简为什么样的问题呢？既然我们根据输入数组有序而考虑使用二分查找法，那为何不尝试直接套用一下**<font color="red">二分法的算法框架</font>**呢？这里大家不要片面地理解二分法就是课本中那个*查找有序序列中是否存在给定值*的代码片段，这里的二分法指的是一个可以用于很多问题的算法框架，我的不规范描述如下（该描述一时看不懂没关系，先看下面的），分为三步：

> 1. 对于给定问题找出其解空间的表达形式，该空间能够以确定的形式表示我们要求的解；

> 2. 找到解空间中的一个点，使得我们能够以该点为分界分离掉解空间中不可能存在解的部分，从而成比例缩小解空间（一般情况该点均为中点）；

> 3. 重复步骤二，直到解空间缩小为只包含有限个元素，所求解可以容易的得出为止（即寻找终止条件）。

要深入理解二分法，就必须了解二分法的一些应用示例：

> **我找到的二分法的一些典型应用如下**：
> > * 首先，当然是地球人都知道的，数据结构课本中的二分查找算法，不过光知道这个算法是不够的，下面的算法才是真正开眼的；
> > * 《编程之美》题2.5中的解法三；
> > * 《编程珠玑》Column 2.2 Ubiquitous Binary Search
> > * 下面的这个解法。

大家可以找出这四种方法进行比较，体会二分法的精髓。

好了，言归正传。首先我们将问题做一个等价转换，即将求中位数转换为求第k个小的数，偶数个元素时即相当于求第k和k+1小的元素的平均数，这里k取`(len1+len2+1)/2`。

下面我们来套用二分法的计算框架。同上对应也分为以下三步：
> 1. 我们来仔细观察一下这个问题的解空间是什么。解空间顾名思义，就是所有可能的解组成的集合。设两个数组为A和B，那么本题的任意一个解可以理解为从A和B中各取前`i`小和前`j`小个元素，且满足限制`i+j=k`，这样取出的元素的最大值（`A[i]`或`B[j]`）才有可能是我们要找的解。满足这种条件的解一共有多少个呢？**一共只有`min(len1+1,len2+1)`个**，因为存在`i+j=k`的限制，较短数组的下标`i`确定以后，`j`就确定了。至此解空间找到了。
> 2. 下面试图找出解空间的中间一点，该点要能够分离出不可能存在解的空间（其实大部分适用二分法的题的所有可能解均满足这个性质，我们只要找到能够将解空间尽可能缩小的一点，所以一般都是中点）。下面我们开看该题解空间的中点是否具有这个性质。由前可知所有解必须满足`i+j=k`且个数不大于两个数组长度的最小值加一，不失一般性，假设A的长度小于B，则不难得出解空间的中点为`(A[i-1],B[k-i-1])其中i=(len1+1)/2`，其中下标减1是由于其从零开始计数。下面我们看看该点能否分离出不可能存在解的空间，我们先在纸上大致画出如下草图：
>      ![划分二个数组草图](/public/post_imgs/Algo_Median_In_Two_Arrays.svg)
>      根据上图，可以得出如下规律：
>      > * **当`A[i] < B[j]`时，所求解一定不在`SA_Low = { A[0], A[i] }`和`SB_High = { B[j+1], B[len2] }`区间中**。
>      > * **当`A[i] >= B[j]`时，所求解一定不在`SA_High = { A[i+1], A[len1] }`和`SB_Low = { B[0], B[j] }`区间中**。

>      > 这可用反证法容易地证明。假设通过这个规律我们将解空间由`S1`缩小到了`S2`，但是由于解空间变化了，所以我们还需要**找出`S1`的解到`S2`中的映射关系**，从而在迭代过程中保证问题的等价性。由于我们要求的是第k小的元素，而且由如上规律，该元素一定大于`SA_Low`或`SB_Low`，所以可得：**缩小前的第k小元素等于缩小后的第`k - len(SA_Low)`或`k - len(SB_Low)`小的元素**。至此我们已经成功地将问题转化为更小规模的相同问题。看到这句话应该想到可以用递归了吧。

> 3. 下面我们讨论下二分的终止条件。这主要通过缩小的过程确定以后，讨论递归中缩小的变量的终止情况得出。对于本题的递归过程，终止只可能发生在缩小的数组或者变量`k`之间，可得如下两个条件：
>      > * 当其中一个数组缩减为空时，直接取另一个数组的第k个元素即为所求。
>      > * 当`k==1`时，取`min(A[0], b[0])`即为所求。

至此我们已经完整地套用二分法的算法框架分析完毕，下面只需要相应编写代码即可：

*Python版本的代码示例如下所示：*
{% highlight python %}
def Median2_others_M2_Var2_(nums1, nums2, k):
    len1, len2 = len(nums1), len(nums2)
    if len1 > len2:
        return Median2_others_M2_Var2_(nums2, nums1, k)
    if len1 == 0:
        return nums2[k-1]
    if k == 1:
        return min(nums1[0], nums2[0])
    i = (len1+1)/2; j = k - i
    if nums1[i-1] < nums2[j-1]:
        return Median2_others_M2_Var2_(nums1[i:len1] if i<len1 else [], nums2[0:j], k-i)
    else:
        return Median2_others_M2_Var2_(nums1[0:i], nums2[j:len2] if j<len2 else [], k-j)
def Median2_others_M2_Var2( nums1, nums2):
    len1, len2 = len(nums1), len(nums2)
    k = (len1 + len2 + 1) / 2
    if((len1+len2) & 1 == 1):
        return Median2_others_M2_(nums1, nums2, k)
    else:
        return (Median2_others_M2_(nums1, nums2, k) + Median2_others_M2_(nums1, nums2, k+1)) / 2.0
{% endhighlight %}

### 解法三 ###

我觉得解法三是三个解法中*最优雅*的一个，思路简洁清晰。该解法从中位数的性质入手，也是基于二分法，但是不需要对原数组的大小进行修改，所以不需要递归（当然解法二也可以不递归，但是那样算法就会显得比较丑）。

首先，我们还是先思考更简单的问题，考虑这个问题：

1. 如果是**一个**有序数组求中位数，那么问题该怎么解决？

   然后考虑简单的问题的解法如何转化为复杂问题的解法，考虑这个问题：

2. 从一个数组变为两个数组以后，问题哪些部分变化了，哪些部分是相通的即不变的？

首先来回答第一个问题，一个有序数组求中位数，我们可以很容易找到中间那个数（或两个数），不过我们的目的是借此归纳出本题中中位数的性质（即中位数一定满足这两条性质，且满足这两条性质的数一定是中位数），如下：

* 数组中比中位数小的数的个数与比中位数大的个数相等（中位数的性质）；

* 具体到有序数组中，我们可以保证中位数前面的数一定都比该数小，其后面的数一定都比该数大（有序数组的性质）。

这两个性质提醒了我们，可以将**中位数作为有序数组的一个<font color="red">划分</font>来考虑**。我们假设中位数前面的数组成集合SA，后面的数组成集合SB。那么以上两个性质便可以简便地描述为：

* 性质一：**集合SA与集合SB的大小相等**；

* 性质二：**集合SA中的任意元素均小于中位数，集合SB中的任意元素均大于中位数**。

有了以上结论，我们来考虑扩展到两个数组的情况。

首先对于两个数组，中位数的个数不变，即依然是元素个数为奇数时的一个元素和元素个数为偶数时的两个元素的平均数。而且其依然对应了一个对这两个数组的划分，将两个数组的元素划分为集合SA与集合SB两部分。更复杂的是，这时集合SA和集合SB是由两个数组中的部分元素共同组成的，即这个划分的表达方式由一个数组时的一个分界点`i`，扩展为两个数组中的两个分界点`(i, j)`。另外，一个数组时，我们可以很容易地找出同时满足以上两个条件的中位数。但是在两个数组中，问题变得复杂一些，但我们只要对这两条性质分别进行考虑，也不难找出规律。

满足以上性质1的中位数的可能情况有`min(i+1, j+1)`种，即较短数组中可能的划分情况。因为性质1要求`i+j=(len1+len2)/2`，使得较短数组中的划分（共`min(i+1, j+1)`种）确定以后，另一个数组的划分就也确定了。

**接下来我们需要做的就是如何在符合性质1的所有可能解中，找到符合性质2的真正解**。

为了方便理解，首先重申性质二：**集合SA中的任意元素均小于中位数，集合SB中的任意元素均大于中位数**。
假设我们以`(A[i], B[j])`作为一个划分，我们画一个示意图如下：
![解法三草图](/public/post_imgs/Algo_Median_In_Two_Arrays_2.svg)
可以看出，**<u>我们要保证A[i]与B[j]满足性质二，只需要保证`A[i] >= B[j-1]`且`B[j] >= A[i-1]`</u>**。相反的，若这两个条件中的任何一个不满足，我们都可以借其将解空间减半（同样二分法思想），即：

> * 若`A[i] < B[j-1]`，则中位数不可能存在于`{A[0], A[i]}`中

> * 若`B[j] < A[i-1]`，则中位数不可能存在于`{B[j], B[len(B)-1}`中

至此，我们已经走完了二分法计算框架的前两步，下面讨论一下终止条件。分为如下两种情况：

1. 若找到了`(i, j)`（其中i+j=(len1+len2)/2）满足`A[i] >= B[j-1]`且`B[j] >= A[i-1]`，那么我们能够确定中位数为`A[i-1], A[i], B[j-1], B[j]`四个数中的两个。按照数组长度的奇偶分如下两种情况：

   > 1. 若数组长度和为偶，则中位数为`A[i-1], A[i], B[j-1], B[j]`四个数中大小在中间的两个，因为已知`A[i-1]<A[i]`且`B[j-1]<B[j]`，我们只需取`A[i-1]`与`B[j-1]`中的较大值与`A[i]`与`B[j]`中的较小值即可。

   > 2. 若数组长度和为奇，则中位数为`A[i]`于`B[j]`中的较小值。

2. 若解空间缩小为大小为1（即较短数组中所有其他可能的解都已排除），则此时得出中位数的逻辑与以上相似，区别只是若i或j到达边界，就在`A[i-1], A[i], B[j-1], B[j]`四个数中排除不存在的数即可。

*至此，我们得出了整个解题过程，对应的Python代码实现如下：*
{% highlight python %}
def Median2_my(nums1, nums2):
    len1, len2 = len(nums1), len(nums2)
    if len1 > len2:
        return Median2_my(nums2, nums1)
    spliter = (len1 + len2) / 2
    low_bound, up_bound = 0, len(nums1)
    while low_bound <= up_bound:        i = (low_bound + up_bound)/2
        j = spliter - i        if i > 0 and j < len2 and nums1[i-1] > nums2[j]:
            up_bound = i-1
        elif j > 0 and i < len1 and nums2[j-1] > nums1[i]:
            low_bound = i+1
        else:
            break
    if i == len1:
        c1 = nums2[j]
    elif j == len2:
        c1 = nums1[i]
    else:
        c1 = min(nums1[i], nums2[j])
    if (len1 + len2) & 1:
        return c1
    if i == 0:
        c2 = nums2[j-1]
    elif j == 0:
        c2 = nums1[i-1]
    else:
        c2 = max(nums1[i-1], nums2[j-1])
    return (c1 + c2) / 2.0
{% endhighlight %}

### 扩展 ###
我能想到的扩展有如下两种形式：

1. 将求中位数改为求第k大或者第k小的数。

2. 将两个数组继续扩展为3个数组，n个数组。

对于第一种情况，以上三种解法均可以通过略加修改求得，在此不详述了，大家可以自己修改后评论在下面。

对于第二种情况：
> 解法一本身就是适应多个数组的，只需要将两个数字的比较修改为多个数字即可。若n比较大，可以考虑用一个小根堆存储n个数组的所有当前最小的元素，从而将比较的复杂度由O(n)降为O(logn)。

> 解法二和解法三在改为n个数组时，算法逻辑不需要怎么修改（只需添加一些边界条件）。即首先在`n`个数组中选出最短的数组，然后对于`n-1`个数组递归调用求第k个数的算法即相当于将后`n-1`个数组抽象成了一个数组。该算法的时间复杂度为`O(log(leni)的乘积)`。

> 对于解法三，需要在函数接口上加上参数k，表示取第k小的数，然后改为递归的调用形式。

*下面是基于解法三修改的对n个数组求中位数的Python程序示例，时间复杂度为`O(log(leni)的乘积)`，其中leni是第i个数组的长度（大家可以自己试着实现一下基于解法二的程序）：*
{% highlight python %}
def MedianN_my_(arrays, k):
    print 'k={0}, arrays={1}'.format(k, arrays)
    if len(arrays) == 0 or len(arrays) == 1 and len(arrays[0]) == 0:
        print arrays
        return None, None
    A = arrays[0]
    if len(arrays) == 1:
        return A[k-1] if k-1 < len(A) else None, A[k] if k < len(A) else None
    min_v = len(A)
    if min_v == 0:
        return MedianN_my_(arrays[1:len(arrays)], k)
    low_bound, up_bound = 0, min(k, min_v)
    i, low_max, high_min = 0, 0, 0
    while low_bound <= up_bound:
        i = low_bound + (up_bound - low_bound) / 2
        low_max, high_min = MedianN_my_(arrays[1:len(arrays)], k-i)
        if i >= 0 and high_min is not None and A[i-1] > high_min:
            up_bound = i-1
        elif i < min_v and low_max is not None and low_max > A[i]:
            low_bound = i+1
        else:
            break
    if i != min_v:
        high_min = A[i] if high_min is None else min(A[i], high_min)
    if i != 0:
        low_max = A[i-1] if low_max is None else max(A[i-1], low_max)
    return low_max, high_min
def MedianN_my(arrays):
    sum = 0
    arrays.sort(key = len)
    print arrays
    for array in arrays:
        sum += len(array)
    k = sum / 2;
    if sum & 1:
        return MedianN_my_(arrays, k)[1]
    else:
        low, high = MedianN_my_(arrays, k)
        return (low + high) / 2. if low is not None else None
{% endhighlight %}








