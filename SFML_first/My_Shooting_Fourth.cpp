#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <iostream>
#include <vector>
#include <cmath>

#define WX 800
#define WY 800

using namespace std;
using namespace sf;

class Bullet {
public:
	CircleShape body;
	bool bullet_alive;
	float dx, dy;

	Bullet(CircleShape _b, float _x, float _y, bool _a = 1) : body(_b), dx(_x), dy(_y), bullet_alive(_a) {}
};

class Me {
public:
	RectangleShape body;
	vector<Bullet> bullets;
	bool body_alive = 1;

	Me();

	void update(RenderWindow& _window);
	void draw(RenderWindow& _window);
	void fire(RenderWindow&);
};

Me::Me() {
	body = RectangleShape(Vector2f(50.0f, 50.0f));
	body.setFillColor(Color::Green);
	body.setPosition(WX / 2 - body.getGlobalBounds().width / 2, WY / 2 - body.getGlobalBounds().height / 2);
}

void Me::draw(RenderWindow& _window) {
	_window.draw(body);

	for (auto& i : bullets) {
		if (i.bullet_alive == true) {
			_window.draw(i.body);
		}
	}
}

void Me::update(RenderWindow& _window) {
	for (int i = 0; i < bullets.size(); i++) {

		if (bullets[i].bullet_alive == true) {

			float sum = abs(bullets[i].dx) + abs(bullets[i].dy);
			float x = (bullets[i].dx / sum) * 20.0f;
			float y = (bullets[i].dy / sum) * 20.0f;

			bullets[i].body.move(x, y);
		}
	}

}

void Me::fire(RenderWindow& _window) {
	CircleShape buf_body(20.0f);
	buf_body.setFillColor(Color::Red);
	Vector2i mouse_pos = Mouse::getPosition(_window);

	if (mouse_pos.x < 400) {
		mouse_pos.x = -mouse_pos.x;
	}

	if (mouse_pos.y < 400) {
		mouse_pos.y = -mouse_pos.y;
	}

	if (mouse_pos.x < 0 && mouse_pos.y < 0) {
		mouse_pos.x = -mouse_pos.x;
	}

	Bullet buf_bullet(buf_body, mouse_pos.x, mouse_pos.y);

	bullets.push_back(buf_bullet);
	bullets.back().body.setPosition(body.getPosition().x, body.getPosition().y);
}

int main() {

	RenderWindow window(VideoMode(WX, WY), "Fourth");
	window.setFramerateLimit(120);

	Me me;

	while (window.isOpen()) {

		Event e;

		if (window.pollEvent(e)) {

			switch (e.type) {
			case Event::Closed:
				window.close();
				break;
			case Event::MouseButtonPressed:
				if (Mouse::isButtonPressed(Mouse::Left) == true) {
					me.fire(window);
				}
			default:
				break;
			}

		}

		window.clear();

		me.update(window);
		me.draw(window);

		window.display();
	}

	return 0;
}