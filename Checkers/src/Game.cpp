#include "Game.h"
#include <GLFW\glfw3.h>
#include "Gizmos.h"
#include "AI.h"

Game::Game(Camera* camera)
{
	m_camera = (FlyCamera*)camera;
	m_selectedPiece = nullptr;
	m_whitePieces = 0;
	m_redPieces = 0;
	m_jumpRequired = false;
	
	setUpPieces(1);
	setUpPieces(2);

	//setting up fmod
	onCreate();
	m_fmodSystem->createSound("./sounds/relaxed.ogg", FMOD_LOOP_NORMAL, 0, &m_music);
	m_fmodSystem->createChannelGroup("background", &m_channelGroup);
	m_fmodSystem->playSound(m_music, m_channelGroup, false, &m_channel);

	m_ai = new AI(this);

	m_gameState = UNKOWN;
}

Game::~Game()
{
	m_music->release();
	m_fmodSystem->close();
	m_fmodSystem->release();
}

int Game::checkPieces(Piece* piece)
{
	if (piece->getPlayer() == 1)
	{
		for (int i = 0; i < m_playerPieces.size(); i++)
		{
			if (m_playerPieces[i]->getPosition() == piece->getPosition())
			{
				return i;
			}
		}
	}
	else if (piece->getPlayer() == 2)
	{
		for (int i = 0; i < m_pieces.size(); i++)
		{
			if (m_pieces[i]->getPosition() == piece->getPosition())
			{
				return i;
			}
		}
	}
	else if (piece->getPlayer() == 3)
	{
		for (int i = 0; i < m_moves.size(); i++)
		{
			if (m_moves[i].getPosition() == piece->getPosition())
			{
				return i;
			}
		}
	}

	return -1;
	
}

void Game::freePiece(int index, int player)
{
	if (player == 1)
	{
		delete m_playerPieces[index];
		m_playerPieces[index] = nullptr;
		m_playerPieces.erase(m_playerPieces.begin() + index);
	}
	
	if (player == 2)
	{
		delete m_pieces[index];
		m_pieces[index] = nullptr;
		m_pieces.erase(m_pieces.begin() + index);
	}
	
}

void Game::cleanPieceArray(int player, int index)
{
	if (player == 1)
	{
		m_pieces.erase(m_playerPieces.begin() + index);
	}
		
	if (player == 2)
	{
		m_pieces.erase(m_pieces.begin() + index);
	}

}

std::vector<Piece*> Game::getPieces()
{
	return m_pieces;
}

std::vector<Piece> Game::getAIMoves()
{
	return m_moves;
}

Piece* Game::getSelectedPiece()
{
	return m_selectedPiece;
}

void Game::setSelectedPiece(Piece* piece)
{
	m_selectedPiece = piece;
}

void Game::performAction(int action)
{

}

void Game::removePiece(int row, int column)
{
	long long pos = getBit(row, column);

	m_legalMoves ^= pos;

}

