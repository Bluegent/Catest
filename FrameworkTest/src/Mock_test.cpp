#include <Camock.h>
#include <Catest.h>
#include <tuple>
#include <optional>
#include <vector>

namespace catest
{

    template <typename ReturnType>
    class ReturnValue
    {
    private:
        int32_t count;
        ReturnType value;
        bool isInfinite;
        bool isBlank;
    public:
        ReturnValue(const ReturnType& value, const int32_t count) : count{ count }, value{ value }, isInfinite{ false }, isBlank(false){}
        ReturnValue(const ReturnType& value) : count{ 0 }, value{ value }, isInfinite{ true }, isBlank(false){}
        ReturnValue() : value{}, count{}, isInfinite{ false }, isBlank(true) {};
        int32_t getCount() const
        {
            return count;
        }
        std::optional<ReturnType> get()
        {
            if (isBlank || (count == 0 && !isInfinite))
            {
                return std::optional<ReturnType>();
            }
            if (!isInfinite)
            {
                --count;
            }
            return value;
        }
    };

    template <typename... Args>
    class ExpectParamPack
    {
    private:
        std::tuple<Args...> params;
    public:
        ExpectParamPack(Args... args) : params(std::make_tuple(args...)) {};
        bool equals(std::tuple<Args...> args) const
        {
            return args == params;
        }
    };

    template <typename ReturnType, typename... Args>
    class Expectation
    {
    private:
        ExpectParamPack<Args...> params;
        ReturnValue<ReturnType> returnValue;
        int32_t callCount;
    public:
        Expectation(Args... args) : params{ args... }, returnValue{}, callCount{0}{};
        friend bool operator == (const Expectation& lhs, const Expectation& rhs);
        bool equals(std::tuple<Args...> args) const
        {
            return params.equals(args);
        }
        void addReturnValue(ReturnType&& value)
        {
            returnValue = ReturnValue(value);
        }
        void addReturnValue(const ReturnType& value, const int32_t count)
        {
            returnValue = ReturnValue(value,count);
        }
        ReturnValue<ReturnType>& call()
        {
            ++callCount;
            return returnValue;
        }

    };

    template <typename ReturnType, typename... Args>
    bool operator==(const Expectation<ReturnType, Args...>& lhs, const Expectation<ReturnType, Args...>& rhs)
    {
        return lhs.params == rhs.params;
    }


    template <typename ReturnType, typename... Args>
    class MockFunction
    {
    public:
        MockFunction(void* ptr) : funcPtr{ ptr } {}
        void* getPtr()
        {
            return ptr;
        }
        Expectation<ReturnType, Args...>& addExpectation(Args... expectedArgs)
        {
            expectations.emplace_back(Expectation<ReturnType, Args...>(expectedArgs...));
            return expectations.back();
        }

        std::optional<Expectation<ReturnType, Args...>*> getExpectation(Args... expectedArgs)
        {
            auto params = std::make_tuple(expectedArgs...);
            auto& it = std::find_if(expectations.begin(), expectations.end(), [&params](const Expectation<ReturnType, Args...>& elem)
                {
                    return elem.equals(params);
                }
            );
            if (it == expectations.end())
            {
                return {};
            }
            return (&(*it));
        }
        ReturnType notFound()
        {
            // fail test? 
            // throw exception?
            return ReturnType{};
        }

        ReturnType call(Args... args)
        {
            auto match = getExpectation(args...);
            if (!match.has_value())
            {
                return notFound();
            }
            auto retVal = match.value()->call().get();
            if (!retVal.has_value())
            {
                return notFound();
            }
            return retVal.value();
        }

    private:
        std::vector<Expectation<ReturnType, Args...>> expectations;
        void* funcPtr;
    };


}
/*

CATEST_F(mock_test, mock_params_test)
{
    catest::MockFunction<int32_t, int32_t, int32_t> sumFunc(reinterpret_cast<void*>(42));
    sumFunc.addExpectation(3, 5).addReturnValue(13);
    EXPECT_TRUE(sumFunc.getExpectation(3, 5).has_value());
    EXPECT_FALSE(sumFunc.getExpectation(5, 3).has_value());
    EXPECT_EQ(sumFunc.call(3, 5), 13);
}
*/

CATEST_F(mock_test, mock_params_non_set_value)
{
    catest::MockFunction<int32_t, int32_t, int32_t> sumFunc(reinterpret_cast<void*>(42));
    EXPECT_EQ(sumFunc.call(3, 5), 0);
    sumFunc.addExpectation(3, 5);
    EXPECT_EQ(sumFunc.call(3, 5), 0);
}


CATEST_F(mock_test, mock_params_noninfinite)
{
    catest::MockFunction<int32_t, int32_t, int32_t> sumFunc(reinterpret_cast<void*>(42));
    sumFunc.addExpectation(3, 5).addReturnValue(13, 2);
    EXPECT_EQ(sumFunc.call(3, 5), 13);
    EXPECT_EQ(sumFunc.call(3, 5), 13);
    EXPECT_EQ(sumFunc.call(3, 5), 0);
}
