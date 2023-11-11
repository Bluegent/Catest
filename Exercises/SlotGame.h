#pragma once
#include <string>
#include <sstream>
#include <iostream>
#include <random>
#include <deque>
#include <iostream>

// Let's implement a slot machine game
// The slot machine shall have 3 slots
// The player shall be able to input a sum to bet and change it at any point
// The minium bet shall be 50 cents
// The player shall be able to pull the slot machine lever once they have set this sum
// The player shall be kicked out once they have hit 0 balance
// The player shall be able to quit the game at any time
// The player shall start out with 10 dollars
// The slots shall contain: 
//  Jack        [J] - JJJ -  x3 bet / JJx - + $1
//  Queen       [Q] - QQQ -  x5 bet / QQx - + $2
//  King        [K] - KKK - x10 bet / KKx - + $3
//  Ace         [A] - AAA - x20 bet / AAx - + $4                    
//  Joker       [☺] - ☺☺☺ - last 10 bets
//  Wildcard    [*] - *** - x3 bet / a pair of any other symbol and a wild card counts as a triple of the other symbol e.g QQ* counts as QQQ
//  Heart       [♥] - ♥♥♥ - $10 / ♥♥x - $2
//  Spade       [♠] - ♠♠♠ - $10 / same
//  Diamond     [♦] - ♦♦♦ - $10 / same
//  Club        [♣] - ♣♣♣ - $10 / same
// specials:           
// ☺*♥ - troll special: double bet and spin again
// ♦☺♦ - x 10 bet + $50


enum class Symbol
{
    Jack,
    Queen,
    King,
    Ace,
    Joker,
    Wildcard,
    Heart,
    Spade,
    Diamond,
    Club,
    Invalid
};

// in order to print our Symbol we need a function to 'translate' it to something printable
// because we decided all our symbols will be represented by a char, that something will be a char
char toChar(const Symbol symbol)
{
    switch (symbol)
    {
    case Symbol::Jack:
        return 'J';
    case Symbol::Queen:
        return 'Q';
    case Symbol::King:
        return 'K';
    case Symbol::Ace:
        return 'A';
    case Symbol::Joker:
        return 1;
    case Symbol::Wildcard:
        return '*';
    case Symbol::Heart:
        return 3;
    case Symbol::Spade:
        return 6;
    case Symbol::Diamond:
        return 4;
    case Symbol::Club:
        return 5;
    default:
        return '?';
    }
}


std::ostream& operator << (std::ostream& lhs, const Symbol& rhs)
{
    lhs << toChar(rhs);
    return lhs;
}

// represents an internal state of the slot machine slots
struct State
{
    Symbol left;
    Symbol middle;
    Symbol right;
    State()
        : left{ Symbol::Wildcard }
        , middle{ Symbol::Wildcard }
        , right{ Symbol::Wildcard }
    {

    }

    State(Symbol left, Symbol middle, Symbol right)
        : left{ left }
        , middle{ middle }
        , right{ right }
    {

    }
};

// encapsulates the internal state of the slot machine slots
class Slots
{
    State state;
public:
    State get() const
    {
        return state;
    }
    Slots()
        : state{ Symbol::Wildcard,Symbol::Wildcard,Symbol::Wildcard }
    {

    }

    Slots(Symbol left, Symbol middle, Symbol right)
        : state{ left,middle,right }
    {

    }

    // returns a human-readable representation of the slots in the form [x][x][x]
    std::string toString() const
    {
        std::stringstream stream;
        stream << '[' << toChar(state.left) << ']';
        stream << '[' << toChar(state.middle) << ']';
        stream << '[' << toChar(state.right) << ']';
        return stream.str();
    }
};


enum class Payout
{
    JackTriple,     // 3x bet
    JackPair,       // $1
    QueenTriple,    // 5x bet
    QueenPair,      // $2
    KingTriple,     // x10 bet
    KingPair,       // $3
    AceTriple,      // x20 bet
    AcePair,        // $4
    JokerTriple,    // sum of last 10 bets
    WildcardTriple, // 3x bet
    HeartTriple,    // $10
    HeartPair,      // $2
    SpadeTriple,    // $10
    SpadePair,      // $2
    DiamondTriple,  // $10
    DiamondPair,    // $2
    ClubTriple,     // $10
    ClubPair,       // $2
    TrollSpecial,   // double bet and spin again
    DiamondEyesSpecial, // x10 bet + $50
    None
};



class SlotEvaluator
{
public:
    static bool hasWildCard(const State& slots)
    {
        return slots.left == Symbol::Wildcard || slots.middle == Symbol::Wildcard || slots.right == Symbol::Wildcard;
    }

