#include "Piece.h"
#include "Gizmos.h"

Piece::Piece(vec3 position, unsigned int player)
{
	m_position = position;
	m_player = player;
	m_selected = false;
	m_king = false;
	m_jump = false;
}

void Piece::setSelected(bool select)
{
	m_selected = select;
}

vec3 Piece::getPosition()
{
	return m_position;
}

void Piece::setJump(bool isJump)
{
	m_jump = isJump;
}


bool Piece::isJump()
{
	return m_jump;
}

void Piece::setKing(bool isKing)
{
	m_king = isKing;
}

void Piece::setPosition(vec3 position)
{
	m_position = position;
}

unsigned int Piece::getPlayer()
{
	return m_player;
}

bool Piece::isKing()
{
	return m_king;
}

void Piece::draw()
{	
	if (m_player == 1)
	{
		m_color = vec4(1, 1, 1, 1);
		if (isKing()) m_color = vec4(0.8f, 0.8f, 0.8f, 1);
	}
	else if (m_player == 2)
	{
		m_color = vec4(1, 0, 0, 1);
		if (isKing()) m_color = vec4(0.5f, 0.3f, 0.3f, 1);
	}
		

	Gizmos::addDisk(m_position, 0.8f, 6, m_color);
		
}