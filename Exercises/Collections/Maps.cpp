#include <Catest.h>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <fstream>

// Read a text from a file and display the word frequency.

/*
1. Open file
2. Read contents line by line and store in a std::vector<std::string>
* create a std::map<std::string,uint32_t> map to store occurences
3. For each string(line) in the vector:
    3.1 split into words => this results in a std::vector<std::string>
    3.2 For each string(word) in the split vector:
        3.2.1 If the word is not in our map yet, insert a pair into the map with the word as a key and the value 1
        3.2.2 Else, the value gets ++'d
4. Print the occurences
*/

bool isSeparator(const char c, const std::string& separators)
{
    return separators.find(c) != std::string::npos;
}


std::vector<std::string> split(const std::string& line,const std::string & separators = " .,!?:")
{
    std::vector<std::string> result;
    const char* str = line.data();
    std::string word;
    do
    {
        const char* begin = str;
        
        while (!isSeparator(*str,separators) && 0 != *str)
            str++;

        if (begin == str)
        {
            continue;
        }
        word = std::string(begin, str);
        if (word.size() != 0)
        {
            result.push_back(word);
        }
    } while (0 != *str++);

    return result;
}

CATEST_F(Word_Freq, Word_Freq_Split)
{
    const std::string line = "In modern C++,  you can  split a string into words !";
    const std::vector<std::string> expected = { "In","modern","C++","you","can","split","a","string","into","words"};
    const auto words = split(line," .,!?:");
    ASSERT_EQ(words.size(), expected.size());
    for (size_t i = 0; i < words.size(); ++i)
    {
        ASSERT_EQ(words[i], expected[i]);
    }
}


using OccurenceDict = std::map<std::string, uint32_t>;
using FileLines = std::vector<std::string>;


OccurenceDict countOccurences(const FileLines& lines)
{
    OccurenceDict result;
    for (const auto & line : lines)
    {
        const auto words = split(line);
        for (const auto & word : words)
        {
            if (result.find(word) != result.end())
            {
                result[word]++;
            }
            else
            {
                //result.emplace(word,1u);
                //result.insert(std::pair<std::string, uint32_t>(word,1u));
                result[word] = 1;
            }
        }
    }
    return result;
}

CATEST_F(Word_Freq, Word_Freq_Count)
{
    FileLines lines = { {"ala bala portocala ala"}};
    OccurenceDict count = countOccurences(lines);
    OccurenceDict expected;
    expected["ala"] = 2;
    expected["bala"] = 1;
    expected["portocala"] = 1;

    ASSERT_EQ(count, expected);
}


FileLines readFile(const std::string& path)
{
    FileLines lines;
    std::ifstream file(path,std::ios_base::in);

    if (!file.is_open())
    {
        std::cout << "Could no open file in path \""<< path<< "\"\n";
        return lines;
    }
    std::string line;
    while (!file.eof())
    {
        std::getline(file,line);
        if (!line.empty())
        {
            lines.push_back(line);
        }
    }
    file.close();

    return lines;
}


CATEST_F(Word_Freq, Word_Freq_Readfile)
{
    FileLines expected = { {"test1","test2","test3"}};
    FileLines read = readFile("test.txt");
    ASSERT_EQ(expected, read);
}


CATEST_F(Word_Freq, Read_File_And_Count)
{
    FileLines lines = readFile("lorem.txt");
    OccurenceDict occurences = countOccurences(lines);
    for (const auto& occurence : occurences)
    {
        std::cout << occurence.first << " - " << occurence.second << "\n";
    }
}

DISABLE_SUITE(Word_Freq);