#include <catest/TestRunner.h>
#include <iostream>
#include <catest/TestBase.h>

namespace catest
{
    
    TestRunner* TestRunner::instance = nullptr;
    TestRunner::TestRunner()
    {
    }

    void TestRunner::run()
    {
        const char* separator = "=====================";
        const char* halfSeparator = "==============";
        size_t failedSuites = 0; //we are very optimistic :)
        size_t passedSuites = 0;
        for (auto& suite : tests)
        {
            if (std::find(disabled.begin(), disabled.end(), suite.first) != disabled.end())
            {
                std::cout << "Skipping suite " << suite.first << '\n';
                continue;
            }
            std::cout << separator << separator << '\n';
            std::cout << "Runnning suite " << suite.first << "\n";
            size_t failed = 0; //we are very optimistic :)
            for (auto& test : suite.second)
            {

                if (!test.run())
                {
                    ++failed;
                }
            }
            std::cout << '\n';
            size_t passed = suite.second.size() - failed;

            std::cout << "Suite " << suite.first << " results: " << (failed ? "\033[31m[FAILED]\033[0m" : "\033[32m[PASSED]\033[0m") << '\n';
            std::cout << "Test Passed: " << passed << '/' << suite.second.size() << '\n';
            if (failed)
            {
                ++failedSuites;
                std::cout << "Test Failed: " << failed << '/' << suite.second.size() << '\n';
            }
            else
            {
                ++passedSuites;
            }
            std::cout << '\n';
        }

        std::cout << separator << separator << '\n';
        std::cout << "Total suites:\t\t\t" << tests.size() << '\n';
        std::cout << "Passed suites:\t\t\t" << passedSuites << '/' << tests.size() << '\n';
        std::cout << "Failed suites:\t\t\t" << failedSuites << '/' << tests.size() << '\n';
        std::cout << "Disabled suites:\t\t" << disabled.size() << '\n';
    }
    void TestRunner::disable_suite(const std::string& name)
    {
        disabled.push_back(name);
    }

    TestRunner& TestRunner::get_instance()
    {
        if (!instance)
        {
            instance = new TestRunner();
        }

        return *instance;
    }

    void TestRunner::run_tests()
    {
        get_instance().run();
    }

    void TestRunner::pushTest(TestBase& test)
    {
        if (tests.find(test.getSuite()) == tests.end())
        {
            tests.insert({ test.getSuite(), std::vector<TestBase>() });
        }
        tests[test.getSuite()].push_back(test);
    }
}