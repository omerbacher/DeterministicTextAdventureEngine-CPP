#pragma once

#include <string>
#include <vector>

struct StateSnapshot {
    // Player 1 data
    int p1_x = 0, p1_y = 0, p1_lives = 0, p1_score = 0, p1_item_id = -1;
    char p1_item = 0;

    // Player 2 data
    int p2_x = 0, p2_y = 0, p2_lives = 0, p2_score = 0, p2_item_id = -1;
    char p2_item = 0;

    // Game data
    int level = 0, timer = 0;
    bool timer_active = false, switches_on = false;
    bool door_open[10] = { false };

	// Map data - keep room data
	char mapData[3][22][80] = {};  // 3 rooms, each 22 rows of 80 chars

    // Meta
    std::string screens = "";
};

class GameStateManager {
public:
    
	static StateSnapshot* showLoadMenu();    // show load menu
	static bool showSaveMenu(const StateSnapshot& snap);  //show save menu

	static bool save(const StateSnapshot& state, const std::string& saveName); // save game state
	static bool load(StateSnapshot& state, const std::string& saveName); // load game state
	static std::vector<std::string> listSaves(); // list all saves
	static bool exists(const std::string& saveName); // check if save exists
	static bool deleteSave(const std::string& saveName); // delete a save

private:
	static const std::string FOLDER; // folder for saves
	static std::string getMetaFilename(const std::string& saveName); // get full filename for a save
};
