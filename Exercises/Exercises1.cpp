#include <Catest.h>
#include <stdint.h>
#include <algorithm>

int32_t max(const std::vector<int32_t>& arr)
{
    // given an array arr
    // return the maximum value from the array
    int32_t max = arr[0]; // we assume that the first element is the biggest
    //process
    for (size_t i = 1; i < arr.size(); ++i)
    {
        if (max < arr[i])
        {
            max = arr[i];
        }
    }
    return max;
}

int32_t max_position(const std::vector<int32_t>& arr)
{
    //given an array arr
    //return the position of the biggest element
    int32_t max = arr[0]; // we assume that the first element is the biggest
    size_t maxPos = 0;
    //process
    for (size_t i = 1; i < arr.size(); ++i)
    {
        if (max < arr[i])
        {
            max = arr[i];
            maxPos = i;
        }
    }
    return maxPos;
}

//given an array arr of int32_t
//return the minimum value from the array

int32_t min(const std::vector<int32_t> & arr)
{
    int32_t min = arr[0];
    for (size_t i = 1; i < arr.size(); ++i)
    {
        if (min > arr[i])
        {
            min = arr[i];
        }
    }
    return min;
}

//given an array arr of int32_t
//return the second biggest value from the array
// if the array only contains one element, return it
int32_t second_b(const std::vector<int32_t>& arr)
{
    if (arr.size() == 1)
        return arr[0];
    //DRY - don't repeat yourself
    int32_t max_v = max(arr);
    int32_t second_b = std::numeric_limits<int32_t>::min();
    
    for (size_t i = 0; i < arr.size(); ++i)
    {
        if ( arr[i] > second_b && arr[i] < max_v)
        {
            second_b = arr[i];
        }
    }
    return second_b;

}

//given an array of int32_t arr
//using the max() function, sort a copy of the array and return it
std::vector<int32_t> sort_with_max(const std::vector<int32_t>& arr)
{
    // while the sorted array's size is smaller than the source array
    //  get the max value from the source array
    //  store that value on the last position of the sorted array
    //  remove the max value from the source array

    std::vector<int32_t> source = arr;
    std::vector<int32_t> sorted;

    while (sorted.size() < arr.size())
    {
        int32_t max_v = max(source);
        sorted.insert(sorted.begin(), max_v);
        source.erase(std::find(source.begin(), source.end(), max_v));
    }
    return sorted;
}




CATEST_F(test_suite, test_2)
{

    std::vector<int32_t> vec1 = {10};
    EXPECT_EQ(max(vec1),10); 
    EXPECT_EQ(max_position(vec1), 0);
    EXPECT_EQ(min(vec1),10);
    EXPECT_EQ(second_b(vec1), 10);

    std::vector<int32_t> vec2 = { 1,3,6,9,10 };
    EXPECT_EQ(max(vec2),10);
    EXPECT_EQ(max_position(vec2), 4);
    EXPECT_EQ(min(vec2), 1);
    EXPECT_EQ(second_b(vec2), 9);

    std::vector<int32_t> vec3 = { 13,3,6,9,10 };
    EXPECT_EQ(max(vec3), 13);
    EXPECT_EQ(max_position(vec3), 0);
    EXPECT_EQ(min(vec3), 3);
    EXPECT_EQ(second_b(vec3), 10);


    std::vector<int32_t> input1 = { 3,2,1 };
    std::vector<int32_t> sorted1 = { 1,2,3 };

    std::vector<int32_t> input2 = {15,20,13,1 };
    std::vector<int32_t> sorted2 = { 1,13,15,20 };

    EXPECT_EQ(sorted1, sort_with_max(input1));
    EXPECT_EQ(sorted2, sort_with_max(input2));
}

DISABLE_SUITE(test_suite);

