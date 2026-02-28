#pragma once
#include <string>
class Screen; // forward declaration
class Key
{
private:
    int doorID;   // The Id of the door this key opens

public:
    Key(int id = -1): doorID(id) {} // constractor

    bool isKey() const {return doorID != -1;} // checks if it is a key

    int getDoorID() const {return doorID; } // returns the door id

    bool matches(int d) const {return doorID == d;} // checks if the key matches the door id

    bool isValid() const {return doorID != -1;}  // check if the key is valid

	static constexpr int COLOR = 6; // Gold color for key display
    // Static methods for key management in screens
    static bool isValidPosition(const Screen& screen, int x, int y);
    static bool existsInMap(const Screen& screen);
    static void placeFromMetadata(Screen& screen);
    static bool validateMetadataPosition(const Screen& screen, const std::string& filename);
private:
    // Helper function to remove all keys from map
    static void removeAllKeysFromMap(Screen& screen);
};

