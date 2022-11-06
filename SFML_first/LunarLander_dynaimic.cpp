
// 1. 동적 메모리 할당으로
// LunarLander를 재구성 해보자.

// 2. 동적 할당으로 class 속 메모리를 줄여보자.
//		---> 포인터로 만들고 필요할 때만 메모리 생성.
//		---> 정적 생성하면 이미 메모리가 만들어짐.

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

	// Sprite : 움직이는 조각. 실제로 조종하는 캐릭터
	// Sprite* spaceship = nullptr;

	// smart pointer 버전
	// p.337 : 멤버도 동적 생성하기!!!
	unique_ptr<Sprite> spaceship; // 스마트하긴 한데, 아무 것도 가리키는 것이 없음.
								  // nullptr을 안 넣어줘도, 자기가 nullptr인 것을 안다...

	// p.339 'this' pointer
	//
	// 클래스 안에서만 사용
	// 자기 자신을 가리키는 포인터


	Sprite burst;
	Font font; // 게임 화면상에 뭔가 출력하기 위한 폰트
	Text text; // 게임 화면상에 출력되는 문자열
public:
	LunarLander(double h, double v, double f);

	~LunarLander(); // spaceship 때문에 메모리 해제를 위해서 소멸자 필요
					// 그 누구도 내가 가져온 쓰레기를 치워주지 않는다.
					// ---> LunarLander만 지우면 spaceship의 메모리는 남아있음...

	void update(double dx, double dy); // dx, dy는 화살표 누름에 따라서 변하는 x,y 위치

	// RenderWindow: 게임이 그래픽적으로 그려지는 창
	void draw(RenderWindow& window);

	// 명확히 member variable과 parameter 구분.
	// this를 사용하여 member variable을 정확히 나타낼 수 있다.
	void setXY(double _x, double _y) {
		this->x = _x;
		this->y = _y;
	}
};

LunarLander::~LunarLander() {
	// delete spaceship; // 일반 pointer를 쓸 떄...
}

void LunarLander::draw(RenderWindow& window) {
	window.draw(*spaceship); // dereferencing을 통해 객체 그대로 받아야 한다.
}

LunarLander::LunarLander(double h, double v, double f) {

	// 달 착륙선 이미지를 texture t1에 로딩한다.
	t1.loadFromFile("images/spaceship.png");
	// 달 착륙선 스프라이트에 t1을 가져다 붙인다.

	// new, delete 타입의 spaceship
	// spaceship = new Sprite(); // 포인터라서 주소가 들어감

	// smart pointer 타입의 spaceship ---> 포인터가 아니라 객체임.
	spaceship.reset(new Sprite()); // reset(주소) : 주소 연결해주는 method.

	spaceship->setTexture(t1);

	x = 500.0;
	this->x = 500.0; // 나 자신(객체) 속에 있는 x. 위 코드와 동일.

	y = 200.0;
	spaceship->setPosition(x, y);
}

void LunarLander::update(double dx, double dy) {
	// x와 y좌표를 변환시킨다. dx, dy 만큼씩
	// 만약, 왼쪽 화살표키를 누르면 dx = -10,
	// 오른쪽 화살표키를 누르면 dx = +10,
	// 아래쪽 화살표키를 누르면 dy = +10,	(주의) y축은 밑으로 내려가면서 +.
	// 위쪽 화살표키를 누르면 dy = -10,

	x = x + dx;
	y = y + dy;
	spaceship->setPosition(x, y);
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
	LunarLander* lander = new LunarLander(0.0, 0.0, 0.0);
	// 두번째 lander
	// LunarLander lander2(0.0, 0.0, 0.0);
	// 두번째 lander의 smart pointer 버전
	unique_ptr<LunarLander> lander2(new LunarLander(0.0, 0.0, 0.0));

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


		// lander2 키 조작
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


		// RenderWindow에 그려진 것이 있으면 싹 지운다.
		window.clear();
		// RenderWindow에 sprite인 background를 그린다.
		window.draw(background);
		// lunarlander를 그린다.
		lander->draw(window);
		// lunarlander2를 그린다.
		lander2->draw(window);

		// RenderWindow를 실제로 화면에 띄운다.
		window.display(); // 버퍼에 저장되어있는 내용을 실제로 띄우는 것.

		Sleep(100); // 100 msec. delay
	}

	// 포인터로 동적할당을 받은 객체는 delete 필수.
	delete lander;
	// smart pointer로 동적할당 받은 객체는 delete 필요 X.

	return 0;
}