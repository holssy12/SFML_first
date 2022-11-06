#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <iostream>
#include <vector>

#define WIN_X 800
#define WIN_Y 600

using namespace std;
using namespace sf;

class Button {
public:
	Texture t;
	Sprite s;
	bool button_alive;

	void setButton();
	bool ButtonWithMouse(RenderWindow& _window);
	void draw(RenderWindow& _window);
};

void Button::setButton() {
	button_alive = true;
	s.setPosition(WIN_X / 2 - t.getSize().x / 2, WIN_Y / 2 - t.getSize().y / 2);
}

bool Button::ButtonWithMouse(RenderWindow& _window) {
	Vector2f button_pos = s.getPosition();
	Vector2i mouse_pos = Mouse::getPosition(_window);

	if (mouse_pos.x >= button_pos.x && mouse_pos.x <= button_pos.x + t.getSize().x && mouse_pos.y >= button_pos.y && mouse_pos.y <= button_pos.y + t.getSize().y) {
		button_alive = false;
		return true;
	}
}

void Button::draw(RenderWindow& _window) {
	if (button_alive == false) return;
	_window.draw(s);
}

/*--------------------------------------------------------------------------------*/

class Enemy {
public:
	RectangleShape body;
	bool enemy_alive;
	float offset_x;

	Enemy(float _x, float _y);
	void update();
	void draw(RenderWindow& _window);
};

Enemy::Enemy(float _x, float _y) {
	enemy_alive = 1;

	offset_x = 2.0f;

	body.setSize(Vector2f(120.0f, 30.0f));
	body.setFillColor(Color::Yellow);

	body.setPosition(Vector2f(_x, _y));
}

void Enemy::update() {

	if (enemy_alive == false) return;

	Vector2f body_pos = body.getPosition();

	if (body_pos.x + body.getSize().x >= WIN_X) offset_x = -2.0f;
	else if (body_pos.x <= 0) offset_x = 2.0f;

	body.move(offset_x, 0);
}

void Enemy::draw(RenderWindow& _window) {
	if (enemy_alive == false) return;
	_window.draw(body);
}

/*----------------------------------------------------------------------------------*/

class Gun {
public:
	CircleShape body;
	CircleShape bullet;
	bool gun_alive;
	bool bullet_alive;

	Gun();

	void update(RenderWindow& _window);
	void draw(RenderWindow& _window);
	void fire();
};

Gun::Gun() {
	gun_alive = true;
	bullet_alive = false;

	body.setRadius(80.0f);
	body.setFillColor(Color::Green);
	body.setPosition(WIN_X / 2, WIN_Y - 100);

	bullet.setRadius(10.0f);
	bullet.setPointCount(3);
	bullet.setFillColor(Color::White);
	bullet.setPosition(999, 999);
}

void Gun::update(RenderWindow& _window) {
	if (gun_alive == false) return;

	Vector2i mouse_pos = Mouse::getPosition(_window);

	if (mouse_pos.x - body.getRadius() <= 0) body.setPosition(0, body.getPosition().y);
	else if (mouse_pos.x + body.getRadius() >= WIN_X) body.setPosition(WIN_X - 2 * body.getRadius(), body.getPosition().y);
	else body.setPosition(mouse_pos.x - body.getRadius(), body.getPosition().y);

	if (bullet_alive == false) return;

	Vector2f bullet_pos = bullet.getPosition();

	if (bullet_pos.y <= 0) {
		bullet_alive = false;
		return;
	}

	bullet.move(0, -5.0f);
}

void Gun::draw(RenderWindow& _window) {
	if (gun_alive == false) return;
	_window.draw(body);
	if (bullet_alive == false) return;
	_window.draw(bullet);
}

void Gun::fire() {
	if (bullet_alive == true) return;
	bullet_alive = true;

	Vector2f body_pos = body.getPosition();

	bullet.setPosition(body_pos.x + body.getRadius(), body_pos.y);
}

/*----------------------------------------------------------------------------------*/

class Clear {
public:
	Font font;
	Text txt;
	string s;

	Clear();
};

Clear::Clear() {
	font.loadFromFile("Fonts/H2PORM.TTF");
	txt.setFont(font);
	txt.setCharacterSize(120);
	txt.setFillColor(Color::Green);
	txt.setStyle(Text::Bold);
	txt.setString("Clear");
	txt.setPosition(WIN_X / 2 - txt.getGlobalBounds().width / 2, WIN_Y / 2 - txt.getGlobalBounds().height);
}

class Game {
public:
	vector<Enemy> enemies;
	Gun gun;
	Font font;
	Text txt;
	string s;
	int enemyNum;

	Clear clear;

	Game();
	void update(RenderWindow& _window);
	void draw(RenderWindow& _window);
	void fire();
	bool enemiesAreAlive();
};

Game::Game() {
	font.loadFromFile("Fonts/H2PORM.TTF");
	txt.setFont(font);
	txt.setCharacterSize(60);
	txt.setFillColor(Color::White);
}

void Game::update(RenderWindow& _window) {

	enemyNum = 0;

	FloatRect bullet_area = gun.bullet.getGlobalBounds();

	for (auto& i : enemies) {
		if (bullet_area.intersects(i.body.getGlobalBounds()) && i.enemy_alive == true && gun.bullet_alive == true) {
			gun.bullet_alive = false;
			i.enemy_alive = false;
		}
	}

	for (auto& i : enemies) {
		if (i.enemy_alive == true) enemyNum++;
		i.update();
	}

	s = to_string(enemyNum);

	txt.setString("Enemies : " + s);
	txt.setPosition(WIN_X / 2 - txt.getLocalBounds().width / 2, WIN_Y / 2 - txt.getLocalBounds().height / 2);

	gun.update(_window);
}

void Game::draw(RenderWindow& _window) {
	if (enemiesAreAlive()) {
		for (auto& i : enemies) {
			i.draw(_window);
		}
		_window.draw(txt);
		gun.draw(_window);
	}
	else {
		_window.draw(clear.txt);
	}
}

void Game::fire() {
	gun.fire();
}

bool Game::enemiesAreAlive() {
	enemyNum = 0;
	for (auto& i : enemies) {
		if (i.enemy_alive == true) enemyNum++;
	}
	return enemyNum;
}


int main(void) {

	RenderWindow window(VideoMode(WIN_X, WIN_Y), "shooting");
	window.setFramerateLimit(60);

	Button start;

	start.t.loadFromFile("images/start.png");
	start.s.setTexture(start.t);
	start.setButton();

	Game game;

	for (float i = 1; i < 4; i++) {
		game.enemies.push_back(Enemy(i * 120.0f, i * 40.0f));
	}

	while (window.isOpen()) {

		Event e;

		while (window.pollEvent(e)) {
			switch (e.type) {
			case Event::Closed:
				window.close();
				break;

			case Event::MouseButtonPressed:
				if (Mouse::isButtonPressed(Mouse::Left)) {
					if (start.ButtonWithMouse(window) == true) {
						break;
					}

					if (start.button_alive == false) {
						game.fire();
					}

				}
			default:
				break;
			}
		}

		window.clear();

		start.draw(window);

		if (start.button_alive == false) {

			game.update(window);
			game.draw(window);
		}

		window.display();
	}
}