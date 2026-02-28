Game Files Format Documentation

Overview
This document explains the format of two game files used for recording and replaying games:
1.adv-world.steps - Contains player input (key presses)
2.adv-world.result - Contains game events and outcomes

File: adv-world.steps

Purpose
Records all player inputs (key presses) together with their corresponding game iteration time and player identifier, allowing the game to be replayed exactly as it happened.

Format

First Line:

Loading the screens: <screen_file_1>|<screen_file_2>|<screen_file_3>

Lists all screen files used in the game, separated by `|`

Number of steps: <N>
Total number of input events recorded in the file.

Following Lines (Player Input):

TIME: <time> PLAYER: <player_id> KEY: <key>


TIME= Game time counter (starts at 0, increments each iteration)

PLAYER=Identifies the source of the input:

1 – Player 1

2 – Player 2

0 – Input context (e.g., riddle or numeric input)

KEY – The key pressed:

- Movement and action keys (W, A, S, D, I, J, K, L, X, M, etc.)
- Digits (0–9) for numeric input
- An empty value represents an Enter action (end of input)

Example

Loading the screens: adv-world_01.screen|adv-world_02.screen|adv-world_03.screen
Number of steps: 82
TIME: 7 PLAYER: 1 KEY: x
TIME: 26 PLAYER: 1 KEY: d
TIME: 29 PLAYER: 0 KEY: y
TIME: 30 PLAYER: 0 KEY: 0
TIME: 31 PLAYER: 0 KEY: 1
TIME: 32 PLAYER: 0 KEY: 2
TIME: 33 PLAYER: 0 KEY:
TIME: 34 PLAYER: 1 KEY: d

Notes
1.The time values do not need to be continuous.
2.The number of input lines must match the value specified in Number of steps.
3.Screen names must match existing screen files in the game directory.


File: adv-world.result

Purpose
Records all significant game events (riddles, life loss, screen changes, game end) with their outcomes.

Format

First Line:

screens= <screen_file_1>|<screen_file_2>|<screen_file_3>

Lists the same screen files used for the steps file.

Following Lines (depends on the Game Events):

1. Riddle Event

time= <time> event= Riddle riddleId= <id> riddleText= <text> answer= <answer> result= <Solved|Failed>

time= - When the riddle was answered
riddleId= - Unique identifier of the riddle
riddleText= - The full text of the riddle question
answer= - The player's answer
result= - Either `Solved` or `Failed`

2. Life Lost Event

time= <time> event= LifeLost

time=  When the player lost a life

3. Screen Change Event

time= <time> event= ScreenChange screenName= <filename>

time= When the player moved to a new screen
screenName= The filename of the new screen

4. Game End Event

time= <time> event= GameEnd score1= <score> score2= <score>

time=When the game ended
score1=Final score of player 1 (&)
score2=Final score of player 2 ($)

Example

screens= adv-world_01.screen|adv-world_02.screen
time= 45 event= LifeLost
time= 123 event= Riddle riddleId= 2 riddleText= What does this print? answer= 012 result= Solved
time= 204 event= Riddle riddleId= 1 riddleText= What's the output? answer= 7 result= Solved
time= 499 event= ScreenChange screenName= adv-world_02.screen
time= 555 event= LifeLost
time= 605 event= GameEnd score1= 200 score2= 100

Game End States

The adv-world.result file always ends with a final game state, represented by the GameEnd event.
 The game can terminate in one of the following three states:

1. GAME_FINISHED  
   The player successfully completed the game and won.

2. GAME_END  
   The player lost the game due to death (e.g., no remaining lives).

3. GAME_PAUSED  
   The player chose to stop the game manually before it ended.

These states represent the final outcome of the game session and appear only once,as the last event recorded in the result file.

Notes

1.Time Counter: Starts at 0 and increments by 1 for each game iteration. Not actual seconds.
2.Screen Names: Must match the actual screen filenames in the game directory.
3.Riddle Text: Can contain newlines and special characters. Will be preserved as-is in the file.
4.Multiple Answers: If a player answers the same riddle multiple times, each attempt is recorded.



Creating Custom Files

To manually create or edit these files:

1.Copy existing files - as a template
2.Edit time values- Ensure they are sequential and make sense
3.Maintain format- Keep spacing and structure consistent
4.Verify screen names- Make sure they match actual files in the game directory
5.Test- Run the game with `-load` to verify the files work correctly


Example: Creating a Simple Test Game

Create `adv-world.steps`:

Loading the screens: adv-world_01.screen
Number of steps: 4
TIME: 0 PLAYER: 1 KEY: W
TIME: 2 PLAYER: 1 KEY: W
TIME: 4 PLAYER: 1 KEY: D
TIME: 10 PLAYER: 1 KEY: E

Create `adv-world.result`:

screens= adv-world_01.screen
time= 120 event= GameEnd score1= 500 score2= 450

Then run: `adv-world.exe -load`
