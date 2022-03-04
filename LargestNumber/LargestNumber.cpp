// LargestNumber.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <type_traits>

template<typename A, typename B>
void largest_number(const A& a, const B& b)
{
    if constexpr (!std::is_arithmetic_v<A> || !std::is_arithmetic_v<B>) {
        static_assert(false, "One of the inputs is not arithmetic");
    }
    else {
        long double lhs = (long double) a;
        long double rhs = (long double) b;
        if (lhs > rhs) {
            std::cout << "A(" << a << ") is bigger than B(" << b << ")!\n";
        }
        else if (rhs > lhs) {
            std::cout << "B(" << b << ") is bigger than A(" << a << ")!\n";
        }
        else {
            std::cout << "B(" << b << ") and A(" << a << ") are equal!\n";
        }
    }
}
struct Test
{
    int x;
    char c;
};
int main()
{
    largest_number(5.1, 3);
}

