// SwapWithTemplates.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <type_traits>

template<typename A, typename B, bool enable, bool is_same>
void swap(A& a, B& b);

// error - Hm.
template
<
    typename A,
    typename B,
    bool enabled = true,
    std::enable_if_t<!std::is_same<A, B>::value, bool> = true
>
void swap(A& a, B& b)
{
    static_assert(false, "Only same types are swapable");
}

// integer
template
<
    typename A, 
    typename B, 
    std::enable_if_t<std::is_integral<A>::value, bool> = true,
    std::enable_if_t<std::is_same<A, B>::value, bool> = true
>
void swap(A& a, B& b) 
{
    std::cout << "Integer\n";
    a = a + b;
    b = a - b;
    a = a - b;
}

// move
template
<
    typename A,
    typename B,
    std::enable_if_t
    <   
        std::is_move_assignable<A>::value && 
        std::is_copy_assignable<A>::value &&
        !std::is_integral<A>::value, bool
    > = true,
    std::enable_if_t<std::is_same<A, B>::value, bool> = true
>
void swap(A& a, B& b)
{
    std::cout << "Move\n";
    A temp = std::move(a);
    a = std::move(b);
    b = std::move(temp);
}

// copy
template
<
    typename A, 
    typename B, 
    std::enable_if_t
    <
        std::is_copy_assignable<A>::value && 
        !std::is_move_assignable<A>::value && 
        !std::is_integral<A>::value, bool
    > = true,
    std::enable_if_t<std::is_same<A, B>::value, bool> = true
>
void swap(A& a, B& b)
{
    std::cout << "Copy\n";
    A temp = a;
    a = b;
    b = temp;
}

// c-style fallback
template
<
    typename A,
    typename B,
    std::enable_if_t
    <
        !std::is_move_assignable<A>::value && 
        !std::is_copy_assignable<A>::value && 
        !std::is_integral<A>::value, bool
    > = true,
    std::enable_if_t<std::is_same<A, B>::value, bool> = true
>void swap(A& a, B& b)
{
    std::cout << "Fallback\n";
    using byte = char; 
    byte buffer[sizeof(A)];
    memmove(&buffer, &a, sizeof(A));
    memmove(&a, &b, sizeof(A));
    memmove(&b, &buffer, sizeof(A));
}

//template<typename A, typename B, std::enable_if_t<std::is_integral<A>::value, bool> = true>
//void swap(A& a, B& b)
//{
//    if constexpr(!std::is_same_v<A, B>) { 
//        static_assert(false, "Only same types are swapable");
//    }
//    else {
//        if constexpr (std::is_integral_v<A>) {
//            a = a + b;
//            b = a - b;
//            a = a - b;
//        }
//        else if constexpr(std::is_copy_assignable_v<A>) {
//            auto temp = a;
//            a = b;
//            b = temp;
//            //std::swap
//        }
//        else { 
//            //A temp; // Triggers constructor
//            //memcpy(&temp, &a, sizeof(A));
//            using byte = char; 
//            byte buffer[sizeof(A)]; // Doesn't call constructor - Only stack allocates
//            memmove(&buffer, &a, sizeof(A));
//            memmove(&a, &b, sizeof(A));
//            memmove(&b, &buffer, sizeof(A));
//        }
//    }
//}


struct Numbers
{
    Numbers(const Numbers& other) = default;
    Numbers(Numbers&& other) = delete;
    Numbers& operator=(const Numbers& other) = default;
    Numbers& operator=(Numbers&& other) = delete;
    int a;
    int b;
    int c;
};


struct DeletedTheRuleOfFive
{
    DeletedTheRuleOfFive(const DeletedTheRuleOfFive& other) = delete;
    DeletedTheRuleOfFive(DeletedTheRuleOfFive&& other) = delete;
    DeletedTheRuleOfFive& operator=(const DeletedTheRuleOfFive& other) = delete;
    DeletedTheRuleOfFive& operator=(DeletedTheRuleOfFive&& other) = delete;

    int a;
    int b;
    int c;
};

struct VeryAnnoyingStruct
{
    VeryAnnoyingStruct& operator=(const VeryAnnoyingStruct& other) = delete;

    int a;
    int b;
    int c;
};

struct VeryAnnoyingStructWithMove
{
    VeryAnnoyingStructWithMove(const VeryAnnoyingStructWithMove& other) = default;
    VeryAnnoyingStructWithMove(VeryAnnoyingStructWithMove&& other) = default;
    VeryAnnoyingStructWithMove& operator=(const VeryAnnoyingStructWithMove& other) = default;
    VeryAnnoyingStructWithMove& operator=(VeryAnnoyingStructWithMove&& other)
    {
        a = other.a;
        b = other.b;
        c = other.c;
        other.a = 0;
        other.b = 0;
        other.c = 0;
        return *this;
    }
    int a;
    int b;
    int c;
};


std::ostream& operator<<(std::ostream& os, const Numbers& obj)
{
    os << "a: " << obj.a << " b: " << obj.b << " c: " << obj.c;
    return os;
}

std::ostream& operator<<(std::ostream& os, const VeryAnnoyingStruct& obj)
{
    os << "a: " << obj.a << " b: " << obj.b << " c: " << obj.c;
    return os;
}

std::ostream& operator<<(std::ostream& os, const VeryAnnoyingStructWithMove& obj)
{
    os << "a: " << obj.a << " b: " << obj.b << " c: " << obj.c;
    return os;
}

std::ostream& operator<<(std::ostream& os, const DeletedTheRuleOfFive& obj)
{
    os << "a: " << obj.a << " b: " << obj.b << " c: " << obj.c;
    return os;
}

int main()
{
    std::cout << "Integers" << "\n";
    {
        int a = 4;
        int b = 5;
        std::cout << "A: " << a << "\t- B: " << b << "\n";
        swap(a, b);
        std::cout << "A: " << a << "\t- B: " << b << "\n";
    }
    std::cout << "\n";
    std::cout << "Copy" << "\n";
    {
        Numbers a{ 5, 5, 5 };
        Numbers b{ 10, 10, 10 };

        std::cout << "A: " << a << "\t- B: " << b << "\n";
        swap(a, b);
        std::cout << "A: " << a << "\t- B: " << b << "\n";
    }
    std::cout << "\n";
    std::cout << "Fallback" << "\n";
    {
        VeryAnnoyingStruct a{ 5, 5, 5 };
        VeryAnnoyingStruct b{ 10, 10, 10 };
        std::cout << "A: " << a << "\t- B: " << b << "\n";
        swap(a, b);
        std::cout << "A: " << a << "\t- B: " << b << "\n";
    }
    std::cout << "\n";
    std::cout << "Move" << "\n";
    {
        VeryAnnoyingStructWithMove a{ 5, 5, 5 };
        VeryAnnoyingStructWithMove b{ 10, 10, 10 };
        std::cout << "A: " << a << "\t- B: " << b << "\n";
        swap(a, b);
        std::cout << "A: " << a << "\t- B: " << b << "\n";
    }
    std::cout << "\n";
    std::cout << "Fallback" << "\n";
    {
        DeletedTheRuleOfFive a{ 5, 5, 5 };
        DeletedTheRuleOfFive b{ 10, 10, 10 };
        std::cout << "A: " << a << "\t- B: " << b << "\n";
        swap(a, b);
        std::cout << "A: " << a << "\t- B: " << b << "\n";
    }
}
