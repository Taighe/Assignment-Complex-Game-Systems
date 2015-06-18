#ifndef _PIECE_H
#define _PIECE_H

#include "glm_header.h"

class Piece
{
public:
	Piece(vec3 position, unsigned int player);

	void draw();
	vec3 getPosition();
	unsigned int getPlayer();
	void setPosition(vec3 position);
	void setSelected(bool select);

	void setKing(bool isKing);
	bool isKing();

	void setJump(bool isJump);
	bool isJump();


private:
	vec4 m_color;
	vec3 m_position;
	unsigned int m_player;
	bool m_king;
	bool m_jump;

	bool m_selected;

	
};

#endif