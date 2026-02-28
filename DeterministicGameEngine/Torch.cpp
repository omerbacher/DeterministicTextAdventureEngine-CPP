#include "Torch.h"
#include "Player.h"
#include "Screen.h"

bool Torch::playerHasTorch(const Player& p) // check if player has torch
{
    return p.hasItem(TORCH_CHAR); 
}

void Torch::handleTorch(const Player& p, Screen& screen) // handle torch lighting
{
	if (!screen.isDark()) // if screen is not dark
    {
		screen.drawMap(); // draw normal map
        return;
    }

    if (playerHasTorch(p))
    {
		screen.drawMapWithTorch(p); // draw map with torchlight
    }
    else
    {
		screen.drawDark(); // draw completely dark screen
    }
}
