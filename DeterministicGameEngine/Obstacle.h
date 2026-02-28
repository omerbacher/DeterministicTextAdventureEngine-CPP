#pragma once
#include <vector>// for std::vector

//forward declarations
class Player;
class Screen;
class Point;
class Direction;

class Obstacle {
public:
	// Handle obstacle interaction for two players
    static void handleObstacle(Player& pFront, Player& pBack, Screen& screen);
    static constexpr int COLOR = 8; // Gray
private:
    // do the player face an obstacle
    static bool isFacingObstacle(const Player& p, Screen& screen, Point& obstaclePos);

    // do both players align and push
    static bool playersAlignedAndPushing(const Player& front,const Player& back);

    // collect the chain of obstacles
    static std::vector<Point> collectChain(Point start, const Direction& dir, const Screen& screen);

    // push the chain of obstacles
    static void pushChain(const std::vector<Point>& chain, const Direction& dir, Screen& screen);
};
