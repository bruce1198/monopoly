#include "game.h"
#include <conio.h>
#include <iostream>
using namespace std;

int main(){
    int num_of_player;
    // cout << "Input Number Of Player : ";
    // cin >> num_of_player;
    num_of_player = 3;
    Game game(num_of_player);
    // game.Show();
    game.Run();
    
}