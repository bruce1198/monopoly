#include "player.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

using namespace std;
int RandInt(int n) {
    if ((n - 1) == RAND_MAX) {
        return rand();
    } 
    else {
        
        long end = RAND_MAX / n;
        assert (end > 0L);
        end *= n;
        int r;
        while ((r = rand()) >= end);
        return r % n;
    }
}

void Player::Init() {
    one_more = true;
    continuous_move = 0;
}
Player::Player() {
    
}
int Player::ThrowDice() {
    srand(time(NULL));
    int max = 6;
    int min = 1;

    int dice_one = RandInt(max - min + 1) + min;
    int dice_two = RandInt(max - min + 1) + min;
    

    // dice_one = 4;
    // dice_two = 4;
    cout << dice_one << " " << dice_two << "\n";
    if(in_jail) {
        if(dice_one == dice_two) {
            LeaveJail();
        }
        one_more = false;
    }
    else {
        // again
        if (dice_one == dice_two) {
            continuous_move++;
            if(continuous_move == 3) {
                GoJail();
            }
            else {
                one_more = true;
            }
        }
        else {
            one_more = false;
        }
    }
    
    return dice_one + dice_two;
}

int Player::PureThrowDice() {
    srand(time(NULL));
    int max = 6;
    int min = 1;

    int dice_one = RandInt(max - min + 1) + min;
    int dice_two = RandInt(max - min + 1) + min;
    

    cout << dice_one << " " << dice_two << "\n";

    return dice_one + dice_two;
}

void Player::Move(int num_of_move) {
    position += num_of_move;
    // position = 39;
    // pass start
    if(position >= 40) {
        money += 2000;
        position %= 40;
    }
    
}
void Player::Rest() {
    is_rest = true;
    one_more = false;
}
void Player::LeaveRest() {
    is_rest = false;
    rest_day = 0;
}
void Player::GoJail() {
    in_jail = true;
    position = 10;
    one_more = false;
}
void Player::LeaveJail() {
    in_jail = false;
    jail_day = 0;
}