// main function
#include "Game.h"
#include "SaveGame.h"
#include "LoadGame.h"
#include <iostream>
#include <cstring>

extern bool LOAD_MODE; // flag for load mode
extern bool SAVE_MODE;   // flag for save mode

int main(int argc, char* argv[]) {
        // No arguments - normal game with menu
    if (argc == 1) {
        Game game;
        game.run();
        return 0;
    }

    // Parse command-line arguments
	if (strcmp(argv[1], "-save") == 0) { // Save mode - ignore -silent flag if provided (it's ignored in save mode)

        SAVE_MODE = true;
        SaveGame game;
        game.run();
    }
    else if (strcmp(argv[1], "-load") == 0) 
    { // load mode
        bool silent = false;
        LOAD_MODE = true;
        if (argc > 2 && strcmp(argv[2], "-silent") == 0)
        { // silent mode
            silent = true;
        }
        LoadGame game(silent);
        game.run();
    }
    else {
		std::cout << "ERROR: Unknown argument '" << argv[1] << "'\n"; // unknown argument
    }

    return 0;
}

// End of main.cpp