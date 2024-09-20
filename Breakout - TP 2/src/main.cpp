#include "sl.h"

#include <iostream>
#include <string>

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
	float speedY = 500 * slGetDeltaTime();
	float speedX = 250 * slGetDeltaTime();
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

void buildObjective(Rectangle& obj, int actualObj, int amountObj);

bool areAllBlocksDead(Rectangle obj[], int amountObjs);

bool ballHasCollide(float cX, float cY, float cRadius, float rX, float rY, float rW, float rH);

int main()
{
	srand(time(0));

	const int screenWidth = 460;
	const int screenHeight = 680;
	const int amountObjectives = 41;

	int screenCenterX = screenWidth / 2;
	int screenCenterY = screenHeight / 2;

	int timer = 0;
	int timerRM = 0;
	int score = 0;
	int lifes = 3;
	int winConditionScore = 12;

	bool rainbowMode = false;
	bool isDead = false;

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
		buildObjective(objectives[i], i, amountObjectives);
	}

	slWindow(screenWidth, screenHeight, "BREAKOUT", false);

	int font = slLoadFont("C:/Users/Aula 1/Desktop/BreakoutRepo/Breakout - TP 2/res/Oswald/static/oswald-medium.ttf");

	while (!slShouldClose() && !slGetKey(SL_KEY_ESCAPE))
	{
		string scoreText = "Puntaje: " + to_string(score);
		string timerRMtext = "Cooldown: " + to_string(timerRM);
		string lifeText = "Vidas: " + to_string(lifes);

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

			if (lifes != 0)
				--lifes;
		}

		if (ballHasCollide(ball.pos.x, ball.pos.y, ball.radius, player.pos.x, player.pos.y, player.width, player.height))
			if (ball.pos.y > player.pos.y && timer == 0)
			{
				ball.speedY *= -1.0f;

				timer = 5;
			}
			else if (ball.pos.y <= player.pos.y && timer == 0)
			{
				ball.speedX *= -1.0f;

				timer = 5;
			}

		for (int i = 0; i < amountObjectives; i++)
			if (ballHasCollide(ball.pos.x, ball.pos.y, ball.radius, objectives[i].pos.x, objectives[i].pos.y, objectives[i].width, objectives[i].height))
				if (objectives[i].isActive)
				{
					objectives[i].isActive = false;
					ball.speedY *= -1.0f;
					++score;
				}

		if (areAllBlocksDead(objectives, amountObjectives))
		{
			for (int i = 0; i < amountObjectives; i++)
			{
				objectives[i].isActive = true;
				objectives[i].r = randColor();
				objectives[i].g = randColor();
				objectives[i].b = randColor();
			}
		}

		if (timer != 0)
			--timer;

		if (timerRM != 0)
			--timerRM;

		if (lifes == 0)
			isDead = true;

		if (ball.speedY < 0 && ball.isActive == false)
			ball.speedY *= -1.0f;

		if (isDead || score >= winConditionScore)
		{
			ball.isActive = false;

			ball.pos.x = player.pos.x;
			ball.pos.y = (player.pos.y * 2) + 15;

			for (int i = 0; i < amountObjectives; i++)
				objectives[i].isActive = true;
		}

		//----------------------------------------

		if (!isDead && score != winConditionScore)
		{
			if (!rainbowMode)
				slSetForeColor(1, 1, 1, 1.5f);
			else
				slSetForeColor(randColor(), randColor(), randColor(), 1.5f);

			slSetFont(font, 20);

			slText(objectives->limitMin, 640, scoreText.c_str());

			slText(objectives->limitMin, 610, lifeText.c_str());

			if (timerRM != 0)
				slText(330, 640, timerRMtext.c_str());

			if (!rainbowMode)
				slSetForeColor(0.1f, 0.5f, 0.9f, 0.4f);
			else
				slSetForeColor(randColor(), randColor(), randColor(), 1.5f);

			slRectangleFill(player.pos.x, player.pos.y, player.width, player.height);

			slCircleFill(ball.pos.x, ball.pos.y, ball.radius, ball.numVertices);

			for (int i = 0; i < amountObjectives; i++)
			{
				if (!rainbowMode)
					slSetForeColor(objectives[i].r, objectives[i].g, objectives[i].b, 1.5f);
				else
					slSetForeColor(randColor(), randColor(), randColor(), 1.5f);

				if (objectives[i].isActive)
					slRectangleFill(objectives[i].pos.x, objectives[i].pos.y, objectives[i].width, objectives[i].height);
			}
		}
		else if (isDead)
		{
			slSetForeColor(1, 0, 0, 1.5f);

			slSetFont(font, 30);

			slText(screenCenterX - 90, screenCenterY, "HAS MUERTO!!!!");

			slSetForeColor(1, 1, 1, 1.5f);

			slSetFont(font, 15);

			slText(screenCenterX - 75, screenCenterY - 30, "Presione R para reiniciar");
		}
		else if (score >= winConditionScore)
		{
			slSetForeColor(0, 1, 0, 1.5f);

			slSetFont(font, 30);

			slText(screenCenterX - 70, screenCenterY, "GANASTE!!!!");

			slSetForeColor(1, 1, 1, 1.5f);

			slSetFont(font, 15);

			slText(screenCenterX - 75, screenCenterY - 30, "Presione R para reiniciar");
		}

		slRender();

		//----------------------------------------

		if (!isDead && score != winConditionScore)
		{
			if (slGetKey(SL_KEY_RIGHT))
				if (player.pos.x != player.limitMax)
					player.pos.x += 5;

			if (slGetKey(SL_KEY_LEFT))
				if (player.pos.x != player.limitMin)
					player.pos.x -= 5;

			if (slGetKey(SL_KEY_ENTER))
				ball.isActive = true;

			if (slGetKey('T') && timerRM == 0)
				if (rainbowMode)
				{
					rainbowMode = false;

					timerRM = 100;
				}
				else
				{
					rainbowMode = true;

					timerRM = 100;
				}
		}
		else
		{
			if (slGetKey('R'))
			{
				isDead = false;
				score = 0;
				lifes = 3;
			}
		}
	}

	slClose();
}