CATEST_F(test_suite, test_1)
{
    // stack
    // - small chunk of RAM memory (~1MB)
    // - allocated for the process used to define all local variables and functions
    // - (theoretically) continuous memory
    // - IMPORTANT: function calls go into the stack and given its small size, you should avoid allocating anything big on it
    // - only use it for small variable types like int32_t or simple classes
    // heap 
    // - large chunk of RAM memory (~4GB on 32 bit systems, essentially infinte on 64 bit system but the OS decides how much heap a process is allowed)
    // - used for dynamic allocations, dll loading etc.
    // - fragmented memory, only guarantees that objects allocated are continuous but two different objects might be in two very different locations of the RAM
    // - use it for declaring big objects or arrays


    const size_t arr_size = 10;

    //static length C-style array
    int32_t arr1[arr_size] = { 0 }; //-> only works if arr_size is const or constexpr (because this array is allocated on the stack)
    // pros: 
    // - simplest to define
    // - does not require freeing memory because it is allocated on the stack
    // cons:
    // - cannot be dynamically sized, its size must be const/constexpr
    // - when passing to a function, we lose size information (because outside of the local scope, it is treated as a pointer)
    // - does not carry size information with itself
    
    //dynamic length C-style array
    int32_t* arr2 = (int32_t*)malloc(arr_size * sizeof(int32_t));
    free(arr2);
    // pros: 
    // - dynamic size at initialization
    // - allocated on the heap so it can be much bigger than an array allocated on the stack
    // cons:
    // - allocates memory on the heap so once we are done with it, we must use free()
    // - requires understadning of pointers
    // - does not carry size information
    // - since it is stored as a pointer, it is not clear when you pass it to a function if this is a pointer to an int or an array

    /* autopsy:
        int32_t *        arr2    =             (int32_t*)      malloc            (arr_size * sizeof(int32_t));
        ^type   ^pointer ^name   ^ assignment   ^C-style cast  ^allocates memory  ^ how much memory we want in bytes


        int32_t * arr2              -> we declare a pointer to int32_t called arr2

        arr_size * sizeof(int32_t)  -> malloc's paramter is in bytes so we have to calculate how many bytes we need
                                    -> arr_size gives us the number of integers in the array
                                    -> we multiply that with the size of an integer
                                    -> to obtain the total byte count of the array

        malloc(arr_size * sizeof(int32_t))  -> malloc is a function that allocates memory
                                            -> malloc's input paramter is the number of bytes to allocate
                                            -> malloc will return a pointer to the alocated memory
                                            -> malloc's return type is void* (a void pointer, aka a pointer with no type size info)

        (int32_t*) (malloc(...))            -> because malloc returns void* we need to convert the address to int32_t*
                                            -> we do that by using a C-style cast:
                                            -> (<type_to_convert_to>)(<expression>)
                                            -> in this case we convert from void* to int32_t*

        free(arr2);     -> once we are done with arr2
                        -> we have to mark the memory that was allocated for it as free
    */

    //dynamic length C++ array
    int32_t* arr3 = new int32_t[arr_size];
    delete[] arr3;
    // pros: 
    // - dynamic size at initialization
    // - allocated on the heap so it can be much bigger than an array allocated on the stack
    // cons:
    // - allocates memory on the heap so once we are done with it, we must use free()
    // - requires understanding of pointers
    // - does not carry size information
    // - since it is stored as a pointer, it is not clear when you pass it to a function if this is a pointer to an int or an array

    //modern c++ vector
    std::vector<int32_t> vector(arr_size);
    // pros: 
    // - dynamic size at any moment ( can grow and shrink and does so in a smart way )
    // - allocated on the heap so it can be much bigger than an array allocated on the stack
    // - despite being allocated on the heap, the memory management is hidden from the user so when the variable goes out of scope, the memory is 
    //      automatically freed
    // - allows removal and insertion to and from any point in the vector
    // - carries size information as well as other useful functions
    // - when passed to a function it shows the clear intent to have an array of elements
    // - provides [] operator so it can be used essentially as any other array
    // cons:
    // - slightly slower as it is a class
    // - requires some understanding of generic/template programming
    // - if passed by value causes a deep copy of the vector which might be memory intensive and slow => pass by reference unless you explicitly want a copy

}