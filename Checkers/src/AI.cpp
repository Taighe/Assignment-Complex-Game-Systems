#include "AI.h"
#include "Game.h"

AI::AI(Game* game)
{
	m_game = game;
}

void AI::update()
{
	if (m_game->getPieces().size() == 0)
	{
		return;
	}

	performAction(m_game->getPieces().size() - 1);  //randomPick(m_game->getPieces().size() - 1)
}

void AI::performAction(int action)
{
	if (action < 0)
		action = 0;
	
	int col = m_game->getBoardPosition( m_game->getPieces()[action]->getPosition()).x;
	int row = m_game->getBoardPosition( m_game->getPieces()[action]->getPosition()).y;

	m_game->setSelectedPiece( m_game->selectPiece(row, col) );

	int _moves = m_game->getAIMoves().size();
	
	if (_moves == 0)
	{
		return;
	}
		
	action = _moves - 1;//randomPick(_moves);

	vec2 target = m_game->getBoardPosition(m_game->getAIMoves()[action].getPosition());

	m_game->movePiece(m_game->getSelectedPiece(), target);
		
}

int AI::randomPick(int range)
{
	if (range <= 0)
		return 0;

	return rand() % range;
}