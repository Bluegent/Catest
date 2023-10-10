#ifndef CATEST_BASE_H
#define CATEST_BASE_H
#include <map>
#include <string>
#include <vector>

namespace catest
{
    class TestBase;
    using func = void(*)(TestBase& base);

    using test_suite =  std::vector<TestBase>;

    using test_collection = std::map<std::string, test_suite>;

    std::string get_filename(const char* filename);
    std::string get_fail_reason(const char* filename, int32_t line, const std::string& reason, const  char* test_func);
}

#endif // !CATEST_BASE_H
