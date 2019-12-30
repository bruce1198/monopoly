#ifndef _GAME_H_
#define _GAME_H_
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
};
#endif