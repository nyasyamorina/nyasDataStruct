#pragma once
#include <type_traits>  // 提供 std::is_same 结构
#include "utils.hpp"
#include "01-ContinuousList.hpp"
#include "02-SinglyLinkedList.hpp"
#include "03-DoublyLinkedList.hpp"
#include "04-Stack.hpp"


/* 跟栈不同, 因为入队和出队必须在容器两端分别进行,
所以对于 顺序表/单链表 来说, 出队/入队 必须是性能低下的,
所以这里就不需要单独给单链表实现一块模板了.

如果使用双链表就没有这种顾虑了, 双链表, yyds.

实际上单链表只要增加指向链尾的成员以优化 push 方法的话,
使用单链表作队列容器甚至比双链表要更好. 但是我懒了. */


template<class T, class L = DoublyLinkedList<T>> class Queue
{
    // 确保容器的元素类型跟栈的元素类型一致
    static_assert(std::is_same<T, typename L::eltype>::value);
protected:
    // 元素容器
    L _list;

public:
    // 元素类型
    typedef T eltype;

    // 获取当前队列的长度
    sztype length() const
    {
        return this->_list.length();
    }

    // 判断队列是否为空
    bool is_empty() const
    {
        return this->_list.is_empty();
    }
    // 清空队列
    void empty()
    {
        this->_list.empty();
    }

    // 把值 x 推入队列
    void push(eltype const& x)
    {
        this->_list.push(x);
    }
    // 弹出底层元素
    eltype pop()
    {
        return this->_list.pop_first();
    }

    // 访问底层元素而不弹出
    eltype & bottom()
    {
        return this->_list.first();
    }
    eltype const& bottom() const
    {
        return this->_list.first();
    }
};


/* 下面则是使用两个栈实现一个队列的邪道方法,
所以为什么要这样浪费计算机性能? */

template<class T, class L> class Queue<T, Stack<T, L>>
{
protected:
    // 入队时使用的栈
    Stack<T, L> _in;
    // 出队时使用的栈
    Stack<T, L> _out;
    // 用于表明现在元素在哪个栈里
    bool _in_out;

    // 这个方法是把所有元素从入队栈 _in 放入出队栈 _out 里
    void _in_to_out()
    {
        // 倒就完事了
        while (!this->_in.is_empty())
        {
            this->_out.push(this->_in.pop());
        }
        this->_in_out = true;
    }
    // 这个方法是把所有元素从出队栈 _out 放入入队栈 _in 里
    void _out_to_in()
    {
        // 倒就完事了
        while (!this->_out.is_empty())
        {
            this->_in.push(this->_out.pop());
        }
        this->_in_out = false;
    }

public:
    typedef T eltype;

    // 在这里需要给成员 _in_out 赋值, 所以不能忽略初始化方法
    Queue()
    : _in_out(false) /* 队列初始化之后修改队列的第一个操作必须是入队, 所以一开始元素必须在 _in 里 */ {}

    sztype length() const
    {
        // 询问当前元素所在栈的长度
        return this->_in_out ? this->_out.length() : this->_in.length();
    }

    bool is_empty() const
    {
        // 询问当前元素所在栈
        return this->_in_out ? this->_out.is_empty() : this->_in.is_empty();
    }
    void empty()
    {
        // 直接两个栈一起清空就完事了 (不推荐)
        this->_in.empty();
        this->_out.empty();
    }

    void push(eltype const& x)
    {
        // 首先要把所有数据从出队栈放回入队栈, 然后才能真的把值推入栈内
        this->_out_to_in();
        this->_in.push(x);
    }
    eltype pop()
    {
        // 首先要把所有数据从入队栈放回出队栈, 然后才能真的把弹出栈
        this->_in_to_out();
        return this->_out.pop();
    }

    eltype & bottom()
    {
        // 跟 pop() 是一样的
        this->_in_to_out();
        return this->_out.top();
    }
    eltype const& bottom() const
    {
        // 跟 pop() 是一样的
        this->_in_to_out();
        return this->_out.top();
    }
};