#include <Catest.h>
#include <vector>
#include <string>
#include <algorithm>

enum class TokenType
{
    Day,
    Dash,
    TimeRange,
    Invalid
};

enum class Day
{
    Mon,
    Tue,
    Wed,
    Thu,
    Fri,
    Sun,
    Sat,
    Invalid
};

using DayRange = std::vector<Day>;

DayRange makeDayRange(const Day start, const Day end)
{
    std::vector<Day> range;
    if (start > end || start == Day::Invalid || end == Day::Invalid)
    {
        return range;
    }
    for (Day d = start; d <= end; d = static_cast<Day>(static_cast<int>(d) + 1))
    {
        range.push_back(d);
    }
    return range;
}

CATEST_F(NOTAM_Test, makeDayRange_Test)
{
    const DayRange range = makeDayRange(Day::Mon, Day::Fri);
    const DayRange expectedRange = { Day::Mon,Day::Tue,Day::Wed,Day::Thu,Day::Fri };
    EXPECT_EQ(range, expectedRange);
}


const std::vector<std::string> days = { "MON","TUE","WED","THU","FRI","SUN","SAT" };
Day getDay(const std::string& token)
{
    auto it = std::find(days.begin(), days.end(), token);

    if (it != days.end())
    {
        return static_cast<Day>(std::distance(days.begin(), it));
    }
    else
    {
        return Day::Invalid;
    }
}





CATEST_F(NOTAM_Test, GetDay_Test)
{
    EXPECT_EQ(getDay("MON"), Day::Mon);
    EXPECT_EQ(getDay("TUE"), Day::Tue);
    EXPECT_EQ(getDay("WED"), Day::Wed);
    EXPECT_EQ(getDay("THU"), Day::Thu);
    EXPECT_EQ(getDay("FRI"), Day::Fri);
    EXPECT_EQ(getDay("SUN"), Day::Sun);
    EXPECT_EQ(getDay("SAT"), Day::Sat);
    EXPECT_EQ(getDay("MOM"), Day::Invalid);
}



bool isTimeRange(const std::string& token)
{
    if (token.size() != 9)
    {
        return false;
    }
    if (token.find("-") != 4)
    {
        return false;
    }
    for (const auto digit : token)
    {
        if (digit == '-')
            continue;
        if (digit < '0' || digit > '9')
            return false;
    }
    return true;
}

TokenType getType(const std::string& token)
{
    if (std::find(days.begin(), days.end(), token) != days.end())
    {
        return TokenType::Day;
    }

    if (token == "-")
    {
        return TokenType::Dash;
    }
    if (isTimeRange(token))
    {
        return TokenType::TimeRange;
    }
    if (token == "CLSD")
    {
        return TokenType::TimeRange;
    }

    return TokenType::Invalid;
}


CATEST_F(NOTAM_Test, TEST_TimeRange)
{
    const std::string validRange = "0050-1500";
    const std::string shortRange = "0050-150";
    const std::string noDashRange = "005011150";
    const std::string letterRange = "0050-a150";
    EXPECT_TRUE(isTimeRange(validRange));
    EXPECT_FALSE(isTimeRange(shortRange));
    EXPECT_FALSE(isTimeRange(noDashRange));
    EXPECT_FALSE(isTimeRange(letterRange));
}

CATEST_F(NOTAM_Test, TEST_GetTokenType)
{
    const std::string range = "0050-1500";
    const std::string day = "WED";
    const std::string dash = "-";
    const std::string invalid = "no";
    const std::string closed = "CLSD";

    EXPECT_EQ(getType(range), TokenType::TimeRange);
    EXPECT_EQ(getType(day), TokenType::Day);
    EXPECT_EQ(getType(dash), TokenType::Dash);
    EXPECT_EQ(getType(invalid), TokenType::Invalid);
}



struct TimeGroup
{
    std::vector<Day> days;
    std::vector<std::string> ranges;
};

using TimeGroups = std::vector<TimeGroup>;

