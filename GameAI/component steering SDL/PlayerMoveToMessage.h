#pragma once

#include "GameMessage.h"
#include "Vector2D.h"
#include "Event.h"

class Vector2D;

class PlayerMoveToMessage:public Event
{
public:
	PlayerMoveToMessage( const Vector2D& pos );
	~PlayerMoveToMessage();

	void process();

private:
	Vector2D mPos;
};