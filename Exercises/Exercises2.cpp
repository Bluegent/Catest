#include <Catest.h>
#include <vector>
#include <iostream>
#include <iomanip>

//given an array of integers, calculate the sum of its elements
int32_t array_sum(const std::vector<int32_t> & arr)
{
    int32_t sum{};
    for (size_t i{}; i < arr.size(); ++i)
    {
        sum += arr[i];
    }
    return sum;
}


//given a matrix (an array of arrays) of integers, calculate the sum of its elements
int32_t matrix_sum(const std::vector<std::vector<int32_t>> & arr)
{
    int32_t sum{};
    for (size_t i{}; i < arr.size(); ++i)
    {
        //arr[0] = { 5,15} -> arr[0][0] =  5, arr[0][1] = 15
        //arr[1] = {10,30} -> arr[1][0] = 10, arr[1][1] = 30
        //sum += array_sum(arr[i]);

        for (size_t j{}; j < arr[i].size(); ++j)
        {
            sum += arr[i][j];
        }
    }
    return sum;
}

// given an array of integers, print it to the console
void print_array(const std::vector<int32_t> & arr)
{
    for (size_t i{}; i < arr.size(); ++i)
    {
        std::cout << arr[i] << ' ';
    }
    std::cout << '\n';
}


// given an array of arrays of integers, print it to the console (without using the print_array function)
void print_matrix(const std::vector<std::vector<int32_t>>& matrix)
{
    for (size_t i{}; i < matrix.size(); ++i)
    {
        for (size_t j{}; j < matrix[i].size(); ++j)
        {
            std::cout <<std::setw(2) << matrix[i][j] << ' ';
        }
        std::cout << '\n';
    }
}

// given two matrixes of integer of the same size ( width x height)
// calculate their sum as a matrix 
// (e.g. if input are two matrixes of 3 lines and 3 columns, the output is a 3 line and 3 column matrix)
// (the element on i,j of the output matrix will be the sum of the element on i,j from the first matrix and the one on i,j from the second matrix
/*
+--+--+--+--+
|  | 0| 1| 2|  -> column
+--+--+--+--+
| 0| 1| 2| 3|   1 2 3      2  4  6
+--+--+--+--+
| 1| 4| 5| 6| + 4 5 6  =   8 10 12
+--+--+--+--+
| 2| 7| 8| 9|   7 8 9     14 16 18
+--+--+--+--+
 |
 v
row
*/

using integer_value  = int32_t;
using integer_vector = std::vector<int32_t>;
using integer_matrix = std::vector<integer_vector>;

integer_matrix sum_of_matrixes(const integer_matrix & matrix1, const integer_matrix& matrix2)
{
    integer_matrix result(matrix1.size());

    for (size_t i{}; i < matrix1.size(); ++i)
    {
        result[i].resize(matrix1[i].size());

        for (size_t j{}; j < matrix1[i].size(); ++j)
        {
            result[i][j] = matrix1[i][j] + matrix2[i][j];
        }
    }
    return result;
}



DISABLE_SUITE(exercises2);

CATEST_F(exercises2, printing_vectors)
{
    print_array(std::vector<int32_t>{5,10,15,20});
    std::cout << '\n';
    std::vector<std::vector<int32_t>> matrix{
        {0,2,4,6},
        {0,3,9,12},
        {0,5,15,20}
    };
    print_matrix(matrix);

}

CATEST_F(exercises2, sum_of_matrixes_test)
{
    integer_matrix matrix1 = {
        {1,2,3},
        {4,5,6},
        {7,8,9}     
    };

    integer_matrix expected = {
        {2,4,6},
        {8,10,12},
        {14,16,18}
    };

    EXPECT_EQ(sum_of_matrixes(matrix1, matrix1), expected);
}

CATEST_F(exercises2,sum_of_vector)
{
   
    EXPECT_EQ(array_sum(std::vector<int32_t>{5,10,3,2}),20);
}

CATEST_F(exercises2, sum_of_matrix)
{

    std::vector<int32_t> line1{5,15};
    std::vector<int32_t> line2{10,30};
    std::vector<std::vector<int32_t>> matrix{ line1,line2 };

    //  5 15
    // 10 30

    EXPECT_EQ(matrix_sum(matrix), 60);
}