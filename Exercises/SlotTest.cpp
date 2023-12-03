#include <Catest.h>
#include "SlotGame.h"
#include <cstdint>

CATEST_F(SlotGame, TestSlotOutput)
{
    const Slots slots{ Symbol::Joker,Symbol::Ace,Symbol::Diamond };

    std::string result = slots.toString();
    std::string expected = "[?][A][?]";
    expected[1] = 1;
    expected[7] = 4;


    EXPECT_EQ(result, expected);
    std::cout << result << '\n';
}



CATEST_F(SlotGame, TestSlotMachineSetBet)
{
    SlotMachine machine;
    EXPECT_EQ(machine.getBet(), SlotMachine::minBet);
    float validBet = 10.f;
    EXPECT_FALSE(machine.setBet(0.3f));
    EXPECT_TRUE(machine.setBet(validBet));
    EXPECT_EQ(machine.getBet(), validBet);
}


CATEST_F(SlotGame, TestSlotMachineBalance)
{
    SlotMachine machine;
    EXPECT_EQ(machine.getBalance(), SlotMachine::startBalance);
}


CATEST_F(SlotGame, TestSlotMachineSpin)
{
    SlotMachine machine;

    EXPECT_TRUE(machine.setBet(100.f));
    EXPECT_FALSE(machine.spin());
    EXPECT_EQ(machine.getBalance(), SlotMachine::startBalance);


    EXPECT_TRUE(machine.setBet(1.f));
    EXPECT_TRUE(machine.spin());
    EXPECT_NEQ(machine.getBalance(), SlotMachine::startBalance);
}



CATEST_F(SlotGame, TestRandomHelper_MIGHT_FAIL)
{

    int32_t top = 9;
    SlotHelper helper(0, top);

    std::vector<int32_t> values(100);
    for (size_t i = 0; i < values.size(); ++i)
    {
        values[i] = helper.get();
    }
    std::vector<int32_t> counts(top + 1);
    for (size_t i = 0; i < values.size(); ++i)
    {
        std::cout << values[i] << ' ';
        ++counts[values[i]];
    }
    std::cout << '\n';

    for (size_t i = 0; i < counts.size(); ++i)
    {
        std::cout << i << ": " << counts[i] << '\n';
        EXPECT_TRUE(counts[i] >= 1);
    }



    std::cout << '\n';
}


CATEST_F(SlotGame, SomeRandomSlots)
{
    SlotHelper helper(static_cast<int32_t>(Symbol::Jack), static_cast<int32_t>(Symbol::Club));


    for (size_t i = 0; i < 10; ++i)
    {
        std::cout << helper.generateSlots().toString() << '\n';
    }
}




CATEST_F(SlotGame, SlotEvaluatorTest)
{
    State stateTripple{ Symbol::Ace, Symbol::Ace, Symbol::Ace };
    State statePair{ Symbol::Ace, Symbol::Queen, Symbol::Ace };
    State stateSingle{ Symbol::Ace,Symbol::Queen,Symbol::Diamond };

    State stateTripleWildcard1{ Symbol::Ace,Symbol::Ace,Symbol::Wildcard };
    State stateTripleWildcard2{ Symbol::Ace,Symbol::Wildcard,Symbol::Ace };
    State stateTripleWildcard3{ Symbol::Wildcard, Symbol::Ace,Symbol::Ace };

    State stateTwoWildcard{ Symbol::Wildcard, Symbol::Wildcard, Symbol::Ace };

    EXPECT_EQ(SlotEvaluator::hasTriple(stateTripple), Symbol::Ace);
    EXPECT_EQ(SlotEvaluator::hasTriple(statePair), Symbol::Invalid);
    EXPECT_EQ(SlotEvaluator::hasPair(statePair), Symbol::Ace);
    EXPECT_EQ(SlotEvaluator::hasPair(stateSingle), Symbol::Invalid);

    EXPECT_EQ(SlotEvaluator::hasTriple(stateTripleWildcard1), Symbol::Ace);
    EXPECT_EQ(SlotEvaluator::hasTriple(stateTripleWildcard2), Symbol::Ace);
    EXPECT_EQ(SlotEvaluator::hasTriple(stateTripleWildcard3), Symbol::Ace);

    EXPECT_EQ(SlotEvaluator::hasPair(stateTwoWildcard), Symbol::Wildcard);
    EXPECT_EQ(SlotEvaluator::hasTriple(stateTwoWildcard), Symbol::Invalid);

}



CATEST_F(SlotGame, SlotEvaluatorPayoutTest)
{
    State stateTripple{ Symbol::Ace, Symbol::Ace, Symbol::Ace };
    State statePair{ Symbol::Ace, Symbol::Queen, Symbol::Ace };
    State stateSingle{ Symbol::Ace,Symbol::Queen,Symbol::Diamond };
    State troll{ Symbol::Joker, Symbol::Wildcard,Symbol::Heart };
    State diamond{ Symbol::Diamond,Symbol::Joker, Symbol::Diamond };

    EXPECT_EQ(SlotEvaluator::getPayout(stateTripple), Payout::AceTriple);
    EXPECT_EQ(SlotEvaluator::getPayout(statePair),Payout::AcePair);
    EXPECT_EQ(SlotEvaluator::getPayout(stateSingle), Payout::None);
    EXPECT_EQ(SlotEvaluator::getPayout(troll), Payout::TrollSpecial);
    EXPECT_EQ(SlotEvaluator::getPayout(diamond), Payout::DiamondEyesSpecial);

}

CATEST_F(SlotGame, SlotGameTest)
{
    SlotMachine machine;

    for (size_t i = 0; i < 5; ++i)
    {
        machine.spin();
    }
}

DISABLE_SUITE(SlotGame);
DISABLE_SUITE(SlotGame_Game)
CATEST_F(SlotGame_Game, SlotGameEntry)
{
    SlotGame game;
    game.loop();
}
