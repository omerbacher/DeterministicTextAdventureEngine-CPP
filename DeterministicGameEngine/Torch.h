#pragma once
class Player; // Forward declaration
class Screen; // Forward declaration

class Torch
{
public:
	static constexpr char TORCH_CHAR = '!';   // Character representing the torch
	static bool playerHasTorch(const Player& p);   // Check if the player has the torch
	static void handleTorch(const Player& p, Screen& screen);  // Handle torch logic
	static constexpr int COLOR = 12; // Red
};