// const std::vector<std::string> tokens = { "MON","WED","0500-1300","FRI","-","SUN","0600-1300"};
TimeGroups getTimeGroups(const std::vector<std::string>& tokens)
{
    TokenType lastTokenType = TokenType::Invalid;
    TokenType currentTokenType = TokenType::Invalid;
    size_t tokenIndex = 0;
    TimeGroups groups;

    // get the first token's type
    lastTokenType = getType(tokens[tokenIndex++]);

    if (lastTokenType != TokenType::Day)
    {
        //malformed token list, day should always be first
        return TimeGroups{};
    }
    std::vector<Day> days;
    days.push_back(getDay(tokens[0]));

    while (tokenIndex < tokens.size())
    {
        currentTokenType = getType(tokens[tokenIndex]);
        if (lastTokenType == TokenType::Day)
        {
            //handle day range
            //e.g. MON-WED 0500-1300
            if (currentTokenType == TokenType::Dash)
            {
                ++tokenIndex;
                if (tokenIndex > tokens.size())
                {
                    // malformed token list, <day> dash should never be the end of the list
                    return TimeGroups{};
                }
                currentTokenType = getType(tokens[tokenIndex]);
                if (currentTokenType != TokenType::Day)
                {
                    // malformed token list, <day> dash should always be followed by <day>
                    return TimeGroups{};
                }
                days.push_back(getDay(tokens[tokenIndex]));
                ++tokenIndex;
                if (tokenIndex > tokens.size())
                {
                    // malformed token list, <day> dash <day> should never be the end of the list
                    return TimeGroups{};
                }
                currentTokenType = getType(tokens[tokenIndex]);
                if (currentTokenType != TokenType::TimeRange)
                {
                    // malformed token list, <day> dash <day> should always be followed by <timerange>
                    return TimeGroups{};
                }
                TimeGroup group;
                group.days = makeDayRange(days[0], days[1]);
                group.ranges.push_back(tokens[tokenIndex]);
                groups.push_back(group);
                // finished processing a <day> dash <day> <timerange> sequence, reset everything
                days.clear();
                lastTokenType = TokenType::Invalid;
                ++tokenIndex;
                continue;
            }
            //handle simple range
            //e.g. MON 0500-1300
            else if (currentTokenType == TokenType::TimeRange)
            {
                TimeGroup group;
                group.days.push_back(days[0]);
                group.ranges.push_back(tokens[tokenIndex]);
                groups.push_back(group);
                // finished processing a <day> <timerange> sequence, reset everything
                days.clear();
                lastTokenType = TokenType::Invalid;
                ++tokenIndex;
                continue;
            }
            //handle day list 
            //e.g. THU WED FRI 0500-1300
            days.push_back(getDay(tokens[tokenIndex]));
            do
            {
                ++tokenIndex;
                if (tokenIndex > tokens.size())
                {
                    // malformed token list, <day> ... <day> should never be the end of the list, this loop will exit when a proper sequence is found
                    return TimeGroups{};
                }
                currentTokenType = getType(tokens[tokenIndex]);
                if (currentTokenType == TokenType::Day)
                {
                    days.push_back(getDay(tokens[tokenIndex]));
                }
                if (currentTokenType == TokenType::TimeRange)
                {
                    TimeGroup group;
                    group.days = days;
                    group.ranges.push_back(tokens[tokenIndex]);
                    groups.push_back(group);
                    // finished processing a <day> ...<day> <timerange> sequence, reset everything
                    days.clear();
                    lastTokenType = TokenType::Invalid;
                    break;
                }
                if (currentTokenType == TokenType::Invalid)
                {
                    //malformed token list, Invalid token present
                    return TimeGroups{};
                }
            } while (true);
            if (lastTokenType == TokenType::Invalid)
            {
                tokenIndex++;
                continue;
            }

        }
        if (lastTokenType == TokenType::Invalid)
        {
            if (currentTokenType != TokenType::Day)
            {
                //malformed token list, oprhaned token different than <day> found at start of sequence
                return TimeGroups{};
            }
            else
            {
                days.push_back(getDay(tokens[tokenIndex]));
            }
        }
        lastTokenType = currentTokenType;
        ++tokenIndex;
    }
    return groups;
}

CATEST_F(NOTAM_Test, GetTimeGroups_Simple_TEST)
{
    const std::vector<std::string> tokens = { "MON","0500-1300" };
    TimeGroups result = getTimeGroups(tokens);
    TimeGroup simple;
    simple.days.push_back(Day::Mon);
    simple.ranges.push_back("0500-1300");
    EXPECT_EQ(simple.days, result[0].days);
    EXPECT_EQ(simple.ranges, result[0].ranges);
}


