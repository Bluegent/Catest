#include <Catest.h>
#include <sstream>
#include <iomanip>
#include <vector>
#include <iostream>
#include <map>

/*
* Implement a data structure to store information about a person
* This data structure should hold the following simple fields  : age, sex, name, CNP, alive status
* The CNP itself shall be automatically generated when creating an instance of person
* This data structure should also be linked to two parents (also people) and any number of children
*
* Then, using this data structure implement the folling functions:
* 1. a function that returns the grandparents of a given person
* 2. a function that returns the eldest descendant of a given person
* 3. a function that returns their youngest female sibling
* The above 3 functions shall return nullptr if no such person can be reached from the given person
* Advanced:
* 4. a function that returns true if the person is the product of incest (at any level)
* 5. a function that counts the number of descendants of a given person
* 6. a function that returns the youngest descendant of a given person
*/

enum class Sex
{
    Male,
    Female
};

class Person
{
private:
    static std::string generateCNP(Sex sex)
    {
        static int32_t counter = 0;
        std::stringstream ss;
        if (sex == Sex::Male)
        {
            ss << 1;
        }
        else
        {
            ss << 2;
        }
        //we ignore date of birth for this exercise
        ++counter;
        ss << std::setw(6) << std::setfill('0') << counter;
        return ss.str();
    }
public:


    int32_t age;
    Sex sex;
    std::string name;
    const std::string CNP;
    bool alive;
    Person* father;
    Person* mother;
    std::vector<Person*> children;

    Person(int32_t age, Sex sex, const std::string& name, Person* father = nullptr, Person* mother = nullptr, bool alive = true)
        : age{ age }
        , sex{ sex }
        , name{ name }
        , CNP{ generateCNP(sex) }
        , alive{ alive }
        , father{ father }
        , mother{ mother }
        , children{}

    {
        if (age < 0)
        {
            throw std::invalid_argument("Age cannot be less than 0.");
        }
    }

    void addChild(Person* child)
    {
        children.push_back(child);
    }

    void haveChildWith(Person* person, Person* child)
    {
        if (sex == person->sex)
        {
            throw std::invalid_argument("We'll talk about this when you get older...");
        }

        if (!this->alive || !person->alive)
        {
            throw std::invalid_argument("Uhhh...");
        }

        this->addChild(child);
        person->addChild(child);

        /*
        if (sex == Sex::Male)
        {
            child->father = this;
            child->mother = person;
        }
        else
        {
            child->father = person;
            child->mother = this;
        }*/
        child->father = this->sex == Sex::Male ? this : person;
        child->mother = this->sex == Sex::Male ? person : this;
    }
    friend bool operator==(const Person& lhs, const Person& rhs);
    friend bool operator!=(const Person& lhs, const Person& rhs);

};


bool operator==(const Person& lhs, const Person& rhs)
{
    return lhs.CNP == rhs.CNP;
}

bool operator!=(const Person& lhs, const Person& rhs)
{
    return lhs.CNP != rhs.CNP;
}



CATEST_F(Person, PersonHasCorrectCNP)
{
    Person dave(30, Sex::Male, "Dave");
    Person gloria(62, Sex::Female, "Gloria");

    EXPECT_EQ(dave.CNP, "1000001");
    EXPECT_EQ(gloria.CNP, "2000002");
}

CATEST_F(Person, NegativeAgeThrows)
{
    try
    {
        Person kromulon(-100, Sex::Male, "Kromulon");
        FAIL("Negative age should throw exception.");
    }
    catch (std::invalid_argument e)
    {

    }
}


CATEST_F(Person, CanAddChildren)
{
    Person dave(30, Sex::Male, "Dave");
    Person gloria(30, Sex::Female, "Gloria");

    Person billy(3, Sex::Male, "William");

    dave.haveChildWith(&gloria, &billy);

    ASSERT_EQ(dave.children.size(), 1);
    ASSERT_EQ(gloria.children.size(), 1);
    ASSERT_EQ(dave.children[0]->CNP, gloria.children[0]->CNP);
}


//1. a function that returns the grandparents of a given person
std::vector<Person*> getGrandParents(const Person& person)
{
    std::vector<Person*> grandParents{};
    if (person.father != nullptr)
    {
        if (person.father->father != nullptr)
        {
            grandParents.push_back(person.father->father);
        }
        if (person.father->mother != nullptr)
        {
            grandParents.push_back(person.father->mother);
        }
    }
    if (person.mother != nullptr)
    {
        if (person.mother->father != nullptr)
        {
            grandParents.push_back(person.mother->father);
        }
        if (person.mother->mother != nullptr)
        {
            grandParents.push_back(person.mother->mother);
        }
    }

    return grandParents;
}


