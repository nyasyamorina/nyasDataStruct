#pragma once

#include <memory>
#include <utility>

using std::swap;
using std::move;
using std::exchange;

using std::memcpy;
using std::memmove;

// 使用无符号整数可以避免负数检测
using sztype = unsigned long long;
// 这个值表示非法索引 (或非法数组大小)
constexpr sztype invalid_size = static_cast<sztype>(-1);

// 取两个数字的最小值
template<class T> inline T const& min(T const& x, T const& y)
{
    return (x < y) ? x : y;
}
// 取两个数字的最大值
template<class T> inline T const& max(T const& x, T const& y)
{
    return (x > y) ? x : y;
}


// 线性表的通用接口, 顺序表 (ContinuousList), 单链表 (SinglyLinkedList),
// 双链表 (DoublyLinkedList) 都会实现这些方法
template<class T> /* 使用模板表示列表可以储存任意特定元素 */ class List
{
public:
    // 元素类型
    typedef T eltype;

    // 返回当前列表的长度
    virtual sztype length() const /* 如果没给出不 const 的方法, 那不管列表是不是 const 都会调用这个方法 */ = 0 /* = 0 表示这个方法未实现, 需要子类去实现具体方法 */;

    // 判断列表是否为空
    virtual bool is_empty() const
    {
        // 如果子类没有专门特化实现这个方法, 那么可以直接调用父类默认的行为
        return this->length() == 0;
    }
    // 清空列表
    virtual void empty() = 0;

    // 索引第 idx 个元素, 返回引用主要是为了避免内存复制, 而且可以让外部程序修改它
    virtual eltype & operator [](sztype idx) = 0;
    virtual eltype const& /* 返回常值引用 */ operator [](sztype idx) const /* 这里的 const 表示当列表为 const 时就会调用这个方法 */ = 0;

    // 返回列表的第一个元素
    virtual eltype & first()
    {
        // 如果子类没有专门特化实现这个方法, 那么可以直接调用父类默认的行为
        return (*this)[0];  // 调用函数 operator [](0) 表示第一个元素
    }
    virtual eltype const& first() const
    {
        // 如果子类没有专门特化实现这个方法, 那么可以直接调用父类默认的行为
        return this->operator [](0);    // 这个写法是与上面非 const 版里的一样的
    }
    // 返回最后一个元素
    virtual eltype & last()
    {
        return (*this)[this->length() - 1];
    }
    virtual eltype const& last() const
    {
        return (*this)[this->length() - 1];
    }

    // 把值 x 放到列表末端
    virtual void push(eltype const& x)
    {
        // 一种极其低效的实现方法仅依赖于其他列表接口方法

        // 显而易见的, 不是吗
        this->insert(this->length(), x);
    }
    // 把值 x 插入到列表索引为 idx 的位置, 其中 idx 取值为 0 到 length (包括 length)
    virtual void insert(sztype idx, eltype const& x) = 0;

    // 删除第 idx 位置的元素, 并返回其值
    virtual eltype pop(sztype idx) = 0;
    // pop, 并且针对列表头部
    virtual eltype pop_first()
    {
        return this->pop(0);
    }
    // pop, 并且针对列表尾部
    virtual eltype pop_last()
    {
        return this->pop(this->length() - 1);
    }

    // 以从头到尾的顺序找到第一个值为 x 的元素并返回其索引
    virtual sztype find(eltype const& x) const
    {
        // 一种极其低效的实现方法仅依赖于其他列表接口方法

        // 暴力就完事了法
        for (sztype idx = 0; idx < this->length(); idx++)
        {
            if ((*this)[idx] == x)
            {
                return idx;
            }
        }
        // 找不到就返回非法索引
        return invalid_size;
    }
    // 删除所有值为 x 的元素
    virtual void remove(eltype const& x)
    {
        // 一种极其低效的实现方法仅依赖于其他列表接口方法

        // 搜索第一个值为 x 的元素
        sztype idx = this->find(x);
        while (idx != invalid_size)
        {
            // 不捕获返回值, 也就是直接删除这个元素
            this->pop(idx);
            // 查找下一个 x
            idx = this->find(x);
        }
    }

    // 逆序列表
    virtual void reverse()
    {
        // 一种极其低效的实现方法仅依赖于其他列表接口方法

        // 获取列表中间元素的索引
        sztype middle = this->length() / 2;
        // 逐个交换头/尾元素
        sztype front = 0;
        sztype back = this->length() - 1;
        // 知道到达中间元素就不用交换了
        while (front < middle)
        {
            // 这就体现了返回引用的重要性了
            swap((*this)[front], (*this)[back]);
            // 下一个
            front++; back--;
        }
    }
};