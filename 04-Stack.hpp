#pragma once
#include <type_traits>  // 提供 std::is_same 结构
#include "utils.hpp"
#include "01-ContinuousList.hpp"
#include "02-SinglyLinkedList.hpp"
#include "03-DoublyLinkedList.hpp"


/* 在这里会直接使用现成的线性表作为栈的内部容器, 而线性表的实现都是"无限长"的,
所以完全不需要检查栈是否已满. 但如果使用数组作为栈的容器的话 (在这里不会实现这个),
就需要在入栈时检查内部容器是否已满. */


// 储存类型为 T, 并且使用 L 作为内部容器的栈
template<class T, class L = DoublyLinkedList<T> /* 用双链表做默认容器 */> class Stack
{
    // 确保容器的元素类型跟栈的元素类型一致
    static_assert(std::is_same<T, typename L::eltype>::value);
protected:
    // 元素容器
    L _list;

public:
    // 元素类型
    typedef T eltype;

    // 初始化空栈 (实际上在这里可以完全不写这个方法)
    Stack()
    : _list() {}
    // 在销毁栈时会自动销毁栈的成员, 所以不需要使用 ~Stack() 手动销毁 _list

    // 获取当前栈的长度
    sztype length() const
    {
        return this->_list.length();
    }

    // 判断栈是否为空
    bool is_empty() const
    {
        return this->_list.is_empty();
    }
    // 清空栈
    void empty()
    {
        this->_list.empty();
    }

    // 把值 x 推入栈
    void push(eltype const& x)
    {
        this->_list.push(x);
    }
    // 弹出顶层元素
    eltype pop()
    {
        return this->_list.pop_last();
    }

    // 访问顶层元素而不弹出
    eltype & top()
    {
        return this->_list.last();
    }
    eltype const& top() const
    {
        return this->_list.last();
    }
};


/* 可以看到上面对栈的实现里, 入栈出栈都是在容器的末端位置进行的,
对于顺序表和双链表来说还不错, 但如果是单链表的话每次都在末端进行的性能开销是非常大的,
所以下面对使用单链表作为容器的栈, 特化为在容器开始位置进行入栈出栈.

但对于顺序表来说在开始位置进行入栈出栈反而是性能低下的. */

template<class T> class Stack<T, SinglyLinkedList<T>>
{
    // 因为这里声明形参的时候已经确定了单链表的元素类型, 所以就不需要进行类型检查了
protected:
    SinglyLinkedList<T> _list;

public:
    typedef T eltype;

    // 这里就直接忽略初始化方法了, 因为在初始化栈的时候, 里面的成员也会自动初始化

    // 因为这里本质上是在定义完全不同的类型, 所以上面定义里完全一模一样的方法都要再写一次

    sztype length() const
    {
        return this->_list.length();
    }

    bool is_empty() const
    {
        return this->_list.is_empty();
    }
    void empty()
    {
        this->_list.empty();
    }

    // 把值 x 推入栈
    void push(eltype const& x)
    {
        this->_list.insert(0, x);
    }
    // 弹出顶层元素
    eltype pop()
    {
        return this->_list.pop_first();
    }

    // 访问顶层元素而不弹出
    eltype & top()
    {
        return this->_list.first();
    }
    eltype const& top() const
    {
        return this->_list.first();
    }
};