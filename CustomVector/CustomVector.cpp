// CustomVector.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <vector>

std::vector<int> test;

template<typename resource_type>
class dynamic_array // cause it is objectively a better name than... Vector...
{
public:
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
    dynamic_array<int> arr;
    for (int i = 0; i < 20; ++i) {
        arr.push_back(i);
        //printf("%d\n", arr.count());
    }

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
    arr.remove(arr.get(5));
    arr.insert(0, 88);
    arr.insert(1, 99);
    arr.insert(19, 111);
    arr.insert(7, 122);

    for (auto current = arr.begin(); current < arr.end(); ++current) {
        printf("%d\n", *current);
    }
}
