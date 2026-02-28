#pragma once
#include <string>
#include <vector>
#include <fstream>
#include "Key.h"
#include "Door.h"

class RoomMeta {
private:
    //  Room state
    bool dark;                 // Is the room initially dark
    bool doorOpen[10];         // doorOpen[1] -> door with id 1
    int doorLeadsTo[10];   // where each door leads
    bool doorInvalidState[10];
    Key roomKey;              // Key object for the room

    static constexpr int MAX_DOORS = 10;
    static constexpr int MAX_RIDDLES = 6;

	//  Riddle positions 
    struct RiddlePosition {
        int riddleID;
        int x;
        int y;
    };
	// Light switch position
    struct LightSwitch {
        int x, y;
        bool exists;
    } lightSwitch;

    // Key position
    struct KeyPosition {
        int x;
        int y;
        int doorID;  // Which door this key opens
        bool exists;
    } keyPosition;

    RiddlePosition riddlePositions[6]; // Max 6 riddles per room
    int riddleCount;
    
public:
    RoomMeta();
    void clear();
	//  load room metadata from a line
    void loadFromLine(const std::string& line);

    static constexpr int LIGHT_SWITCH_COLOR = 14; // Yellow
    // setters
    void setDark(bool v);
    void setDoorOpen(int id, bool open);
    void setKeyOpens(int id);
	
    // getters
    bool isDark() const;
    bool isDoorOpen(int id) const;
    int getKeyOpens() const;
    int getDoorLeadsTo(int id) const;
	Key getRoomKey() const;

    bool isDoorStateInvalid(int id) const;
        // Riddle position management
    void addRiddlePosition(int riddleID, int x, int y);
    int getRiddleCount() const;
    int getRiddleID(int index) const;
    int getRiddleX(int index) const;
    int getRiddleY(int index) const;
   
	// Light switch management
    void setLightSwitch(int x, int y);
    bool hasLightSwitchAt(int x, int y) const;
    void activateLightSwitch();
	
    // Key position methods
    void setKeyPosition(int x, int y, int doorID);
    bool hasKeyPosition() const;
    int getKeyX() const;
    int getKeyY() const;
    int getKeyDoorID() const;

    // Light switch metadata placement
    void placeLightSwitchFromMetadata(class Screen& screen) const;
    bool validateLightSwitchPosition(const class Screen& screen, const std::string& filename) const;
};