    static Symbol hasTriple(const State& slots)
    {
        if (slots.left == slots.middle && slots.middle == slots.right)
        {
            return slots.left;
        }

        if (hasWildCard(slots))
        {
            Symbol pair = hasPair(slots);
            if (pair == Symbol::Wildcard)
            {
                return Symbol::Invalid;
            }
            return pair;
        }

        return Symbol::Invalid;
    }
    static Symbol hasPair(const State& slots)
    {
        if (slots.left == slots.middle)
        {
            return slots.left;
        }
        else if (slots.left == slots.right)
        {
            return slots.left;
        }
        else if (slots.middle == slots.right)
        {
            return slots.right;
        }
        return Symbol::Invalid;
    }

    // ☺*♥ - troll special: double bet and spin again
    // ♦☺♦ - diamond eyes special: x 10 bet + $50
    static bool hasTrollSpecial(const State& slots)
    {
        return slots.left == Symbol::Joker && slots.middle == Symbol::Wildcard && slots.right == Symbol::Heart;
    }
    static bool hasDiamondEyes(const State& slots)
    {
        return slots.left == Symbol::Diamond && slots.middle == Symbol::Joker && slots.right == Symbol::Diamond;
    }

    static Payout getTriple(Symbol triple)
    {
        switch (triple)
        {
        case Symbol::Jack:
            return Payout::JackTriple;
        case Symbol::Queen:
            return Payout::QueenTriple;
        case Symbol::King:
            return Payout::KingTriple;
        case Symbol::Ace:
            return Payout::AceTriple;
        case Symbol::Joker:
            return Payout::JokerTriple;
        case Symbol::Wildcard:
            return Payout::WildcardTriple;
        case Symbol::Heart:
            return Payout::HeartTriple;
        case Symbol::Spade:
            return Payout::SpadeTriple;
        case Symbol::Diamond:
            return Payout::DiamondTriple;
        case Symbol::Club:
            return Payout::ClubTriple;
        default:
            return Payout::None;
        }
    }

    static Payout getPair(Symbol pair)
    {
        switch (pair)
        {
        case Symbol::Jack:
            return Payout::JackPair;
        case Symbol::Queen:
            return Payout::QueenPair;
        case Symbol::King:
            return Payout::KingPair;
        case Symbol::Ace:
            return Payout::AcePair;
        case Symbol::Heart:
            return Payout::HeartPair;

        case Symbol::Spade:
            return Payout::SpadePair;
        case Symbol::Diamond:
            return Payout::DiamondPair;
        case Symbol::Club:
            return Payout::ClubPair;
        default:
            return Payout::None;
        }
    }

    static Payout getPayout(const State& slots)
    {
        if (hasTrollSpecial(slots))
        {
            return Payout::TrollSpecial;
        }
        if (hasDiamondEyes(slots))
        {
            return Payout::DiamondEyesSpecial;
        }

        Payout triple = getTriple(hasTriple(slots));
        if (triple != Payout::None)
        {
            return triple;
        }

        Payout pair = getPair(hasPair(slots));
        if (pair != Payout::None)
        {
            return pair;
        }

        return Payout::None;
    }



};

class SlotHelper
{
private:
    std::mt19937 mt;
    std::uniform_int_distribution<int32_t> dist;
public:
    SlotHelper(int32_t min, int32_t max)
        : dist{ min,max }
    {
        std::random_device rd;
        mt.seed(rd());
    }
    int32_t get()
    {
        return dist(mt);
    }

    Slots generateSlots()
    {
        Symbol left = static_cast<Symbol>(get());
        Symbol middle = static_cast<Symbol>(get());
        Symbol right = static_cast<Symbol>(get());

        return Slots(left, middle, right);
    }
};

class SlotMachine
{
private:
    float bet;
    float balance;
    SlotHelper helper;
    std::deque<float> lastTenBets;

    Slots _spinSlots()
    {
        return helper.generateSlots();
    }
    void evaluate(const State& slots)
    {
        Payout payout = SlotEvaluator::getPayout(slots);
        float payoutAmount = 0.f;
        switch (payout)
        {
        case Payout::JackTriple:
            payoutAmount = bet * 3.f;
            break;
        case Payout::JackPair:
            payoutAmount = 1.f;
            break;
        case Payout::QueenTriple:
            payoutAmount = bet * 5.f;
            break;
        case Payout::QueenPair:
            payoutAmount = 2.f;
            break;
        case Payout::KingTriple:
            payoutAmount = bet * 10.f;
            break;
        case Payout::KingPair:
            payoutAmount = 3.f;
            break;
        case Payout::AceTriple:
            payoutAmount = bet * 20.f;
            break;
        case Payout::AcePair:
            payoutAmount = 4;
            break;
        case Payout::JokerTriple:
            payoutAmount = getLastTenBetsSum();
            break;
        case Payout::WildcardTriple:
            payoutAmount = bet * 3.f;
            break;
        case Payout::ClubTriple:
        case Payout::DiamondTriple:
        case Payout::SpadeTriple:
        case Payout::HeartTriple:
            payoutAmount = 10.f;
            break;
        case Payout::ClubPair:
        case Payout::DiamondPair:
        case Payout::SpadePair:
        case Payout::HeartPair:
            payoutAmount = 2.f;
            break;
        case Payout::TrollSpecial:
            bet *= 2;
            spin();
            return;
        case Payout::DiamondEyesSpecial:
            payoutAmount = bet * 10.f + 50.f;
            break;
        case Payout::None:
            break;
        default:
            break;
        }
        balance += payoutAmount;
        std::cout << "Payout: $" << payoutAmount <<'\n';
    }


