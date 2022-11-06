#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <iostream>
#include <string>
#include <vector>

#define WX 1200
#define WY 800

#define GOAL 3

using namespace std;
using namespace sf;

int stage = 0;

int goal_me = 0;
int goal_enemy = 0;

/*-------------------------------------------------------------------------------------------------*/

class Me {
public:
	RectangleShape body;

	Me();

	void update();
	void draw(RenderWindow& _window);
};

Me::Me() {
	body = RectangleShape(Vector2f(50.0f, 200.0f));
	body.setFillColor(Color::Blue);
	body.setPosition(body.getSize().x, WY / 2 - body.getSize().y / 2);
}

void Me::update() {

}

void Me::draw(RenderWindow& _window) {
	_window.draw(body);
}

/*-------------------------------------------------------------------------------------------------*/

class Enemy {
public:
	RectangleShape body;
	float dy = 2.0f;

	Enemy();

	void update();
	void draw(RenderWindow& _window);
};

Enemy::Enemy() {
	body = RectangleShape(Vector2f(50.0f, 200.0f));
	body.setFillColor(Color::Red);
	body.setPosition(WX - body.getSize().x * 2, WY / 2 - body.getSize().y / 2);
}

void Enemy::update() {
	body.move(0, dy);
}

void Enemy::draw(RenderWindow& _window) {
	_window.draw(body);
}

/*-------------------------------------------------------------------------------------------------*/

class Ball {
public:
	CircleShape body;
	float dx, dy;

	Ball();

	void update();
	void draw(RenderWindow& _window);
};

Ball::Ball() {
	body = CircleShape(30.0f);
	body.setFillColor(Color::Green);
	body.setPosition(WX / 2 - body.getRadius(), WY / 2 - body.getRadius());

	dx = rand() % 5 + 15;
	dy = rand() % 5 + 15;

	if (int(dx) % 2 == 0) dx = -dx;
	if (int(dy) % 2 == 1) dy = -dy;
}

void Ball::update() {

	Vector2f body_pos = body.getPosition();

	if (body_pos.x <= 0 && dx <= 0) {
		dx = -dx;
		goal_enemy++;
		stage = 0;
		if (goal_enemy >= GOAL) stage = 3;
		return;
	}
	if (body_pos.x + 2 * body.getRadius() >= WX && dx >= 0) {
		dx = -dx;
		goal_me++;
		stage = 0;
		if (goal_me >= GOAL) stage = 2;
		return;
	}
	if (body_pos.y <= 0 && dy <= 0) {
		dy = -dy;
	}
	if (body_pos.y + 2 * body.getRadius() >= WY && dy >= 0) {
		dy = -dy;
	}

	body.move(dx, dy);
}

void Ball::draw(RenderWindow& _window) {
	_window.draw(body);
}


/*-------------------------------------------------------------------------------------------------*/

class Block {
public:
	RectangleShape body;

	Block();

	void update();
	void draw(RenderWindow& _window);
};

Block::Block() {
	body = RectangleShape(Vector2f(float(WX), 50.0f));
	body.setFillColor(Color::Cyan);
	body.setPosition(0, 150.0f);
}

void Block::update() {

}

void Block::draw(RenderWindow& _window) {
	_window.draw(body);
}


/*-------------------------------------------------------------------------------------------------*/


class Game {
public:
	Me me;
	Enemy enemy;
	Ball ball;
	Block block;

	Font font;
	Text text_me_goal;
	Text text_enemy_goal;

	Game();

	void update();
	void draw(RenderWindow& _window);
};

Game::Game() {
	font.loadFromFile("images/OpenSans-Bold.ttf");
	text_me_goal.setFont(font);
	text_me_goal.setCharacterSize(100);
	text_me_goal.setFillColor(Color::Blue);
	text_me_goal.setPosition(50.0f, 20.0f);

	text_enemy_goal.setFont(font);
	text_enemy_goal.setCharacterSize(100);
	text_enemy_goal.setFillColor(Color::Red);
	text_enemy_goal.setPosition(1050.0f, 20.0f);
}

