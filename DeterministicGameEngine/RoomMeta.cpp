#include "RoomMeta.h"
#include "Utils.h"
#include "Screen.h"
#include <sstream>
#include <iostream>


//  RoomMeta implementation 
RoomMeta::RoomMeta()
{
    clear();
}

void RoomMeta::clear()
{
    dark = false;
    roomKey = Key(-1);
    
    for (int i = 0; i < MAX_DOORS; i++)
    {
		doorOpen[i] = false; // all doors closed by default
		doorLeadsTo[i] = -1; // initialize to -1 (no destination)
		doorInvalidState[i] = false; // all states valid by default
    }
	// clear riddle positions
    riddleCount = 0;
    lightSwitch.exists = false;
    lightSwitch.x = -1;
    lightSwitch.y = -1;

	// clear key position
    keyPosition.exists = false;
    keyPosition.x = -1;
    keyPosition.y = -1;
	keyPosition.doorID = -1;
}

void RoomMeta::loadFromLine(const std::string& line) // parse a metadata line helped by AI
{
    // Skip empty lines and non-metadata lines
    if (line.empty() || line[0] != '#')
        return;

    std::istringstream iss(line);
    std::string hash, key;

    iss >> hash >> key;

    //  DARK 
    if (key == "DARK")
    {
        std::string value;
        iss >> value;
        dark = stringToBool(value);
    }

    //  KEY 
    else if (key == "KEY")
    {
        int x, y;
        std::string opensToken;
		if (iss >> x >> y >> opensToken) // Expected format: # KEY x y opens=1
        {
            if (opensToken.find("opens=") == 0)
            {
				std::string numStr = opensToken.substr(6);// take after "opens="
				if (!numStr.empty())// ensure there's a number
                {
					int doorId = std::stoi(numStr);// Extract door ID
                    setKeyPosition(x, y, doorId);
                    roomKey = Key(doorId);
                }
            }
        }
    }

    //  DOOR 
    else if (key == "DOOR")
    { // Expected format: # DOOR id=1 state=open leads=2
        std::string token;
        int id = -1;
        bool open = false;
        int leads = -1;
        std::string stateStr = "";
        while (iss >> token) // Read each token
        {
            if (token.find("id=") == 0)
            {// door ID
                std::string numStr = token.substr(3);  // take after "id="
                id = std::stoi(numStr); // Extract ID from the end of the token
            }
            // door state
            else if (token.find("state=") == 0)
                stateStr = token.substr(6); // Check if state is "open"

            // where the door leads
            else if (token.find("leads=") == 0)
            {
                std::string numStr = token.substr(6);  // take after "leads="
                leads = std::stoi(numStr) - 1;
            }
        }

        // Set door properties if ID is valid
        if (id >= 0 && id < MAX_DOORS)
        {
			// set door state
            if (stateStr == "open" || stateStr == "OPEN")
                doorOpen[id] = true;
            else if (stateStr == "closed" || stateStr == "CLOSED")
                doorOpen[id] = false;
            else if (!stateStr.empty())
            {
				// invalid state specified
                doorOpen[id] = false;  // default to closed
                doorInvalidState[id] = true;  
            }

            doorLeadsTo[id] = leads;
        }
    }

	// RIDDLE 
    else if (key == "RIDDLE")
    {
        // Expected format: # RIDDLE x y id , Example: # RIDDLE 4 22 1
        int x, y, riddleID;
        if (iss >> x >> y >> riddleID)
        {
            addRiddlePosition(riddleID, x, y);
        }
    }
    else if (key == "LIGHT_SWITCH") {
        int x, y;
        if (iss >> x >> y) {
			setLightSwitch(x, y);
        }
    }
}
// Light switch methods
void RoomMeta::setLightSwitch(int x, int y)
{
    lightSwitch.x = x;
    lightSwitch.y = y;
    lightSwitch.exists = true;
}
    bool RoomMeta::hasLightSwitchAt(int x, int y) const {
        return lightSwitch.exists && lightSwitch.x == x && lightSwitch.y == y;
    }

    void RoomMeta::activateLightSwitch() {
        dark = !dark; // Turn the lights on
    }
    


// Setters 

void RoomMeta::setDark(bool value)
{
    dark = value;
}

