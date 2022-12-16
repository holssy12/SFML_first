#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <cmath>

#define WX 800
#define WY 600

using namespace std;
using namespace sf;

int status = 0;

/***********************************************************************************************
 
 벡터 속도를 잘 생각하자...

***********************************************************************************************/


class Ball {
public:
	CircleShape body;
	float dx = 0, dy = 0;

	Ball();

	void update();
	void draw(RenderWindow& _window);
};

Ball::Ball() {
	body = CircleShape(30.0f);
	body.setFillColor(Color::Green);
}

void Ball::update() {
	body.move(dx, dy);
}

void Ball::draw(RenderWindow& _window) {
	_window.draw(body);
}

/***********************************************************************************************/

class StartArea {
public:
	RectangleShape body;

	StartArea();

	void draw(RenderWindow& _window);
};

StartArea::StartArea() {
	body = RectangleShape(Vector2f(150.0f, 150.0f));
	body.setFillColor(Color::Blue);
	body.setPosition(50, 50);
}

void StartArea::draw(RenderWindow& _window) {
	_window.draw(body);
}

/***********************************************************************************************/

class TargetArea {
public:
	RectangleShape body;

	TargetArea();

	void draw(RenderWindow& _window);
};

TargetArea::TargetArea() {
	body = RectangleShape(Vector2f(150.0f, 150.0f));
	body.setFillColor(Color::White);
	body.setPosition(600, 400);
}

void TargetArea::draw(RenderWindow& _window) {
	_window.draw(body);
}

/***********************************************************************************************/

class Game {
public:
	vector<Ball> balls;
	StartArea sA;
	TargetArea tA;

	Time time;
	Clock clock;

	Text text;
	Font font;

	int makeFlag = 0;
	
	float tx = 0, ty = 0;

	Game();

	void update();
	void draw(RenderWindow& _window);
};

Game::Game() {

	clock.restart();

	font.loadFromFile("images/OpenSans-Bold.ttf");
	text.setFont(font);
	text.setCharacterSize(60);	
	text.setFillColor(Color::White);
	text.setPosition(200, 300);
}

void Game::update() {
	time = clock.getElapsedTime();

	float real_sec = time.asSeconds();
	int sec = real_sec;

	string buf;
	buf = to_string(real_sec);
	string result = "";

	int cnt = 0;
	int dot_flag = 0;

	for (auto& c : buf) {
		if (dot_flag) cnt++;
		result = result + c;
		if (c == '.') dot_flag = 1;
		if (cnt == 2) break;
	}

	text.setString(result);

	if (sec % 2 == 0 && makeFlag == 1) makeFlag = 0;

	else if (sec % 2 == 1 && makeFlag == 0) {

		makeFlag = 1;

		balls.push_back(Ball());

		FloatRect tRect = tA.body.getGlobalBounds();
		FloatRect sRect = sA.body.getGlobalBounds();

		float sx = sRect.left + rand() % (int)sRect.width;
		float sy = sRect.left + rand() % (int)sRect.width;

		balls.back().body.setPosition(sx, sy);

		tx = tRect.left + rand() % (int)tRect.width;
		ty = tRect.top + rand() % (int)tRect.height;

		// 속도를 바꾸려면 여기

		float _dx = tx - sx;
		float _dy = ty - sy;
		float distance = sqrt(_dx * _dx + _dy * _dy);

		int r = rand() % 5 + 1;

		switch(r) {
		case 1:
			balls.back().body.setFillColor(Color::Red);
			break;
		case 2:
			balls.back().body.setFillColor(Color::Magenta);
			break;
		case 3:
			balls.back().body.setFillColor(Color::Green);
			break;
		case 4:
			balls.back().body.setFillColor(Color::Yellow);
			break;
		case 5:
			balls.back().body.setFillColor(Color::Cyan);
			break;
		}

		float x = balls.back().dx = (_dx / distance) * r;
		float y = balls.back().dy = (_dy / distance) * r;

		cout << (_dx / distance) << ", " << (_dy/distance) << ", "<< sqrt(x * x + y * y) << endl;
	}


	for (auto& i : balls) {
		i.update();
	}

	
}

void Game::draw(RenderWindow& _window) {

	if (status == 0) {
		tA.draw(_window);
		sA.draw(_window);

		for (auto& i : balls) {
			i.draw(_window);
		}
	}

	/*else if (status == 1)*/ _window.draw(text);
}

int main() {

	srand(time(NULL));

	RenderWindow window(VideoMode(WX, WY), "TeachingFinal");

	window.setFramerateLimit(60);

	Game game;

	while (window.isOpen()) {

		Event e;

		if (window.pollEvent(e)) {

			switch (e.type) {
			case Event::Closed:
				window.close();
				break;
			case Event::KeyPressed:
				if (Keyboard::isKeyPressed(Keyboard::Space)) {
					status = 1;
				}
				break;
			default:
				break;
			}

		}

		window.clear();

		if(status == 0) game.update();
		game.draw(window);

		window.display();
	}


	return 0;
}