void Game::update() {

	if (stage == 0) {
		enemy.body.setPosition(WX - enemy.body.getSize().x * 2, WY / 2 - enemy.body.getSize().y / 2);
		me.body.setPosition(me.body.getSize().x, WY / 2 - me.body.getSize().y / 2);
		ball.body.setPosition(WX / 2 - ball.body.getRadius(), WY / 2 - ball.body.getRadius());
		block.body.setPosition(0, 150.0f);
		return;
	}

	ball.update();
	enemy.update();

	FloatRect ball_area = ball.body.getGlobalBounds();
	FloatRect me_area = me.body.getGlobalBounds();
	FloatRect enemy_area = enemy.body.getGlobalBounds();
	FloatRect block_area = block.body.getGlobalBounds();

	if (ball_area.intersects(me_area) && ball.dx <= 0) {
		ball.dx = -ball.dx;
	}
	if (ball_area.intersects(enemy_area) && ball.dx >= 0) {
		ball.dx = -ball.dx;
	}
	if (ball_area.intersects(block_area) && ball.dy <= 0) {
		ball.dy = -ball.dy;
	}

	if (enemy_area.top + enemy_area.height >= WY) {
		enemy.dy = -enemy.dy;
	}
	if (enemy_area.intersects(block_area) && enemy.dy <= 0) {
		enemy.dy = -enemy.dy;
	}

	if (me_area.intersects(block_area)) {
		me.body.setPosition(me.body.getPosition().x, block_area.top + block_area.height);
	}


}

void Game::draw(RenderWindow& _window) {
	me.draw(_window);
	enemy.draw(_window);
	ball.draw(_window);
	block.draw(_window);
	
	text_me_goal.setString(to_string(goal_me));
	text_enemy_goal.setString(to_string(goal_enemy));

	_window.draw(text_me_goal);
	_window.draw(text_enemy_goal);
}

/*-------------------------------------------------------------------------------------------------*/

class Round {
public:
	RectangleShape body;
	Texture start;
	Texture win;
	Texture lose;
	
	Round();

	void update();
	void draw(RenderWindow& _window);
	bool isMouseInButton(RenderWindow& _window);
};

Round::Round() {

	body = RectangleShape(Vector2f(400.0f, 200.0f));

	start.loadFromFile("images/START.PNG");
	win.loadFromFile("images/WIN.PNG");
	lose.loadFromFile("images/LOSE.PNG");
}

void Round::update() {
	if (stage == 0) {
		body = RectangleShape(Vector2f(100.0f, 50.0f));
		body.setTexture(&start);
		body.setPosition(WX / 2 - body.getGlobalBounds().width / 2, 50.0f);
	}
	else if (stage == 2) {
		body = RectangleShape(Vector2f(400.0f, 200.0f));
		body.setTexture(&win);
		body.setPosition(WX / 2 - body.getGlobalBounds().width, WY / 2 - body.getGlobalBounds().width);
	}
	else if (stage == 3) {
		body = RectangleShape(Vector2f(400.0f, 200.0f));
		body.setTexture(&lose);
	}
}

void Round::draw(RenderWindow& _window) {
	_window.draw(body);
}

bool Round::isMouseInButton(RenderWindow& _window) {
	Vector2i mouse_pos = Mouse::getPosition(_window);
	FloatRect body_rect = body.getGlobalBounds();

	if (mouse_pos.x >= body_rect.left && mouse_pos.x <= body_rect.left + body_rect.width) {
		if (mouse_pos.y >= body_rect.top && mouse_pos.y <= body_rect.top + body_rect.height) {
			return true;
		}
	}

	return false;
}

int main() {

	RenderWindow window(VideoMode(WX, WY), "test");
	window.setFramerateLimit(60);


	Game game;
	Round round;


	while (window.isOpen()) {

		Event e;

		if (window.pollEvent(e)) {

			switch (e.type) {
			case Event::Closed:
				window.close();
				break;

			case Event::MouseButtonPressed:
				
				if (round.isMouseInButton(window)) {
					stage = 1;
				}
				
				break;

			case Event::KeyPressed:

				if (stage == 1) {
					if (Keyboard::isKeyPressed(Keyboard::W)) {
						game.me.body.move(0, -10.0f);
					}
					else if (Keyboard::isKeyPressed(Keyboard::S)) {
						game.me.body.move(0, +10.0f);
					}
				}

				break;

			default:
				break;
			}
		}


		window.clear();

		round.update();

		game.update();
		
		if(stage==0 || stage==1) game.draw(window);

		round.draw(window);

		window.display();

	}

	return 0;
}