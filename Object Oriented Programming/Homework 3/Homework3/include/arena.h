/* @Author
 * Student Name: Erblina Nivokazi
 * Student ID : 150200917
 */

#include "ElectricPokemon.h"
#include "PsychicPokemon.h"
#include "Pokemon.h"

#include <string>
#include <cstdlib>

class Arena
{
public:
    void rounds();
    void addPokemon(char type, std::string name, int hp, int damage);
    void simulateBattle();
    void spawnTerrain();
    void restartTerrain();
    void printRoundStats(int round, int heads);
    void printMatchResults();
    void printPokemon(Pokemon &, char);
    static int terr_rounds; // static int to manage terrains (if a Psychic/Electric terrain is spawn it stays for 5 rounds)
    static int round;       // static int to manage the number of rounds in a game
private:
    char terrain = 'n';
    ElectricPokemon *ElectricPtr; // Since in the Arena it is stated that two Pokemons will battle I made two pointers
    PsychicPokemon *PsychicPtr;   // One for the Electric Pokemon and one for the Psychic Pokemon
};