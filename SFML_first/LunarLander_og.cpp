// p.302

#include <SFML/Graphics.hpp>
#include <windows.h> // 타이머...
#include <string>
#include <iostream>

using namespace sf;
using namespace std;

class LunarLander {
private:
	double x, y;		// 착륙선 현재 위치 ( y축은 아래로 갈수록 커짐. )
	double velocity;	// 착륙 속도
	double fuel;		// 남아있는 연료량

	string status;	// 현재 상태
	Texture t1, t2;	// Texture : 겉에 입히는 패턴, 무늬
	Sprite spaceship, burst; // Sprite : 움직이는 조각. 실제로 조종하는 캐릭터
	Font font; // 게임 화면상에 뭔가 출력하기 위한 폰트
	Text text; // 게임 화면상에 출력되는 문자열
public:
	LunarLander(double h, double v, double f);
	void update(double dx, double dy); // dx, dy는 화살표 누름에 따라서 변하는 x,y 위치

	// RenderWindow: 게임이 그래픽적으로 그려지는 창
	void draw(RenderWindow& window);
};

void LunarLander::draw(RenderWindow& window) {
	window.draw(spaceship);
}

LunarLander::LunarLander(double h, double v, double f) {

	// 달 착륙선 이미지를 texture t1에 로딩한다.
	t1.loadFromFile("images/spaceship.png");
	// 달 착륙선 스프라이트에 t1을 가져다 붙인다.
	spaceship.setTexture(t1);
	x = 500.0;
	y = 200.0;
	spaceship.setPosition(x, y);
}

void LunarLander::update(double dx, double dy) {
	// x와 y좌표를 변환시킨다. dx, dy 만큼씩
	// 만약, 왼쪽 화살표키를 누르면 dx = -10,
	// 오른쪽 화살표키를 누르면 dx = +10,
	// 아래쪽 화살표키를 누르면 dy = +10,	(주의) y축은 밑으로 내려가면서 +.
	// 위쪽 화살표키를 누르면 dy = -10,

	x = x + dx;
	y = y + dy;
	spaceship.setPosition(x, y);
}


int main() {

	// 게임이 나타나는 창을 만든다.
	// 가로 600, 세로 480창을 만들고
	// 좌상단에 "Lunar Lander" 표시
	RenderWindow window(VideoMode(1025, 475), "Lunar Lander");

	window.setFramerateLimit(60); // 프레임. 초당 60번 무한루프를 돌며 그려준다.

	// Sprite는 실제로 화면에 그려지는 요소이고,
	// Texture는 그 Sprite 위에 그려지는 패턴.
	Texture t; // 무늬, 패턴
	Sprite background; // 
	t.loadFromFile("images/background.png");
	background.setTexture(t);

	// 무한루프 들어가기 전에 달 착륙선을 만든다.
	// 0, 0, 0은 사용하지 않기 때문에 그냥 아무거나 생성자에 입력.
	LunarLander lander(0.0, 0.0, 0.0);
	// 두번째 lander
	LunarLander lander2(0.0, 0.0, 0.0);

	// 윈도우가 열려 있는 한 계속해서 반복된다.
	// 초당 60번
	while (window.isOpen()) {

		Event e;
		while (window.pollEvent(e)) {
			// 마우스로 window 닫힘버튼을 누르면,
			if (e.type == Event::Closed) {
				window.close();
			}
		}


		if (Keyboard::isKeyPressed(Keyboard::Up)) {
			lander.update(0, -5);
		}
		else if (Keyboard::isKeyPressed(Keyboard::Down)) {
			lander.update(0, +5);
		}
		else if (Keyboard::isKeyPressed(Keyboard::Left)) {
			lander.update(-5, 0);
		}
		else if (Keyboard::isKeyPressed(Keyboard::Right)) {
			lander.update(+5, 0);
		}


		// lander2 키 조작
		if (Keyboard::isKeyPressed(Keyboard::W)) {
			lander2.update(0, -5);
		}
		else if (Keyboard::isKeyPressed(Keyboard::S)) {
			lander2.update(0, +5);
		}
		else if (Keyboard::isKeyPressed(Keyboard::A)) {
			lander2.update(-5, 0);
		}
		else if (Keyboard::isKeyPressed(Keyboard::D)) {
			lander2.update(+5, 0);
		}


		// RenderWindow에 그려진 것이 있으면 싹 지운다.
		window.clear();
		// RenderWindow에 sprite인 background를 그린다.
		window.draw(background);
		// lunarlander를 그린다.
		lander.draw(window);
		// lunarlander2를 그린다.
		lander2.draw(window);

		// RenderWindow를 실제로 화면에 띄운다.
		window.display(); // 버퍼에 저장되어있는 내용을 실제로 띄우는 것.

		Sleep(100); // 100 msec. delay
	}


	return 0;
}