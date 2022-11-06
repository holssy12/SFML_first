#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <iostream>
#include <vector>

using namespace std;
using namespace sf;

#define WX 1200
#define WY 800

#define BOUNDS_ENABLE 5
#define BULLETS_AVAILABLE 3
#define ENEMIES_AVAILABLE 4

/*-------------------------------------------------------------------------------------------------------------------------*/

class Bullet {
public:
	CircleShape body;
	float dx, dy;
	int boundNum = 0;

	bool bar_intersected = false;
	bool is_dead = false;
	bool doing_intersected = false;
	int enemy_intersected = 0;

	Bullet();

	void update();
	void draw(RenderWindow& _window);
};

Bullet::Bullet() {
	body = CircleShape(20.0f, rand() % 3 + 3);
	body.setFillColor(Color::Green);
	body.setPosition(2000, 2000);
}

void Bullet::update() {

	Vector2f body_pos = body.getPosition();

	if (body_pos.x <= 0 && dx <= 0) {
		dx = -dx;
		boundNum++;
		if (boundNum >= BOUNDS_ENABLE) return;
	}
	if (body_pos.x + 2 * body.getRadius() >= WX && dx >= 0) {
		dx = -dx;
		boundNum++;
		if (boundNum >= BOUNDS_ENABLE) return;
	}
	if (body_pos.y <= 0 && dy <= 0) {
		dy = -dy;
		boundNum++;
		if (boundNum >= BOUNDS_ENABLE) return;
	}
	if (body_pos.y + 2 * body.getRadius() >= WY && dy >= 0) {
		is_dead = true;
	}
	if (bar_intersected == true && dy >= 0) {
		dy = -dy;
		boundNum++;
		if (boundNum >= BOUNDS_ENABLE) return;
	}
	if (enemy_intersected == 1 || enemy_intersected == 3) {
		dy = -dy;
		boundNum++;
		if (boundNum >= BOUNDS_ENABLE) return;
		body.move(dx, dy);
		return;
	}
	else if (enemy_intersected == 2 || enemy_intersected == 4) {
		dx = -dx;
		boundNum++;
		if (boundNum >= BOUNDS_ENABLE) return;
		body.move(dx, dy);
		return;
	}

	body.move(dx, dy);
}

void Bullet::draw(RenderWindow& _window) {
	_window.draw(body);
}

/*-------------------------------------------------------------------------------------------------------------------------*/

class Bar {
public:
	RectangleShape body;
	vector<Bullet> bullets;
	bool is_inst = false;
	int fired_bullets = 0;

	Bar();

	void update(RenderWindow& _window);
	void draw(RenderWindow& _window);
	void fire(float _x, float _y);
};

Bar::Bar() {
	body = RectangleShape(Vector2f(200.0f, 50.0f));
	body.setFillColor(Color::Cyan);
	FloatRect body_rect = body.getGlobalBounds();

	body.setPosition(WX / 2 - body_rect.width / 2, WY - body_rect.height);
}

void Bar::update(RenderWindow& _window) {
	Vector2i mouse_pos = Mouse::getPosition(_window);
	Vector2f body_pos = body.getPosition();
	FloatRect body_rect = body.getGlobalBounds();

	if (mouse_pos.x - body_rect.width / 2 <= 0) {
		body.setPosition(0, body_pos.y);
	}
	else if (mouse_pos.x + body_rect.width / 2 >= WX) {
		body.setPosition(WX - body_rect.width, body_pos.y);
	}
	else {
		body.setPosition(mouse_pos.x - body_rect.width / 2, body_pos.y);
	}

	FloatRect body_area = body.getGlobalBounds();

	for (int i = 0; i < bullets.size(); i++) {

		FloatRect bullet_area = bullets[i].body.getGlobalBounds();
		if (bullet_area.intersects(body_area)) {
			bullets[i].bar_intersected = true;
		}

		bullets[i].update();

		if (bullets[i].boundNum >= BOUNDS_ENABLE || bullets[i].is_dead == true) {
			bullets.erase(bullets.begin() + i);
			i--;
			continue;
		}

		bullets[i].bar_intersected = false;
	}
}

