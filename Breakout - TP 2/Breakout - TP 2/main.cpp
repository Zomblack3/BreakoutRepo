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
	double radius = 10.0;
	int numVertices = 15;
	float speedY = 10;
	float speedX = 5;
	bool isActive = false;
};

struct Rectangle
{
	Pos pos;
	int width = 0;
	int height = 0;
	int limitMax = 420;
	int limitMin = 40;
	bool isObjective = false;
	bool isActive = true;
	float r = 0;
	float g = 0;
	float b = 0;
};

float randColor();

void buildObjective(Rectangle& obj, int actualObj);

bool BallHasCollide(float cX, float cY, float cRadius, float rX, float rY, float rW, float rH);

int main()
{
	srand(time(0));

	const int screenWidth = 460;
	const int screenHeight = 680;
	const int amountObjectives = 20;

	int screenCenterX = screenWidth / 2;
	int screenCenterY = screenHeight / 2;

	Ball ball;
	Rectangle player;
	Rectangle objectives[amountObjectives];

	ball.pos.x = screenCenterX;
	ball.pos.y = screenCenterY;

	player.width = 80;
	player.height = 20;
	player.pos.x = screenWidth / 2;
	player.pos.y = 20;

	for (int i = 0; i < amountObjectives; i++)
	{
		buildObjective(objectives[i], i);
	}

	slWindow(screenWidth, screenHeight, "BREAKOUT", false);

	while (!slShouldClose() && !slGetKey(SL_KEY_ESCAPE))
	{
		if (ball.isActive == true)
		{
			ball.pos.x += ball.speedX;
			ball.pos.y += ball.speedY;
		}
		else
		{
			ball.pos.x = player.pos.x;
			ball.pos.y = (player.pos.y * 2) + 15;
		}

		if ((ball.pos.x >= (screenWidth - ball.radius)) || (ball.pos.x <= ball.radius))
			ball.speedX *= -1.0f;

		if (ball.pos.y >= (screenHeight - ball.radius))
			ball.speedY *= -1.0f;
		else if (ball.pos.y <= ball.radius)
		{
			ball.pos.x = player.pos.x;
			ball.pos.y = player.pos.y * 2;
			ball.isActive = false;

			if (ball.speedY > 0)
				ball.speedY *= -1.0f;
		}

		if (BallHasCollide(ball.pos.x, ball.pos.y, ball.radius, player.pos.x, player.pos.y, player.width, player.height))
			ball.speedY *= -1.0f;

		for (int i = 0; i < amountObjectives; i++)
			if (BallHasCollide(ball.pos.x, ball.pos.y, ball.radius, objectives[i].pos.x, objectives[i].pos.y, objectives[i].width, objectives[i].height))
				if (objectives[i].isActive)
				{
					objectives[i].isActive = false;
					ball.speedY *= -1.0f;
				}

		//----------------------------------------

		slSetForeColor(0.1f, 0.5f, 0.9f, 0.4f);

		slRectangleFill(player.pos.x, player.pos.y, player.width, player.height);

		slCircleFill(ball.pos.x, ball.pos.y, ball.radius, ball.numVertices);

		for (int i = 0; i < amountObjectives; i++)
		{
			slSetForeColor(objectives[i].r, objectives[i].g, objectives[i].b, 0.4f);

			if (objectives[i].isActive)
				slRectangleFill(objectives[i].pos.x, objectives[i].pos.y, objectives[i].width, objectives[i].height);
		}

		slRender();

		//----------------------------------------

		if (slGetKey(SL_KEY_RIGHT))
			if (player.pos.x != player.limitMax)
				player.pos.x += 5;

		if (slGetKey(SL_KEY_LEFT))
			if (player.pos.x != player.limitMin)
				player.pos.x -= 5;

		if (slGetKey(SL_KEY_ENTER))
			ball.isActive = true;
	}

	slClose();
}

float randColor()
{
	float color = rand() % 5;

	return color;
}

void buildObjective(Rectangle& obj, int actualObj)
{
	static bool reachLimit = false;
	static int posX = 10;
	static int posY = 500;

	int modNumX = 40;
	int modNumY = 50;

	obj.width = 30;
	obj.height = 20;
	obj.isObjective = true;
	obj.r = randColor();
	obj.g = randColor();
	obj.b = randColor();

	obj.pos.x = obj.limitMax - posX;
	obj.pos.y = posY;

	if (actualObj < 9)
		posX += modNumX;
	else if (actualObj > 9)
		posX -= modNumX;
	else
		posY += modNumY;
}

bool BallHasCollide(float cX, float cY, float cRadius, float rX, float rY, float rW, float rH)
{
	// temporary variables to set edges for checkin
	float pointX = cX;
	float pointY = cY;

	if (cX < rX - rW / 2)
		pointX = rX - rW / 2; // test left edge
	else if (cX > rX + rW / 2)
		pointX = rX + rW / 2; // test right edge

	if (cY < rY)
		pointY = rY; // test up edge
	else if (cY > rY + rH)
		pointY = rY + rH; // test down edge

	// get distance from closest edges
	float distX = cX - pointX;
	float distY = cY - pointY;
	float distance = sqrt((distX * distX) + (distY * distY));

	// if the distance is less than the radius, collision!
	if (distance <= cRadius)
	{
		return true;
	}

	return false;
}