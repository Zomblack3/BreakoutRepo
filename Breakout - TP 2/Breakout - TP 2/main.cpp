#include "sl.h"

#include <iostream>

using namespace std;

struct Pos
{
	double x = 0.0;
	double y = 0.0;
};

struct Ball
{
	Pos pos;
	double radius = 15.0;
	int numVertices = 15;
};

struct Rectangle
{
	Pos pos;
	int width = 0;
	int height = 0;
	int limitMax = 420;
	int limitMin = 40;
	bool isObjective = false;
};

float randColor();
void buildObjective(Rectangle obj);

int main()
{
	const int screenWidth = 460;
	const int screenHeight = 680;
	const int amountObjectives = 10;

	Ball ball;
	Rectangle player;
	Rectangle objectives[amountObjectives];

	ball.pos.x = screenWidth / 2;
	ball.pos.y = screenHeight / 2;

	player.width = 80;
	player.height = 20;
	player.pos.x = screenWidth / 2;
	player.pos.y = 20;

	for (int i = 0; i < amountObjectives; i++)
	{
		buildObjective(objectives[i]);
	}

	slWindow(screenWidth, screenHeight, "Prueba", false);

	while (!slShouldClose() && !slGetKey(SL_KEY_ESCAPE))
	{
		slSetForeColor(0.1f, 0.5f, 0.9f, 0.4f);

		slRectangleFill(player.pos.x, player.pos.y, player.width, player.height);

		slCircleFill(ball.pos.x, ball.pos.y, ball.radius, ball.numVertices);

		slRender();

		if (slGetKey(SL_KEY_RIGHT))
			if (player.pos.x != player.limitMax)
				player.pos.x += 5;

		if (slGetKey(SL_KEY_LEFT))
			if (player.pos.x != player.limitMin)
				player.pos.x -= 5;
	}

	slClose();
}

float randColor()
{
	float color = rand() % 10;

	return color;
}

void buildObjective(Rectangle obj)
{
	static bool reachLimit = false;
	int posSum = 20;

	obj.width = 20;
	obj.height = 20;
	obj.isObjective = true;

	if (obj.pos.x < obj.limitMax)
	{


		if (obj.pos.x == obj.limitMax)
		{
			obj.pos.x += posSum;
			bool rechLimitMax = true;
		}
		else
		{
			obj.pos.x += posSum;
			obj.pos.y == 600;
		}
	}
	else if (obj.pos.x == obj.limitMax)
	{
		if (obj.pos.x == obj.limitMin)
		{
			obj.pos.x -= posSum;
			bool rechLimitMax = false;
		}
		else
			obj.pos.x -= posSum;
	}
}