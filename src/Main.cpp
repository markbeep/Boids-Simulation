#include "Platform/Platform.hpp"
#include <Boid.hpp>
#include <cstdlib>
#include <math.h>
#include <string>

#define PI 3.1415926

int main()
{
	util::Platform platform;

#if defined(_DEBUG)
	std::cout << "Running window..." << std::endl;
#endif

	sf::RenderWindow window;
	// in Windows at least, this must be called before creating the window
	float screenScalingFactor = platform.getScreenScalingFactor(window.getSystemHandle());

	// Use the screenScalingFactor
	window.create(sf::VideoMode(800.0f * screenScalingFactor, 500.0f * screenScalingFactor), "Boids go brrr");

	sf::Font font;
	if (!font.loadFromFile("content/rainyhearts.ttf"))
	{
		return -1;
	}

	int n = 100;
	Boid* boids = new Boid[n];
	for (int i = 0; i < n; i++)
	{
		boids[i] = Boid(rand() % window.getSize().x, rand() % window.getSize().y, rand() % 360);
	}

	bool DEBUG = false;

	sf::Event event;

	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.clear(sf::Color::White);

		unsigned int xSize = window.getSize().x;
		unsigned int ySize = window.getSize().y;

		// draws the boids
		for (int i = 0; i < n; i++)
		{
			Boid* boid = &boids[i];

			// move the boids a step forward
			boid->move(xSize, ySize);

			sf::ConvexShape triangle;
			triangle.setPointCount(3);
			// defines the three points of the triangle
			triangle.setPoint(0, sf::Vector2f(5.f, 0.f));
			triangle.setPoint(1, sf::Vector2f(0.f, 15.f));
			triangle.setPoint(2, sf::Vector2f(10.f, 15.f));
			triangle.setOrigin(sf::Vector2f(5.f, 5.f));

			triangle.setFillColor(sf::Color::Blue);

			triangle.setPosition(sf::Vector2f(boid->x, boid->y));
			triangle.setRotation(boid->angle);

			window.draw(triangle);

			// draws the view range around the boids
			if (DEBUG)
			{
				// VIEW DISTANCE
				sf::CircleShape circle(boid->viewDistance);
				circle.setOutlineThickness(3.f);
				circle.setOrigin(sf::Vector2f(boid->viewDistance, boid->viewDistance));
				circle.setPosition(sf::Vector2f(boid->x, boid->y));
				circle.setOutlineColor(sf::Color::Red);
				circle.setFillColor(sf::Color(255, 255, 255, 0));
				window.draw(circle);

				// SEPARATION CIRCLE
				sf::CircleShape separation(boid->separation);
				separation.setOutlineThickness(3.f);
				separation.setOrigin(sf::Vector2f(boid->separation, boid->separation));
				separation.setPosition(sf::Vector2f(boid->x, boid->y));
				separation.setOutlineColor(sf::Color::Green);
				separation.setFillColor(sf::Color(255, 255, 255, 0));
				window.draw(separation);
			}

			// cohesion
			float averageX = 0;
			float averageY = 0;
			float amountNearby = 0;

			// separation
			Boid* closest = nullptr;

			// alignment
			float totalAngle = 0;

			for (int j = 0; j < n; j++)
			{
				if (j == i)
					continue;

				Boid* other = &boids[j];
				float angleTo = boid->angleTo(other);
				float distance = boid->distanceTo(other);
				// gets the closest other boid in the field of view (180°)
				if ((angleTo > 360 - boid->fieldOfView / 2 || angleTo < boid->fieldOfView / 2) && distance <= boid->viewDistance)
				{
					averageX += other->x;
					averageY += other->y;
					amountNearby++;
					totalAngle += other->angle;
				}

				// we get the closest boid for the separation part
				if (distance < boid->separation && (closest == nullptr || distance < boid->distanceTo(closest)))
				{
					closest = other;
				}
			}
			// COHESION: determine the angle change for cohesion
			if (amountNearby > 0)
			{
				// slowly turn left if its to the left, otherwise slowly turn right
				float angleTo = boid->angleTo(averageX / amountNearby, averageY / amountNearby);
				if (angleTo > 180)
					angleTo -= 360;
				boid->angle += angleTo / 45;

				// ALIGNMENT: determine the angle change depending on nearby angles
				float averageAngle = totalAngle / amountNearby;
				if (boid->angle > averageAngle)
					averageAngle++;
				else
					averageAngle--;
			}
			// SEPARATION: we also want to try to get away if we're too close together
			if (closest != nullptr)
			{
				float angleTo = boid->angleTo(closest);
				if (angleTo > 180)
					angleTo -= 360;
				boid->angle -= angleTo / 5 / boid->distanceTo(closest);
			}

			// draw coords above the boids
			/*sf::Text text;
			text.setFont(font);
			text.setString("(" + std::to_string(boid->x) + ", " + std::to_string(boid->y) + ", " + std::to_string(boid->angle) + ")");
			text.setFillColor(sf::Color::Black);
			text.setPosition(sf::Vector2f(boid->x, boid->y));
			text.setCharacterSize(10);
			window.draw(text);*/
		}
		sf::sleep(sf::milliseconds(3));

		window.display();
	}

	return 0;
}
