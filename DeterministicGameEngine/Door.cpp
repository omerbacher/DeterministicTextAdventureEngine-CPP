#include "Door.h"
#include <iostream>
#include <Windows.h>
#include "Screen.h"

// Static member initialization
bool Door::switchesAreOn = false;
bool Door::openDoors[MAX_DOOR_ID] = { false }; // static array to track open doors
static bool justWarned = false;

//DOOR LOGIC
bool Door::tryOpen(int keyId)
{
	if (open)// the door is already open
	{
		return true;
	}

	if (!switchesAreOn) // switches are not all on
	{
		return false;
	}

	if (keyId == id) // macthed key for door
	{
		open = true;
		return true;
	}
	return false;
}

// Handle door interaction for the player (helped by AI)
bool Door::handleDoor(Player& p, Screen& screen, char& foundDoor,bool isSilent)
{
	if (!p.isActive()) return false; // do nothing if player is inactive

	Point playerPos = p.getPosition(); // get player's current position

	// Check directions of the player
	int dirX[] = { 1, -1, 0, 0 };
	int dirY[] = { 0, 0, 1, -1 };

	// Loop through each direction
	for (int i = 0; i < 4; i++)
	{
		Point checkPos = playerPos;
		checkPos.set(playerPos.getX() + dirX[i], playerPos.getY() + dirY[i]);// position next to player

		char cell = screen.getCharAt(checkPos);

		// Check if the cell is a door
		if (isDoorChar(cell))
		{
			int doorIndex = cell - '0';
			Door* door = screen.getDoor(checkPos);// get door object
			if (door == nullptr) continue;
			// If door is already open, move player through
			if (Door::openDoors[doorIndex]) {
				p.setInactive();
				p.erase();
				foundDoor = cell;
				return true;
			}
			bool doorCanPass = false;
			bool doorOpen = door->isOpen();
			if (!doorOpen)// door is closed
			{
				if (!Door::switchesAreOn) // Check if switches are on
				{
					if (!isSilent)
					{
						screen.drawAnimatedBox(10, 5, 50, 12);// draw message box
						gotoxy(13, 7);
						std::cout << "you cannot enter";
						gotoxy(15, 9);
						std::cout << "All switches must be ON!";
						Sleep(1200);
						screen.closeAnimatedBox(10, 5, 50, 12);

					}
					
					p.stepBack();// move player back
					if (!isSilent)
					{
						screen.drawMap(); // redraw map to clear any artifacts
					}
					p.draw(); // redraw player
					return false;
				}
				char heldKey = p.getHeldItem();
				int keyId = p.getItemId();
				if (heldKey == 'K' && keyId == doorIndex)// player is holding a key that matches the door
				{
					door->setOpen();
					Door::openDoors[doorIndex] = true;
					p.keyUsed();// mark key as used
					if (!isSilent)
					{
						screen.drawAnimatedBox(10, 5, 50, 12);
						gotoxy(17, 7);
						std::cout << "Door " << doorIndex << " unlocked!";
						Sleep(1100);
						screen.closeAnimatedBox(10, 5, 50, 12);
					}
					doorCanPass = true;
				}
				else // player is not holding a key that matches the door 
				{
					if (!isSilent)
					{
						screen.drawAnimatedBox(10, 5, 50, 12);
						gotoxy(17, 7);
						std::cout << "you need the correct key!";
						Sleep(1100);
						screen.closeAnimatedBox(10, 5, 50, 12);
					}
					
					p.stepBack();
					if (!isSilent)
					{
						screen.drawMap(); // redraw map to clear any artifacts
					}
					p.draw(); // redraw player
					return false;
				}
			}
			else
			{
				doorCanPass = true; // door is open
			}
			if (doorCanPass) // move player through door
			{
				p.setInactive(); // deactivate player
				p.erase(); // erase player from current position
				foundDoor = cell;
				return true;
			}
		}

	}
	return false;
}