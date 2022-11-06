
// 1. ���� �޸� �Ҵ�����
// LunarLander�� �籸�� �غ���.

// 2. ���� �Ҵ����� class �� �޸𸮸� �ٿ�����.
//		---> �����ͷ� ����� �ʿ��� ���� �޸� ����.
//		---> ���� �����ϸ� �̹� �޸𸮰� �������.

#include <SFML/Graphics.hpp>
#include <windows.h> // Ÿ�̸�...
#include <string>
#include <iostream>

using namespace sf;
using namespace std;

class LunarLander {
private:
	double x, y;		// ������ ���� ��ġ ( y���� �Ʒ��� ������ Ŀ��. )
	double velocity;	// ���� �ӵ�
	double fuel;		// �����ִ� ���ᷮ

	string status;	// ���� ����
	Texture t1, t2;	// Texture : �ѿ� ������ ����, ����

	// Sprite : �����̴� ����. ������ �����ϴ� ĳ����
	// Sprite* spaceship = nullptr;

	// smart pointer ����
	// p.337 : ����� ���� �����ϱ�!!!
	unique_ptr<Sprite> spaceship; // ����Ʈ�ϱ� �ѵ�, �ƹ� �͵� ����Ű�� ���� ����.
								  // nullptr�� �� �־��൵, �ڱⰡ nullptr�� ���� �ȴ�...

	// p.339 'this' pointer
	//
	// Ŭ���� �ȿ����� ���
	// �ڱ� �ڽ��� ����Ű�� ������


	Sprite burst;
	Font font; // ���� ȭ��� ���� ����ϱ� ���� ��Ʈ
	Text text; // ���� ȭ��� ��µǴ� ���ڿ�
public:
	LunarLander(double h, double v, double f);

	~LunarLander(); // spaceship ������ �޸� ������ ���ؼ� �Ҹ��� �ʿ�
					// �� ������ ���� ������ �����⸦ ġ������ �ʴ´�.
					// ---> LunarLander�� ����� spaceship�� �޸𸮴� ��������...

	void update(double dx, double dy); // dx, dy�� ȭ��ǥ ������ ���� ���ϴ� x,y ��ġ

	// RenderWindow: ������ �׷��������� �׷����� â
	void draw(RenderWindow& window);

	// ��Ȯ�� member variable�� parameter ����.
	// this�� ����Ͽ� member variable�� ��Ȯ�� ��Ÿ�� �� �ִ�.
	void setXY(double _x, double _y) {
		this->x = _x;
		this->y = _y;
	}
};

LunarLander::~LunarLander() {
	// delete spaceship; // �Ϲ� pointer�� �� ��...
}

void LunarLander::draw(RenderWindow& window) {
	window.draw(*spaceship); // dereferencing�� ���� ��ü �״�� �޾ƾ� �Ѵ�.
}

LunarLander::LunarLander(double h, double v, double f) {

	// �� ������ �̹����� texture t1�� �ε��Ѵ�.
	t1.loadFromFile("images/spaceship.png");
	// �� ������ ��������Ʈ�� t1�� ������ ���δ�.

	// new, delete Ÿ���� spaceship
	// spaceship = new Sprite(); // �����Ͷ� �ּҰ� ��

	// smart pointer Ÿ���� spaceship ---> �����Ͱ� �ƴ϶� ��ü��.
	spaceship.reset(new Sprite()); // reset(�ּ�) : �ּ� �������ִ� method.

	spaceship->setTexture(t1);

	x = 500.0;
	this->x = 500.0; // �� �ڽ�(��ü) �ӿ� �ִ� x. �� �ڵ�� ����.

	y = 200.0;
	spaceship->setPosition(x, y);
}

void LunarLander::update(double dx, double dy) {
	// x�� y��ǥ�� ��ȯ��Ų��. dx, dy ��ŭ��
	// ����, ���� ȭ��ǥŰ�� ������ dx = -10,
	// ������ ȭ��ǥŰ�� ������ dx = +10,
	// �Ʒ��� ȭ��ǥŰ�� ������ dy = +10,	(����) y���� ������ �������鼭 +.
	// ���� ȭ��ǥŰ�� ������ dy = -10,

	x = x + dx;
	y = y + dy;
	spaceship->setPosition(x, y);
}


int main() {

	// ������ ��Ÿ���� â�� �����.
	// ���� 600, ���� 480â�� �����
	// �»�ܿ� "Lunar Lander" ǥ��
	RenderWindow window(VideoMode(1025, 475), "Lunar Lander");

	window.setFramerateLimit(60); // ������. �ʴ� 60�� ���ѷ����� ���� �׷��ش�.

	// Sprite�� ������ ȭ�鿡 �׷����� ����̰�,
	// Texture�� �� Sprite ���� �׷����� ����.
	Texture t; // ����, ����
	Sprite background; // 
	t.loadFromFile("images/background.png");
	background.setTexture(t);

	// ���ѷ��� ���� ���� �� �������� �����.
	// 0, 0, 0�� ������� �ʱ� ������ �׳� �ƹ��ų� �����ڿ� �Է�.
	LunarLander* lander = new LunarLander(0.0, 0.0, 0.0);
	// �ι�° lander
	// LunarLander lander2(0.0, 0.0, 0.0);
	// �ι�° lander�� smart pointer ����
	unique_ptr<LunarLander> lander2(new LunarLander(0.0, 0.0, 0.0));

	// �����찡 ���� �ִ� �� ����ؼ� �ݺ��ȴ�.
	// �ʴ� 60��
	while (window.isOpen()) {

		Event e;
		while (window.pollEvent(e)) {
			// ���콺�� window ������ư�� ������,
			if (e.type == Event::Closed) {
				window.close();
			}
		}


		if (Keyboard::isKeyPressed(Keyboard::Up)) {
			lander->update(0, -5);
		}
		else if (Keyboard::isKeyPressed(Keyboard::Down)) {
			lander->update(0, +5);
		}
		else if (Keyboard::isKeyPressed(Keyboard::Left)) {
			lander->update(-5, 0);
		}
		else if (Keyboard::isKeyPressed(Keyboard::Right)) {
			lander->update(+5, 0);
		}


		// lander2 Ű ����
		if (Keyboard::isKeyPressed(Keyboard::W)) {
			lander2->update(0, -5);
		}
		else if (Keyboard::isKeyPressed(Keyboard::S)) {
			lander2->update(0, +5);
		}
		else if (Keyboard::isKeyPressed(Keyboard::A)) {
			lander2->update(-5, 0);
		}
		else if (Keyboard::isKeyPressed(Keyboard::D)) {
			lander2->update(+5, 0);
		}


		// RenderWindow�� �׷��� ���� ������ �� �����.
		window.clear();
		// RenderWindow�� sprite�� background�� �׸���.
		window.draw(background);
		// lunarlander�� �׸���.
		lander->draw(window);
		// lunarlander2�� �׸���.
		lander2->draw(window);

		// RenderWindow�� ������ ȭ�鿡 ����.
		window.display(); // ���ۿ� ����Ǿ��ִ� ������ ������ ���� ��.

		Sleep(100); // 100 msec. delay
	}

	// �����ͷ� �����Ҵ��� ���� ��ü�� delete �ʼ�.
	delete lander;
	// smart pointer�� �����Ҵ� ���� ��ü�� delete �ʿ� X.

	return 0;
}