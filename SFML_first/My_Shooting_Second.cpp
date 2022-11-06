#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

using namespace std;
using namespace sf;

class Button {
public:
	Texture texture;
	Sprite sprite;
	bool button_alive;

	Button(Texture _t, Sprite _s) : texture(_t), sprite(_s), button_alive(true) {
		sprite.setPosition(800.0 / 2 - sprite.getTexture()->getSize().x / 2, 600.0 / 2 - sprite.getTexture()->getSize().y / 2);
	}
	bool isButtonWithMouse(RenderWindow& _window);
	void draw(RenderWindow& _window);
};

bool Button::isButtonWithMouse(RenderWindow& _window) {
	Vector2i mouse_pos = Mouse::getPosition(_window);
	Vector2u sprite_scale = texture.getSize();

	if (mouse_pos.x >= sprite.getPosition().x && mouse_pos.x <= sprite.getPosition().x + sprite_scale.x &&
		mouse_pos.y >= sprite.getPosition().y && mouse_pos.y <= sprite.getPosition().y + sprite_scale.y) {
		return true;
	}
	else return false;

}

void Button::draw(RenderWindow& _window) {
	if (button_alive == false) return;
	_window.draw(sprite);
}

class Enemy {
public:
	RectangleShape rect;
	float offset_x;
	bool enemy_alive;

	Enemy(float _x, float _y);
	void draw(RenderWindow& _winodw);
	void update();
};

Enemy::Enemy(float _x, float _y) {
	enemy_alive = false;
	rect = RectangleShape(Vector2f(80.0f, 20.0f));
	rect.setFillColor(Color::Magenta);
	rect.setPosition(_x, _y);

	offset_x = 2.0f;
}

void Enemy::draw(RenderWindow& _window) {

	if (enemy_alive == false) return;
	_window.draw(rect);
}

void Enemy::update() {
	if (enemy_alive == false) return;

	Vector2f rect_pos = rect.getPosition();

	if (rect_pos.x <= 0) offset_x = 2.0f;
	else if (rect_pos.x >= 800.0f - rect.getSize().x) offset_x = -2.0f;

	rect.move(offset_x, 0);
}

class Gun {
public:
	CircleShape circle;
	CircleShape bullet;
	bool circle_alive;
	bool bullet_alive;

	Gun();
	void update(RenderWindow& _window);
	void draw(RenderWindow& _window);
	void fire();
};

Gun::Gun() {
	circle_alive = false;
	bullet_alive = false;

	circle = CircleShape(50);
	circle.setFillColor(Color::Green);
	circle.setPosition(800 / 2 - circle.getRadius(), 600 - circle.getRadius());

	bullet = CircleShape(10, 3);
	bullet.setFillColor(Color::Red);
	bullet.setPosition(999, 999);
}

void Gun::update(RenderWindow& _window) {
	if (circle_alive == false) return;

	Vector2i mouse_pos = Mouse::getPosition(_window);

	circle.setPosition(mouse_pos.x - circle.getRadius(), circle.getPosition().y);

	Vector2f circle_pos = circle.getPosition();

	if (circle_pos.x <= 0) circle.setPosition(0, circle_pos.y);
	else if (circle_pos.x >= 800 - circle.getRadius() * 2) circle.setPosition(800 - circle.getRadius() * 2, circle_pos.y);


	if (bullet_alive == true) {
		bullet.move(0, -5.0f);
	}

	Vector2f bullet_pos = bullet.getPosition();
	if (bullet_pos.y <= 0) {
		bullet_alive = false;
	}
}

void Gun::draw(RenderWindow& _window) {
	if (circle_alive == false) return;
	_window.draw(circle);
	if (bullet_alive == false) return;
	_window.draw(bullet);
}

void Gun::fire() {
	if (circle_alive == false) return;
	if (bullet_alive == true) return;

	bullet_alive = true;

	Vector2f circle_pos = circle.getPosition();

	bullet.setPosition(circle_pos.x + circle.getRadius(), circle_pos.y);
}

class Game {
public:
	vector<Enemy> enemies;
	Gun gun;

	void update(RenderWindow& _window);
	void draw(RenderWindow& _window);
	void fire();
	bool allEnemiesAreDead();
};

void Game::update(RenderWindow& _window) {
	gun.update(_window);
	for (auto& i : enemies) {
		i.update();
	}

	vector<FloatRect> enemy_area;

	for (auto& i : enemies) {
		enemy_area.push_back(i.rect.getGlobalBounds());
	}

	FloatRect bullet_area = gun.bullet.getGlobalBounds();

	for (auto i = 0; i < enemies.size(); i++) {
		if (enemy_area.at(i).intersects(bullet_area) && enemies.at(i).enemy_alive == true && gun.bullet_alive == true) {
			gun.bullet_alive = false;
			enemies.at(i).enemy_alive = false;
		}
	}

}

void Game::draw(RenderWindow& _window) {
	gun.draw(_window);
	for (auto& i : enemies) {
		i.draw(_window);
	}
}

void Game::fire() {
	gun.fire();
}

bool Game::allEnemiesAreDead() {
	for (auto& i : enemies) {
		if (i.enemy_alive == true) return false;
	}
	return true;
}

int main(void) {

	RenderWindow window(VideoMode(800, 600), "Shooting_Second");
	window.setFramerateLimit(60);

	Texture start_text;
	Sprite start_sprt;
	start_text.loadFromFile("images/start.png");
	start_sprt.setTexture(start_text);

	Button start(start_text, start_sprt);

	Texture clear_text;
	Sprite clear_sprt;
	clear_text.loadFromFile("images/clear.png");
	clear_sprt.setTexture(clear_text);

	Button clear(clear_text, clear_sprt);
	clear.button_alive = false;

	Game game;

	for (float i = 1; i < 4; i++) {
		game.enemies.push_back(Enemy(i * 80.0f, i * 30.0f));
	}

	while (window.isOpen()) {

		Event e;

		while (window.pollEvent(e)) {

			if (start.button_alive == false && game.allEnemiesAreDead()) {
				game.gun.circle_alive = game.gun.bullet_alive = false;
				clear.button_alive = true;
			}

			switch (e.type) {
			case Event::Closed:
				window.close();
				break;
			case Event::MouseButtonPressed:
				if (Mouse::isButtonPressed(Mouse::Left) == true) {

					game.fire();

					if (start.button_alive == true && start.isButtonWithMouse(window)) {
						start.button_alive = false;

						game.gun.circle_alive = true;

						for (auto& i : game.enemies) {
							i.enemy_alive = true;
						}

					}

				}

			default:
				break;
			}

		}

		game.update(window);

		window.clear();

		start.draw(window);
		clear.draw(window);
		game.draw(window);

		window.display();

	}

}