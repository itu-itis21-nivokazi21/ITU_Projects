/* @Author
 * Student Name: Erblina Nivokazi
 * Student ID : 150200917
 */

#include "PsychicPokemon.h"
#include "Pokemon.h"
#include "ElectricPokemon.h"
#include <iostream>

PsychicPokemon::PsychicPokemon(std::string n, int hp_, int dmg) : Pokemon(n, hp_, dmg, pwrUp, recharge_p) //The values for power up chance and recharge
{                                                                                                         //are static for Psychic Pokemon
}

int PsychicPokemon::round = 6;          //static value to keep the round of attempt to power up

void PsychicPokemon::attemptPowerUp()
{
    int b = recharge_p;
    if (round <= b)                     //The Psychic Pokemon cannot attempt a Power Up until recharge is filled up
    {                                   //Recharge for Psychic Pokemon is 5 rounds that's I turn the round value to 0 when power up is initialized
        Pokemon::in_powerUp = 0;
        return;
    }
    int prob = rand() % 100;            //Generating a random number
    int chance = pwrUp;
    if (prob <= chance)                 //Seeing if the generated number is lower than the power up chance that the psychic pokemon has
    {                                   //If the generated number is lower than 30 then a Power up is initialized
        Pokemon::in_powerUp = 1;
        round = 0;
    }
    else
    {
        Pokemon::in_powerUp = 0;
    }
}

void PsychicPokemon::attack(ElectricPokemon &electric)
{
    int subtrahend = this->getDmg();        //getting the damage value of the Psychic Pokemon 
    int minuend = electric.getHP();         //Getting the Electric Pokemon's HP value so that we can lower it 
    int newHP;

    if (Pokemon::inTerrain)                 //If the Psychic Pokemon is in Psychic Terrain then its' damage value gets doubled
    {
        subtrahend = subtrahend * 2;
    }
    if (Pokemon::in_powerUp)                //If the Psychic Pokemon is in Power Up then it can also confuse the Electric Pokemon
    {
        electric.setConfusion();
    }

    newHP = minuend - subtrahend;
    electric.setHP(newHP);

    if (newHP < 0)                          //If the new HP value is lower than 0 there is no need to represent it since the Pokemon faints at 0
        electric.setHP(0);
}