#include <Catest.h>
#include <random>
#include <iostream>

DISABLE_SUITE(NumberGuess);
CATEST_F(NumberGuess, Guess)
{
    //define some constants/variables
    const int32_t min = 1;
    const int32_t max = 100;
    const int32_t maxAttemptCount = 5;

    //set up random number generation   
    std::random_device rd;
    std::seed_seq seed{ rd(),rd(),rd(),rd(),rd() };
    std::mt19937 twister{ seed };
    std::uniform_int_distribution<int32_t> dist(min, max);
    int8_t choice;
    do
    {
        //generate a random number
        int32_t randomNumber = dist(twister);

        //tell the user what we're actually doing
        std::cout << "I'm thinking of a number between " << min << " and " << max << ". You have " << maxAttemptCount << " attempts to guess it. If you fail, I will take your kidneys.\n";

        //loop over the attempts
        for (int32_t attempt = 1; attempt <= maxAttemptCount; ++attempt)
        {

            //prompt the user for a guess
            std::cout << "Please enter guess #" << attempt << ":";
            int32_t userGuess;
            std::cin >> userGuess;

            //evaluate the guess and report to user
            //end the game when user guessed correctly/runs out of attempts
            if (randomNumber == userGuess)
            {
                std::cout << "Congrats, you win, woohoo, yay!\n";
                break;
            }
            else if (randomNumber < userGuess)
            {
                std::cout << "Your number is too high!\n";
                if (attempt == maxAttemptCount)
                {
                    std::cout << "You lose! Gimme your kidney!\n";
                    std::cout << "The number was " << randomNumber << " .\n";
                }
            }
            else
            {
                std::cout << "Your number is too low.\n";
                if (attempt == maxAttemptCount)
                {
                    std::cout << "You lose! Gimme your kidney!\n";
                    std::cout << "The number was " << randomNumber << " .\n";
                }
            }
        }
        //end loop

        //restart the game if the user chooses to play again
        
        std::cout << "Would you like to play again? (y/n)";
        std::cin >> choice;
    } while (choice == 'y');

}