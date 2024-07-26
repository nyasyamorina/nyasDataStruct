/* 纯粹测试用的 main */
#include <iostream>
#include <string>

#include "01-List.hpp"
#include "02-SinglyLinkedList.hpp"

using std::cout;
using std::endl;



// 使用数组检查线性表里的元素
template<class T> bool check_list(T const& list, typename T::eltype const* arr, sztype n)
{
    // 首先比较长度
    if (list.length() != n)
    {
        return false;
    }
    // 然后逐个元素检查值
    for (sztype idx = 0; idx < n; idx++)
    {
        if (list[idx] != *(arr++))
        {
            return false;
        }
    }
    return true;
}

// 测试线性表方法的通用函数 (push, insert, pop, find, reverse, remove, empty)
template<class T> std::string test_list()
{
    T list;

    int arr1[5] = {1,2,3,4,5};
    for (int * ptr = arr1; ptr < arr1 + 5; ptr++)
    {
        list.push(*ptr);
    }
    if (!check_list(list, arr1, 5))
    {
        return "push failed";
    }

    int arr2[7] = {6,1,2,3,6,4,5};
    list.insert(3, 6);
    list.insert(0, 6);
    if (!check_list(list, arr2, 7))
    {
        return "insert failed";
    }

    int arr3[5] = {6,2,3,6,5};
    int pop1 = list.pop(1);
    int pop2 = list.pop(4);
    if (pop1 != 1 || pop2 != 4 || !check_list(list, arr3, 5))
    {
        return "pop failed";
    }

    sztype idx1 = list.find(3);
    sztype idx2 = list.find(0);
    if (idx1 != 2 || idx2 != invalid_size)
    {
        return "find failed";
    }

    int arr4[5] = {5,6,3,2,6};
    list.reverse();
    if (!check_list(list, arr4, 5))
    {
        return "reverse failed";
    }

    int arr5[3] = {5,3,2};
    list.remove(6);
    if (!check_list(list, arr5, 3))
    {
        return "remove failed";
    }

    list.empty();
    if (!check_list(list, nullptr, 0))
    {
        return "empty failed";
    }

    return "pass";
}


int main()
{
    std::string info1 = test_list<List<int>>();
    cout << "List test: " << info1 << endl;

    std::string info2 = test_list<SinglyLinkedList<int>>();
    cout << "SinglyLinkedList test: " << info2 << endl;
}