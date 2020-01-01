#include "game.h"
#include <iostream>
#include <string>
#include <algorithm>
#include <math.h>
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock

using namespace std;

Game::Game(int num_of_player) {
    this -> num_of_player = num_of_player;
    for(int iter = 0; iter < num_of_player; iter++) {
        player.push_back(Player());
    }
    // player.resize(num_of_player);
    for(int card_iter = 0; card_iter < NUM_OF_CARD; card_iter++) {
        opportunity_fix.push_back(card_iter);
        fate_fix.push_back(card_iter);
    }
    ShuffleFate();
    ShuffleOpportunity();
}
void Game::ShuffleFate() {
    fate_random = vector<int>(fate_fix);
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    shuffle(fate_random.begin(), fate_random.end(), default_random_engine(seed));
}
void Game::ShuffleOpportunity() {
    opportunity_random = vector<int>(fate_fix);
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    shuffle(opportunity_random.begin(), opportunity_random.end(), default_random_engine(seed));
}
void Game::ReturnAsset(int player_num) {
    for(int iter = 0; iter < player[player_num].grid_red.size(); iter++) {
        map.grid[player[player_num].grid_red[iter]].Init();
    }
    for(int iter = 0; iter < player[player_num].grid_orange.size(); iter++) {
        map.grid[player[player_num].grid_orange[iter]].Init();
    }
    for(int iter = 0; iter < player[player_num].grid_yellow.size(); iter++) {
        map.grid[player[player_num].grid_yellow[iter]].Init();
    }
    for(int iter = 0; iter < player[player_num].grid_green.size(); iter++) {
        map.grid[player[player_num].grid_green[iter]].Init();
    }
    for(int iter = 0; iter < player[player_num].grid_blue.size(); iter++) {
        map.grid[player[player_num].grid_blue[iter]].Init();
    }
    for(int iter = 0; iter < player[player_num].grid_light.size(); iter++) {
        map.grid[player[player_num].grid_light[iter]].Init();
    }
    for(int iter = 0; iter < player[player_num].grid_purple.size(); iter++) {
        map.grid[player[player_num].grid_purple[iter]].Init();
    }
    for(int iter = 0; iter < player[player_num].grid_pink.size(); iter++) {
        map.grid[player[player_num].grid_pink[iter]].Init();
    }
    for(int iter = 0; iter < player[player_num].grid_infrastructure.size(); iter++) {
        map.grid[player[player_num].grid_infrastructure[iter]].Init();
    }
    for(int iter = 0; iter < player[player_num].grid_station.size(); iter++) {
        map.grid[player[player_num].grid_station[iter]].Init();
    }
}
void Game::Run() {
    while(!is_gameover) {
        for(int player_iter = 0; player_iter < num_of_player; player_iter++) {
            if(!player[player_iter].GetBankRupt()) {
                cout << "\n\n\n";
                cout << "Turn for player" << player_iter << "\n";
                player[player_iter].Init();
                // count jail day
                if(player[player_iter].GetInJail()) {
                    player[player_iter].SetJailDay(player[player_iter].GetJailDay() + 1);
                    cout << "----------------" << "\n";
                    cout << "Player is in jail day " << player[player_iter].GetJailDay() << "\n";
                    cout << "----------------" << "\n";
                    if(player[player_iter].GetJailDay() > 3) {
                        player[player_iter].LeaveJail();
                    }
                }
                if(player[player_iter].GetIsRest()) {
                    player[player_iter].SetRestDay(player[player_iter].GetRestDay() + 1);
                    cout << "----------------" << "\n";
                    cout << "Player is in jail day " << player[player_iter].GetRestDay() << "\n";
                    cout << "----------------" << "\n";
                    if(player[player_iter].GetRestDay() > 1) {
                        player[player_iter].LeaveRest();
                    }
                }
                while(player[player_iter].GetOneMore()) {
                    int num_of_move = player[player_iter].ThrowDice();
                    if(player[player_iter].GetInJail()) {
                        // do nothing
                        cout << "Player" << player_iter << " Position : " << player[player_iter].GetPosition() << "\n";
                    }
                    else {
                        player[player_iter].Move(num_of_move);
                        cout << "Player" << player_iter << " Position : " << player[player_iter].GetPosition() << "\n";
                        Event(player[player_iter].GetPosition(), player_iter);
                        if(player[player_iter].GetMoney() < 0) {
                            // player[player_iter].GoBankrupt();
                            ReturnAsset(player_iter);
                            player[player_iter].SetBankRupt(true);
                            cout << "----------------" << "\n";
                            cout << "Player" << player_iter << " go bankrupt" << "\n";
                            cout << "----------------" << "\n";
                            int count_bankrupt = 0;
                            for(int iter = 0; iter < num_of_player; iter++) {
                                if(player[iter].GetBankRupt()) {
                                    count_bankrupt++;
                                }
                            }
                            if(count_bankrupt == num_of_player - 1) {
                                is_gameover = true;
                            }
                            break;
                        }
                    }
                    system("pause");
                }
                if(is_gameover) {
                    break;
                }
            }
        }
    }
    GameOver();
}
void Game::GameOver() {
    cout << "GAMEOVER" << "\n";
}
void Game::Event(int position, int player_num) {
    string option;
    int toll = 0;
    int cost_build = 0; // cost to build house
    int card; // for opportunity and fate card
    switch (map.grid[position].GetLabel())
    {
    // land
    case red: case orange: case yellow: case green: case blue: case light: case purple: case pink:
        // buy land
        if(map.grid[position].GetEmpty()) { 
            if(player[player_num].GetMoney() >= map.grid[position].GetValue()) { // check whether player have enough money
                cout << "----------------" << "\n";
                cout << "Player" << player_num << " have " << player[player_num].GetMoney() << " dollars" << "\n";
                cout << "This land cost " << map.grid[position].GetValue() << " dollars" << "\n";
                cout << "yes to buy, no to not buy" << "\n";
                cout << "----------------" << "\n";
                cin >> option;
                if(option == "yes") {
                    player[player_num].SetMoney(player[player_num].GetMoney() - map.grid[position].GetValue());
                    map.grid[position].SetOwner(player_num);
                    map.grid[position].SetEmpty(false);
                    // update player land list
                    switch (map.grid[position].GetLabel())
                    {
                        case red:
                            player[player_num].grid_red.push_back(position);
                            break;
                        case orange:
                            player[player_num].grid_orange.push_back(position);
                            break;
                        case yellow:
                            player[player_num].grid_yellow.push_back(position);
                            break;
                        case green:
                            player[player_num].grid_green.push_back(position);
                            break;
                        case blue:
                            player[player_num].grid_blue.push_back(position);
                            break;
                        case light:
                            player[player_num].grid_light.push_back(position);
                            break;
                        case purple:
                            player[player_num].grid_purple.push_back(position);
                            break;
                        case pink:
                            player[player_num].grid_pink.push_back(position);
                            break;
                        default:
                            break;
                    }
                    cout << "----------------" << "\n";
                    cout << "Player" << player_num << " have " << player[player_num].GetMoney() << " dollars" << "\n";
                    cout << "This land belong to player" << map.grid[position].GetOwner() << "\n";
                    cout << "----------------" << "\n";
                }
                else if(option == "no") {
                    
                }
            }
        }
        else {
            // pay toll
            if(player_num != map.grid[position].GetOwner()) { // not owner
                toll = map.grid[position].GetCost() + 0.2 * map.grid[position].GetValue() * map.grid[position].GetNumOfHouse();
                cout << "----------------" << "\n";
                cout << "This land belong to player" << map.grid[position].GetOwner() << "\n";
                cout << "Player" << map.grid[position].GetOwner() << " have " << player[map.grid[position].GetOwner()].GetMoney() << " dollars" << "\n";
                cout << "Player" << player_num << " have " << player[player_num].GetMoney() << " dollars" << "\n";
                cout << "Toll are " << toll << " dollars" << "\n";
                cout << "----------------" << "\n";
                
                player[player_num].SetMoney(player[player_num].GetMoney() - toll);
                player[map.grid[position].GetOwner()].SetMoney(player[map.grid[position].GetOwner()].GetMoney() + toll);

                cout << "----------------" << "\n";
                cout << "Player" << map.grid[position].GetOwner() << " have " << player[map.grid[position].GetOwner()].GetMoney() << " dollars" << "\n";
                cout << "Player" << player_num << " have " << player[player_num].GetMoney() << " dollars" << "\n";
                cout << "----------------" << "\n";
            }
            // build house
            else {
                cost_build = map.grid[position].GetValue() * 0.5;
                if(player[player_num].GetMoney() > cost_build) {
                    cout << "----------------" << "\n";
                    cout << "This land belong to you" << "\n";
                    cout << "You have " << player[player_num].GetMoney() << " dollars" << "\n";
                    cout << "Build house cost " << cost_build << " dollars" << "\n";
                    cout << "yes to bulid, no to not build" << "\n";
                    cout << "----------------" << "\n";
                    cin >> option;
                    if(option == "yes") {
                        map.grid[position].SetNumOfHouse(map.grid[position].GetNumOfHouse() + 1);
                        player[player_num].SetMoney(player[player_num].GetMoney() - cost_build);
                    }
                    else if(option == "no") {

                    }

                    cout << "----------------" << "\n";
                    cout << "This land have " << map.grid[position].GetNumOfHouse() << " house" << "\n";
                    cout << "You have " << player[player_num].GetMoney() << " dollars" << "\n";
                    cout << "----------------" << "\n";
                }
            }
        }
        break;
    case station:
        // buy station
        if(map.grid[position].GetEmpty()) { 
            if(player[player_num].GetMoney() >= map.grid[position].GetValue()) { // check whether player have enough money
                cout << "----------------" << "\n";
                cout << "Player" << player_num << " have " << player[player_num].GetMoney() << " dollars" << "\n";
                cout << "This station cost " << map.grid[position].GetValue() << " dollars" << "\n";
                cout << "yes to buy, no to not buy" << "\n";
                cout << "----------------" << "\n";
                cin >> option;
                if(option == "yes") {
                    player[player_num].SetMoney(player[player_num].GetMoney() - map.grid[position].GetValue());
                    map.grid[position].SetOwner(player_num);
                    map.grid[position].SetEmpty(false);
                    player[player_num].grid_station.push_back(position);
                    cout << "----------------" << "\n";
                    cout << "Player" << player_num << " have " << player[player_num].GetMoney() << " dollars" << "\n";
                    cout << "This station belong to player" << map.grid[position].GetOwner() << "\n";
                    cout << "----------------" << "\n";
                }
                else if(option == "no") {
                    
                }
            }
        }
        // pay toll
        else {
            if(player_num != map.grid[position].GetOwner()) { // not owner
                toll = map.grid[position].GetCost() * pow(2, player[map.grid[position].GetOwner()].grid_station.size() - 1);
                cout << "----------------" << "\n";
                cout << "This station belong to player" << map.grid[position].GetOwner() << "\n";
                cout << "Player" << map.grid[position].GetOwner() << " have " << player[map.grid[position].GetOwner()].GetMoney() << " dollars" << "\n";
                cout << "Player" << player_num << " have " << player[player_num].GetMoney() << " dollars" << "\n";
                cout << "Toll are " << toll << " dollars" << "\n";
                cout << "----------------" << "\n";
                
                
                player[player_num].SetMoney(player[player_num].GetMoney() - toll);
                player[map.grid[position].GetOwner()].SetMoney(player[map.grid[position].GetOwner()].GetMoney() + toll);

                cout << "----------------" << "\n";
                cout << "Player" << map.grid[position].GetOwner() << " have " << player[map.grid[position].GetOwner()].GetMoney() << " dollars" << "\n";
                cout << "Player" << player_num << " have " << player[player_num].GetMoney() << " dollars" << "\n";
                cout << "----------------" << "\n";
            }
        }
        break;
    case infrastructure:
        // buy infrastructure
        if(map.grid[position].GetEmpty()) { 
            if(player[player_num].GetMoney() >= map.grid[position].GetValue()) { // check whether player have enough money
                cout << "----------------" << "\n";
                cout << "Player" << player_num << " have " << player[player_num].GetMoney() << " dollars" << "\n";
                cout << "This infrastructure cost " << map.grid[position].GetValue() << " dollars" << "\n";
                cout << "yes to buy, no to not buy" << "\n";
                cout << "----------------" << "\n";
                cin >> option;
                if(option == "yes") {
                    player[player_num].SetMoney(player[player_num].GetMoney() - map.grid[position].GetValue());
                    map.grid[position].SetOwner(player_num);
                    map.grid[position].SetEmpty(false);
                    player[player_num].grid_infrastructure.push_back(position);
                    cout << "----------------" << "\n";
                    cout << "Player" << player_num << " have " << player[player_num].GetMoney() << " dollars" << "\n";
                    cout << "This infrastructure belong to player" << map.grid[position].GetOwner() << "\n";
                    cout << "----------------" << "\n";
                }
                else if(option == "no") {
                    
                }
            }
        }
        // pay toll
        else {
            if(player_num != map.grid[position].GetOwner()) { // not owner
                int times = player[player_num].ThrowDiceForToll(); // throw dice to how many times tolls are
                toll = map.grid[position].GetCost() * times; 

                cout << "----------------" << "\n";
                cout << "This infrastructure belong to player" << map.grid[position].GetOwner() << "\n";
                cout << "Player" << map.grid[position].GetOwner() << " have " << player[map.grid[position].GetOwner()].GetMoney() << " dollars" << "\n";
                cout << "Player" << player_num << " have " << player[player_num].GetMoney() << " dollars" << "\n";
                cout << "The result of dice is " << times << "\n";
                cout << "Toll are " << toll << " dollars" << "\n";
                cout << "----------------" << "\n";

                
                player[player_num].SetMoney(player[player_num].GetMoney() - toll);
                player[map.grid[position].GetOwner()].SetMoney(player[map.grid[position].GetOwner()].GetMoney() + toll);

                cout << "----------------" << "\n";
                cout << "Player" << map.grid[position].GetOwner() << " have " << player[map.grid[position].GetOwner()].GetMoney() << " dollars" << "\n";
                cout << "Player" << player_num << " have " << player[player_num].GetMoney() << " dollars" << "\n";
                cout << "----------------" << "\n";
            }
        }
        break;
    case opportunity:
        card = opportunity_random.back();
        opportunity_random.pop_back();
        switch (card)
        {
        case 0:
            cout << "----------------" << "\n";
            cout << "Player" << player_num << " have " << player[player_num].GetMoney() << " dollars" << "\n";
            cout << "----------------" << "\n";
            player[player_num].SetMoney(player[player_num].GetMoney() - 200);
            cout << "----------------" << "\n";
            cout << "Player" << player_num << " have " << player[player_num].GetMoney() << " dollars" << "\n";
            cout << "----------------" << "\n";
            break;
        case 1:
            cout << "----------------" << "\n";
            cout << "Player" << player_num << " have " << player[player_num].GetMoney() << " dollars" << "\n";
            cout << "----------------" << "\n";
            player[player_num].SetMoney(player[player_num].GetMoney() + 200);
            cout << "----------------" << "\n";
            cout << "Player" << player_num << " have " << player[player_num].GetMoney() << " dollars" << "\n";
            cout << "----------------" << "\n";
            break;
        case 2:
            /* code */
            break;
        case 3:
            /* code */
            break;
        case 4:
            /* code */
            break;
        case 5:
            /* code */
            break;
        case 6:
            /* code */
            break;
        case 7:
            /* code */
            break;
        case 8:
            /* code */
            break;
        case 9:
            /* code */
            break;
        case 10:
            /* code */
            break;
        case 11:
            /* code */
            break;
        case 12:
            /* code */
            break;
        case 13:
            /* code */
            break;
        case 14:
            /* code */
            break;
        case 15:
            /* code */
            break;
        case 16:
            /* code */
            break;
        case 17:
            /* code */
            break;
        case 18:
            /* code */
            break;
        case 19:
            /* code */
            break;
        default:
            break;
        }
        if(opportunity_random.empty()) {
            ShuffleOpportunity();
        }
        break;
    case fate:
        card = fate_random.back();
        fate_random.pop_back();
        switch (card)
        {
        case 0:
            // rest one turn
            player[player_num].Rest();
            break;
        case 1:
            cout << "----------------" << "\n";
            cout << "Player" << player_num << " have " << player[player_num].GetMoney() << " dollars" << "\n";
            cout << "----------------" << "\n";
            player[player_num].SetMoney(player[player_num].GetMoney() - 1000);
            cout << "----------------" << "\n";
            cout << "Player" << player_num << " have " << player[player_num].GetMoney() << " dollars" << "\n";
            cout << "----------------" << "\n";
            break;
        case 2:
            /* code */
            break;
        case 3:
            /* code */
            break;
        case 4:
            /* code */
            break;
        case 5:
            /* code */
            break;
        case 6:
            /* code */
            break;
        case 7:
            /* code */
            break;
        case 8:
            /* code */
            break;
        case 9:
            /* code */
            break;
        case 10:
            /* code */
            break;
        case 11:
            /* code */
            break;
        case 12:
            /* code */
            break;
        case 13:
            /* code */
            break;
        case 14:
            /* code */
            break;
        case 15:
            /* code */
            break;
        case 16:
            /* code */
            break;
        case 17:
            /* code */
            break;
        case 18:
            /* code */
            break;
        case 19:
            /* code */
            break;
        default:
            break;
        }
        if(fate_random.empty()) {
            ShuffleOpportunity();
        }
        break;
    case jail:
        player[player_num].GoJail();
        break;
    case pay:
        switch (position)
        {
        case 4: 
            cout << "----------------" << "\n";
            cout << "Player" << player_num << " have " << player[player_num].GetMoney() << " dollars" << "\n";
            cout << "1 to cost 2000, 2 to cost 10%" << "\n";
            cout << "----------------" << "\n";
            cin >> option;
            if(option == "1") {
                player[player_num].SetMoney(player[player_num].GetMoney() - 2000);
            }
            else if(option == "2") {
                player[player_num].SetMoney(player[player_num].GetMoney() * 0.9);
            }
            cout << "----------------" << "\n";
            cout << "Player" << player_num << " have " << player[player_num].GetMoney() << " dollars" << "\n";
            cout << "----------------" << "\n";
            break;
        case 38:
            cout << "----------------" << "\n";
            cout << "Player" << player_num << " have " << player[player_num].GetMoney() << " dollars" << "\n";
            cout << "----------------" << "\n";
            player[player_num].SetMoney(player[player_num].GetMoney() - map.grid[position].GetValue());
            cout << "----------------" << "\n";
            cout << "Player" << player_num << " have " << player[player_num].GetMoney() << " dollars" << "\n";
            cout << "----------------" << "\n";
        default:
            break;
        }
        break;
    case park:
        // do nothing
        break;
    case jail_visit:
        cout << "----------------" << "\n";
        cout << "1 to go jail, 2 to visit" << "\n";
        cout << "----------------" << "\n";
        cin >> option;
        if(option == "1") {
            player[player_num].GoJail();
        }
        else if(option == "2") {
            // do nothing
        }
        break;
    default:
        break;
    }
}
void Game::Show() {
    // cout << num_of_player << "\n";
    for(int player_iter = 0; player_iter < num_of_player; player_iter++){
        // cout << player[player_iter].GetMoney() << "\n";
        // cout << player[player_iter].GetPosition() << "\n";
    }
    for(int grid_iter = 0; grid_iter < NUM_OF_GRID; grid_iter++){
        // cout << map.GetGrid()[grid_iter].GetLabel() << "\n";
        cout << map.grid[grid_iter].GetValue() << "\n";
    }
}