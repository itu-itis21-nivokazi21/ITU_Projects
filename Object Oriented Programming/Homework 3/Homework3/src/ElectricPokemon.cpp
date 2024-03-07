/* @Author
 * Student Name: Erblina Nivokazi
 * Student ID : 150200917
 */

#include "PsychicPokemon.h"
#include "Pokemon.h"
#include "ElectricPokemon.h"

#include <iostream>

ElectricPokemon::ElectricPokemon(std::string n, int hp_, int dmg) : Pokemon(n, hp_, dmg, pwrUp, recharge_e) //power up chances and recharge
{                                                                                                           //is a set number for all electric
}                                                                                                           //pokemons

int ElectricPokemon::round = 4;         //static number to help with the recharge process in attemptPowerUp

int ElectricPokemon::cnf_round = 4;     //static number to help count the rounds until confusion wears off

void ElectricPokemon::attemptPowerUp()
{
    int b = recharge_e;
    if (round <= b)                     //Here I check whether or not the round number is smaller than the recharge value
    {                                   //If it is smaller it means that it is not yet recharged to attempt another power up
        Pokemon::in_powerUp = 0;        //That's why I made the round value at first bigger than the power up so that it will
        return;
    }
    int prob = rand() % 100;            //Here I am taking the modules of a random number in between 0 and 100
    int chance = pwrUp;                 
    if (prob <= chance)                 //If the random number is lower than 20 then Elecric Pokemon can do a Power Up
    {
        Pokemon::in_powerUp = 1;
        round = 0;
    }
    else
    {
        Pokemon::in_powerUp = 0;
    }
}

void ElectricPokemon::setConfusion()    //This function is used by the psychic Pokemon if it wants to confuse the Electric Pokemon
{
    if (!Pokemon::inTerrain)            //If the Electric Pokemon is not in electric terrain then the Electric Pokemon cannot get confused
    {
        confusion = 1;
        cnf_round = 0;
    }
    else
    {
        confusion = 0;
    }
}

void ElectricPokemon::electrocute(Pokemon &psychic)
{
    int b = recharge_e;
    if (cnf_round <= b)                 //If the Electric Pokemon is confused then it cannot electrocute the Psychic Pokemon
    {                                   //The Pokemon will stay confused until it is bigger than the recharge value which is 3
        return;
    }
    int subtrahend = this->getDmg();    //I get the damage of the electric pokemon
    int minuend = psychic.getHP();      //HP of psychic Pokemon

    if (Pokemon::in_powerUp)
    {
        subtrahend = subtrahend * 3;    //If the Electric Pokemon is in power up mode then the Psychic Pokemon will get triple damage
    }

    int newHP = minuend - subtrahend;
    psychic.setHP(newHP);

    if (newHP < 0)                      //Since the Pokemon dies when its' HP gets to zero there is no need for the HP to be a minus value
        psychic.setHP(0);
}
