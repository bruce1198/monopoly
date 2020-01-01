#ifndef _GAME_H_
#define _GAME_H_
#define NUM_OF_CARD 20
#include <vector>
#include "player.h"
#include "map.h"
using namespace std;
class Game {
public:
    Game(int);
    void Show();
    void Run();
    void Event(int, int);
    void ReturnAsset(int);
    void GameOver();
    void ShuffleOpportunity();
    void ShuffleFate();
    int GetNumOfPlayer() {
        return num_of_player;
    }
    // void SetNumOfPlayer(int num_of_player) {
    //     this -> num_of_player = num_of_player;
    // }
private:
    vector<Player> player;
    Map map;
    int num_of_player;
    bool is_gameover = false;
    vector<int> opportunity_fix;
    vector<int> opportunity_random;
    vector<int> fate_fix;
    vector<int> fate_random;
};
#endif