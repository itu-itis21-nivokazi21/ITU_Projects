/* @Author
 * Student Name: Erblina Nivokazi
 * Student ID : 150200917
 */

#ifndef ELECTRICPOKEMON_H
#define ELECTRICPOKEMON_H

#include <string>
#include "Pokemon.h"

class ElectricPokemon : public Pokemon
{
public:
    ElectricPokemon(std::string n, int hp_, int dmg);
    using Pokemon::faint;
    using Pokemon::getHP;
    using Pokemon::getName;
    using Pokemon::setHP;
    void attemptPowerUp();
    void setConfusion();
    void electrocute(Pokemon &); // When attacking a Psychic Pokemon
    static int round;            // I made static interger values to help keep the count of rounds
    static int cnf_round;        // It is necessary for both power up initiations and confusion
    const int pwrUp = 20;        // All electric Pokemons have a Power Up chance of 20 percent
    const int recharge_e = 3;    // All ellectric Pokemons have a recharge value of 3
private:
    bool confusion = 0;
};

#endif