void Game::movePiece(Piece* piece, vec2 target)
{
	vec2 boardPos = getBoardPosition(piece->getPosition());
	long long pos = getBit(boardPos.y, boardPos.x);
	long long dest = getBit(target.y, target.x);

	if (piece->getPlayer() == 1)
	{
		int playerPiece = checkPieces(piece);

		vec2 t = target;
		if (getPieceAt(target.y, target.x) == 2) //if the piece is jumping, else it just moves the piece to the target
		{
			vec2 diff = vec2(target.x - boardPos.x, target.y - boardPos.y);

			t = vec2(target.x + diff.x, target.y + diff.y);
			long long jump = getBit(t.y, t.x);

			Piece redPiece = Piece(board2WorldPosition(target), 2);
			int index = checkPieces(&redPiece);
			freePiece(index, 2);

			m_redPieces &= ~dest;
			dest = jump;
		}
		
		m_whitePieces &= ~pos;
		m_whitePieces |= dest;

		if (playerPiece != -1)
			m_playerPieces[playerPiece]->setPosition(board2WorldPosition(t));
		
		if (playerPiece != -1 && m_playerPieces[playerPiece]->isKing() == false && t.y == 0)
		{
			m_whitePieces ^= dest;
			m_whiteKings ^= dest;
			m_playerPieces[playerPiece]->setKing(true);
		}

	}
	
	if (piece->getPlayer() == 2)
	{
		int aiPiece = checkPieces(piece);

		vec2 t = target;
		if (getPieceAt(target.y, target.x) == 1) //if the piece is jumping, else it just moves the piece to the target
		{
			vec2 diff = vec2(target.x - boardPos.x, target.y - boardPos.y);
			t = vec2(target.x + diff.x, target.y + diff.y);
			long long jump = getBit(t.y, t.x);
			
			Piece whitePiece = Piece(board2WorldPosition(target), 1);
			int index = checkPieces(&whitePiece);
			freePiece(index, 1);
			m_whitePieces &= ~dest;
			dest = jump;
		}
		
		m_redPieces &= ~pos;
		m_redPieces |= dest;
		
		if (aiPiece != -1)
			m_pieces[aiPiece]->setPosition(board2WorldPosition(t));

		if (aiPiece != -1 && m_pieces[aiPiece]->isKing() == false && t.y == ROWS - 1)
		{
			m_redPieces ^= dest;
			m_redKings ^= dest;
			m_pieces[aiPiece]->setKing(true);
		}
	}

	m_moves.clear(); //make sure to clear moves before end of turn
	m_legalMoves = 0;

	m_turn = (TURN)!m_turn;
}

vec3 Game::board2WorldPosition(vec2 position)
{
	vec3 pos = vec3(-COLUMNS + 1 + position.x * 2, 0.1f, -ROWS + 1 + position.y * 2);

	return pos;
}

void Game::getMoves(Piece* piece)
{	
	if (piece->isKing())
	{
		getMovesKing(piece);
		return;
	}
	
	vec2 pos = getBoardPosition(piece->getPosition());
	
	int offsetY = 0;

	bool right = false;
	bool left = false;

	if (piece->getPlayer() == 1)
	{
		offsetY = -1;
	}
	else
		offsetY = 1;
	
	//can I move right?
	int test = getPieceAt((int)pos.y + offsetY, (int)pos.x + 1); 

	vec3 rightTestPos = board2WorldPosition( vec2( pos.x + 1, pos.y + offsetY ) );

	Piece p = Piece(vec3(), piece->getPlayer() );
	
	if (test == 0)
	{
		p.setPosition(rightTestPos);
		addPiece(getBoardPosition(rightTestPos).y, getBoardPosition(rightTestPos).x, 3);
		p.setJump(false);
		m_moves.push_back(p);
	}
	//can I jump over a piece on the right?
	else if (piece->getPlayer() != test && test > 0 && test < 3) 
	{
		test = getPieceAt((int)pos.y + (offsetY * 2), (int)pos.x + 2);
		if (test == 0)
		{
			right = true;
			p.setPosition(rightTestPos);
			addPiece(getBoardPosition(rightTestPos).y, getBoardPosition(rightTestPos).x, 3);
			p.setJump(true);
			m_moves.push_back(p);
		}
	}

	//can I move left?
	test = getPieceAt((int)pos.y + offsetY, (int)pos.x - 1);
	vec3 leftTestPos = board2WorldPosition(vec2(pos.x - 1, pos.y + offsetY));

	if (test == 0)
	{
		p.setPosition(leftTestPos);
		addPiece(getBoardPosition(leftTestPos).y, getBoardPosition(leftTestPos).x, 3);
		p.setJump(false);
		m_moves.push_back(p);
	}
	//can I jump over a piece on the left?
	else if (piece->getPlayer() != test && test > 0 && test < 3)
	{
		test = getPieceAt((int)pos.y + (offsetY * 2), (int)pos.x - 2);
		if (test == 0)
		{
			left = true;
			p.setPosition(leftTestPos);
			addPiece(getBoardPosition(leftTestPos).y, getBoardPosition(leftTestPos).x, 3);
			p.setJump(true);
			m_moves.push_back(p);
		}
	}
	
	//if (m_moves.size() > 1)
	//{
	//	if (left == true && right == false)
	//	{
	//		removePiece(getBoardPosition(rightTestPos).y, getBoardPosition(rightTestPos).x);
	//	}

	//	if (left == false && right == true)
	//	{
	//		removePiece(getBoardPosition(leftTestPos).y, getBoardPosition(leftTestPos).x);
	//	}
	//}

}

