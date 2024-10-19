#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <fstream>
#include <conio.h>


bool isSeparator(const char c, const std::string& separators)
{
    return separators.find(c) != std::string::npos;
}


std::vector<std::string> split(const std::string& line, const std::string& separators = " .,!?:")
{
    std::vector<std::string> result;
    const char* str = line.data();
    std::string word;
    do
    {
        const char* begin = str;

        while (!isSeparator(*str, separators) && 0 != *str)
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


using OccurenceDict = std::map<std::string, uint32_t>;
using FileLines = std::vector<std::string>;


OccurenceDict countOccurences(const FileLines& lines, const std::string & separators)
{
    OccurenceDict result;
    for (const auto& line : lines)
    {
        const auto words = split(line, separators);
        for (const auto& word : words)
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

FileLines readFile(const std::string& path)
{
    FileLines lines;
    std::ifstream file(path, std::ios_base::in);

    if (!file.is_open())
    {
        std::cout << "Could no open file in path \"" << path << "\"\n";
        return lines;
    }
    std::string line;
    while (!file.eof())
    {
        std::getline(file, line);
        if (!line.empty())
        {
            lines.push_back(line);
        }
    }
    file.close();

    return lines;
}


int main(int argc, const char* argv[])
{
    const char* usage = "Usage: Standalone.exe <file_name> \"<separators>\"(optional)\n";
    if (argc == 1)
    {
        std::cout << "No file given as an argument. " << usage;
        return 0;
    }

    if (argc != 3 && argc !=2)
    {
        std::cout << "Incorrect argument count. "<< usage;
        return 0;
    }

    std::string filePath = argv[1];
    std::string separators = " ,.!?";
    if (argc == 3)
    {
        separators = argv[2];
    }
    std::cout << "Using separators: \"" << separators << "\"\n";

    FileLines lines = readFile(filePath);
    OccurenceDict occurences = countOccurences(lines,separators);
    for (const auto& occurence : occurences)
    {
        std::cout << occurence.first << " - " << occurence.second << "\n";
    }
    _getch();

    return 0;
}