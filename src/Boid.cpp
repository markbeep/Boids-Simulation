#include <Boid.hpp>
#include <math.h>

#define PI 3.1415926

Boid::Boid()
{}

Boid::Boid(float startX, float startY, float startingAngle)
{
	x = startX;
	y = startY;
	angle = startingAngle;
	fieldOfView = 180.f;
	viewDistance = 100.f;
	separation = 15.f;
}

void Boid::move(int width, int height)
{
	x += sin(angle * PI / 180.f);
	y -= cos(angle * PI / 180.f);
	if (x <= 0)
		x = width - 1;
	if (x >= width)
		x = 1;
	if (y <= 0)
		y = height - 1;
	if (y >= height)
		y = 1;
}

// relative distance and angle to another boid
float Boid::angleTo(Boid* b)
{
	return angleTo(b->x, b->y);
}

float Boid::angleTo(float x, float y)
{
	float deg = atan2(y - this->y, x - this->x) - atan2(-1, 0);
	deg = deg * 180 / PI;
	if (deg < 0)
		deg += 360;
	deg -= angle;
	if (deg < 0)
		deg += 360;
	return deg;
}

float Boid::distanceTo(Boid* b)
{
	return abs(b->x - x) + abs(b->y - y);
}