void Game::getMovesKing(Piece* piece)
{
	vec2 pos = getBoardPosition(piece->getPosition());

	int offsetY = 0;

	bool jump = false;

	offsetY = 1;

	//can I move right?
	int test = getPieceAt((int)pos.y + offsetY, (int)pos.x + 1);

	vec3 rightTestPos = board2WorldPosition(vec2(pos.x + 1, pos.y + offsetY));

	Piece p = Piece(vec3(), piece->getPlayer());

	if (test == 0)
	{
		p.setPosition(rightTestPos);
		addPiece(getBoardPosition(rightTestPos).y, getBoardPosition(rightTestPos).x, 3);
		m_moves.push_back(p);
	}
	//can I jump over a piece on the right?
	else if (piece->getPlayer() != test && test > 0 && test < 3)
	{
		test = getPieceAt((int)pos.y + (offsetY * 2), (int)pos.x + 2);
		if (test == 0)
		{
			p.setPosition(rightTestPos);
			addPiece(getBoardPosition(rightTestPos).y, getBoardPosition(rightTestPos).x, 3);
			jump = true;
			p.setJump(true);
			m_moves.push_back(p);
		}
	}

	//can I move left?
	test = getPieceAt((int)pos.y + offsetY, (int)pos.x - 1);
	vec3 leftTestPos = board2WorldPosition(vec2(pos.x - 1, pos.y + offsetY));

	if (test == 0)
	{
		p.setPosition(leftTestPos);
		addPiece(getBoardPosition(leftTestPos).y, getBoardPosition(leftTestPos).x, 3);
		p.setJump(false);
		m_moves.push_back(p);
	}
	//can I jump over a piece on the left?
	else if (piece->getPlayer() != test && test > 0 && test < 3)
	{
		test = getPieceAt((int)pos.y + (offsetY * 2), (int)pos.x - 2);
		if (test == 0)
		{
			p.setPosition(leftTestPos);
			addPiece(getBoardPosition(leftTestPos).y, getBoardPosition(leftTestPos).x, 3);
			jump = true;
			p.setJump(true);
			m_moves.push_back(p);
		}
	}

	offsetY = -1;

	//can I move right?
	test = getPieceAt((int)pos.y + offsetY, (int)pos.x + 1);

	rightTestPos = board2WorldPosition(vec2(pos.x + 1, pos.y + offsetY));

	p = Piece(vec3(), piece->getPlayer());

	if (test == 0)
	{
		p.setPosition(rightTestPos);
		addPiece(getBoardPosition(rightTestPos).y, getBoardPosition(rightTestPos).x, 3);
		m_moves.push_back(p);
	}
	//can I jump over a piece on the right?
	else if (piece->getPlayer() != test && test > 0 && test < 3)
	{
		test = getPieceAt((int)pos.y + (offsetY * 2), (int)pos.x + 2);
		if (test == 0)
		{
			p.setPosition(rightTestPos);
			addPiece(getBoardPosition(rightTestPos).y, getBoardPosition(rightTestPos).x, 3);
			jump = true;
			p.setJump(true);
			m_moves.push_back(p);
		}
	}

	//can I move left?
	test = getPieceAt((int)pos.y + offsetY, (int)pos.x - 1);
	leftTestPos = board2WorldPosition(vec2(pos.x - 1, pos.y + offsetY));

	if (test == 0)
	{
		p.setPosition(leftTestPos);
		addPiece(getBoardPosition(leftTestPos).y, getBoardPosition(leftTestPos).x, 3);
		p.setJump(false);
		m_moves.push_back(p);
	}
	//can I jump over a piece on the left?
	else if (piece->getPlayer() != test && test > 0 && test < 3)
	{
		test = getPieceAt((int)pos.y + (offsetY * 2), (int)pos.x - 2);
		if (test == 0)
		{
			p.setPosition(leftTestPos);
			addPiece(getBoardPosition(leftTestPos).y, getBoardPosition(leftTestPos).x, 3);
			jump = true;
			p.setJump(true);
			m_moves.push_back(p);
		}
	}

	if (m_moves.size() > 1 && jump == true)
	{
		for (int i = 0; i < m_moves.size(); i++)
		{
			if (m_moves[i].isJump() == false)
			{
				vec2 pos = getBoardPosition(m_moves[i].getPosition());
				removePiece(pos.y, pos.x);
				m_moves[i] = Piece(vec3(), 0);
			}
		}
	}
}

