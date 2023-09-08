#include "FlexNode.h"
#include <iostream>

//void uninitializedFlexNodeOptions(int &userChoice);
//void initializationOptions();

int main() {
    FlexNode myFlexNode;
    int userChoice;
    std::cout   << "You have an uninitialized FlexNode.\n"
                << "Choose from the following:\n"
                << "1. Initialize FlexNode\n"
                << "2. Exit Program"
                << std::endl;
    
    std::cin >> userChoice;
    switch(userChoice) {
        case 1: /*Prompt Initialization Choices*/
        case 2: /*Output Exit Message*/
    }

    return EXIT_SUCCESS;
}