    float getLastTenBetsSum()
    {
        float sum = 0.f;
        for (float bet : lastTenBets)
        {
            sum += bet;
        }
        return sum;
    }
public:

    static const float minBet;
    static const float startBalance;

    SlotMachine()
        : bet{ minBet }
        , balance{ startBalance }
        , helper{ static_cast<int32_t>(Symbol::Jack), static_cast<int32_t>(Symbol::Club) }
        , lastTenBets{}
    {

    }

    SlotMachine(const SlotHelper& helper)
        : bet{ minBet }
        , balance{ startBalance }
        , helper{ helper }
    {

    }

    bool setBet(float input)
    {
        // minimum bet is 0.50 dollars/ 50 cents
        if (input < minBet)
        {
            return false;
        }
        bet = input;
        return true;
    }

    float getBet() const
    {
        return bet;
    }

    float getBalance() const
    {
        return balance;
    }

    bool spin()
    {
        //check if the user has enough money to bet
        if (balance < bet)
        {
            return false;
        }
        balance -= bet;
        lastTenBets.push_back(bet);
        if (lastTenBets.size() > 10)
        {
            lastTenBets.pop_front();
        }
        std::cout << "Inserted $" << bet << " into machine and pulled the lever. Balance: $" << balance << '\n';
        std::cout << "Spinning...\n";
        // spin the slots
        Slots result = _spinSlots();
        std::cout << "+=========+" << '\n';
        std::cout << '|' << result.toString() << '|' << '\n';
        std::cout << "+=========+" << '\n';
        
        // evaluate the result of the spin
        evaluate(result.get());
        std::cout << "New Balance: $" << balance << '\n';

        return true;
    }

};
const float SlotMachine::minBet = 0.5f; // 50 cents
const float SlotMachine::startBalance = 10.f; // 10 dollars



enum class MenuOption
{
    SetBet,
    Spin,
    Quit,
    Interrogate,
    Invalid
};

class SlotGame
{
private:
    SlotMachine machine;
    bool running;
public:
    SlotGame()
        : machine{}
        , running{true}
    {

    }

    void setBet()
    {
        float newBet = 0.f;
        do
        {
            std::cout << "Enter new bet:";
            std::string line;
            std::getline(std::cin, line);
            std::getline(std::cin, line);
            try
            {
                newBet = std::stof(line);
            }
            catch (...)
            {
                newBet = 0.f;
            }

            bool result = machine.setBet(newBet);
            if (!result)
            {
                std::cout << "Invalid bet! The minimum bet is $" << SlotMachine::minBet <<".\n";
            }
        }
        while (newBet < SlotMachine::minBet);
    }

    void spin()
    {
        bool result = machine.spin();
        if (!result)
        {
            std::cout << "Insufficient funds!\n";
        }
    }


    MenuOption getOption()
    {
        char read = 0;
        do
        {
            std::cout << "Enter Option:";
            std::cin.get(read);
            switch (read)
            {
            case 'q':
                return MenuOption::Quit;
            case 'b':
                return MenuOption::SetBet;
            case 'i':
                return MenuOption::Interrogate;
            case '\n':
                return MenuOption::Spin;
            }
            std::cout << "Invalid option!\n";
        } while (read == 0);
    }
    
    void interrogate()
    {
        std::cout << "Current balance: $" << machine.getBalance()<<'\n';
        std::cout << "Current bet: $" << machine.getBet() << '\n';
    }

    void printMenu()
    {
        std::cout << "q \t- quit\n";
        std::cout << "b \t- set bet\n";
        std::cout << "i \t- interrogate balance/bet\n";
        std::cout << "enter \t- spin\n";
    }



    void loop()
    {
        while (running)
        {
            printMenu();
            MenuOption option = getOption();
            switch (option)
            {
            case MenuOption::SetBet:
                setBet();
                break;
            case MenuOption::Spin:
                spin();
                break;
            case MenuOption::Quit:
                return;
            case MenuOption::Interrogate:
                interrogate();
                break;
            default:
                break;
            }

            if (machine.getBalance() <= 0.f)
            {
                std::cout << "You lose :(!\n";
                return;
            }
        }
    }
};