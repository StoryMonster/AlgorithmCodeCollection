#include <random>
#include <iostream>
#include <chrono>
#include <ctime>

std::default_random_engine rd(time(0));
void swap(int& a, int& b)
{
    int temp = a;
    a = b;
    b = temp;
}

void random_quick_sort(int* arr, int startIndex, int endIndex)
{
    std::uniform_int_distribution<int> dist(startIndex, endIndex);
    int refPos = dist(rd);
    swap(*(arr+refPos), *(arr+startIndex));
    int i = startIndex + 1;
    int j = endIndex;
    int refVal = *(arr+startIndex);
    while (true)
    {
        while (i < j && *(arr+i) <= refVal) { ++i; }
        while (j >= i && *(arr+j) > refVal) { --j; }
        if (i >= j) break;
        swap(*(arr+i), *(arr+j));
        ++i; --j;
    }
    swap(*(arr+startIndex), *(arr+j));
    if (j-1 > startIndex) { random_quick_sort(arr, startIndex, j-1); }
    if (j+1 < endIndex) { random_quick_sort(arr, j+1, endIndex); }
}

void normal_quick_sort(int* arr, int startIndex, int endIndex)
{
    int i = startIndex + 1;
    int j = endIndex;
    int refVal = *(arr+startIndex);
    while (true)
    {
        while (i < j && *(arr+i) <= refVal) { ++i; }
        while (j >= i && *(arr+j) > refVal) { --j; }
        if (i >= j) break;
        swap(*(arr+i), *(arr+j));
        ++i; --j;
    }
    swap(*(arr+startIndex), *(arr+j));
    if (j-1 > startIndex) { normal_quick_sort(arr, startIndex, j-1); }
    if (j+1 < endIndex) { normal_quick_sort(arr, j+1, endIndex); }
}

int main()
{
    const int MAX_VAL = 100000;
    std::uniform_int_distribution<int> dist(0, MAX_VAL);
    int nums1[MAX_VAL] = {};
    int nums2[MAX_VAL] = {};
    for (int i = 0; i < MAX_VAL; ++i)
    {
        nums1[i] = nums2[i] = dist(rd);     // 理论上此处不应该使用随机值，使用随机的话后面的随机快排将没有任何意义。此处应该读取非随机数据
    }
    auto start = std::chrono::steady_clock::now();
    random_quick_sort(nums1, 0, MAX_VAL-1);
    auto mid = std::chrono::steady_clock::now();
    normal_quick_sort(nums2, 0, MAX_VAL-1);
    auto end = std::chrono::steady_clock::now();
    
    std::cout << "random quick sort cost: " << std::chrono::duration <double, std::milli> (mid-start).count() << "us"<< std::endl;
    std::cout << "normal quick sort cost: " << std::chrono::duration <double, std::milli> (end-mid).count() << "us"<< std::endl;
}