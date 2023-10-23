#include <catest/TestBase.h>
#include <iostream>
#include <chrono>
#include <catest/TestRunner.h>
#include <sstream>

namespace catest
{
    const char* separator = "=====================";
    const char* halfSeparator = "==============";
    static test_collection* tests = nullptr;

    TestBase::TestBase(const std::string& name, const std::string& suiteName, func function)
        : name{ name }
        , suiteName{ suiteName }
        , function{ function }
    {
        TestRunner::get_instance().pushTest(*this);
    }

    bool TestBase::run()
    {
        std::cout << "\nRunning test " << name << "\n";
        auto before = std::chrono::steady_clock::now();
        try
        {
            function(*this);
        }
        catch (const std::exception& exception)
        {
            std::stringstream ss;
            ss << "Unhandled exception: " << exception.what();
            addFailure(FailureReason(ss.str()));
        }
        catch (...)
        {
            addFailure(FailureReason("Unhandled unknown exception"));
        }
        auto after = std::chrono::steady_clock::now();

        const char* result = isFailed() ? "\033[31m[FAILED]\033[0m" : "\033[32m[PASSED]\033[0m";
        if (isFailed())
        {

            printFailure();
        }
        std::cout << "Duration: " << std::chrono::duration_cast<std::chrono::milliseconds>(after - before).count() << " ms\n";
        std::cout << halfSeparator << result << halfSeparator << "\n";

        return !isFailed();
    }

    void TestBase::addFailure(const FailureReason& reason)
    {
        reasons.push_back(reason);
    }

    bool TestBase::isFailed() const
    {
        return reasons.size() != 0;
    }

    const std::string& TestBase::getName() const
    {
        return name;
    }

    const std::string& TestBase::getSuite() const
    {
        return suiteName;
    }

    const void TestBase::printFailure()
    {
        for (const auto& reason : reasons)
        {
            std::cout << reason.get() << '\n';
        }
    }
    void pushTest(TestBase& test)
    {
        if (!tests)
        {
            tests = new test_collection();
        }

        if (tests->find(test.getSuite()) == tests->end())
        {
            tests->insert({ test.getSuite(), std::vector<TestBase>() });
        }
        (*tests)[test.getSuite()].push_back(test);
    }
}