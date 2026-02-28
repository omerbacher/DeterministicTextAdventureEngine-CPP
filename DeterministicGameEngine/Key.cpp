#include "Key.h"
#include "Screen.h"
#include "RoomMeta.h"
#include "Constants.h"
#include <iostream>

using  BoardSymbols::KEY;
// Static helper function to check if a position is valid for placing a key
bool Key::isValidPosition(const Screen& screen, int x, int y)
{
    if (x < 0 || x >= Screen::WIDTH || y < 0 || y >= Screen::MAP_HEIGHT)
        return false;

    char cell = screen.getCharAt(x, y);
    // Key can only be placed on empty space
    return (cell == ' '||cell== KEY ||cell=='k');
}

// Static helper function to check if there's already a 'K' in the map
bool Key::existsInMap(const Screen& screen)
{
    for (int y = 0; y < Screen::MAP_HEIGHT; y++)
    {
        for (int x = 0; x < Screen::WIDTH; x++)
        {
            if (screen.getCharAt(x, y) == KEY)
                return true;
        }
    }
    return false;
}

// Static function to place key from metadata - metadata is the authority
void Key::placeFromMetadata(Screen& screen)
{
    const RoomMeta& roomMeta = screen.getRoomMeta();

    // If there's a key position in metadata, use it as the authority
    if (roomMeta.hasKeyPosition())
    {
        int metaKeyX = roomMeta.getKeyX();
        int metaKeyY = roomMeta.getKeyY();

        // First, remove ALL existing 'K' characters from the map
        removeAllKeysFromMap(screen);

        // Then place the key at the metadata position (if valid)
        if (isValidPosition(screen, metaKeyX, metaKeyY))
        {
            if (Screen::isSilent())
            {
                screen.setCharAtSilent(metaKeyX, metaKeyY, KEY);
            }
            else
            {
                screen.setCharAt(metaKeyX, metaKeyY, KEY);
            }
        }
    }
}
// Helper function to remove all 'K' characters from the map
void Key::removeAllKeysFromMap(Screen& screen)
{
    for (int y = 0; y < Screen::MAP_HEIGHT; y++)
    {
        for (int x = 0; x < Screen::WIDTH; x++)
        {
            if (screen.getCharAt(x, y) == KEY)
            {
                screen.setCharAt(x, y, ' ');  // Replace with empty space
            }
        }
    }
}

// Static function to validate key position from metadata
bool Key::validateMetadataPosition(const Screen& screen, const std::string& filename)
{
    const RoomMeta& roomMeta = screen.getRoomMeta();

    // Check if key position is defined in metadata
    if (!roomMeta.hasKeyPosition())
        return true;  // No key position defined, nothing to validate

    int keyX = roomMeta.getKeyX();
    int keyY = roomMeta.getKeyY();

    
    // Validate that the position is on free space
    if (!isValidPosition(screen, keyX, keyY))
    {
        std::cout << "\n========================================" << std::endl;
        std::cout << "ERROR in " << filename << std::endl;
        std::cout << "Invalid KEY position at (" << keyX << ", " << keyY << ")" << std::endl;
        std::cout << "The key must be placed on empty space (' ')." << std::endl;
        char currentChar = screen.getCharAt(keyX, keyY);
        if (currentChar == 'W' || currentChar == 'w')
            std::cout << "Position contains a wall." << std::endl;
        else if (currentChar != ' ')
            std::cout << "Position contains: '" << currentChar << "'" << std::endl;
        std::cout << "========================================\n" << std::endl;
        return false;
    }

    return true;
}