void Game::setUpPieces(int player)
{
	if (player == 1)
	{
		int offsetX = 0;

		for (int i = 0; i < COLUMNS / 2; i++)
		{
			addPiece(7, offsetX, player);
			offsetX += 2;
		}

		offsetX = 1;

		for (int i = 0; i < COLUMNS / 2; i++)
		{
			addPiece(6, offsetX, player);
			offsetX += 2;
		}

		offsetX = 0;

		for (int i = 0; i < COLUMNS / 2; i++)
		{
			addPiece(5, offsetX, player);
			offsetX += 2;
		}
	}
	else
	{
		int offsetX = 1;

		for (int i = 0; i < COLUMNS / 2; i++)
		{
			addPiece(0, offsetX, player);
			offsetX += 2;
		}

		offsetX = 0;

		for (int i = 0; i < COLUMNS / 2; i++)
		{
			addPiece(1, offsetX, player);
			offsetX += 2;
		}

		offsetX = 1;

		for (int i = 0; i < COLUMNS / 2; i++)
		{
			addPiece(2, offsetX, player);
			offsetX += 2;
		}
	}
}

unsigned long long Game::getBit(int row, int column)
{
	long long pos = ((long long)1 << (row * 8 + column));
	return pos;
}

void Game::addPiece(int row, int column, int player)
{
	long long pos = getBit(row, column);
	
	if (player == 1)
	{
		m_whitePieces |= pos;
		m_playerPieces.push_back(new Piece(board2WorldPosition(vec2(column, row)), 1));
	}
		
	if (player == 2)
	{
		m_redPieces |= pos;
		m_pieces.push_back( new Piece(board2WorldPosition(vec2(column, row)), 2) );	
	}

	if (player == 3)
	{
		m_legalMoves |= pos;
	}
		
}

int Game::getMoveAt(int row, int column)
{
	if (row < 0 || row > ROWS - 1)
		return -1;

	if (column < 0 || column > COLUMNS - 1)
		return -1;

	long long pos = getBit(row, column);
	
	bool test = (m_legalMoves & pos) > 0;

	if (test)
		return 1;

	return 0;
}

int Game::getPieceAt(int row, int column)
{
	if (row < 0 || row > ROWS - 1)
		return -1;

	if (column < 0 || column > COLUMNS - 1)
		return -1;

	long long pos = getBit(row, column);

	bool test = (m_whitePieces & pos) > 0;

	if (test)
		return 1;
	
	test = (m_whiteKings & pos) > 0;

	if (test)
		return 11;

	test = (m_redPieces & pos) > 0;

	if (test)
		return 2;

	test = (m_redKings & pos) > 0;

	if (test)
		return 21;

	return 0;

}

Piece* Game::selectPiece(int row, int column)
{
	m_moves.clear();
	m_legalMoves = 0;
	delete m_selectedPiece;

	Piece* piece = nullptr;

	long long loc = getBit(row, column);
	
	vec3 pos = vec3(-COLUMNS + 1 + column * 2, 0.1f, -ROWS + 1 + row * 2);
	
	bool test = (m_whitePieces & loc) > 0;

	if (test)
	{
		piece = new Piece(pos, 1);
	}
		
	test = (m_whiteKings & loc) > 0;

	if (test)
	{
		piece = new Piece(pos, 1);
		piece->setKing(true);
	}

	test = (m_redPieces & loc) > 0;

	if (test)
	{
		piece = new Piece(pos, 2);
	}
		

	test = (m_redKings & loc) > 0;

	if (test)
	{
		piece = new Piece(pos, 2);
		piece->setKing(true);
	}

	if (piece != nullptr)
		getMoves(piece);

	return piece;
}

