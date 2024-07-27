#pragma once
#include "utils.hpp"


// 用于储存类型 T 的单链表
template<class T> class SinglyLinkedList
{
public:
    // 单链表里的节点
    class Node
    {
    public:
        // 数据类型
        typedef T eltype;

        // 节点里储存的数据
        eltype data;
        // 指向的下一个节点
        Node * next;

        // 检查节点的下一个是否为空
        bool is_stop() const
        {
            return this->next == nullptr;
        }

        // 创建一个空节点
        static Node * create()
        {
            // 申请一块内存储存 Node
            Node * node = new Node;
            // 给 node 赋值
            node->next = nullptr;
            return node;
        }
        // 创建一个值为 x 的节点
        static Node * create(eltype const& x)
        {
            // 申请一块内存储存 Node
            Node * node = new Node;
            // 给 node 赋值
            node->data = x;
            node->next = nullptr;
            return node;
        }
    };

    // 元素类型
    typedef T eltype;
    // 节点类型
    typedef Node ndtype;

protected:
    // 单链表的头节点
    ndtype * _start;
    // 单链表的长度
    sztype _length;

public:
    // 初始化空链表 (长度为0)
    SinglyLinkedList()
    : _start(nullptr), _length(0) {}
    // 释放链表的时候需要先把链清空了
    ~SinglyLinkedList()
    {
        this->empty();
    }

    // 复制已有的链表
    SinglyLinkedList(SinglyLinkedList const& l)
    : _start(nullptr), _length(l._length) {
        // 如果链的长度为 0 那就不需要创建链了
        if (l._length != 0)
        {
            // 创建头节点
            this->_start = ndtype::create(l._start->data);
            // 历遍传入链表的节点并在当前链表创建相应的节点
            ndtype * node_a = this->_start; // 当前链表的末端节点
            ndtype * node_b = l._start;     // 用于历遍传入链表的节点
            while (!node_b->is_stop())
            {
                // 进入传入链表的下一个节点
                node_b = node_b->next;
                // 为当前链表的末端节点创建下一个节点
                node_a->next = ndtype::create(node_b->data);
                // 声明当前链表的末端节点
                node_a = node_a->next;
            }
        }
    }
    // 复制已有的链表
    SinglyLinkedList & operator =(SinglyLinkedList const& l)
    {
        // 如果传入链表就是本链表, 则跳过
        if (this == &l)
        {
            return *this;
        }

        // 直接从传入链表 B 复制到当前链表 A (共同长度部分)
        sztype co_length = min(this->_length, l._length);
        ndtype * node_a = this->_start; // 用于历遍当前链表
        ndtype * node_b = l._start;     // 用于历遍传入链表
        for (sztype idx = 0; idx < co_length; idx++)
        {
            // 复制值
            node_a->data = node_b->data;
            // 下一个节点
            node_a = node_a->next;
            node_b = node_b->next;
        }
        // 因为上面部分已经历遍了共同长度部分, 所以到这里 node_a 和 node_b 起码有一个是 nullptr
        // 通过判断哪个节点不是 nullptr 就可以知道哪个链表更长

        // 如果原本当前链表比传入链表长, 那么则删除后面多余的节点
        if (node_a != nullptr)
        {
            while (node_a != nullptr)
            {
                ndtype * next_node = node_a->next;
                // 释放节点内存
                delete node_a;
                // 转到下一个节点
                node_a = next_node;
            }
            // 如果传入链表是空的, 需要把当前链表的头节点也设为空
            if (l._start == nullptr)
            {
                this->_start = nullptr;
            }
        }

        // 如果传入链表比原本当前链表长, 那么历遍后面的节点去创建后续的链
        if (node_b != nullptr)
        {
            // 如果当前链表是空的, 那么要先创建头节点
            if (this->_start == nullptr)
            {
                this->_start = node_a = ndtype::create(node_b->data);
                node_b = node_b->next;
            }
            // 因为进入这里时 node_a 已经是 nullptr 了, 所以需要重新索引 node_a 到链表末端
            else
            {
                node_a = this->stop();
                // 更改这整个方法的结构是可以做到更优的,
                // 不过确实有点懒了, 总之先塞一个 TODO 在这里
            }
            while (node_b != nullptr)
            {
                // 为当前链表的末端节点创建下一个节点
                node_a->next = ndtype::create(node_b->data);
                // 声明当前链表的末端节点
                node_a = node_a->next;
                // 转到下一个节点
                node_b = node_b->next;
            }
        }

        this->_length = l._length;
        return *this;
    }

    // 移动已有的链表 (移动就是把传入对象的东西全部偷走, 并且把空值塞给传入对象让它空手而归)
    SinglyLinkedList(SinglyLinkedList && l)
    : _start(exchange(l._start, nullptr)), _length(exchange(l._length, 0)) {}
    // 移动已有的链表 (移动就是把传入对象的东西全部偷走, 并且把空值塞给传入对象让它空手而归)
    SinglyLinkedList & operator =(SinglyLinkedList && l)
    {
        // 如果传入链表就是本链表, 则跳过
        if (this == &l)
        {
            return *this;
        }
        this->_start  = exchange(l._start, nullptr);
        this->_length = exchange(l._length, 0);
        return *this;
    }

    // 返回头节点
    ndtype * start()
    {
        return this->_start;
    }
    // 返回头节点
    ndtype const* /* 返回不可变指针 */ start() const /* 当链表不可变时调用这个方法 */
    {
        return this->_start;
    }
    // 返回末端节点
    ndtype * stop()
    {
        // 如果链表为空, 则返回空指针
        if (this->_length == 0)
        {
            return nullptr;
        }
        // node 用于历遍链
        ndtype * node = this->_start;
        while (!node->is_stop())
        {
            // 转到下一个节点
            node = node->next;
        }
        return node;
    }
    // 返回末端节点 (不可变版本)
    ndtype const* stop() const
    {
        // 如果链表为空, 则返回空指针
        if (this->_length == 0)
        {
            return nullptr;
        }
        // node 用于历遍链
        ndtype const* node = this->_start;
        while (!node->is_stop())
        {
            // 转到下一个节点
            node = node->next;
        }
        return node;
    }
    // 返回当前长度
    sztype length() const /* 如果没有定义可变成员方法, 那无论链表可变不可变都会调用这个方法 */
    {
        return this->_length;
    }

    // 检查链表是不是空的
    bool is_empty() const
    {
        return this->_length == 0;
    }
    // 清空链表
    void empty()
    {
        // node 用于历遍所有节点
        ndtype * node = this->_start;
        // 历遍链
        while (node != nullptr)
        {
            // 先暂存下一个节点的指针
            ndtype * next_node = node->next;
            // 释放当前节点的内存
            delete node;
            // 转到下一个节点
            node = next_node;
        }
        // 设为空链表
        this->_start = nullptr;
        this->_length = 0;
    }

    // 索引节点
    ndtype * get(sztype idx)
    {
        // node 用于历遍链
        ndtype * node = this->_start;
        // 递减 idx 当达到 0 时即得到所需节点
        for (; idx > 0; idx--)
        {
            // 转到下一个节点
            node = node->next;
        }
        return node;
    }
    // 索引节点 (不可变版本)
    ndtype const* get(sztype idx) const
    {
        // node 用于历遍链
        ndtype const* node = this->_start;
        // 递减 idx 当达到 0 时即得到所需节点
        for (; idx > 0; idx--)
        {
            // 转到下一个节点
            node = node->next;
        }
        return node;
    }

    // 索引元素 (返回引用是为了可以在外部修改链表里的值)
    eltype & operator [](sztype idx)
    {
        return this->get(idx)->data;
    }
    // 索引元素 (返回引用是为了避免创建新对象)
    eltype const& operator [](sztype idx) const
    {
        return this->get(idx)->data;
    }

    // 把值 x 插到链表的末端
    void push(eltype const& x)
    {
        // 创建新节点
        ndtype * new_node = ndtype::create(x);
        // 如果当前链表为空, 那么新节点就是头节点
        if (this->_start == nullptr)
        {
            this->_start = new_node;
        }
        // 否则在末端节点后面加入新节点
        else
        {
            this->stop()->next = new_node;
        }
        // 链表长度 + 1
        this->_length++;
    }
    // 把值 x 插入到位置 idx
    void insert(sztype idx, eltype const& x)
    {
        // 创建新节点
        ndtype * new_node = ndtype::create(x);
        // 如果插入到索引 0, 那么新节点就是头节点
        if (idx == 0)
        {
            // 原本的头节点是新头节点的下一个
            new_node->next = this->_start;
            // 声明新的头节点
            this->_start = new_node;
        }
        // 否则在第 idx-1 个节点后面插入新节点
        else
        {
            // 更改链表的结构
            ndtype * prev = this->get(idx - 1);
            ndtype * next = prev->next;
            prev->next = new_node;
            new_node->next = next;
        }
        // 链表长度 + 1
        this->_length++;
    }
    // 删除第 idx 个元素并返回它的值
    eltype pop(sztype idx)
    {
        // node 储存需要删除的节点
        ndtype * node;
        // 如果弹出的节点是头节点, 那么新的头节点就是原本头节点的下一个
        if (idx == 0)
        {
            // 先抓住原本的头节点
            node = this->_start;
            // 声明新的头节点
            this->_start = node->next;
        }
        else
        {
            // 先找到第 idx-1 个节点
            ndtype * prev = this->get(idx - 1);
            // 那么需要删除的节点就是 prev 的下一个
            node = prev->next;
            // 需要删掉的节点的下一个节点接上去
            ndtype * next = node->next;
            prev->next = next;
        }
        // 拿到节点内的数据
        eltype result = node->data;
        // 释放节点
        delete node;
        // 链表长度 - 1
        this->_length--;
        // 返回数据
        return result;
    }

    // 从左到右查找第一个值为 x 的元素并返回其索引
    sztype find(eltype const& x) const
    {
        // node 用于历遍链
        ndtype * node = this->_start;
        // 历遍链
        for (sztype idx = 0; idx < this->_length; idx++)
        {
            // 如果当前节点的数据等于 x, 直接返回索引
            if (node->data == x)
            {
                return idx;
            }
            // 下一个节点
            node = node->next;
        }
        // 找不到值 x 就返回非法索引
        return invalid_size;
    }
    // 删除所有值为 x 的元素
    void remove(eltype const& x)
    {
        // 创建一个假的链头方便后续操作
        ndtype * fake_start = ndtype::create();
        fake_start->next = this->_start;
        // node 用作历遍链
        ndtype * node = this->_start;
        // prev 和 next 分别为 node 的上一个和下一个
        ndtype * prev = fake_start;
        ndtype * next /* = node->next */;
        // 历遍链表
        while (node != nullptr)
        {
            // 因为只有 node 非空才可以获取下一个节点, 所以 next 的赋值放到这里了
            next = node->next;
            // 检查值是否相等
            if (node->data != x)
            {
                // 跳到下一个
                prev = node;
                node = next;
                // next 的赋值会在循环开始赋值, 这里就不管了
            }
            else
            {
                // 更改链的结构
                prev->next = next;
                // 删除当前节点
                delete node;
                // 链的长度 - 1
                this->_length--;

                // 跳到下一个
                // 因为 node 已经被删除了, 所以 node 的下一个 (也就是现在的 next) 的上一个是现在的 prev,
                // 所以 prev 保持不变
                node = next;
                // 同理 next 在这里也不管了
            }
        }
        // 还原链头
        this->_start = fake_start->next;
    }

    // 把链表逆序
    void reverse()
    {
        // node 用作历遍链
        ndtype * node = this->_start;
        // prev 为原本链表节点的上一个
        ndtype * prev = nullptr;
        // 历遍链表
        while (node != nullptr)
        {
            // 获取当前节点原本的下一个
            ndtype * next = node->next;
            // 更改当前节点的指向
            node->next = prev;
            // 下一个节点
            prev = node;
            node = next;
        }
        // 更改链头
        this->_start = prev;
    }


    // 下面一整块都是顺序迭代器用于支持 C++ 语法 `for (eltype & el : list)`
    /* 迭代器循环 `for (eltype & el : list)` 完全等价于以下语句:

        iter __iter = list.begin();
        iter __end  = list.end();
        while (__iter != __end)
        {
            eltype & el = *__iter;
            // things in for loop
            __iter++;
        }

    所以需要先实现迭代器类型 `iter`, 然后给线性表实现 `iter begin()` 和 `iter end()`,
    而类型 `iter` 需要实现 `eltype & operator *()`, `iter & operator ++()` 和 `bool operator !=(iter const&) const`
    */

    class iter
    {
    public:
        // 当前迭代器指向的节点
        ndtype * node;

        // 返回节点里数据的引用
        eltype & operator *()
        {
            return this->node->data;
        }
        // 迭代器递增, 也就是换到下一个节点
        iter & operator ++()
        {
            this->node = this->node->next;
            return *this;
        }
        // 迭代器比较是否相等, 也就是比较节点是否相等
        bool operator !=(iter const& other) const
        {
            return this->node != other.node;
        }
    };

    // 返回指向链头的迭代器
    iter begin()
    {
        return iter{this->_start};
    }
    // 返回指向链尾下一个节点的迭代器, 也就是空
    iter end()
    {
        return iter{nullptr};
    }

    // 对于不可变链表其实还需要额外实现常值迭代器

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
        return const_iter{this->_start};
    }
    const_iter end() const
    {
        return const_iter{nullptr};
    }
};