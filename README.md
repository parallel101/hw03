# 高性能并行编程与优化 - 第0x讲的回家作业

1. template<class T> for vector function
2. vector的加法函数，使用 using T0 = decltype(T1{} + T2{}) 获取结果的类型
3. 添加 std::variant<std::vector<int>, std::vector<double> > 与 std::vector<double> 相加的函数
4. 使用 std::visit 实现自动匹配容器的加法和打印
5. 由auto自动推断返回类型可以有效避免在vector中推断所含成员类型
6. 用decltype推导出两个vector相加的类型


## 评分规则

- 完成作业基本要求 50 分（作业要求详见作业代码）
- 能够在 PR 描述中用自己的话解释 25 分
- 代码格式规范、能够跨平台 5 分
- 有自己独特的创新点 20 分
- 明显抄袭现象 -100 分

## 关于内卷

如果你把 variant 的 operator<< 改成了基于变长模板参数的，或是实现了其他运算符：
只要是在 **满足作业要求的基础** 上，这是件好事！
老师会酌情加分，视为“独特的创新点”，但最多不超过 20 分。
