#include <memory>
#include <utility>

using std::swap;
using std::move;
using std::exchange;

using std::memcpy;
using std::memmove;

template<class T> inline T const& min(T const& x, T const& y)
{
    return (x < y) ? x : y;
}
template<class T> inline T const& max(T const& x, T const& y)
{
    return (x > y) ? x : y;
}