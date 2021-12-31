# Answer

## Q1

```cpp
// 请修复这个函数的定义：10 分
std::ostream &operator<<(std::ostream &*os*, std::vector<T> const &a) 
```

单纯从函数的定义上来说，缺少template T的定义，因此修复非常简单：

```cpp
template<class T>
std::ostream &operator<<(std::ostream &os, std::vector<T> const &a)
```

## Q2

```cpp
// 请修复这个函数的定义：10 分
template <class T1, class T2>
std::vector<T0> operator+(std::vector<T1> const &a, std::vector<T2> const &b) {
    // 请实现列表的逐元素加法！10 分
    // 例如 {1, 2} + {3, 4} = {4, 6}
}
```

这个题目其实做起来挺纠结的，因为在template的定义上来说，缺少了T0的定义，如果template的定义无需修复的话，那么我们首先修复的是function的return data type。使用auto来代替std::vector<T0>

```cpp
template <class T1, class T2>
auto operator+(std::vector<T1> const &a, std::vector<T2> const &b) {
    // 请实现列表的逐元素加法！10 分
    // 例如 {1, 2} + {3, 4} = {4, 6}
    using T = std::decay_t<decltype(a[0]+b[0])>; //Find the output type when given two vectors.
    std::vector<T> ret;
    for(int i=0; i<std::min(a.size(), b.size()); i++){
        ret.push_back(a[i]+b[i]);
    }
    return ret;
}
```

如果template的定义需要修复的话，首先应该定义T0, 并且预设T0的data type. 在测试代码中，因为输入数据只含有int, double两种类型，在这里 T0=double

```cpp
template <class T1, class T2, class T0=double>
std::vector<T0> operator+(std::vector<T1> const &a, std::vector<T2> const &b) {
    // 请实现列表的逐元素加法！10 分
    // 例如 {1, 2} + {3, 4} = {4, 6}
    // using T = std::decay_t<decltype(a[0]+b[0])>; //I find the output type when given two vectors.
    // T0 = T;
    std::vector<T0> ret;
    for(int i=0; i<std::min(a.size(), b.size()); i++){
        ret.push_back(a[i]+b[i]);
    }
    return ret;
}
```

## Q3

```cpp
template <class T1, class T2>
std::variant<T1, T2> operator+(std::variant<T1, T2> const &a, std::variant<T1, T2> const &b) {
    // 请实现自动匹配容器中具体类型的加法！10 分
}
```

这个题目有两种解决思路：

1. 将a, b两个variant的具体数据都提取出来后进行计算。但是这种方式带来的if分支过多： 假设a和b的type分别为m和n的情况下，就会出现n*m种分支结果。如果进行优化的话最终能够优化到m+n。在不考虑编程难度的情况下的话，这种代码的工程量还是太大。
2. 批量进行处理，具体来说就是使用std::visit函数来批量进行处理。std::visit虽然在编译中会生成大量的分支「static polymorph」，但是避免了人工编写大量的代码。数据的处理可以使用一个lambda函数进行编写，借助&来捕获当前function中的变量。

```cpp
template <class T1, class T2>
std::variant<T1, T2> operator+(std::variant<T1, T2> const &a, std::variant<T1, T2> const &b) {
    // 请实现自动匹配容器中具体类型的加法！10 分
    std::variant<T1, T2> ret;
    std::visit([&](auto const &aa, auto const &bb){ret = aa + bb;}, a, b);
    return ret;
}
```

但是，在测试函数中出现了std::vector\<T0\>和std::variant\<std::vector<T1\>, std::vector\<T2>>进行相加的情况。我在这里针对这种情况overload了“+”

```cpp
template<class T1, class T2, class T3>
auto operator+(std::variant<T1, T2> const& a, T3 const& b){
    /* 
    因为出现了std::variant跟std::vector进行相加的运算
    然而在之前的代码中没有出现相应的输入类型
    因此对这种加法进行overload
    */
    std::variant<T1,T2> ret = b;
    return a + ret;
}

template<class T1, class T2, class T3>
auto operator+(T3 const& a, std::variant<T1, T2> const& b){
    return b + a;
}
```



## Q4

```cpp
template <class T1, class T2>
std::ostream &operator<<(std::ostream &os, std::variant<T1, T2> const &a) {
    // 请实现自动匹配容器中具体类型的打印！10 分
}
```

函数的具体思路跟之前的Q3中一样，利用std::visit来实现相应的功能。

```cpp
template <class T1, class T2>
std::ostream &operator<<(std::ostream &os, std::variant<T1, T2> const &a) {
    std::visit([&](auto const& input){os<<input;}, a);
    return os;
}
```

## Q5【关于内卷】

**题目：把 variant 的 operator<< 改成了基于变长模板参数的**

实现思路，其实只是variant中的data type的长度变得不确定了。处理方式参照我们之前的Q4的处理方式就行。但是需要有一些修改。

实现代码：

```cpp
template<class... T>
std::ostream &operator<<(std::ostream &os, std::variant<T...> &inputs){
/*
    注：相比起之前的问题来说，variadic template输入的函数中需要考虑到输入可能是std::endl或者是其他的function的问题。
    remove const限定之后能够保证代码的正常运行，但是为什么呢？
    没办法理解, 期望得到解答。
*/
    std::visit([&](auto const& input){os<<input;}, inputs);
    return os;
}
```

