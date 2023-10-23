#include <Catest.h>
#include <fstream>
#include <iostream>

// implement a function that simulates a level B safety structure
// a level A safety structure has one sensor and another sensor for redundancy
// if the data from the two sensors is different, the level A shall report an error
// a level B safety structure has two level As as sensors
// the level B shall report its sensor data as the majority value given from its 4 sensors
// in our case, the sensors return true/false
bool safetyB(bool sensor1, bool sensor2, bool sensor3, bool sensor4, bool& error)
{
    error = false; // we are optimistic
    // good case: all four the same
    if (sensor1 == sensor2 && sensor2 == sensor3 && sensor3 == sensor4)
    {
        return sensor1;
    }

    //good case: 3 are equal and 1 is different - 4 possibilities
    // possibility1 : sensor4 is different    
    // possibility2 : sensor3 is different
    // possibility3 : sensor2 is different
    if (
        (sensor1 == sensor2 && sensor2 == sensor3 && sensor3 != sensor4) ||
        (sensor1 == sensor2 && sensor2 == sensor4 && sensor3 != sensor4) ||
        (sensor1 == sensor3 && sensor3 == sensor4 && sensor2 != sensor4)
        )
    {
        return sensor1;
    }
    // possibility4 : sensor1 is different
    if (sensor4 == sensor2 && sensor2 == sensor3 && sensor3 != sensor1)
    {
        return sensor4;
    }

    //bad case : 2 are equal and 2 are different - 2 possibilities

    error = true;
    return sensor1;
}


int func()
{
    static int counter = 0;
    static std::string str; //""
    ++counter;
    return counter;
}

int func(int& value)
{
    ++value;
    return value;
}


CATEST_F(exercises4, static_vs_ref)
{
    func();
    func();
    int staticInt = func(); //returns 3
    int myInt = 3;
    int myInt2 = func(myInt); // will return 4 and also make myInt 4

    int myInt3 = 20;
    int myInt4 = func(myInt3); //will return 21 and make myInt3 21
}


CATEST_F(exercises4, test_safetyb)
{
    bool error;
    EXPECT_TRUE(safetyB(true, true, true, true, error));

    EXPECT_TRUE(safetyB(true, true, true, false, error));
    EXPECT_TRUE(safetyB(true, true, false, true, error));
    EXPECT_TRUE(safetyB(true, false, true, true, error));
    EXPECT_TRUE(safetyB(false, true, true, true, error));

    safetyB(true, true, false, false, error);
    EXPECT_TRUE(error);
    safetyB(false, false, true, true, error);
    EXPECT_TRUE(error);
    safetyB(false, true, true, false, error);
    EXPECT_TRUE(error);
    safetyB(true, false, false, true, error);
    EXPECT_TRUE(error);

    EXPECT_FALSE(safetyB(false, false, false, false, error));

    EXPECT_FALSE(safetyB(false, false, false, true, error));
    EXPECT_FALSE(safetyB(true, false, false, false, error));
    EXPECT_FALSE(safetyB(false, true, false, false, error));
    EXPECT_FALSE(safetyB(false, false, true, false, error));

}



// write a function that takes a file name as input (string)
// the function shall open the file of the given file name, read the first line and return it (as a string)
std::string file_func(const std::string& fileName)
{
    std::string line;
    std::ifstream file;

    file.open(fileName); // attempts to open the file with the name given by the parameter

    if (!file.is_open()) // we need to check if the opening was successful 
    {
        return line;
    }

    std::getline(file, line);
    file.close();

    return line;
}


CATEST_F(exercises4, test_file_func)
{
    EXPECT_EQ(std::string("banane"), file_func("banane.txt"));
}



// write a function that takes a file name as input (string)
// the function shall open the file of the given file name, read it line by line and return its contents as  vector of strings'
std::vector<std::string> get_file_contents(const std::string& fileName)
{
    std::vector<std::string> contents;
    std::ifstream file;

    file.open(fileName); // attempts to open the file with the name given by the parameter

    if (!file.is_open()) // we need to check if the opening was successful 
    {
        // if the file is not open, it might be because we don't have access or the file does not exist
        return contents;
    }
    std::string line;
    while (!file.eof())
    {
        std::getline(file, line);
        contents.push_back(line);
    }


    file.close();
    return contents;
}