float randColor()
{
	float color = rand() % 11 + 1;

	color /= 10.f;

	return color;
}

void buildObjective(Rectangle& obj, int actualObj, int amountObj)
{
	static bool reachLimit = false;
	static bool hadStop = false;
	static int posX = 10;
	static int posY = 500;

	int modNumX = 40;
	int modNumY = 25;

	obj.width = 30;
	obj.height = 20;
	obj.isObjective = true;
	obj.r = randColor();
	obj.g = randColor();
	obj.b = randColor();

	obj.pos.x = obj.limitMax - posX;
	obj.pos.y = posY;

	if (actualObj % 10 == 0)
	{
		if (!hadStop)
			hadStop = true;
		else
			hadStop = false;

		if (actualObj % 10 == 0 && actualObj != 0)
			posY += modNumY;
	}
	else
		if (hadStop)
			posX += modNumX;
		else
			posX -= modNumX;
}

bool areAllBlocksDead(Rectangle obj[], int amountObjs)
{
	for (int i = 0; i < amountObjs; i++)
	{
		if (obj[i].isActive)
			return false;
	}

	return true;
}

bool ballHasCollide(float cX, float cY, float cRadius, float rX, float rY, float rW, float rH)
{
	// temporary variables to set edges for checkin
	float pointX = cX;
	float pointY = cY;

	if (cX < rX - rW / 2)
		pointX = rX - rW / 2; // test left edge
	else if (cX > rX + rW / 2)
		pointX = rX + rW / 2; // test right edge

	if (cY < rY - rH / 2)
		pointY = rY; // test down edge
	else if (cY > rY + rH / 2)
		pointY = rY + rH; // test up edge

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