#include <Catest.h>
#include <vector>
#include <string>

template <typename T>
void swap(T& left, T& right)
{
    T swap = left;
    left = right;
    right = swap;
}

//write a function that takes a vector of strings and sorts it based on string length
// e.g. for input "aa","a","bbbbbb", "aa", "bbb" the sorted vector should be "a", "aa","aa","bbb", "bbbbbb"
void sort(std::vector<std::string>& vec)
{
    for (size_t i = 0; i < vec.size() - 1; ++i)
    {
        for (size_t j = i + 1; j < vec.size(); ++j)
        {
            if (vec[i].length() > vec[j].length())
            {
                swap(vec[i], vec[j]);
            }
        }
    }
}

// interchange sort
// for i = 0  to n - 2 
//     for j = i + 1 to n - 1 
//         if vec[i] > vec[j]
//            swap vec[i] and vec[j]



CATEST_F(sorting_vectors, string_vector)
{
    std::vector<std::string> input = { "aa","a","bbbbbb", "aa", "bbb" };
    std::vector<std::string> expected = { "a", "aa","aa","bbb", "bbbbbb" };
    sort(input);

    for (size_t i = 0; i < input.size(); ++i)
    {
        ASSERT_EQ(input[i], expected[i]);
    }
}


//write a function that takes a vector of vectors of int32_t and sorts it based on the first element of each sub-vector
// e.g. for input [ [13,1,5], [1,3], [-5,20,13]] the sorted vector should be [[-5,20,13], [1,3],[13,1,5]]
void sort(std::vector<std::vector<int32_t>>& vec)
{
    for (size_t i = 0; i < vec.size() - 1; ++i)
    {
        for (size_t j = i + 1; j < vec.size(); ++j)
        {
            if (vec[i][0] > vec[j][0])
            {
                swap(vec[i], vec[j]);
            }
        }
    }
}

CATEST_F(sorting_vectors, vector_of_vector_of_int32_t)
{
    std::vector<std::vector<int32_t>> input = { {13,1,5 }, { 1,3 }, { -5,20,13 } };
    std::vector<std::vector<int32_t>> expected = { {-5, 20, 13}, {1, 3}, {13, 1, 5} };
    sort(input);

    for (size_t i = 0; i < input.size(); ++i)
    {
        for (size_t j = 0; j < input[i].size(); ++j)
        {
            ASSERT_EQ(input[i][j], expected[i][j]);
        }
    }
}
// 1 5 12 6 13 10 99



int32_t max(std::vector<int32_t>& vec)
{
    int32_t max = vec[0];
    for (size_t i = 1; i < vec.size(); ++i)
    {
        if (vec[i] > max)
        {
            max = vec[i];
        }
    }
    return max;
    
}
DISABLE_SUITE(sorting_vectors);
CATEST_F(sorting_vectors, max_of_vector)
{
    std::vector<int32_t> input = { 13,99,20,-15,20 };
    ASSERT_EQ(max(input), 99);
}

//write a function that takes a vector of vectors of int32_t and sorts it based on the maximum value of each sub-vector
// e.g. for input [ [13,1,5], [1,3], [-5,20,13]] the sorted vector should be [[1,3],[13,1,5],[-5,20,13]]
void sort_on_max(std::vector<std::vector<int32_t>>& vec)
{
    for (size_t i = 0; i < vec.size() - 1; ++i)
    {
        for (size_t j = i + 1; j < vec.size(); ++j)
        {
            if (max(vec[i]) > max(vec[j]))
            {
                swap(vec[i], vec[j]);
            }
        }
    }
}

CATEST_F(sorting_vectors, vector_of_vector_of_int32_t_max)
{
    std::vector<std::vector<int32_t>> input = { {13,1,5 }, { 1,3 }, { -5,20,13 } };
    std::vector<std::vector<int32_t>> expected = { {1, 3}, {13, 1, 5},{-5, 20, 13} };
    sort_on_max(input);

    for (size_t i = 0; i < input.size(); ++i)
    {
        for (size_t j = 0; j < input[i].size(); ++j)
        {
            ASSERT_EQ(input[i][j], expected[i][j]);
        }
    }
}
