#include "Bomb.h"
#include "Screen.h"
#include "Player.h"
#include "Results.h"
#include "Utils.h"
#include "SaveGame.h"
#include <cmath> // For std::abs
#include <iostream>

// Update signature to match Bomb.h (3 arguments)
void Bomb::explode(Screen& screen, Player& p1, Player& p2, bool isInCurrentRoom)
{
  
    int centerX = position.getX(); // Center of explosion
    int centerY = position.getY(); // Center of explosion

    // 1. & 2. Delegate environmental damage to the Screen (Room)
    screen.explodeBomb(centerX, centerY);
    if (isInCurrentRoom)
    {
        // 3. Check for player damage in range 3 (Chebyshev distance)
        // If player is within 3 cells (including diagonals), they lose a life
        if (std::abs(p1.getX() - centerX) <= EXPLOSION_RANGE && std::abs(p1.getY() - centerY) <= EXPLOSION_RANGE) {
            p1.loseLife();
        }
        if (std::abs(p2.getX() - centerX) <= EXPLOSION_RANGE && std::abs(p2.getY() - centerY) <= EXPLOSION_RANGE) {
            p2.loseLife();
        }
    }
}

bool Bomb::tick(Screen& screen, Player& p1, Player& p2, int currentRoomID)
{

    // Decrease internal timer
    timer--;

    bool isInCurrentRoom = (this->roomID == currentRoomID);
    if (timer <= 0)
    {
        explode(screen, p1, p2, isInCurrentRoom); // Trigger explosion with player references
        return true;     // Bomb has finished its life cycle
    }
    return false;        // Bomb is still active


}

void Bomb::attachResults(Results* results, size_t timer) // Attach results to the results file
{
    gameResults = results;
    eventTimer = timer;
}