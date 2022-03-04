// CustomVector.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <math.h>

int abs(const int& a)
{
    if (a < 0) {
        return -a;
    }
    return a;
}

float celsius_to_fahrenheit(float celsius)
{
    return celsius * 9.0f / 5.0f + 32.0f;
}

float is_odd(int number)
{
    return (number & 1);
}

long int table[] = { 10, 100, 1000, 10000, 1000000 };
#define TABLE_LENGHT sizeof(table) / sizeof(long int)

int bisect_log10(long int n, int s, int e) {
    int a = (e - s) / 2 + s;
    if (s >= e)
        return s;
    if ((table[a] - n) <= 0)
        return bisect_log10(n, a + 1, e);
    else
        return bisect_log10(n, s, a);
}

int fast_log10(long int n) {
    return bisect_log10(n, 0, TABLE_LENGHT);
}

// Make sure digits buffer is big enough : ) 
unsigned int count_digits(unsigned int number)
{
    auto log = log10(number);
    auto rounded = lround(log);
    return rounded + 1;
}

void number_to_digits(unsigned int number, char** digits, int count)
{
    const float base_number = 10.0f;
    const int digit_count = count_digits(number);

    unsigned int next = number;
    for (int i = 0; i < digit_count; i++)
    {
        int power = digit_count - i - 1;
        int divider = base_number * base_number;
        float use = next / divider;
        *digits[i] = (int)use;
        next = next % divider;
    }
}

template<typename type>
using memory_address = type*;

template<typename resource_type>
class dynamic_array // cause it is objectively a better name than... Vector...
{
public:
    memory_address<int> x, y;

    //using resource_type = int;
    using resource_pointer = resource_type*;
    using iterator = resource_pointer;
    using const_iterator = const iterator;
    using size = size_t;

    const size RESIZE_MULTIPLIER = 2;
    const size DEFAULT_SIZE = 1;

    dynamic_array()
    {
        allocate(front, back, DEFAULT_SIZE);
        next = front;
    }

    dynamic_array(int capacity) 
    {
        allocate(front, back, capacity); 
        next = front;
    }

    ~dynamic_array()
    {
        free(front, back);
        next = nullptr; // just for consistency
    }

    void push_back(const resource_type& element) 
    {
        try_resize();
        *(next++) = element;
    }

    void remove(iterator it)
    {
        const_iterator end = this->end() - 1;
        for (iterator current = it; current < end; current++) {
            *current = current[1];
        }
        --next;
    }

    void push_front(const resource_type& element)
    {
        insert(0, element);
    }

    void insert(size at, const resource_type& element)
    {
        try_resize();
        
        const_iterator last = this->end();
        const_iterator start = this->begin() + at;
        for (iterator current = last; current > start; current--) {
            *current = *(current - 1);
        }
        *start = element;
        ++next;
    }

    bool pop_back(resource_type& out_element) 
    {
        if (count() > 0) {
            out_element = *(--next);
            return true;
        }
        return false;
    }

    resource_type& at(size at)
    {
        return front[at];
    }

    iterator get(size at) 
    {
        return front + at;
    }

    size count() 
    {
        return next - front;
    }

    size capacity() 
    {
        return back - front;
    }

    void resize(size new_capacity) 
    {
        if (new_capacity == capacity()) {
            return;
        }

        internal_resize(new_capacity);
    }

    iterator begin() 
    {
        return front;
    }

    iterator end() 
    {
        return next;
    }

    void clear()
    {
        next = front;
    }

    // Reset function, to also reset capacity to 1 :) 

private:
    static void allocate(iterator& begin, iterator& end, int capacity) 
    {
        begin = new resource_type[capacity];
        end = begin + capacity;
    }

    static void free(iterator& begin, iterator& end) 
    {
        delete[] begin;
        begin = nullptr;
        end = nullptr;
    }

    void internal_resize(size new_capacity)
    {
        int copy_capacity = new_capacity < capacity() ? new_capacity : capacity();

        iterator old_begin = this->front;
        iterator old_end = this->back;
        allocate(this->front, this->back, new_capacity);
        for (int i = 0; i < copy_capacity; ++i) {
            this->front[i] = old_begin[i];
        }
        next = this->front + copy_capacity;

        free(old_begin, old_end);
    }

    void try_resize() 
    {
        if (next >= back) { 
            internal_resize(capacity() * RESIZE_MULTIPLIER);
        }
    }

    iterator front{ nullptr };
    iterator next{ nullptr };
    iterator back{ nullptr };
};

int main()
{
    printf("%d\n", fast_log10(9999));
    printf("%d\n", fast_log10(10000));
    printf("%d\n", fast_log10(100001));
    //dynamic_array<int> arr;
    //for (int i = 0; i < 20; ++i) {
    //    arr.push_back(i);
    //    //printf("%d\n", arr.count());
    //}

    //for (int i = 0; i < 20; ++i) {
    //    int number;
    //    arr.pop_back(number);
    //    printf("%d\n", number);
    //}

    //for (auto& current : arr)
    //{
    //    printf("%d\n", current);
    //}

    //for (auto current = arr.begin(); current < arr.end(); ++current) {
    //    printf("%d\n", *current);
    //}
    //arr.remove(arr.get(5));
    //arr.insert(0, 88);
    //arr.insert(1, 99);
    //arr.insert(19, 111);
    //arr.insert(7, 122);

    //for (auto current = arr.begin(); current < arr.end(); ++current) {
    //    printf("%d\n", *current);
    //}
}