CATEST_F(NOTAM_Test, GetTimeGroups_Range_TEST)
{
    const std::vector<std::string> tokens = { "MON","-","WED","0500-1300" };
    TimeGroups result = getTimeGroups(tokens);
    TimeGroup simple;
    simple.days = makeDayRange(Day::Mon, Day::Wed);
    simple.ranges.push_back("0500-1300");
    EXPECT_EQ(simple.days, result[0].days);
    EXPECT_EQ(simple.ranges, result[0].ranges);
}


CATEST_F(NOTAM_Test, GetTimeGroups_List_TEST)
{
    const std::vector<std::string> tokens = { "MON","TUE","FRI","0500-1300" };
    TimeGroups result = getTimeGroups(tokens);
    TimeGroup simple;
    simple.days.push_back(Day::Mon);
    simple.days.push_back(Day::Tue);
    simple.days.push_back(Day::Fri);
    simple.ranges.push_back("0500-1300");
    EXPECT_EQ(simple.days, result[0].days);
    EXPECT_EQ(simple.ranges, result[0].ranges);
}

CATEST_F(NOTAM_Test, GetTimeGroups_SimpleTwice_TEST)
{
    const std::vector<std::string> tokens = { "MON","0500-1300","WED","0600-1300" };
    TimeGroups result = getTimeGroups(tokens);
    TimeGroup simple0;
    simple0.days.push_back(Day::Mon);
    simple0.ranges.push_back("0500-1300");
    EXPECT_EQ(simple0.days, result[0].days);
    EXPECT_EQ(simple0.ranges, result[0].ranges);
    TimeGroup simple1;
    simple1.days.push_back(Day::Wed);;
    simple1.ranges.push_back("0600-1300");
    EXPECT_EQ(simple1.days, result[1].days);
    EXPECT_EQ(simple1.ranges, result[1].ranges);
}

CATEST_F(NOTAM_Test, GetTimeGroups_ListAndRange_TEST)
{
    const std::vector<std::string> tokens = { "MON","WED","0500-1300","FRI","-","SUN","0600-1300" };
    TimeGroups result = getTimeGroups(tokens);
    TimeGroup simple;
    simple.days.push_back(Day::Mon);
    simple.days.push_back(Day::Wed);
    simple.ranges.push_back("0500-1300");
    EXPECT_EQ(simple.days, result[0].days);
    EXPECT_EQ(simple.ranges, result[0].ranges);


    TimeGroup simple1;
    simple1.days = makeDayRange(Day::Fri, Day::Sun);
    simple1.ranges.push_back("0600-1300");
    EXPECT_EQ(simple1.days, result[1].days);
    EXPECT_EQ(simple1.ranges, result[1].ranges);
}



class TimeGroupStateMachine
{
private:
    const std::vector<std::string>& tokens;
    TimeGroups groups;
    size_t index;
    std::vector<Day> days;
    std::vector<std::string> ranges;
    bool error;
    bool reset;
    TokenType current;
public:
    TimeGroupStateMachine(const std::vector<std::string>& tokens)
        : tokens{ tokens }
        , index{ 0 }
        , error{ false }
        , reset{ false }
        , current{ TokenType::Invalid }
    {
        InitialState();
    }
    TimeGroups getGroups()
    {
        return groups;
    }
private:

    void getCurrentTokenType()
    {
        current = getType(tokens[index]);
    }

    void incIndex()
    {
        ++index;
        if (index >= tokens.size())
        {
            error = true;
        }
    }

