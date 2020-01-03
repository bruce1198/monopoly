#include "grid.h"
#include <iostream>

using namespace std;


Grid::Grid(){
    value = 0;
}
void Grid::Init() {
    is_empty = true;
    owner = -1;
    num_of_house = 0;
}
