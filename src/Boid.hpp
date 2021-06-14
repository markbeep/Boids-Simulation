class Boid
{
public:
	Boid();
	Boid(float startX, float startY, float startingAngle);
	void move(int width, int height);
	float distanceTo(Boid* b);
	float angleTo(Boid* b);
	float angleTo(float x, float y);
	float x;
	float y;
	float angle;
	float fieldOfView;
	float viewDistance;
	float separation;
};