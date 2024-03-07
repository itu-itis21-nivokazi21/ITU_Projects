/* @Author
 * Student Name: Erblina Nivokazi
 * Student ID : 150200917
 */

#ifndef POKEMON_H
#define POKEMON_H

#include <string>

class Pokemon
{
public:
    Pokemon(std::string name, int HP, int damage, int powerUp, int recharge); // Constructor for Pokemon
    void setHP(int);                                                          // Necessary methods for when the Pokemons are in a battle
    int getHP() const;
    int getRecharge() const;
    int getPwr() const;
    int getDmg() const;
    std::string getName() const;
    bool faint();
    bool in_powerUp = 0;
    bool inTerrain = 0;

private:
    std::string name;
    int HP;
    const int damage;
    const int powerUp;
    const int recharge;
};

#endif