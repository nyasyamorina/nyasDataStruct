#pragma once
#include "utils.hpp"


/*
* 双链表实在太多部分跟单链表一样了, 就不塞这么多备注了
*/

// 用于储存类型 T 的双链表
template<class T> class DoublyLinkedList
{
public:
    // 双链表里的节点
    class Node
    {
    public:
        typedef T eltype;

        eltype data;
        // 指向上一个或下一个节点
        Node * prev;
        Node * next;

        // 检查节点的上一个是否为空
        bool is_begin() const
        {
            return this->prev == nullptr;
        }
        // 检查节点的下一个是否为空
        bool is_end() const
        {
            return this->next == nullptr;
        }

        static Node * create()
        {
            Node * node = new Node;
            node->prev = node->next = nullptr;
            return node;
        }
        static Node * create(eltype const& x)
        {
            Node * node = new Node;
            node->data = x;
            node->prev = node->next = nullptr;
            return node;
        }
    };

    typedef T eltype;
    typedef Node ndtype;

    // 用于连接两个双端节点
    static inline void connect(ndtype * prev, ndtype * next)
    {
        prev->next = next;
        next->prev = prev;
    }

protected:
    // 链头
    ndtype * _begin;
    // 链尾
    ndtype * _end;
    // 链长
    sztype _length;

public:
    DoublyLinkedList()
    : _begin(nullptr), _end(nullptr), _length(0) {}
    ~DoublyLinkedList()
    {
        this->empty();
    }

    DoublyLinkedList(DoublyLinkedList const& l)
    : _begin(nullptr), _end(nullptr), _length(l._length) {
        if (l._length != 0)
        {
            this->_begin = this->_end = ndtype::create(l._begin->data);
            // 与单链表不同, 这里可以直接使用 this->_end 替代 node_a
            ndtype * node_b = l._begin;
            while (!node_b->is_begin())
            {
                node_b = node_b->next;
                // 使用 connect 方法直接连接链尾与新节点
                connect(this->_end, ndtype::create(node_b->data));
                this->_end = this->_end->next;
            }
        }
    }
    DoublyLinkedList & operator =(DoublyLinkedList const& l)
    {
        if (this == &l) return *this;

        sztype co_length = min(this->_length, l._length);
        ndtype * node_a = this->_begin;
        ndtype * node_b = l._begin;
        for (sztype idx = 0; idx < co_length; idx++)
        {
            node_a->data = node_b->data;
            node_a = node_a->next;
            node_b = node_b->next;
        }

        if (node_a != nullptr)
        {
            // 因为即将开始删除多余链, 所以先声明链尾位置
            this->_end = node_a->prev;
            // 如果传入链表为空, 那 node_a 就是链头, node_a->prev 就是 nullptr,
            // 所以这里需要一次额外检查
            if (this->_end != nullptr)
            {
                this->_end->next = nullptr;
            }

            while(node_a != nullptr)
            {
                ndtype * next_node = node_a->next;
                delete node_a;
                node_a = next_node;
            }
            if (l._begin == nullptr)
            {
                this->_begin = this->_end = nullptr;
            }
        }
        if (node_b != nullptr)
        {
            if (this->_begin == nullptr)
            {
                this->_begin = node_a = ndtype::create(node_b->data);
                node_b = node_b->next;
            }
            else
            {
                node_a = this->_end;
            }
            while (node_b != nullptr)
            {
                connect(node_a, ndtype::create(node_b->data));
                node_a = node_a->next;
                node_b = node_b->next;
            }
            // 重新声明链尾
            this->_end = node_a;
        }

        this->_length = l._length;
        return *this;
    }

    DoublyLinkedList(DoublyLinkedList && l)
    : _begin(exchange(l._begin, nullptr)), _end(exchange(l._end, nullptr)), _length(exchange(l._length, 0)) {}
    DoublyLinkedList & operator =(DoublyLinkedList && l)
    {
        if (this == &l) return *this;
        this->_begin  = exchange(l._begin, nullptr);
        this->_end    = exchange(l._end,   nullptr);
        this->_length = exchange(l._length, 0);
        return *this;
    }

    ndtype * begin()
    {
        return this->_begin;
    }
    ndtype const* begin() const
    {
        return this->_begin;
    }
    ndtype * end()
    {
        return this->_end;
    }
    ndtype const* end() const
    {
        return this->_end;
    }
    sztype length() const
    {
        return this->_length;
    }

    bool is_empty() const
    {
        return this->_length == 0;
    }
    void empty()
    {
        ndtype * node = this->_begin;
        while (node != nullptr)
        {
            ndtype * next_node = node->next;
            delete node;
            node = next_node;
        }
        this->_begin = this->_end = nullptr;
        this->_length = 0;
    }

    ndtype * get(sztype idx)
    {
        ndtype * node = this->_begin;
        for (; idx > 0; idx--)
        {
            node = node->next;
        }
        return node;
    }
    ndtype const* get(sztype idx) const
    {
        ndtype const* node = this->_begin;
        for (; idx > 0; idx--)
        {
            node = node->next;
        }
        return node;
    }

    eltype & operator [](sztype idx)
    {
        return this->get(idx)->data;
    }
    eltype const& operator [](sztype idx) const
    {
        return this->get(idx)->data;
    }

    void push(eltype const& x)
    {
        ndtype * new_node = ndtype::create(x);
        // 如果当前链表为空, 那么新节点就是同时为头节点和尾节点
        if (this->_length == 0)
        {
            this->_begin = this->_end = new_node;
        }
        else
        {
            // 连接链尾与新节点
            connect(this->_end, new_node);
            // 声明新节点是新链尾
            this->_end = new_node;
        }
        this->_length++;
    }
    void insert(sztype idx, eltype const& x)
    {
        ndtype * new_node = ndtype::create(x);
        // 如果插入到索引 0, 那么新节点就是同时为头节点和尾节点
        if (idx == 0)
        {
            // 因为 connect 方法会使用 next 内部, 所以需要额外检查空值
            if (this->_begin != nullptr)
            {
                // 接连接新节点与原链头
                connect(new_node, this->_begin);
            }
            // 声明新节点是新链头
            this->_begin = new_node;
        }
        // 否则在第 idx-1 个节点后面插入新节点
        else
        {
            // 更改链表的结构
            ndtype * prev = this->get(idx - 1);
            ndtype * next = prev->next;
            connect(prev, new_node);
            // 因为 connect 方法会使用 next 内部, 所以需要额外检查空值
            if (next != nullptr)
            {
                connect(new_node, next);
            }
        }
        // 如果 idx 刚好等于 length, 那么等价于 push, 需要更新链尾
        if (idx == this->_length)
        {
            this->_end = new_node;
        }
        // 链表长度 + 1
        this->_length++;
    }
    eltype pop(sztype idx)
    {
        // 不像单链表, 单链表需要先拿到 prev 再拿 node,
        // 双链表拿到 node 就可以直接拿到 prev 了
        ndtype * node = this->get(idx);
        ndtype * prev = node->prev;
        ndtype * next = node->next;
        // connect (附带一大堆空值检查版)
        if (prev != nullptr)
        {
            prev->next = next;
        }
        else
        {
            // 如果弹出节点是头节点, 那就需要重新声明头
            this->_begin = next;
        }
        if (next != nullptr)
        {
            next->prev = prev;
        }
        else
        {
            // 如果弹出节点是尾节点, 那就需要重新声明尾
            this->_end = prev;
        }
        eltype result = node->data;
        delete node;
        this->_length--;
        return result;
    }

    sztype find(eltype const& x) const
    {
        ndtype * node = this->_begin;
        for (sztype idx = 0; idx < this->_length; idx++)
        {
            if (node->data == x)
            {
                return idx;
            }
            node = node->next;
        }
        return invalid_size;
    }
    void remove(eltype const& x)
    {
        // 如果链表本身就是空的, 直接啥都不做
        if (this->_length == 0)
        {
            return;
        }
        // 跟单链表不同, 这里需要假的链头和链尾才能方便起来
        ndtype * fake_begin = ndtype::create();
        ndtype * fake_end   = ndtype::create();
        connect(fake_begin, this->_begin);
        connect(this->_end, fake_end);
        ndtype * node = this->_begin;
        // 跟单链表不同, prev 和 next 直接在 node 里面拿到就可以了
        while (node != fake_end)
        {
            if (node->data != x)
            {
                node = node->next;
            }
            else
            {
                // 更改链的结构
                ndtype * prev = node->prev;
                ndtype * next = node->next;
                connect(prev, next);

                delete node;
                this->_length--;
                node = next;
            }
        }
        // 还原链头和链尾
        this->_begin = fake_begin->next;
        this->_end   = fake_end->prev;
        // 如果链直接被清空了
        if (this->_length == 0)
        {
            this->_begin = this->_end = nullptr;
        }
    }

    void reverse()
    {
        // 双链表的逆序直接把每个节点里的 prev 和 next 换一下就可以了
        ndtype * node = this->_begin;
        while (node != nullptr)
        {
            swap(node->prev, node->next);
            // 原本链的下一个是改变后链的上一个
            node = node->prev;
        }
        // 把链头和链尾也换一下
        swap(this->_begin, this->_end);
    }
};