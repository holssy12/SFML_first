#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <iostream>
#include <iostream>

#define WX 1200
#define WY 800

using namespace sf;
using namespace std;

bool isGoalFull = false;

int bound_num = 0;
int goal_num = 0;
int stage = 0;

/*
	1. 버튼 구현
	2. 라운드 구분

	RectangleShape에 Texture입히기
	Text와 Font 불러오기
*/
class Round {
public:
	RectangleShape body;
	Texture texture;
	Font font;
	Text text;

	Round();

	void update(RenderWindow& _window);
	void draw(RenderWindow& _window);
	bool isBodyPressed(RenderWindow& _window);
};

Round::Round() {
	body = RectangleShape(Vector2f(400.0f, 200.0f));
	texture.loadFromFile("images/start.png");
	body.setTexture(&texture);
	body.setPosition(WX / 2 - body.getGlobalBounds().width / 2, WY / 2 - body.getGlobalBounds().height / 2);

	font.loadFromFile("images/OpenSans-Bold.ttf");
	text.setFont(font);
	text.setCharacterSize(100);
	text.setFillColor(Color::Green);
	text.setPosition(WX / 2 - body.getGlobalBounds().width / 2, WY / 2 - body.getGlobalBounds().height / 2);
	text.setString("Finish");
}

void Round::update(RenderWindow& _window) {
	if (stage == 0) {
		if (isBodyPressed(_window) == true) stage = 1;
		return;
	}

	if (stage == 1) {
		if (isGoalFull == true) stage = 2;
		return;
	}
}

void Round::draw(RenderWindow& _window) {
	if (stage == 0) {
		_window.draw(body);
	}
	else if (stage == 2) {
		_window.draw(text);
	}
	else {
		return;
	}
}

bool Round::isBodyPressed(RenderWindow& _window) {
	Vector2i mouse_pos = Mouse::getPosition(_window);
	Vector2f body_pos = body.getPosition();
	FloatRect body_rect = body.getGlobalBounds();

	if (stage == 0) {
		if (mouse_pos.x >= body_pos.x && mouse_pos.x <= body_pos.x + body_rect.width) {
			if (mouse_pos.y >= body_pos.y && mouse_pos.y <= body_pos.y + body_rect.height) {
				return true;
			}
		}
		else {
			return false;
		}
	}
}

/*
	1. 공의 움직임
	2. 튕기면 방향 꺾기

	rand()
	방향 바꾸는 조건
*/
class Ball {
public:
	CircleShape body;
	float dx, dy;
	bool is_goal = false;
	bool bar_intersected = false;
	bool block_intersected = false;

	Ball();
	void update();
	void draw(RenderWindow& _window);
};

Ball::Ball() {
	body = CircleShape(20.0f);
	body.setFillColor(Color::Green);

	dx = rand() % 10 + 1;
	if (int(dx) % 2 == 0) dx = -dx;
	dy = rand() % 10 + 1;
	if (int(dy) % 2 == 1) dy = -dy;

	body.setPosition(WX / 2 + dx, WY / 2 + dy);
}

void Ball::update() {

	Vector2f body_pos = body.getPosition();

	if (body_pos.x <= 0 && dx <= 0) {
		dx = -dx;
		goal_num++;
	}
	if (body_pos.x + 2 * body.getRadius() >= WX && dx >= 0) {
		dx = -dx;
		bound_num++;
	}
	if (body_pos.y <= 0 && dy <= 0) {
		goal_num++;
		is_goal = true;
	}
	if (body_pos.y + 2 * body.getRadius() >= WY && dy >= 0) {
		dy = -dy;
		bound_num++;
	}
	if (bar_intersected == true && dy <= 0) {
		dy = -dy;
		bound_num++;
		bar_intersected = false;
	}
	if (block_intersected == true) {

		if (dx <= 0) dx = -dx;
		else if (dx >= 0) dx = -dx;
		bound_num++;
		block_intersected = false;
	}

	body.move(dx, dy);
}

void Ball::draw(RenderWindow& _window) {
	_window.draw(body);
}

/*
	1. 전역 변수 이용해 Text 표현
*/
class Count {
public:
	Font font;
	Text bound, goal;

	Count();

	void draw(RenderWindow& _window);
};

Count::Count() {
	font.loadFromFile("images/OpenSans-Bold.ttf");
	bound.setFont(font);
	goal.setFont(font);

	bound.setCharacterSize(20);
	goal.setCharacterSize(20);

	bound.setFillColor(Color::White);
	goal.setFillColor(Color::Yellow);

	bound.setPosition(1000.0f, 100.0f);
	goal.setPosition(1000.0f, 200.0f);
}

