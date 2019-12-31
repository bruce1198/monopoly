#include "logicmap.h"
#include <iostream>
using namespace std;
LogicMap::LogicMap(){
    for(int iter = 0; iter < NUM_OF_GRID; iter++) {
        grid.push_back(Grid());
    }
    // initialize grid label
    grid[0].SetLabel(start);
    grid[1].SetLabel(purple);
    grid[2].SetLabel(fate);
    grid[3].SetLabel(purple);
    grid[4].SetLabel(pay);
    grid[5].SetLabel(station);
    grid[6].SetLabel(light);
    grid[7].SetLabel(opportunity);
    grid[8].SetLabel(light);
    grid[9].SetLabel(light);
    grid[10].SetLabel(jail);
    grid[11].SetLabel(pink);
    grid[12].SetLabel(infrastructure);
    grid[13].SetLabel(pink);
    grid[14].SetLabel(pink);
    grid[15].SetLabel(station);
    grid[16].SetLabel(orange);
    grid[17].SetLabel(fate);
    grid[18].SetLabel(orange);
    grid[19].SetLabel(orange);
    grid[20].SetLabel(park);
    grid[21].SetLabel(red);
    grid[22].SetLabel(opportunity);
    grid[23].SetLabel(red);
    grid[24].SetLabel(red);
    grid[25].SetLabel(station);
    grid[26].SetLabel(yellow);
    grid[27].SetLabel(yellow);
    grid[28].SetLabel(infrastructure);
    grid[29].SetLabel(yellow);
    grid[30].SetLabel(jail_visit);
    grid[31].SetLabel(green);
    grid[32].SetLabel(green);
    grid[33].SetLabel(opportunity);
    grid[34].SetLabel(green);
    grid[35].SetLabel(station);
    grid[36].SetLabel(fate);
    grid[37].SetLabel(blue);
    grid[38].SetLabel(pay);
    grid[39].SetLabel(blue);
    // initialize grid label
    grid[0].SetValue(0);
    grid[1].SetValue(600);
    grid[2].SetValue(0);
    grid[3].SetValue(600);
    grid[4].SetValue(0);
    grid[5].SetValue(2000);
    grid[6].SetValue(1000);
    grid[7].SetValue(0);
    grid[8].SetValue(1000);
    grid[9].SetValue(1200);
    grid[10].SetValue(0);
    grid[11].SetValue(1400);
    grid[12].SetValue(1500);
    grid[13].SetValue(1500);
    grid[14].SetValue(1600);
    grid[15].SetValue(2000);
    grid[16].SetValue(1800);
    grid[17].SetValue(0);
    grid[18].SetValue(1800);
    grid[19].SetValue(2000);
    grid[20].SetValue(0);
    grid[21].SetValue(2200);
    grid[22].SetValue(0);
    grid[23].SetValue(2200);
    grid[24].SetValue(2400);
    grid[25].SetValue(1500);
    grid[26].SetValue(2600);
    grid[27].SetValue(2600);
    grid[28].SetValue(2000);
    grid[29].SetValue(2800);
    grid[30].SetValue(0);
    grid[31].SetValue(3000);
    grid[32].SetValue(3000);
    grid[33].SetValue(0);
    grid[34].SetValue(3200);
    grid[35].SetValue(2000);
    grid[36].SetValue(0);
    grid[37].SetValue(3500);
    grid[38].SetValue(750);
    grid[39].SetValue(4000);

    for(int iter = 0; iter < NUM_OF_GRID; iter++) {
        if(iter == 12 || iter == 28) { // infrastructure
            grid[iter].SetCost(100);
        }
        else {
            grid[iter].SetCost(grid[iter].GetValue() * 0.4);
        }
    }
    for(int iter = 0; iter < NUM_OF_GRID; iter++) {
        grid[iter].SetBuild(grid[iter].GetValue() * 0.5);
    }

}