    void InitialState()
    {
        if (error)
            return;
        getCurrentTokenType();
        if (current == TokenType::Day)
        {
            days.push_back(getDay(tokens[index]));
            HasDayState();
        }
        else
        {
            //<day> not first in sequence
            Reject();
            return;
        }
        incIndex();
        if (error) return;
        InitialState();
    }
    void Reset()
    {
        if (error)
            return;
        days.clear();
        ranges.clear();

    }
    void HasDayState()
    {
        if (error) return;
        incIndex();
        if (error) return;
        getCurrentTokenType();
        if (current == TokenType::Dash)
        {
            HasDayDashState();
            return;
        }
        if (current == TokenType::Day)
        {
            days.push_back(getDay(tokens[index]));
            HasDayListState();
            return;
        }
        if (current == TokenType::TimeRange)
        {
            ranges.push_back(tokens[index]);
            HasTimeRangeState();
            return;
        }
    }
    void HasDayDashState()
    {
        if (error) return;
        incIndex();
        if (error) return;
        getCurrentTokenType();
        if (current == TokenType::Day)
        {
            days.push_back(getDay(tokens[index]));
            HasDayDashDayState();
            return;
        }
        else
        {
            // <day> dash should only be followed by <day>
            Reject();
            return;
        }
    }

    void HasDayDashDayState()
    {
        if (error) return;
        incIndex();
        if (error) return;
        getCurrentTokenType();
        if (current == TokenType::TimeRange)
        {
            ranges.push_back(tokens[index]);
            Day end = days.back();
            days.pop_back();
            Day start = days.back();
            days.pop_back();
            std::vector<Day> range = makeDayRange(start, end);
            days.insert(days.end(),range.begin(),range.end());
            HasTimeRangeState();
            return;
        }
        else if(current == TokenType::Day)
        {
            Day end = days.back();
            days.pop_back();
            Day start = days.back();
            days.pop_back();
            std::vector<Day> range = makeDayRange(start, end);
            days.insert(days.end(), range.begin(), range.end());
            days.push_back(getDay(tokens[index]));
            HasDayState();
            return;
        }
        else
        {
            //<day> dash <day> followed by dash
            Reject();
        }
    }
    void HasTimeRangeState()
    {
        if (error) return;
        incIndex();
        if (error)
        {
            BuildTimeGroup();
            return;
        }
        getCurrentTokenType();
        if (current == TokenType::TimeRange)
        {
            ranges.push_back(tokens[index]);
            HasTimeRangeState();
            return;
        }
        else
        {
            // go back a token and let the initial state process whatever is left
            --index;
            BuildTimeGroup();
        }
    }

    void HasDayListState()
    {
        if (error) return;
        incIndex();
        if (error) return;
        getCurrentTokenType();
        if (current == TokenType::Day)
        {
            days.push_back(getDay(tokens[index]));
            HasDayListState();
            return;
        }
        if (current == TokenType::TimeRange)
        {
            ranges.push_back(tokens[index]);
            HasTimeRangeState();
            return;
        }
        else if (current == TokenType::Dash)
        {
            HasDayDashState();
            return;
        }
    }
    void Reject()
    {
        if (error)
            return;
        groups.clear();
        error = true;
    }

    void BuildTimeGroup()
    {
        TimeGroup group;
        group.days = days;
        group.ranges = ranges;
        groups.push_back(group);
        Reset();
    }
};




CATEST_F(NOTAM_Test, GetTimeGroupsSM_Simple_TEST)
{
    const std::vector<std::string> tokens = { "MON","0500-1300" };
    TimeGroupStateMachine sm(tokens);
    TimeGroups result = sm.getGroups();
    TimeGroup simple;
    simple.days.push_back(Day::Mon);
    simple.ranges.push_back("0500-1300");
    EXPECT_EQ(simple.days, result[0].days);
    EXPECT_EQ(simple.ranges, result[0].ranges);
}

CATEST_F(NOTAM_Test, GetTimeGroupsSM_Simple_MultiTimeRange_TEST)
{
    const std::vector<std::string> tokens = { "MON","0500-1300" ,"1400-1500"};
    TimeGroupStateMachine sm(tokens);
    TimeGroups result = sm.getGroups();
    TimeGroup simple;
    simple.days.push_back(Day::Mon);
    simple.ranges.push_back("0500-1300");
    simple.ranges.push_back("1400-1500");
    EXPECT_EQ(simple.days, result[0].days);
    EXPECT_EQ(simple.ranges, result[0].ranges);
}


