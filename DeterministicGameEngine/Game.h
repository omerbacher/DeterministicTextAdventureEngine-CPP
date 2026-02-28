#pragma once
#include <array> // for std::array (taken from AI)
#include "Player.h"
#include "Results.h"
#include "Screen.h"
#include "RiddleBank.h"
#include "UIScreens.h"
#include "Utils.h"
#include "Bomb.h"
#include "RoomMeta.h"
#include <vector>   // vector for storing screen file names
#include <string>   // for string handling
#include <filesystem> // for directory iteration
#include "Steps.h"  

struct StateSnapshot; // forward declaration
enum class Mode { REGULAR, SAVE, LOAD };

class Game {
	friend class GameStateManager;
protected:

	// Constants
    static constexpr int ESC = 27;
    static constexpr int GAME_DELAY = 80;

	// Player starting positions
    static constexpr int P1_START_X = 2;
    static constexpr int P1_START_Y = 2;
    static constexpr int P2_START_X = 77;
    static constexpr int P2_START_Y = 2;
	// Menu keys
	static constexpr char START_KEY = '1'; // start new game
	static constexpr char INSTRUCTIONS_KEY = '8'; // show instructions
	static constexpr char EXIT_KEY = '9'; // exit game
	static constexpr char Colors_ON_OFF = '2'; // toggle colors
	static constexpr char LOAD_GAME_KEY = '3'; // quick load game

	void drawCurrentScreen(); // draw the current screen
	void redrawGame();         // redraw game elements
	void updateBomb();        // update bomb state
	void drawActiveBomb();   // draw the active bomb
	void updatePlayers();      // draw both players
	virtual bool checkGameOver();      // check if game over
	void updateDisplay();	 // update players and status bar
    void getAllScreenFileNames(std::vector<std::string>& vec_to_fill); 	//function to get all screen file names from the directory
	void showMenu(); // show main menu
	void showInstructions(); // show instructions screen
	void initLevel(const std::string& filename, int specificDoor = -1);  //run the game
	virtual void gameLoop();// main game loop -> virtual for derived classes       
	bool handleTile(Player& player);// handle tile interaction for a player
	void showWinScreen(); // show win screen
	void placePlayersAtEntrance(int apecificDoor = -1);// place players next to the door they entered from (manager)
	void resetGame();// reset game to initial state
	void placeNextToDoor(const Point& targetDoorPos);// place player next to a specific door (helper)
	void handlePause(Screen& currentScreen, bool& gameRunning);
	virtual bool checkLevel();// check if level is completed
	void quickLoad();                              // quick load last saved game
	void createSaveSnapshot(StateSnapshot& snap);  // create a snapshot of the current game state for saving

	std::vector<std::string> screenFileNames; // list of screen file names
	Screen currentScreen;             // current game screen      
	RoomMeta currentRoomMeta;       // current room metadata
	std::vector<Screen> allLevels;    // all game levels
	int currentLevelIdx = 0;         // current level index         
	size_t eventTimer = 0;		// game timer
	
	char activeDoor = ' '; // to track last door used
	GameModes currStatus = GameModes::MENU; // Game modes - options of the game
    Player player1;
    Player player2;
	RiddleBank riddleBank; // riddle bank to manage riddles
	Results gameResults; // game results logger
    
	Point player1LastPos; // last position of player 1
	Point player2LastPos;// last position of player 2
    Bomb* activeBomb = nullptr;

	int gameTimer;           // Current time remaining
	int maxGameTime;         // Starting time 
	bool timerActive;        // Is timer running
    
	Steps recordedSteps;          // steps for replay mode
	Mode currentMode = Mode::REGULAR; //current game mode
	bool silentMode = false;      // silent mode flag

public:
	Game(); // constructor
	// virtual destructor to handle cleanup
	virtual ~Game() {
		if (activeBomb != nullptr) {
			delete activeBomb;
			activeBomb = nullptr;
		}
	}
	Game(const Game&) = delete; // delete copy ctor
	Game& operator=(const Game&) = delete; // delete assignment operator

	virtual void run(); // main game loop
	static bool pauseRequestedFromRiddle; // stop in the middle of riddle
};