void Bar::draw(RenderWindow& _window) {
	_window.draw(body);

	for (auto& i : bullets) {
		_window.draw(i.body);
	}
}

void Bar::fire(float _x, float _y) {

	if (fired_bullets >= BULLETS_AVAILABLE) return;

	Bullet buf;
	buf.dx = _x;
	buf.dy = _y;

	Vector2f body_pos = body.getPosition();
	buf.body.setPosition(body_pos.x + body.getGlobalBounds().width / 2, body_pos.y - body.getGlobalBounds().height);

	bullets.push_back(buf);

	fired_bullets++;
}

/*-------------------------------------------------------------------------------------------------------------------------*/

class Enemy {
public:
	RectangleShape body;
	float dx, dy;

	Enemy();

	void update();
	void draw(RenderWindow& _window);
};

Enemy::Enemy() {
	body = RectangleShape(Vector2f(80.0f, 20.0f));
	body.setFillColor(Color::Red);
	FloatRect body_rect = body.getGlobalBounds();

	int x_rand = rand() % 200 + 200;
	int y_rand = rand() % 400;
	int dy_rand = rand() % 2 + 3;

	if (int(x_rand) % 2 == 0) x_rand = -x_rand;

	dy = dy_rand;

	body.setPosition(WX / 2 - (body_rect.width / 2) + x_rand, y_rand);
}

void Enemy::update() {
	body.move(0, 0);
}

void Enemy::draw(RenderWindow& _window) {
	_window.draw(body);
}

/*-------------------------------------------------------------------------------------------------------------------------*/

class Game {
public:
	Bar bar;
	vector<Enemy> enemies;

	Game();

	void update(RenderWindow& _window);
	void draw(RenderWindow& _window);
	int intersectedDirection(Enemy& _e, Bullet& _b);
};

Game::Game() {

	for (int i = 0; i < ENEMIES_AVAILABLE; i++) {
		Enemy buf;
		if (rand() % 2 == 0) enemies.push_back(buf);
	}

}

void Game::update(RenderWindow& _window) {
	bar.update(_window);

	for (int i = 0; i < enemies.size(); i++) {
		enemies[i].update();
	}

	vector<FloatRect> bullets_area;
	for (int i = 0; i < bar.bullets.size(); i++) {
		bullets_area.push_back(bar.bullets[i].body.getGlobalBounds());
	}

	for (int i = 0; i < enemies.size(); i++) {

		FloatRect enemy_area = enemies[i].body.getGlobalBounds();

		for (int j = 0; j < bar.bullets.size(); j++) {

			if (bullets_area[j].intersects(enemy_area)) {

				int direct = intersectedDirection(enemies[i], bar.bullets[j]);

				bar.bullets[j].enemy_intersected = direct;
				bar.bullets[j].doing_intersected = true;

				bar.update(_window);
				cout << bar.bullets[j].enemy_intersected << endl;

				bar.bullets[j].enemy_intersected = false;

			}

		}

	}
}

void Game::draw(RenderWindow& _window) {
	bar.draw(_window);

	for (int i = 0; i < enemies.size(); i++) {
		enemies[i].draw(_window);
	}
}

int Game::intersectedDirection(Enemy& _e, Bullet& _b) {

	FloatRect enemy_pos = _e.body.getGlobalBounds();
	FloatRect bullet_pos = _b.body.getGlobalBounds();

	if (bullet_pos.left + bullet_pos.width <= enemy_pos.left) {
		return 4;
	}
	else if (bullet_pos.left >= enemy_pos.left + enemy_pos.width) {
		return 2;
	}
	if (bullet_pos.top + bullet_pos.height <= enemy_pos.top) {
		return 1;
	}
	else if (bullet_pos.top >= enemy_pos.top + enemy_pos.height) {
		return 3;
	}

}


int main() {

	RenderWindow window(VideoMode(WX, WY), "final shooting2");
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
				if (Keyboard::isKeyPressed(Keyboard::A)) {
					game.bar.fire(-5, -5);
				}
				else if (Keyboard::isKeyPressed(Keyboard::D)) {
					game.bar.fire(5, -5);
				}
				break;
			default:
				break;
			}
		}

		window.clear();



		game.update(window);
		game.draw(window);


		window.display();
	}

	return 0;
}