CATEST_F(NOTAM_Test, GetTimeGroupsSM_Range_TEST)
{
    const std::vector<std::string> tokens = { "MON","-","WED","0500-1300" };
    TimeGroupStateMachine sm(tokens);
    TimeGroups result = sm.getGroups();
    TimeGroup simple;
    simple.days = makeDayRange(Day::Mon, Day::Wed);
    simple.ranges.push_back("0500-1300");
    EXPECT_EQ(simple.days, result[0].days);
    EXPECT_EQ(simple.ranges, result[0].ranges);
}


CATEST_F(NOTAM_Test, GetTimeGroupsSM_List_TEST)
{
    const std::vector<std::string> tokens = { "MON","TUE","FRI","0500-1300" };
    TimeGroupStateMachine sm(tokens);
    TimeGroups result = sm.getGroups();
    TimeGroup simple;
    simple.days.push_back(Day::Mon);
    simple.days.push_back(Day::Tue);
    simple.days.push_back(Day::Fri);
    simple.ranges.push_back("0500-1300");
    EXPECT_EQ(simple.days, result[0].days);
    EXPECT_EQ(simple.ranges, result[0].ranges);
}

CATEST_F(NOTAM_Test, GetTimeGroupsSM_SimpleTwice_TEST)
{
    const std::vector<std::string> tokens = { "MON","0500-1300","WED","0600-1300" };
    TimeGroupStateMachine sm(tokens);
    TimeGroups result = sm.getGroups();
    TimeGroup simple0;
    simple0.days.push_back(Day::Mon);
    simple0.ranges.push_back("0500-1300");
    EXPECT_EQ(simple0.days, result[0].days);
    EXPECT_EQ(simple0.ranges, result[0].ranges);
    TimeGroup simple1;
    simple1.days.push_back(Day::Wed);;
    simple1.ranges.push_back("0600-1300");
    EXPECT_EQ(simple1.days, result[1].days);
    EXPECT_EQ(simple1.ranges, result[1].ranges);
}

CATEST_F(NOTAM_Test, GetTimeGroupsSM_ListAndRange_TEST)
{
    const std::vector<std::string> tokens = { "MON","WED","0500-1300","FRI","-","SUN","0600-1300" };
    TimeGroupStateMachine sm(tokens);
    TimeGroups result = sm.getGroups();
    TimeGroup simple;
    simple.days.push_back(Day::Mon);
    simple.days.push_back(Day::Wed);
    simple.ranges.push_back("0500-1300");
    EXPECT_EQ(simple.days, result[0].days);
    EXPECT_EQ(simple.ranges, result[0].ranges);


    TimeGroup simple1;
    simple1.days = makeDayRange(Day::Fri, Day::Sun);
    simple1.ranges.push_back("0600-1300");
    EXPECT_EQ(simple1.days, result[1].days);
    EXPECT_EQ(simple1.ranges, result[1].ranges);
}


CATEST_F(NOTAM_Test, GetTimeGroupsSM_Combo_TEST)
{
    const std::vector<std::string> tokens = { "MON","WED","-","FRI","SUN","0500-1300"};
    TimeGroupStateMachine sm(tokens);
    TimeGroups result = sm.getGroups();
    TimeGroup simple;
    simple.days.push_back(Day::Mon);
    simple.days.push_back(Day::Wed);
    simple.days.push_back(Day::Thu);
    simple.days.push_back(Day::Fri);
    simple.days.push_back(Day::Sun);
    simple.ranges.push_back("0500-1300");
    EXPECT_EQ(simple.days, result[0].days);
    EXPECT_EQ(simple.ranges, result[0].ranges);
}

CATEST_F(NOTAM_Test, GetTimeGroupsSM_Combo_MultiTime_TEST)
{
    const std::vector<std::string> tokens = { "MON","WED","-","FRI","SUN","0500-1300","1500-1600"};
    TimeGroupStateMachine sm(tokens);
    TimeGroups result = sm.getGroups();
    TimeGroup simple;
    simple.days.push_back(Day::Mon);
    simple.days.push_back(Day::Wed);
    simple.days.push_back(Day::Thu);
    simple.days.push_back(Day::Fri);
    simple.days.push_back(Day::Sun);
    simple.ranges.push_back("0500-1300");
    simple.ranges.push_back("1500-1600");
    EXPECT_EQ(simple.days, result[0].days);
    EXPECT_EQ(simple.ranges, result[0].ranges);
}