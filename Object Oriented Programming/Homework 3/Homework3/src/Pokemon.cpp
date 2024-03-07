/* @Author
 * Student Name: Erblina Nivokazi
 * Student ID : 150200917
 */

#include "Pokemon.h"
#include <iostream>

Pokemon::Pokemon(std::string name, int HP, int dmg, int pwr, int rechrg) : damage(dmg), powerUp(pwr), recharge(rechrg)  //constructor for class 
{                                                                                                                       //Pokemon setting const
    this->name = name;                                                                                                  //values
    this->HP = HP;
}

//Setters and getters for class Pokemon

void Pokemon::setHP(int val)
{
    HP = val;
}

int Pokemon::getHP() const
{
    return HP;
}

int Pokemon::getRecharge() const
{
    return recharge;
}

int Pokemon::getPwr() const
{
    return powerUp;
}

int Pokemon::getDmg() const
{
    return damage;
}

std::string Pokemon::getName() const
{
    return name;
}

bool Pokemon::faint()   //Checking if Pokemon has died
{
    if (HP <= 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

