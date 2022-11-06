#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <time.h>

using namespace std;
using namespace sf;

#define WINDOW_X 1600
#define WINDOW_Y 900

class Enemy {
public:
	RectangleShape rect;
	float offset_x;
	bool enemy_alive;
	CircleShape bullet;
	bool bullet_alive;

	Enemy();
	void draw(RenderWindow& _window);
	void update();
	void fire();
};

Enemy::Enemy() {

	enemy_alive = true;

	rect = RectangleShape(Vector2f(80.0f, 20.0f));
	rect.setFillColor(Color::Magenta);
	rect.setPosition(30, 30);

	bullet = CircleShape(20, 4);
	bullet.setFillColor(Color::Red);
	bullet.setPosition(999, 999);

	bullet_alive = false;

	offset_x = 2.0f;
}

void Enemy::draw(RenderWindow& _window) {

	if (enemy_alive == false) return;

	if (bullet_alive == true) _window.draw(bullet);

	_window.draw(rect);
}

void Enemy::update() {
	if (enemy_alive == false) {
		bullet.setPosition(999, 999);
		return;
	}

	Vector2f rect_pos = rect.getPosition();

	if (bullet_alive == true) {

		srand(time(NULL));
		int rand_x = rand() % 6;

		if (rand_x % 2 == 0) rand_x = -rand_x;

		bullet.move(rand_x, +5.0f);
	}

	if (rect_pos.x <= 0) offset_x = 2.0f;
	else if (rect_pos.x + rect.getSize().x >= WINDOW_X) offset_x = -2.0f;

	Vector2f bullet_pos = bullet.getPosition();

	if (bullet_pos.y + 2 * bullet.getRadius() >= 900) {
		bullet_alive = false;
	}

	rect.move(offset_x, 0);
}

void Enemy::fire() {
	if (bullet_alive == true) return;

	else {
		bullet_alive = true;

		Vector2f rect_pos = rect.getPosition();

		bullet.setPosition(rect_pos.x + rect.getSize().x / 2 - bullet.getRadius(), rect_pos.y);
	}
}

class Gun {
public:
	CircleShape body;
	CircleShape bullet;
	bool bullet_alive;
	bool gun_alive;

	Gun();
	void draw(RenderWindow& _window);
	// void update(RenderWindow& _window);
	void update(double dx, double dy);
	void fire();
};

Gun::Gun() {

	gun_alive = true;

	body = CircleShape(50);
	body.setFillColor(Color::Green);
	body.setPosition(WINDOW_X / 2 - body.getRadius(), WINDOW_Y - body.getRadius());

	bullet = CircleShape(20, 3);
	bullet.setFillColor(Color::White);
	bullet.setPosition(999, 999);

	bullet_alive = false;
}

void Gun::update(double dx, double dy) {

	if (gun_alive == false) {
		bullet.setPosition(999, 999);
		return;
	}

	Vector2f body_pos = body.getPosition();

	body.setPosition(body_pos.x + dx, body_pos.y + dy);

	if (bullet_alive == true) {
		bullet.move(0, -5.0f);
	}

	Vector2f bullet_pos = bullet.getPosition();

	if (bullet_pos.y <= 0) {
		bullet_alive = false;
	}
}

void Gun::draw(RenderWindow& _window) {
	if (gun_alive == false) return;
	_window.draw(body);
	if (bullet_alive == true) _window.draw(bullet);
}

void Gun::fire() {
	if (bullet_alive == true) return;

	bullet_alive = true;

	Vector2f body_pos = body.getPosition();
	bullet.setPosition(body_pos.x + body.getRadius() - bullet.getRadius(), body_pos.y);
}

class Game {
public:
	Enemy enemy;
	Gun gun;

	void update(RenderWindow& _window, double dx, double dy);
	void draw(RenderWindow& _window);
};

void Game::update(RenderWindow& _window, double dx, double dy) {
	enemy.update();
	gun.update(dx, dy);

	FloatRect enemy_area = enemy.rect.getGlobalBounds();
	FloatRect bullet_area = gun.bullet.getGlobalBounds();

	FloatRect gun_area = gun.body.getGlobalBounds();
	FloatRect enemy_bullet_area = enemy.bullet.getGlobalBounds();

	if (enemy_area.intersects(bullet_area) == true && enemy.enemy_alive == true && gun.bullet_alive == true) {
		gun.bullet_alive = false;
		enemy.enemy_alive = false;
	}
	if (gun_area.intersects(enemy_bullet_area) == true && gun.gun_alive == true && enemy.bullet_alive == true) {
		enemy.bullet_alive = false;
		gun.gun_alive = false;
	}
	if (bullet_area.intersects(enemy_bullet_area) == true && gun.bullet_alive == true && enemy.bullet_alive == true) {
		gun.bullet_alive = false;
		enemy.bullet_alive = false;
	}
}

void Game::draw(RenderWindow& _window) {
	enemy.draw(_window);
	gun.draw(_window);
}

int main(void) {
	RenderWindow window(VideoMode(WINDOW_X, WINDOW_Y), "shoot");
	window.setFramerateLimit(60);

	Game game;

	while (window.isOpen()) {

		Event event;

		while (window.pollEvent(event)) {
			switch (event.type) {

			case Event::Closed:
				window.close();
				break;
			case Event::KeyPressed:
				if (Keyboard::isKeyPressed(Keyboard::Space) == true) {
					game.gun.fire();
				}
				if (Keyboard::isKeyPressed(Keyboard::Up) == true) {
					game.update(window, 0, -10.0);
				}
				if (Keyboard::isKeyPressed(Keyboard::Down) == true) {
					game.update(window, 0, +10.0);
				}
				if (Keyboard::isKeyPressed(Keyboard::Left) == true) {
					game.update(window, -10.0, 0);
				}
				if (Keyboard::isKeyPressed(Keyboard::Right) == true) {
					game.update(window, +10.0, 0);
				}
				break;
			default:
				break;

			}

			game.enemy.fire();

		}

		game.update(window, 0, 0);
		window.clear();
		game.draw(window);
		window.display();

	}

	return 0;

}