void Count::draw(RenderWindow& _window) {
	string s;
	s = "Bounds : " + to_string(bound_num);
	bound.setString(s);

	s = "Goal : " + to_string(goal_num);
	goal.setString(s);

	_window.draw(bound);
	_window.draw(goal);
}

/*
	1. Bar의 움직임

	Mouse 위치 따라가기
*/
class Bar {
public:
	RectangleShape body;
	bool is_inst = false;

	Bar();

	void update(RenderWindow& _window);
	void draw(RenderWindow& _window);
};

Bar::Bar() {
	body = RectangleShape(Vector2f(200.0f, 50.0f));
	body.setFillColor(Color::Magenta);

	body.setPosition(WX * (3 / 4), 0);
}

void Bar::update(RenderWindow& _window) {
	Vector2i mouse_pos = Mouse::getPosition(_window);
	Vector2f body_pos = body.getPosition();

	if (mouse_pos.x - body.getGlobalBounds().width / 2 <= 0) {
		body.setPosition(0, body_pos.y);
	}

	else if (mouse_pos.x + body.getGlobalBounds().width / 2 >= WX) {
		body.setPosition(WX - body.getGlobalBounds().width, body_pos.y);
	}

	else {
		body.setPosition(mouse_pos.x - body.getGlobalBounds().width / 2, body_pos.y);
	}
}

void Bar::draw(RenderWindow& _window) {
	_window.draw(body);
}

/*
	1. Block
*/
class Block {
public:
	RectangleShape body;

	Block();

	void draw(RenderWindow& _window);
};

Block::Block() {
	body = RectangleShape(Vector2f(50.0f, WY));
	body.setFillColor(Color::Blue);

	body.setPosition(200.0f, 0);
}

void Block::draw(RenderWindow& _window) {
	_window.draw(body);
}

/*
	1. Game
*/
class Game {
public:
	vector<Ball> balls;
	Bar bar;
	Block block;
	bool is_cleared = false;

	Game();

	void update(RenderWindow& _window);
	void draw(RenderWindow& _window);
};

Game::Game() {
	for (int i = 0; i < 5; i++) {
		balls.push_back(Ball());
	}
}

void Game::update(RenderWindow& _window) {

	bar.update(_window);
	FloatRect bar_area = bar.body.getGlobalBounds();
	FloatRect block_area = block.body.getGlobalBounds();

	for (int i = 0; i < balls.size(); i++) {
		if (balls[i].body.getGlobalBounds().intersects(bar_area)) balls[i].bar_intersected = true;

		if (balls[i].body.getGlobalBounds().intersects(block_area)) balls[i].block_intersected = true;

		balls[i].update();

		if (balls[i].is_goal == true) {
			balls.erase(balls.begin() + i);
			i--;
		}
	}

	if (balls.size() == 0) isGoalFull = true;
}

void Game::draw(RenderWindow& _window) {
	for (int i = 0; i < balls.size(); i++) {
		balls[i].draw(_window);
	}

	bar.draw(_window);
	block.draw(_window);
}

int main() {

	RenderWindow window(VideoMode(WX, WY), "reteaching");
	window.setFramerateLimit(60);

	Game game;
	Count cnt;
	Round round;

	while (window.isOpen()) {

		Event e;

		if (window.pollEvent(e)) {
			switch (e.type) {
			case Event::Closed:
				window.close();
				break;
			case Event::MouseButtonPressed:
				if (Mouse::isButtonPressed(Mouse::Left)) {
					if (stage == 0) {
						round.update(window);
					}
				}

			case Event::KeyPressed:
				if (stage == 1) {
					if (Keyboard::isKeyPressed(Keyboard::W)) {
						game.block.body.move(0, -10.0f);
					}
					if (Keyboard::isKeyPressed(Keyboard::S)) {
						game.block.body.move(0, +10.0f);
					}
					if (Keyboard::isKeyPressed(Keyboard::A)) {
						game.block.body.move(-10.0f, 0);
					}
					if (Keyboard::isKeyPressed(Keyboard::D)) {
						game.block.body.move(+10.0f, 0);
					}
				}
			default:
				break;
			}
		}

		window.clear();

		if (stage == 0) {
			round.draw(window);
		}

		else if (stage == 1) {
			game.update(window);
			game.draw(window);
			cnt.draw(window);
			round.update(window);
		}

		else if (stage == 2) {
			round.draw(window);
		}

		window.display();
	}

	return 0;
}