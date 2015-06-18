#ifndef _GAME_H
#define _GAME_H

#include <vector>
#include "Piece.h"
#include "FlyCamera.h"

#include <fmod.hpp>
#include <fmod_errors.h>

class AI;

const int ROWS = 8;
const int COLUMNS = 8;

enum STATE
{
	PLAYER_ONE,
	PLAYER_TWO,
	UNKOWN,
};

enum TURN
{
	P1,
	P2,
};

class Game
{
public:
	Game(Camera* camera);
	~Game();

	STATE getGameState();

	std::vector<Piece*> getPieces();
	std::vector<Piece> getAIMoves();

	Piece* getSelectedPiece();

	int checkPieces(Piece* piece);
	void freePiece(int index, int player);

	void cleanPieceArray(int player, int index);

	bool isJumpRequired();
	void checkJumpRequired();

	void setSelectedPiece(Piece* piece);

	void addPiece(int row, int column, int player);

	void getMoves(Piece* piece);
	void getMovesKing(Piece* piece);

	void movePiece(Piece* piece, vec2 target);

	void removePiece(int row, int column);

	void performAction(int action);

	void setUpPieces(int player);

	void playerAction(vec3 position, int row, int column);

	vec2 getBoardPosition(vec3 position);

	vec3 board2WorldPosition(vec2 position);

	Piece* selectPiece(int row, int column);

	int getPieceAt(int row, int column);
	int getMoveAt(int row, int column);
	
	unsigned long long getBit(int row, int column);

	void update();

	void draw();
	
	//Fmod functions
	bool onCreate();
	

private:
	std::vector<Piece> m_moves;
	std::vector<Piece*> m_pieces;
	std::vector<Piece*> m_playerPieces;
	
	long long m_redPieces;
	long long m_redKings;
	long long m_whitePieces;
	long long m_whiteKings;

	long long m_legalMoves;

	AI* m_ai;
	
	bool m_jumpRequired;

	//Sounds
	FMOD::System* m_fmodSystem; //fmod system
	FMOD::Sound* m_music; // sound, music
	FMOD::Channel* m_channel; // channel
	FMOD::ChannelGroup* m_channelGroup;

	STATE m_gameState;
	TURN m_turn;

	FlyCamera* m_camera;

	Piece* m_selectedPiece;

};

#endif