#pragma once

#include <Trackable.h>
#include <Vector2D.h>

class GraphicsBuffer;

class Score :public Trackable
{
public:
	Score(const Vector2D& pos);
	~Score();

	void addToScore(int addScore);
	int getCurrentScore();
	void draw(GraphicsBuffer* pBuffer);
	void draw();

private:
	Vector2D mPos;
	int mScore;
	GraphicsBuffer* mBuffer;

};