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