void RoomMeta::setDoorOpen(int id, bool open)
{
    if (id >= 0 && id < MAX_DOORS)
        doorOpen[id] = open;
}

void RoomMeta::setKeyOpens(int id)
{
	roomKey = Key(id);  // Create key that opens the specified door
}
// Key position methods
void RoomMeta::setKeyPosition(int x, int y, int doorID)
{
    keyPosition.x = x;
    keyPosition.y = y;
    keyPosition.doorID = doorID;
    keyPosition.exists = true;
}
bool RoomMeta::hasKeyPosition() const // check if key position is set
{
    return keyPosition.exists;
}

int RoomMeta::getKeyX() const // getter for key x position
{
    return keyPosition.x;
}

int RoomMeta::getKeyY() const // getter for key y position
{
    return keyPosition.y;
}

int RoomMeta::getKeyDoorID() const // getter for key door ID
{
    return keyPosition.doorID;
}
int RoomMeta::getKeyOpens() const
{
	return roomKey.getDoorID();  // Return the door ID that the key opens
}

Key RoomMeta::getRoomKey() const  // getter for room key
{
    return roomKey;
}

// Getters
bool RoomMeta::isDark() const
{
    return dark;
}

bool RoomMeta::isDoorOpen(int id) const
{
    if (id >= 0 && id < 10)
        return doorOpen[id];
    return false;
}

int RoomMeta::getDoorLeadsTo(int id) const
{
    if (id >= 0 && id < 10)
        return doorLeadsTo[id];
    return -1;
}

bool RoomMeta::isDoorStateInvalid(int id) const
{
        if (id >= 0 && id < 10)
            return doorInvalidState[id];
        return false;
}
// Riddle position management
void RoomMeta::addRiddlePosition(int riddleID, int x, int y)
{
    if (riddleCount < MAX_RIDDLES)
    {
        riddlePositions[riddleCount].riddleID = riddleID;
        riddlePositions[riddleCount].x = x;
        riddlePositions[riddleCount].y = y;
        riddleCount++;
    }
}

int RoomMeta::getRiddleCount() const
{
    return riddleCount;
}

int RoomMeta::getRiddleID(int index) const
{
    if (index >= 0 && index < riddleCount)
        return riddlePositions[index].riddleID;
    return -1;
}

int RoomMeta::getRiddleX(int index) const
{
    if (index >= 0 && index < riddleCount)
        return riddlePositions[index].x;
    return -1;
}

int RoomMeta::getRiddleY(int index) const
{
    if (index >= 0 && index < riddleCount)
        return riddlePositions[index].y;
    return -1;
}

// Place light switch from metadata - only appears at correct position if valid
void RoomMeta::placeLightSwitchFromMetadata(Screen& screen) const
{
    if (lightSwitch.exists)
    {
        int switchX = lightSwitch.x;
        int switchY = lightSwitch.y;

        // Remove all 'A' characters except at metadata position
        for (int y = 0; y < 22; y++)
        {
            for (int x = 0; x < 80; x++)
            {
                if (screen.getCharAt(x, y) == 'A')
                {
                    // Remove 'A' if it's NOT at the metadata position
                    if (x != switchX || y != switchY)
                    {
                        screen.setCharAt(x, y, ' ');
                    }
                }
            }
        }

        // Check if there's already an 'A' at the correct position
        char currentChar = screen.getCharAt(switchX, switchY);

        if (currentChar == 'A')
        {
            // Already there and at correct position, keep it
            return;
        }
        else if (currentChar == ' ')
        {
            // Position is empty, place the light switch
            screen.setCharAt(switchX, switchY, 'A');
        }
        
    }
    else
    {
        // No metadata position defined, remove ALL 'A' characters
        for (int y = 0; y < 22; y++)
        {
            for (int x = 0; x < 80; x++)
            {
                if (screen.getCharAt(x, y) == 'A')
                {
                    screen.setCharAt(x, y, ' ');
                }
            }
        }
    }
}

// Validate light switch position - no error, just don't place if invalid
bool RoomMeta::validateLightSwitchPosition(const Screen& screen, const std::string& filename) const
{
    // No validation errors - we just won't place the 'A' if position is invalid
    return true;
}