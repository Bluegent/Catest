#include <Catest.h>
#include <vector>
#include <map>
#include <iostream>


enum class BloodType
{
    APlus,
    AMinus,
    BPlus,
    BMinus,
    ABPlus,
    ABMinus,
    ZeroPlus,
    ZeroMinus,
};


struct Person
{
private:
    uint32_t age = 30;
public:
    std::string name;
    std::string cnp;
    std::string profession = "Unemployed";
    float monthlyIncome = 0 ;
    BloodType type;
    bool arrestRecord = false;

    /*uint32_t getAge() const
    {
        return age;
    }*/

    const uint32_t& getAge() const
    {
        return age;
    }

    void setAge(const uint32_t newAge)
    {
        if (newAge < age)
        {
            return;
        }
        age = newAge;
    }

    void giveRaise(const float percentage)
    {
        monthlyIncome += monthlyIncome * percentage;
    }

};


void printPerson(const Person & person)
{
    std::cout << person.name;
}

void hirePerson(Person & person)
{
    person.monthlyIncome = 3000;
}


void giveRaise(Person& person, const float percentage)
{
    person.monthlyIncome += person.monthlyIncome * percentage;
}

CATEST_F(MapTest, MapTest)
{
    std::vector<int32_t> vec(10);

    vec[3] = 42;

    std::map<std::string, int32_t> age;

    age["Dan"] = 30;
    age["Alexa"] = 25;


    std::map<std::string, std::string> passwords;

    passwords["user"] = "password";
    passwords["admin"] = "admin";

    std::vector<Person> people; 
    /*
    Person juan;
    juan.name = "Juan";
    juan.cnp = "1";

    Person lupita;
    lupita.name = "Lupita";
    lupita.cnp = "2";

    Person chivo;
    chivo.name = "Uay Chivo";
    chivo.cnp = "3";


    people.push_back(juan);
    people.push_back(lupita);
    people.push_back(chivo);

    for (Person & person : people)
    {
        if (person.cnp == "3")
        {
            person.giveRaise(0.2f);
            break;
        }
    }

    std::map<std::string, Person> peopleMap;


    peopleMap.emplace(juan.cnp, juan);
    peopleMap.emplace(lupita.cnp, lupita);
    peopleMap.emplace(chivo.cnp, chivo);

    //peopleMap.insert(std::pair<std::string, Person>(chivo.cnp, chivo));


    peopleMap["3"].giveRaise(0.2f);

    peopleMap.find("3");
    */
    //big O notation - gives you the complexity of an aglorithm
    // O(n)
    // O(1) - best possbile complexity - basically free/instant
    // O(log n) 2^5 = 32 log 2 (32) = 5 / log x (y) = z => x ^ z = y
    // O(log n) < O(n)
}
;
DISABLE_SUITE(MapTest)