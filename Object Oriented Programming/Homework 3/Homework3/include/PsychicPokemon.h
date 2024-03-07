/* @Author
 * Student Name: Erblina Nivokazi
 * Student ID : 150200917
 */

#ifndef PSYCHICPOKEMON_H
#define PSYCHICPOKEMON_H

#include <string>
#include "Pokemon.h"
#include "ElectricPokemon.h"

class PsychicPokemon : public Pokemon
{
public:
    PsychicPokemon(std::string n, int hp_, int dmg);
    using Pokemon::faint;
    using Pokemon::getHP;
    using Pokemon::getName;
    using Pokemon::setHP;
    void attemptPowerUp();
    void attack(ElectricPokemon &);
    static int round;
    const int pwrUp = 30;     // All Psychic Pokemons have a Power Up chance of 30 percent
    const int recharge_p = 5; // All Psychic Pokemons have a recharge value of 5
};

#endif