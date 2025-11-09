#pragma once

#include "context/ball.h"
#include "context/misc.h"
#include <box2d/box2d.h>
#include <memory>

class Physics {

private:
	context::Misc &cm;

	int ballSize;
	int region;
	b2WorldId world;
	std::vector<b2BodyId> bl;
	std::shared_ptr<context::BallGroup> bg;
	std::vector<b2BodyId> brick;

public:
	Physics(context::Misc &cm, std::shared_ptr<context::BallGroup> bg);
	~Physics();

	void update(float dt);

private:
	void _update(float deltaTime);
	void _updateBall(int idx);
	b2BodyId createBall(std::shared_ptr<context::Ball> b);
	void createWall();
	void createBrick();
	bool contactCheck(b2ShapeId *shapeId);

	bool ballSpeed(b2Vec2 &v);
	bool ballSpeedClassic(b2Vec2 &v);
};
