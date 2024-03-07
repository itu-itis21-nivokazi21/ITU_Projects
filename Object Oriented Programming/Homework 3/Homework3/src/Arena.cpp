/* @Author
 * Student Name: Erblina Nivokazi
 * Student ID : 150200917
 */

#include "arena.h"
#include "ElectricPokemon.h"
#include "PsychicPokemon.h"
#include "Pokemon.h"

#include <string>
#include <iostream>
#include <cstdlib>

int Arena::terr_rounds = 6; // Static numer to keep the rounds that pass in terrain so that we can keep the terrain electric or psychic for
                            // five rounds
int Arena::round = 1;       // static number of rounds that will get displayed when we print the results

void Arena::rounds()        // Method to keep the counts of each function that requires the round change
{
    round++;
    terr_rounds++;
    ElectricPtr->round++;
    ElectricPtr->cnf_round++;
    PsychicPtr->round++;
}

void Arena::restartTerrain() // Method to restart the terrain makin the static number round 1 and returnig the terr_rounds number to 6
{                            // so that a new terrain can be spawned
    round = 1;
    terr_rounds = 6;
    this->spawnTerrain();
}

void Arena::addPokemon(char type, std::string name, int hp, int damage)
{
    if (type == 'e') // Based on the type if type is e then the constructor for Electric Pokemon is called
    {
        ElectricPtr = new ElectricPokemon(name, hp, damage);
        printPokemon(*ElectricPtr, type); // Based on the outputs given to us in every addPokemon the pokemon added is printed
    }                                     // on screen
    else if (type == 'p')
    {
        PsychicPtr = new PsychicPokemon(name, hp, damage);
        printPokemon(*PsychicPtr, type);
    }
}

void Arena::spawnTerrain()
{
    if (terr_rounds >= 5)        // Here I check if the counter that I've made is bigger than 5 so that if it is not recharged up until 5
    {                            // then it will not spawn a new terrain, this counter will turn to 0 everytime a psychic or electric terrain is spawned
        int prob = rand() % 100; // Generating a random number between 0 and 100
        if (prob <= 20)          // 20 percent to be electric
        {
            terrain = 'e';
            terr_rounds = 0;            // terrain counter gets set to zero so that Electric terrain will stay for 5 rounds
            ElectricPtr->inTerrain = 1; // The in terrain value in Electric Pointer is turned into 1 so that the Electric Pokemon won't be prone
        }                               // to confusion
        else if (prob <= 40)            // 20-40 another 20 percent chance for Psychic terrain
        {
            terrain = 'p';
            terr_rounds = 0;
            PsychicPtr->inTerrain = 1; // This way Psychic Pokemon's damage value gets doubled
        }
        else
        {
            terrain = 'n';
            ElectricPtr->inTerrain = 0; // The Pokemon's won't have any special powers if they're in none terrain
            ElectricPtr->inTerrain = 0;
        }
    }
}

void Arena::simulateBattle()
{
    this->restartTerrain();            // Whenever we simulate a new battle the terrain gets reset
    while (true)                       // Loop that will work until one of the Pokemon's faints
    {
        this->spawnTerrain();          // Spawn terrain in each round

        int heads = rand() % 2;        // Randomly choosing which Pokemon attacks first

        ElectricPtr->attemptPowerUp(); // Both Pokemon's attempt Power Up each round
        PsychicPtr->attemptPowerUp();

        if (heads)
        {
            ElectricPtr->electrocute(*PsychicPtr);
        }
        else
        {
            PsychicPtr->attack(*ElectricPtr);
        }

        if (ElectricPtr->faint() || PsychicPtr->faint()) // After the first Pokemon attacks we check whether one of the Pokemons has fainted
        {                                                // If they fainted then the round ends
            printRoundStats(round, heads);
            printMatchResults();
            break;
        }

        if (heads) // Second attack from the other Pokemon
        {
            PsychicPtr->attack(*ElectricPtr);
        }
        else
        {
            ElectricPtr->electrocute(*PsychicPtr);
        }

        printRoundStats(round, heads);                   // Printing the round stats of each round

        if (ElectricPtr->faint() || PsychicPtr->faint()) // Checking if a Pokemon has fainted after the second attack
        {
            printMatchResults();
            break;
        }

        rounds();
    }
    delete ElectricPtr; // Deleting the pointers of the Pokemons when each battle ends
    delete PsychicPtr;
}

void Arena::printRoundStats(int round, int heads)
{
    std::cout << std::endl; // Here I added two end lines since in the output file
    std::cout << std::endl; // given to us each printing of round stats had two lines in between
 
    std::cout << "Round: " << round << std::endl;
    std::string terrname;
    
    if (terrain == 'e') terrname = "electric";
    if (terrain == 'p') terrname = "psychic";
    if (terrain == 'n') terrname = "none";
    
    std::cout << "Current Terrain: " << terrname << std::endl;
    if (heads)
    {
        std::cout << ElectricPtr->getName() << " goes first." << std::endl;
    }
    else
    {
        std::cout << PsychicPtr->getName() << " goes first." << std::endl;
    }
    if (ElectricPtr->in_powerUp)
        std::cout << ElectricPtr->getName() << " has initiated a power up. " << std::endl;
    if (PsychicPtr->in_powerUp)
        std::cout << PsychicPtr->getName() << " has initiated a power up. " << std::endl;

    std::cout << ElectricPtr->getName() << " HP: " << ElectricPtr->getHP() << std::endl;
    std::cout << PsychicPtr->getName() << " HP: " << PsychicPtr->getHP() << std::endl;

    if (ElectricPtr->getHP() > PsychicPtr->getHP())
    {
        std::cout << ElectricPtr->getName() << " is in the lead!" << std::endl;
    }
    else
    {
        std::cout << PsychicPtr->getName() << " is in the lead!" << std::endl;
    }
}

void Arena::printMatchResults()
{
    std::cout << std::endl;
    std::cout << std::endl;

    std::cout << "Match Results:" << std::endl;
    if (ElectricPtr->getHP() > 0)
    {
        std::cout << ElectricPtr->getName() << " has won the match!" << std::endl;
    }
    else
    {
        std::cout << PsychicPtr->getName() << " has won the match!" << std::endl;
    }
}

void Arena::printPokemon(Pokemon &p, char type)
{
    std::cout << std::endl;
    std::cout << std::endl;

    std::string p_class;

    if (type == 'e')
    {
        p_class = "Electric";
    }
    else
    {
        p_class = "Psychic";
    }

    std::cout << p_class << " Pokemon " << p.getName() << " has entered the arena. " << std::endl;
    std::cout << "Class " << p_class << std::endl;

    std::cout << "    Name: " << p.getName() << std::endl;
    std::cout << "    HP: " << p.getHP() << std::endl;
    std::cout << "    Damage: " << p.getDmg() << std::endl;
}