#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <string>
#include <vector>
#include <cstdlib>
#include <time.h>
#include <iostream>

#define WX 800
#define WY 800

using namespace std;
using namespace sf;

int n = 0;
int goal = 0;

class Ball {
public:
	CircleShape body;
	float dx, dy;
	bool alive = true;

	Ball(float t1, float t2);
	void update(bool _is, bool _mov);
	void draw(RenderWindow& _window);
};

Ball::Ball(float t1, float t2) {
	body = CircleShape(10.0f);
	body.setFillColor(Color::Green);

	srand(time(NULL));
	float x = 200.0f + rand() % 5;
	srand(time(NULL));
	float y = 30.0f + rand() % 5;

	body.setPosition(x, y);

	dx = t1;
	dy = t2;
}

void Ball::update(bool _is, bool _mov) {

	Vector2f body_pos = body.getPosition();

	if (body_pos.x <= 0) {
		if (dx < 0) dx = -dx;
		n++;
	}

	if (body_pos.x + body.getRadius() * 2 >= WX) {
		if (dx > 0) dx = -dx;
		n++;
	}

	if (body_pos.y + body.getRadius() * 2 >= WY) {
		if (dy > 0) dy = -dy;
		n++;
	}

	if (body_pos.y <= 0) {
		goal++;
		alive = false;
	}

	if (_is == true) {
		if (dx < 0) dx = -dx;
		n++;
	}

	if (_mov == true) {
		if (dy < 0) dy = -dy;
		n++;
	}

	body.move(dx, dy);

}

void Ball::draw(RenderWindow& _window) {
	_window.draw(body);
}


class Game {
public:
	vector<Ball> balls;
	int num = 0;
	Text text;
	Text text2;
	Font font;
	Font font2;
	RectangleShape wall;
	RectangleShape moving_wall;

	Game();
	void update();
	void draw(RenderWindow& _window);
};

Game::Game() {

	wall = RectangleShape(Vector2f(40.0f, 1000.0f));
	wall.setFillColor(Color::White);
	wall.setPosition(100.0f, 0);

	moving_wall = RectangleShape(Vector2f(300.0f, 40.0f));
	moving_wall.setFillColor(Color::Red);
	moving_wall.setPosition(300.0f, 0);

	int t1 = 3;
	int t2 = -3;

	for (int i = 0; i < 10; i++) {
		t1++;
		t2++;
		balls.push_back(Ball(t1 * i + 3, t2 * i + 3));
		if (t2 % 2 == 0) t2 = -t2;
		if (t1 % 2 == 1) t1 = -t1;
	}

	font.loadFromFile("images/OpenSans-Bold.ttf");
	text.setFont(font);
	text.setCharacterSize(30);
	text.setFillColor(Color::Magenta);
	text.setStyle(Text::Bold);
	text.setPosition(400, 20);

	font2.loadFromFile("images/OpenSans-Bold.ttf");
	text2.setFont(font2);
	text2.setCharacterSize(30);
	text2.setFillColor(Color::Yellow);
	text2.setStyle(Text::Bold);
	text2.setPosition(400, 100);
}

void Game::update() {
	for (auto& i : balls) {

		FloatRect ball_area = i.body.getGlobalBounds();
		FloatRect wall_area = wall.getGlobalBounds();
		FloatRect moving_wall_area = moving_wall.getGlobalBounds();

		i.update(ball_area.intersects(wall_area), ball_area.intersects(moving_wall_area));
	}

	for (int i = 0; i < balls.size(); i++) {
		if (balls[i].alive == false) {
			balls.erase(balls.begin() + i);
			i--;
		}
	}
}

void Game::draw(RenderWindow& _window) {
	for (auto& i : balls) {
		i.draw(_window);
	}

	_window.draw(wall);
	_window.draw(moving_wall);

	string s = to_string(n);
	text.setString(s);

	string s2 = "Goal : " + to_string(goal);

	text2.setString(s2);

	_window.draw(text);
	_window.draw(text2);
}

int main() {

	RenderWindow window(VideoMode(WX, WY), "teaching");

	window.setFramerateLimit(30);

	Game ball;

	while (window.isOpen()) {

		Event e;

		if (window.pollEvent(e)) {
			switch (e.type) {
			case Event::Closed:
				window.close();
				break;

			case Event::KeyPressed:
				if (Keyboard::isKeyPressed(Keyboard::Left)) {
					ball.moving_wall.move(-5.0f, 0);
				}
				if (Keyboard::isKeyPressed(Keyboard::Right)) {
					ball.moving_wall.move(+5.0f, 0);
				}
				if (Keyboard::isKeyPressed(Keyboard::Up)) {
					ball.moving_wall.move(0, -5.0f);
				}
				if (Keyboard::isKeyPressed(Keyboard::Down)) {
					ball.moving_wall.move(0, +5.0f);
				}
			defualt:
				break;
			}
		}

		window.clear();

		ball.update();
		ball.draw(window);

		window.display();

	}

	return 0;
}