CATEST_F(Person, GetGrandParents)
{
    Person dave(60, Sex::Male, "Dave");
    Person gloria(62, Sex::Female, "Gloria");
    Person billy(30, Sex::Male, "William");
    Person carla(28, Sex::Female, "Carla");
    Person mildred(2, Sex::Female, "Mildred");

    dave.haveChildWith(&gloria, &billy);
    billy.haveChildWith(&carla, &mildred);

    std::vector<Person*> grandParents = getGrandParents(mildred);
    ASSERT_EQ(grandParents.size(), 2);
    ASSERT_EQ(*grandParents[0], dave);
    ASSERT_EQ(*grandParents[1], gloria);
}



//2. a function that returns the eldest descendant of a given person
Person* eldestDescendant(const Person& person)
{
    if (person.children.size() == 0)
    {
        return nullptr;
    }

    Person* eldest = person.children[0];
    for (size_t i = 1; i < person.children.size(); ++i)
    {
        if (person.children[i]->age > eldest->age)
        {
            eldest = person.children[i];
        }
    }
    return eldest;

}

CATEST_F(Person, EldestDescendant)
{
    Person dave(60, Sex::Male, "Dave");
    Person gloria(62, Sex::Female, "Gloria");
    Person billy(30, Sex::Male, "William");
    dave.haveChildWith(&gloria, &billy);

    Person steve(58, Sex::Male, "Steve");
    Person olive(56, Sex::Female, "Olive");
    Person carla(28, Sex::Female, "Carla");
    Person edwin(35, Sex::Male, "Edwin");
    steve.haveChildWith(&olive, &edwin);
    steve.haveChildWith(&olive, &carla);

    Person priscilla(5, Sex::Female, "Priscilla");
    Person elvis(3, Sex::Male, "Elvis");
    Person mildred(2, Sex::Female, "Mildred");
    billy.haveChildWith(&carla, &priscilla);
    billy.haveChildWith(&carla, &elvis);
    billy.haveChildWith(&carla, &mildred);

    Person cara(33, Sex::Female, "Cara Santa-Maria de Dominique de Palmas");
    Person louise(8, Sex::Female, "Louise");
    cara.haveChildWith(&edwin, &louise);

    ASSERT_EQ(*eldestDescendant(gloria), billy);
    ASSERT_EQ(*eldestDescendant(carla), priscilla);
    ASSERT_EQ(*eldestDescendant(olive), edwin);
}


//3. a function that returns their youngest female sibling
Person* youngestFemaleSibling(const Person& person)
{
    Person* yfs = nullptr;
    if (person.father != nullptr)
    {
        //check for eldest female sibling among father's children
        for (size_t i = 0; i < person.father->children.size(); ++i)
        {
            if (person.father->children[i]->sex == Sex::Female && person != (*person.father->children[i]))
            {
                if (yfs == nullptr)
                {
                    yfs = person.father->children[i];
                }
                else if (yfs->age > person.father->children[i]->age)
                {
                    yfs = person.father->children[i];
                }
            }
        }
    }
    if (person.mother != nullptr)
    {
        for (size_t i = 0; i < person.mother->children.size(); ++i)
        {
            if (person.mother->children[i]->sex == Sex::Female && person != (*person.mother->children[i]))
            {
                if (yfs == nullptr)
                {
                    yfs = person.mother->children[i];
                }
                else if (yfs->age > person.mother->children[i]->age)
                {
                    yfs = person.mother->children[i];
                }
            }
        }
    }
    return yfs;
}

Person* youngestFemaleSibling2(const Person& person)
{
    std::vector<Person*> fSiblings;
    if (person.father != nullptr)
    {
        for (Person* fathersChild : person.father->children)
        {
            if (fathersChild->sex == Sex::Female && *fathersChild != person)
            {
                fSiblings.push_back(fathersChild);
            }
        }
    }

    if (person.mother != nullptr)
    {
        for (Person* mothersChild : person.mother->children)
        {
            if (mothersChild->sex == Sex::Female && *mothersChild != person)
            {
                fSiblings.push_back(mothersChild);
            }
        }
    }
    if (fSiblings.size() == 0)
    {
        return nullptr;
    }
    Person* yfs = fSiblings[0];
    for (size_t i = 1; i < fSiblings.size(); ++i)
    {
        if (yfs->age > fSiblings[i]->age)
        {
            yfs = fSiblings[i];
        }
    }
    return yfs;

}


