#pragma once
#include "utils.hpp"


/*
* 双链表实在太多部分跟单链表一样了, 就不塞这么多备注了
*/

// 用于储存类型 T 的双链表
template<class T> class DoublyLinkedList : public List<T>
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
        bool is_first() const
        {
            return this->prev == nullptr;
        }
        // 检查节点的下一个是否为空
        bool is_last() const
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
    ndtype * _first;
    // 链尾
    ndtype * _last;
    // 链长
    sztype _length;

public:
    DoublyLinkedList()
    : _first(nullptr), _last(nullptr), _length(0) {}
    ~DoublyLinkedList()
    {
        this->empty();
    }

    DoublyLinkedList(DoublyLinkedList const& l)
    : _first(nullptr), _last(nullptr), _length(l._length) {
        if (l._length != 0)
        {
            this->_first = this->_last = ndtype::create(l._first->data);
            // 与单链表不同, 这里可以直接使用 this->_last 替代 node_a
            ndtype * node_b = l._first;
            while (!node_b->is_first())
            {
                node_b = node_b->next;
                // 使用 connect 方法直接连接链尾与新节点
                connect(this->_last, ndtype::create(node_b->data));
                this->_last = this->_last->next;
            }
        }
    }
    DoublyLinkedList & operator =(DoublyLinkedList const& l)
    {
        if (this == &l) return *this;

        sztype co_length = min(this->_length, l._length);
        ndtype * node_a = this->_first;
        ndtype * node_b = l._first;
        for (sztype idx = 0; idx < co_length; idx++)
        {
            node_a->data = node_b->data;
            node_a = node_a->next;
            node_b = node_b->next;
        }

        if (node_a != nullptr)
        {
            // 因为即将开始删除多余链, 所以先声明链尾位置
            this->_last = node_a->prev;
            // 如果传入链表为空, 那 node_a 就是链头, node_a->prev 就是 nullptr,
            // 所以这里需要一次额外检查
            if (this->_last != nullptr)
            {
                this->_last->next = nullptr;
            }

            while(node_a != nullptr)
            {
                ndtype * next_node = node_a->next;
                delete node_a;
                node_a = next_node;
            }
            if (l._first == nullptr)
            {
                this->_first = this->_last = nullptr;
            }
        }
        if (node_b != nullptr)
        {
            if (this->_first == nullptr)
            {
                this->_first = node_a = ndtype::create(node_b->data);
                node_b = node_b->next;
            }
            else
            {
                node_a = this->_last;
            }
            while (node_b != nullptr)
            {
                connect(node_a, ndtype::create(node_b->data));
                node_a = node_a->next;
                node_b = node_b->next;
            }
            // 重新声明链尾
            this->_last = node_a;
        }

        this->_length = l._length;
        return *this;
    }

    DoublyLinkedList(DoublyLinkedList && l)
    : _first(exchange(l._first, nullptr)), _last(exchange(l._last, nullptr)), _length(exchange(l._length, 0)) {}
    DoublyLinkedList & operator =(DoublyLinkedList && l)
    {
        if (this == &l) return *this;
        this->_first  = exchange(l._first, nullptr);
        this->_last   = exchange(l._last,   nullptr);
        this->_length = exchange(l._length, 0);
        return *this;
    }

    virtual sztype length() const override
    {
        return this->_length;
    }

    virtual void empty() override
    {
        ndtype * node = this->_first;
        while (node != nullptr)
        {
            ndtype * next_node = node->next;
            delete node;
            node = next_node;
        }
        this->_first = this->_last = nullptr;
        this->_length = 0;
    }

    ndtype * get(sztype idx)
    {
        ndtype * node = this->_first;
        for (; idx > 0; idx--)
        {
            node = node->next;
        }
        return node;
    }
    ndtype const* get(sztype idx) const
    {
        ndtype const* node = this->_first;
        for (; idx > 0; idx--)
        {
            node = node->next;
        }
        return node;
    }
    ndtype * get_first()
    {
        return this->_first;
    }
    ndtype const* get_first() const
    {
        return this->_first;
    }
    ndtype * get_last()
    {
        return this->_last;
    }
    ndtype const* get_last() const
    {
        return this->_last;
    }

    virtual eltype & operator [](sztype idx) override
    {
        return this->get(idx)->data;
    }
    virtual eltype const& operator [](sztype idx) const override
    {
        return this->get(idx)->data;
    }

    virtual eltype & first() override
    {
        return this->_first->data;
    }
    virtual eltype const& first() const override
    {
        return this->_first->data;
    }
    virtual eltype & last() override
    {
        return this->_last->data;
    }
    virtual eltype const& last() const override
    {
        return this->_last->data;
    }

    virtual void push(eltype const& x) override
    {
        ndtype * new_node = ndtype::create(x);
        // 如果当前链表为空, 那么新节点就是同时为头节点和尾节点
        if (this->_length == 0)
        {
            this->_first = this->_last = new_node;
        }
        else
        {
            // 连接链尾与新节点
            connect(this->_last, new_node);
            // 声明新节点是新链尾
            this->_last = new_node;
        }
        this->_length++;
    }
    virtual void insert(sztype idx, eltype const& x) override
    {
        ndtype * new_node = ndtype::create(x);
        // 如果插入到索引 0, 那么新节点就是同时为头节点和尾节点
        if (idx == 0)
        {
            // 因为 connect 方法会使用 next 内部, 所以需要额外检查空值
            if (this->_first != nullptr)
            {
                // 接连接新节点与原链头
                connect(new_node, this->_first);
            }
            // 声明新节点是新链头
            this->_first = new_node;
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
            this->_last = new_node;
        }
        // 链表长度 + 1
        this->_length++;
    }

    virtual eltype pop(sztype idx) override
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
            this->_first = next;
        }
        if (next != nullptr)
        {
            next->prev = prev;
        }
        else
        {
            // 如果弹出节点是尾节点, 那就需要重新声明尾
            this->_last = prev;
        }
        eltype result = node->data;
        delete node;
        this->_length--;
        return result;
    }
    // 使用更快的方法覆写弹出第一个元素
    virtual eltype pop_first() override
    {
        ndtype * node = this->_first;
        // 重新声明链头
        this->_first = node->next;
        if (this->_first != nullptr)
        {
            this->_first->prev = nullptr;
        }
        // 如果链已空, 那么需要更改链尾为空
        else
        {
            this->_last = nullptr;
        }
        eltype result = node->data;
        delete node;
        this->_length--;
        return result;
    }
    // 使用更快的方法覆写弹出最后一个元素
    virtual eltype pop_last() override
    {
        ndtype * node = this->_last;
        // 重新声明链尾
        this->_last = node->prev;
        if (this->_last != nullptr)
        {
            this->_last->next = nullptr;
        }
        // 如果链已空, 那么需要更改链头为空
        else
        {
            this->_first = nullptr;
        }

        eltype result = node->data;
        delete node;
        this->_length--;
        return result;
    }

    virtual sztype find(eltype const& x) const override
    {
        ndtype * node = this->_first;
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
    virtual void remove(eltype const& x) override
    {
        // 如果链表本身就是空的, 直接啥都不做
        if (this->_length == 0)
        {
            return;
        }
        // 跟单链表不同, 这里需要假的链头和链尾才能方便起来
        ndtype * fake_first = ndtype::create();
        ndtype * fake_last  = ndtype::create();
        connect(fake_first, this->_first);
        connect(this->_last, fake_last);
        ndtype * node = this->_first;
        // 跟单链表不同, prev 和 next 直接在 node 里面拿到就可以了
        while (node != fake_last)
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
        if (this->_length != 0)
        {
            this->_first = fake_first->next;
            this->_last  = fake_last->prev;
            this->_first->prev = nullptr;
            this->_last->next  = nullptr;
        }
        // 如果链直接被清空了
        if (this->_length == 0)
        {
            this->_first = this->_last = nullptr;
        }
    }

    virtual void reverse() override
    {
        // 双链表的逆序直接把每个节点里的 prev 和 next 换一下就可以了
        ndtype * node = this->_first;
        while (node != nullptr)
        {
            swap(node->prev, node->next);
            // 原本链的下一个是改变后链的上一个
            node = node->prev;
        }
        // 把链头和链尾也换一下
        swap(this->_first, this->_last);
    }


    // 下面一整块都是顺序迭代器用于支持 C++ 语法 `for (eltype & el : list)`
    // 具体细节可以看看单链表对顺序迭代器的实现.

    class iter
    {
    public:
        ndtype * node;

        eltype & operator *()
        {
            return this->node->data;
        }
        iter & operator ++()
        {
            this->node = this->node->next;
            return *this;
        }
        bool operator !=(iter const& other) const
        {
            return this->node != other.node;
        }
    };

    iter begin()
    {
        return iter{this->_first};
    }
    iter end()
    {
        return iter{nullptr};
    }

    class const_iter
    {
    public:
        ndtype const* node;

        eltype const& operator *()
        {
            return this->node->data;
        }
        const_iter & operator ++()
        {
            this->node = this->node->next;
            return *this;
        }
        bool operator !=(const_iter const& other) const
        {
            return this->node != other.node;
        }
    };

    const_iter begin() const
    {
        return const_iter{this->_first};
    }
    const_iter end() const
    {
        return const_iter{nullptr};
    }
};