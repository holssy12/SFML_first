#include <SFML/Graphics.hpp>
#include <iostream>

#define WX 800
#define WY 600

using namespace std;
using namespace sf;

int stage = 0;

/*********************************************************************************************************/

class Ball {
public:
	CircleShape body;
	float dx, dy;
	float v;

	Ball();

	void update();
	void draw(RenderWindow& _window);
};

Ball::Ball() {
	body = CircleShape(20.0f);
	body.setFillColor(Color::Green);
	body.setPosition(100.0f, 100.0f);

	v = 100;
}

void Ball::update() {
	body.move(dx, dy);
}

void Ball::draw(RenderWindow& _window) {
	_window.draw(body);
}

/*********************************************************************************************************/

class Target {
public:
	RectangleShape body;

	Target();

	void update();
};

Target::Target() {

	body = RectangleShape(Vector2f(200.0f, 200.0f));
	body.setPosition(500.0f, 300.0f);
}

/*********************************************************************************************************/

class Game {
public:
	Ball ball;
	Target target;

	Clock clock;
	Time time;

	Text text;
	Font font;

	bool isBallKnowsTarget = false;

	Game();

	void update();
	void draw(RenderWindow& _window);
};

Game::Game() {

	clock.restart();

	font.loadFromFile("images/OpenSans-Bold.ttf");
	text.setFont(font);
	text.setCharacterSize(50);
	text.setFillColor(Color::White);
	text.setPosition(200.0f, 300.0f);
}

void Game::update() {

	if (stage == 0) {
		time = clock.getElapsedTime();

		if (!isBallKnowsTarget) {

			Vector2f ballPos = ball.body.getPosition();
			FloatRect targetRect = target.body.getGlobalBounds();

			float tx = targetRect.left + rand() % (int)targetRect.width;
			float ty = targetRect.top + rand() % (int)targetRect.height;

			ball.dx = tx / ball.v;
			ball.dy = ty / ball.v;

			isBallKnowsTarget = true;
		}

		ball.update();

		FloatRect ballArea = ball.body.getGlobalBounds();
		FloatRect targetArea = target.body.getGlobalBounds();

		if (ballArea.left + ballArea.width >= WX || ballArea.top + ballArea.height >= WY) {
			ball = Ball();
			target = Target();
			isBallKnowsTarget = false;
		}
	}

}

void Game::draw(RenderWindow& _window) {

	if (stage == 0) {
		ball.draw(_window);

		target.body.setFillColor(Color::Red);
		_window.draw(target.body);
	}

	else if (stage == 1) {
		int sec = time.asSeconds();
		int hour = sec / 60;
		sec = sec % 60;

		string s = "Play Time = " + to_string(hour) + " : " + to_string(sec);

		text.setString(s);

		_window.draw(text);
	}
}

/*********************************************************************************************************/


int main() {

	srand(time(NULL));

	Game game;


	RenderWindow window(VideoMode(WX, WY), "shooting ball");
	window.setFramerateLimit(60);

	while (window.isOpen()) {

		Event e;

		if (window.pollEvent(e)) {

			switch (e.type) {

			case Event::Closed:
				window.close();
				break;

			case Event::MouseButtonPressed:
				if (Mouse::isButtonPressed(Mouse::Left)) {
					stage = 1;
				}

			default:
				break;
			}
		}

		window.clear();

		game.update();

		game.draw(window);

		window.display();

	}

	return 0;
}