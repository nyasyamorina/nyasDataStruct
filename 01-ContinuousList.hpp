#pragma once
#include "utils.hpp"


// 用于储存类型 T 的顺序表
template<class T> class ContinuousList : public List<T> /* 因为顺序表是线性表的一种, 所以继承线性表并实现里面的接口方法 */
{
public:
    typedef T eltype;

protected:
    // 储存元素的数组
    eltype * _data;
    // 元素总数 (长度)
    sztype _length;
    // 当前数组可以储存的最大数量 (容量)
    sztype _capacity;

    // 把容量设为目标容量
    void _resize(sztype size)
    {
        // 如果目标容量就是当前容量, 则什么都不做
        if (this->_capacity == size)
        {
            return;
        }

        eltype * old = this->_data;
        // 如果目标容量为 0 则不分配新数组
        if (size == 0)
        {
            this->_data = nullptr;
        }
        else
        {
            // 分配一个新数组
            this->_data = new eltype[size];
            // 如果原数组里有内容, 就复制到新数组里
            if (this->_capacity > 0)
            {
                sztype els = min(this->_capacity, size);
                memcpy(this->_data, old, els * sizeof(eltype));
            }
        }
        // 元素总数是不能超过容量的
        this->_length = min(this->_length, size);
        this->_capacity = size;
        // 释放旧数组
        delete[] old;
    }
    // 扩展数组到至少能储存 size 个元素
    void _expand(sztype size)
    {
        // 当前数组可以放入 l 个元素, 什么都不做
        if (size <= this->_capacity)
        {
            return;
        }
        else
        {
            // 扩展到至少原来的两倍大小避免过多的分配/释放内存操作
            sztype new_capacity = max(2 * this->_capacity, size);
            this->_resize(new_capacity);
        }
    }

public:
    // 初始化空顺序表
    ContinuousList()
    : _data(nullptr), _length(0), _capacity(0) {}
    // 当顺序表被销毁前会自动调用这个函数, 在这里主要是确保申请了的内存被释放
    ~ContinuousList()
    {
        this->empty();
    }

    // 复制已有的顺序表
    ContinuousList(ContinuousList const& l)
    : _data(nullptr), _length(l._length), _capacity(l._capacity) {
        // 分配新数组并且复制数组内容
        if (l._capacity > 0)
        {
            this->_data = new eltype[l._capacity];
            memcpy(this->_data, l._data, l._capacity * sizeof(eltype));
        }
    }
    // 复制已有的顺序表
    ContinuousList & operator =(ContinuousList const& l)
    {
        // 如果传入的参数就是本顺序表, 则跳过
        if (this == &l)
        {
            return *this;
        }
        // 如果两数组容量不一样, 则重新分配本顺序表里的数组
        if (this->_capacity != l._capacity)
        {
            this->_capacity = l._capacity;
            delete[] this->_data;

            if (l._capacity == 0)
            {
                this->_data = nullptr;
            }
            else
            {
                this->_data = new eltype[l._capacity];
            }
        }
        // 复制元素
        if (l._capacity != 0)
        {
            memcpy(this->_data, l._data, l._capacity * sizeof(eltype));
        }
        this->_length = l._length;
        return *this;
    }

    // 移动已有的顺序表 (移动就是把传入对象的东西全部偷走, 并且把空值塞给传入对象让它空手而归)
    ContinuousList(ContinuousList && l)
    : _data(exchange(l._data, nullptr)), _length(exchange(l._length, 0)), _capacity(l._capacity, 0) {}
    // 移动已有的顺序表 (移动就是把传入对象的东西全部偷走, 并且把空值塞给传入对象让它空手而归)
    ContinuousList & operator =(ContinuousList && l)
    {
        // 如果传入的参数就是本顺序表, 则跳过
        if (this == &l)
        {
            return *this;
        }
        // 偷东西之前释放自己拿着的东西
        delete[] this->_data;
        // 开偷
        this->_data     = exchange(l._data, nullptr);
        this->_length   = exchange(l._length, 0);
        this->_capacity = exchange(l._capacity, 0);
        return *this;
    }

    // 返回当前数组
    eltype * data()
    {
        return this->_data;
    }
    // 返回当前数组
    eltype const* /* 返回不可变数组 */ data() const /* 当顺序表不可变时调用这个方法 */
    {
        return this->_data;
    }
    // 返回当前长度 (这个是父类已经声明的虚方法, 覆写它)
    virtual sztype length() const override /* override 就表示子类覆写父类的虚方法 */
    {
        return this->_length;
    }
    // 返回当前容量
    sztype capacity() const
    {
        return this->_capacity;
    }

    // 检查顺序表是不是空的
    virtual bool is_empty() const override
    {
        return this->_length == 0;
    }
    // 清空顺序表
    virtual void empty() override
    {
        // 释放数组
        delete[] this->_data;
        // 设为空值
        this->_data = nullptr;
        this->_length = 0;
        this->_capacity = 0;
    }

    // 索引元素 (返回引用是为了可以在外部修改顺序表里的值)
    virtual eltype & operator [](sztype idx) override
    {
        return this->_data[idx];
    }
    // 索引元素 (返回引用是为了避免创建新对象)
    virtual eltype const& operator [](sztype idx) const override
    {
        return this->_data[idx];
    }

    // 把值 x 插到顺序表的末端
    virtual void push(eltype const& x) override
    {
        // 确保数组可以多塞一个元素
        this->_expand(this->_length + 1);
        // 把值放入末端位置
        this->_data[this->_length] = x;
        // 递增数组长度
        this->_length++;
    }
    // 把值 x 插入到位置 idx
    virtual void insert(sztype idx, eltype const& x) override
    {
        // 确保数组可以多塞一个元素
        this->_expand(this->_length + 1);

        // 把从 idx 开始的元素往后挪一格
        // ptr 用于历遍数组
        eltype * ptr = this->_data + idx;
        // end 指向当前数组末端元素的后一个位置 (用于判断 ptr 是否历遍完数组)
        eltype * end = this->_data + this->_length;
        // tmp 用于存放暂时多出来的元素
        eltype tmp = *ptr;
        while (ptr < end)
        {
            // 把 tmp 里的值放入下一格, 并把下一格的值放到 tmp 里 (也就是交换值)
            swap(tmp, *(ptr + 1));  // 这一行又可以写成 swap(tmp, ptr[1]);
            // 递增指针
            ptr++;
            // 实际上面两句可以直接合并为 `swap(tmp, *(++ptr));`
        }

        // 实际上如果从数组末端开始移动元素的话是可以不需要 tmp 的, 但是实现出来可读性有点差
        // 可以参考 pop 里面的往前移动元素部分

        // 又实际上这一块可以直接使用自带的内存移动函数
        // memmove(this->_data + idx + 1, this->_data + idx, (this->_length - idx) * sizeof(eltype));

        // 把 x 放入空出来的位置 idx
        this->_data[idx] = x;
        // 递增长度
        this->_length++;
    }

    // 删除第 idx 个元素并返回它的值
    virtual eltype pop(sztype idx) override
    {
        // 拿到第 idx 个元素的值
        eltype result = this->_data[idx];
        // 递减长度
        this->_length--;

        // 把从 idx+1 开始的元素往前挪一格
        // ptr 用于历遍数组
        eltype * ptr = this->_data + idx;
        // end 指向当前数组末端元素的后一个位置 (用于判断 ptr 是否历遍完数组)
        eltype * end = this->_data + this->_length;
        while (ptr < end)
        {
            // 把下一个元素拿到当前位置
            *ptr = *(ptr + 1);  // 这一行又可以写成 ptr[0] = ptr[1];
            // 递增指针
            ptr++;
        }

        // 又实际上这一块可以直接使用自带的内存移动函数
        // memmove(this->_data + idx, this->_data + idx + 1, (this->_length - idx) * sizeof(eltype));

        // 返回数值
        return result;
    }
    // 使用超快的方法覆写弹出最后一个元素
    virtual eltype pop_last() override
    {
        // 拿到最后一个元素
        eltype result = this->_data[this->_length - 1];
        // 长度直接减一就完事了
        this->_length--;
        return result;
    }


    // 从左到右查找第一个值为 x 的元素并返回其索引
    virtual sztype find(eltype const& x) const override
    {
        // ptr 用于历遍数组
        eltype * ptr = this->_data;
        // 历遍数组并递增索引
        for (sztype idx = 0; idx < this->_length; idx++)
        {
            // 如果当前元素的值就是 x 则返回当前索引
            if (*ptr == x)
            {
                return idx;
            }
            // 如果值不相等则跳到下一个元素
            ptr++;
        }
        // 如果当前数组不存在值 x, 则返回非法索引
        return invalid_size;
    }
    // 删除所有值为 x 的元素
    virtual void remove(eltype const& x) override
    {
        // 注意到如果索引为 k 的元素前面有 m 个值为 x 的元素,
        // 那么删除所有 x 后这个元素的索引应为 k-m.
        // 所以直接用两个指针 A 和 B, 从 B 复制值到 A,
        // 当遇到值 x 时, B 递增但 A 不递增, 最后就可以删除所有值 x 了.
        eltype * ptr_a = this->_data;
        eltype * ptr_b = this->_data;
        // end 指向原本数组末端元素的后一个位置 (用于判断 ptr_b 是否历遍完数组)
        eltype * end = this->_data + this->_length;
        while (ptr_b < end)
        {
            // 检查 B 当前的元素的值是否为 x
            if (*ptr_b == x)
            {
                // 跳过值为 x 的元素, 并且最终数组长度会变短
                this->_length--;
                ptr_b++;
            }
            else
            {
                // 如果值不为 x 则复制到 A
                *ptr_a = *ptr_b;
                // 下一个
                ptr_a++; ptr_b++;
            }
        }
    }

    // 把顺序表逆序
    virtual void reverse() override
    {
        // 使用两个指针, 一个从头开始, 另一个从尾开始, 交换两个指针指向的值
        eltype * ptr_a = this->_data;
        eltype * ptr_b = this->_data + (this->_length - 1);
        // end 指向数组中间 (靠后一点) 的元素, 表示 ptr_a 的结束位置
        eltype * end = this->_data + (this->_length / 2);
        while (ptr_a < end)
        {
            // 交互两指针指向的值
            swap(*ptr_a, *ptr_b);
            // 下一个
            ptr_a++; ptr_b--;

            // 上面两行可以合并为 `swap(*(ptr_a++), *(ptr_b--));`
        }
    }


    // 下面一整块都是顺序迭代器用于支持 C++ 语法 `for (eltype & el : list)`
    // 具体细节可以看看单链表对顺序迭代器的实现.

    eltype * begin()
    {
        return this->_data;
    }
    eltype const* begin() const
    {
        return this->_data;
    }

    eltype * end()
    {
        return this->_data + this->_length;
    }
    eltype const* end() const
    {
        return this->_data + this->_length;
    }
};