vec2 Game::getBoardPosition(vec3 position)
{
	int column = (int)((position.x + COLUMNS) / 2);
	int row = (int)((position.z + ROWS) / 2);

	return vec2(column, row);
}

bool Game::isJumpRequired()
{
	return m_jumpRequired;
}

void Game::checkJumpRequired()
{	
	std::vector<Piece*> _pieces;
	if (m_turn == P1) _pieces = m_playerPieces;
	if (m_turn == P2) _pieces = m_pieces;

	for (int i = 0; i < _pieces.size(); i++)
	{
		getMoves(_pieces[i] );
		for (int j = 0; j < m_moves.size(); j++)
		{
			if (m_moves[j].isJump() )
			{
				m_jumpRequired = true;
				m_legalMoves = 0;
				m_moves.clear();
				return;
			}			
		}			
	}

	m_jumpRequired = false;
}

void Game::playerAction(vec3 position, int row, int column)
{
	if (getMoveAt(getBoardPosition(position).y, getBoardPosition(position).x) == 1)
	{
		movePiece(m_selectedPiece, getBoardPosition(position));
	}
	else
	{
		m_selectedPiece = selectPiece(row, column);
	}
	
	if (m_selectedPiece != nullptr)
	{
		m_selectedPiece->setSelected(true);

		if (m_selectedPiece->getPlayer() == 2)
		{
			delete m_selectedPiece;
			m_selectedPiece = nullptr;
			m_legalMoves = 0;
			m_moves.clear();
		}
	}
}

void Game::update()
{
	double x = 0, y = 0;
	glfwGetCursorPos(glfwGetCurrentContext(), &x, &y);
	vec3 pickPos = m_camera->pickAgainstPlane((float)x, (float)y, vec4(0, 1, 0, 0));
	int column = (int)((pickPos.x + COLUMNS) / 2);
	int row = (int)((pickPos.z + ROWS) / 2);

	if (m_turn == P1)
	{
		if (glfwGetMouseButton(glfwGetCurrentContext(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{
			playerAction(pickPos, row, column);	
		}
	}
	else
	{		
		if (m_ai != nullptr)
		{
			m_ai->update();
		}
	}

	m_fmodSystem->update();

}

void Game::draw()
{
	vec4 color = vec4(0, 1, 0, 1);
	
	if (m_selectedPiece != nullptr)
		Gizmos::addAABB(vec3(m_selectedPiece->getPosition().x, 0.1f, m_selectedPiece->getPosition().z), vec3(1, 0, 1), color);

	color = vec4(1, 0, 1, 1);

	for (int row = 0; row < ROWS; row++)
	{
		for (int col = 0; col < COLUMNS; col++)
		{
			vec3 pos = board2WorldPosition(vec2(col, row));
			
			if (getPieceAt(row, col) == 1)
			{
				Piece piece = Piece(pos, 1);
				piece.draw();
			}
			
			if (getPieceAt(row, col) == 11)
			{
				Piece piece = Piece(pos, 1);
				piece.setKing(true);
				piece.draw();
			}

			if (getPieceAt(row, col) == 2)
			{
				Piece piece = Piece(pos, 2);
				piece.draw();
			}

			if (getMoveAt(row, col) == 1)
			{
				Gizmos::addAABB(pos, vec3(1, 0, 1), color);
			}
				
		}
	}

}

bool Game::onCreate()
{
	FMOD_RESULT result;
	m_fmodSystem = nullptr;

	const char* error = "FMOD error! (%d) %s \n";

	result = FMOD::System_Create(&m_fmodSystem);

	if (result != FMOD_OK)
	{
		printf(error, result, FMOD_ErrorString(result));
		return false;
	}

	// Initialize FMOD
	result = m_fmodSystem->init(512, FMOD_INIT_NORMAL, 0);

	if (result != FMOD_OK)
	{
		printf(error, result, FMOD_ErrorString(result));
		return false;
	}

	return true;
}