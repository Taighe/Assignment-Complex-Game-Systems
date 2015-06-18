#ifndef _AI_H
#define _AI_H

class Game;

class AI
{
public:
	AI(Game* game);

	void update();

	int randomPick(int range);

	void performAction(int action);

private:
	Game* m_game;
};

#endif