CATEST_F(exercises4, test_get_file_contents)
{
    EXPECT_EQ(std::vector<std::string>({ "banane","pere","mere" }), get_file_contents("banane.txt"));
}


// write a function that takes two file names (strings)
// the function shall open the first file and read two integer values from it then close it
// the function shall then open the second file and write the sum of the integers into it, then close it
// the function shall also return the sum of the two integers (for testing purpose)
int32_t file_addition(const std::string& inputFileName, const std::string& outputFileName)
{
    std::ifstream inputFile;

    inputFile.open(inputFileName);

    if (!inputFile.is_open())
    {
        return 0;
    }

    int32_t num1;
    int32_t num2;
    inputFile >> num1;
    inputFile >> num2;
    inputFile.close();

    std::ofstream outputFile;

    outputFile.open(outputFileName);

    if (!outputFile.is_open())
    {
        return 0;
    }

    int32_t sum = num1 + num2;

    outputFile << sum;
    outputFile.flush();
    outputFile.close();
    return sum;
}


CATEST_F(exercises4, test_file_sum)
{
    EXPECT_EQ(30, file_addition("input.txt", "output.txt"));
}


// write a function that takes a file name (string)
// the first line of the file contains the number of integer numbers that follow (n)
// the following n lines contain integers
// the function shall open the file, read n, read each integer from the file and store it into a vector and return the vector
std::vector<int32_t> readVector(const std::string& fileName)
{
    std::ifstream file;

    file.open(fileName);
    if (!file.is_open())
    {
        return {};
    }

    std::vector<int32_t> vec;
    size_t n{};
    file >> n;
    vec.reserve(n);

    for (size_t i = 0; i < n; ++i)
    {
        int32_t number;
        file >> number;
        vec.push_back(number);
    }
    return vec;

}


CATEST_F(exercises4, test_read_vector)
{
    std::vector<int32_t> vec = readVector("vector.txt");
    EXPECT_EQ(vec.size(), 10);
    EXPECT_EQ(vec, std::vector<int32_t>({ 123, 500, 12, 20, 1, 1, 1,-1,1,1 }));
}


// write a function that takes a vector of float and returns its average
float average(const std::vector<float>& vec)
{
    float sum = 0.f;
    for (size_t i = 0; i < vec.size(); ++i)
    {
        sum += vec[i];
    }
    return sum / static_cast<float>(vec.size());
}



CATEST_F(exercises4, test_avg)
{
    EXPECT_EQ(10.f, average(std::vector<float>({ 20.f,0.f })));
}


// write a function that takes two vectors
// the first is a vector of floats
// the second is a vector of percentages
// the function shall check that all the numbers in the percentage vector add up to 1
// then the function shall multiply each number with its corresponding percentage and return the percentage based average (which is a sum of those multiplications)
// if the percentages do not add up to 1, then the function shall return 0
// if the vectors are not of the same size, return 0
float percentageAverage(const std::vector<float>& vec, const std::vector<float>& percentages)
{
    if (vec.size() != percentages.size())
    {
        return 0.f;
    }

    float sum = 0.f;
    float percentageSum = 0.f;
    for (size_t i = 0; i < percentages.size(); ++i)
    {
        sum += percentages[i];
        percentageSum += percentages[i] * vec[i];

    }
    if (sum != 1.f)
    {
        return 0.f;
    }

    return percentageSum;
}

CATEST_F(exercises4, test_percentage_average)
{

    std::vector<float> values = { 10.f,5.f,5.f };
    std::vector<float> percentages = { 0.5f,0.25f,0.25f };
    std::vector<float> badPercentages = { 100.f,0.3f,0.3f };
    EXPECT_EQ(7.5f, percentageAverage(values, percentages));
    EXPECT_EQ(0.f, percentageAverage(values, badPercentages));
}

DISABLE_SUITE(exercises4);