CATEST_F(Person, YoungestFemaleSibling)
{
    Person dave(60, Sex::Male, "Dave");
    Person gloria(62, Sex::Female, "Gloria");
    Person billy(30, Sex::Male, "William");
    dave.haveChildWith(&gloria, &billy);

    Person steve(58, Sex::Male, "Steve");
    Person olive(56, Sex::Female, "Olive");
    Person carla(28, Sex::Female, "Carla");
    Person edwin(35, Sex::Male, "Edwin");
    steve.haveChildWith(&olive, &edwin);
    steve.haveChildWith(&olive, &carla);

    Person priscilla(5, Sex::Female, "Priscilla");
    Person elvis(3, Sex::Male, "Elvis");
    Person mildred(2, Sex::Female, "Mildred");
    billy.haveChildWith(&carla, &priscilla);
    billy.haveChildWith(&carla, &elvis);
    billy.haveChildWith(&carla, &mildred);

    Person cara(33, Sex::Female, "Cara Santa-Maria de Dominique de Palmas");
    Person louise(8, Sex::Female, "Louise");
    cara.haveChildWith(&edwin, &louise);


    Person jorj(1, Sex::Male, "Jorj");
    Person jojefina(1, Sex::Female, "Jojefina");
    billy.haveChildWith(&cara, &jorj);
    billy.haveChildWith(&cara, &jojefina);

    ASSERT_EQ(*youngestFemaleSibling(jorj), jojefina);
    ASSERT_EQ(*youngestFemaleSibling(elvis), jojefina);
    ASSERT_EQ(youngestFemaleSibling2(carla), nullptr);

}

void printTabs(int32_t level)
{
    for (int32_t i = 0; i < level; ++i)
    {
        std::cout << "    ";
    }
}

void printTree(const Person& person, int32_t level = 0)
{

    std::map<Person*, std::vector<Person*>> othersChildren;
    for (Person* child : person.children)
    {
        Person* otherParent = ((*child->mother) == person) ? child->father : child->mother;
        othersChildren[otherParent].push_back(child);
    }

    printTabs(level);
    std::cout << person.name << '\n';

    for (const auto child : othersChildren)
    {
        printTabs(level);
        std::cout << "+" << child.first->name << '\n';
        if (child.second.size() != 0)
        {
            printTabs(level);
            std::cout << "{\n";
            for (Person* person : child.second)
            {
                printTree(*person, level + 1);
            }
            printTabs(level);
            std::cout << "}\n";
        }

    }
}

CATEST_F(Person, PrintTree)
{
    Person dave(60, Sex::Male, "Dave");
    Person gloria(62, Sex::Female, "Gloria");
    Person billy(30, Sex::Male, "William");
    dave.haveChildWith(&gloria, &billy);

    Person steve(58, Sex::Male, "Steve");
    Person olive(56, Sex::Female, "Olive");
    Person carla(28, Sex::Female, "Carla");
    Person edwin(35, Sex::Male, "Edwin");
    steve.haveChildWith(&olive, &edwin);
    steve.haveChildWith(&olive, &carla);

    Person priscilla(5, Sex::Female, "Priscilla");
    Person elvis(3, Sex::Male, "Elvis");
    Person mildred(2, Sex::Female, "Mildred");
    billy.haveChildWith(&carla, &priscilla);
    billy.haveChildWith(&carla, &elvis);
    billy.haveChildWith(&carla, &mildred);

    Person cara(33, Sex::Female, "Cara Santa-Maria de Dominique de Palmas");
    Person louise(8, Sex::Female, "Louise");
    cara.haveChildWith(&edwin, &louise);


    Person jorj(1, Sex::Male, "Jorj");
    Person jojefina(1, Sex::Female, "Jojefina");
    billy.haveChildWith(&cara, &jorj);
    billy.haveChildWith(&cara, &jojefina);

    printTree(dave);
    printTree(gloria);
    printTree(olive);
}


//5. a function that counts the number of descendants of a given person
int32_t countDescendants(const Person& person)
{
    if (person.children.size() == 0)
    {
        return 0;
    }
    int32_t descendants = person.children.size();
    for (Person* child : person.children)
    {
        descendants += countDescendants(*child);
    }
    return descendants;
}

CATEST_F(Person, CountDescendants)
{
    Person dave(60, Sex::Male, "Dave");
    Person gloria(62, Sex::Female, "Gloria");
    Person billy(30, Sex::Male, "William");
    dave.haveChildWith(&gloria, &billy);

    Person steve(58, Sex::Male, "Steve");
    Person olive(56, Sex::Female, "Olive");
    Person carla(28, Sex::Female, "Carla");
    Person edwin(35, Sex::Male, "Edwin");
    steve.haveChildWith(&olive, &edwin);
    steve.haveChildWith(&olive, &carla);

    Person priscilla(5, Sex::Female, "Priscilla");
    Person elvis(3, Sex::Male, "Elvis");
    Person mildred(2, Sex::Female, "Mildred");
    billy.haveChildWith(&carla, &priscilla);
    billy.haveChildWith(&carla, &elvis);
    billy.haveChildWith(&carla, &mildred);

    Person cara(33, Sex::Female, "Cara Santa-Maria de Dominique de Palmas");
    Person louise(8, Sex::Female, "Louise");
    cara.haveChildWith(&edwin, &louise);


    Person jorj(1, Sex::Male, "Jorj");
    Person jojefina(1, Sex::Female, "Jojefina");
    billy.haveChildWith(&cara, &jorj);
    billy.haveChildWith(&cara, &jojefina);


    EXPECT_EQ(countDescendants(dave), 6);
    EXPECT_EQ(countDescendants(jorj), 0);
}
