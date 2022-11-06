#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#define DIRECTION_LEFT 0
#define DIRECTION_RIGHT 1

enum stage {
    Start = 0, Playing = 1, Clear = 2
};

using namespace sf;
using namespace std;

class Gun {

public:
    CircleShape gun;
    CircleShape bullet;
    bool bulletFired = false;

    Gun() {
        gun = CircleShape(50.0f);
        gun.setFillColor(Color::Green);
        gun.setPosition(640.0f - 50.0f, 720.0f - 50.0f);

        bullet = CircleShape(10);
        bullet.setFillColor(Color::Red);
    }
    void draw(RenderWindow& _window) {
        _window.draw(총);

        if (bulletFired == true) {
            _window.draw(bullet);
        }
    }
    void moveLeft() {

        if (gun.getPosition().x <= 0.0f) {
            return;
        }

        gun.move(-10.0f, 0.0f);
    }
    void moveRight() {

        if (gun.getPosition().x >= 1280.0f - 100.0f) {
            return;
        }

        gun.move(10.0f, 0.0f);
    }
    void fireBullet() {

        if (bulletFired == true) {
            return;
        }

        bulletFired = true;
        bullet.setPosition(gun.getPosition().x + 50.0f, gun.getPosition().y);
    }
    void moveBullet() {
        if (bulletFired == true) {

            Vector2f bulletPos = bullet.getPosition();

            if (bulletPos.y < -10.0f) {
                bulletFired = false;
                return;
            }

            bullet.move(0.0f, -30.0f);
        }
    }
};

class Enemy {
public:
    int direction = DIRECTION_RIGHT;
    RectangleShape enemy;

    Enemy(float _x, float _y) {
        enemy = RectangleShape(Vector2f(50.0f, 10.0f));
        enemy.setFillColor(Color::Blue);
        enemy.setPosition(_x, _y);
    }
    void move() {
        if (direction == DIRECTION_RIGHT) {
            enemy.move(10.0f, 0.0f);

            if (enemy.getPosition().x > 1180.0f - 50.0f) {
                direction = DIRECTION_LEFT;
            }
        }
        else if (direction == DIRECTION_LEFT) {
            enemy.move(-10.0f, 0.0f);

            if (enemy.getPosition().x < 100.0f) {
                direction = DIRECTION_RIGHT;
            }
        }
    }
};

class MyText {
public:
    int stage = Start;
    Vector2f mousePos;

    MyText() {
        font.loadFromFile("fonts/NotoSans-Bold.ttf");
        textRect = RectangleShape(Vector2f(300.0f, 100.0f));
        textRect.setFillColor(Color::Red);
        textRect.setPosition(640.0f - 150.0f, 360.0f - 50.0f);
    }
    void setText(string _str) {
        text = Text(_str, font);
        text.setCharacterSize(100);
        text.setStyle(Text::Regular);
        text.setFillColor(Color::White);
        text.setOutlineColor(Color::Red);
        text.setPosition(640.0f - 140.0f, 360.0f - 65.0f);
    }
    void draw(RenderWindow& _window) {
        _window.draw(textRect);
        _window.draw(text);
    }
    void updateMousePos() {
        Vector2i mousePos = Mouse::getPosition();
        float mousePosX = (float)mousePos.x;
        float mousePosY = (float)mousePos.y;
        this->mousePos.x = mousePosX;
        this->mousePos.y = mousePosY;
    }
    /*void checkRectPressed() {

       if (this->textRect.getGlobalBounds().contains(mousePos)) {
          if (Mouse::isButtonPressed(Mouse::Left) == true) {
             this->stage = Playing;
          }
       }
    }*/

private:
    Font font;
    Text text;
    RectangleShape textRect;
};

class Enemies {
public:
    vector<Enemy> enemies;

    Enemies() {
        for (int i = 0; i <= 4; i++) {
            Enemy enemy(100.0f + 100.0f * i, 100.0f + 30.0f * i);
            enemies.push_back(enemy);
        }
    }
    void move() {
        for (auto& _e : enemies) {
            _e.move();
        }
    }
    void draw(RenderWindow& _window) {
        for (auto& _e : enemies) {
            _window.draw(_e.enemy);
        }
    }
    void checkHit(Gun& _gun) {
        for (int i = 0; i < enemies.size(); i++) {
            FloatRect bulletRect = _gun.bullet.getGlobalBounds();
            FloatRect enemyRect = enemies[i].enemy.getGlobalBounds();

            // 만약 getGlobalBounds()를 통해 얻은 FloatRect끼리
            // intersects method로 충돌 여부를 판단하여 충돌이 확인되면
            // enemies에서 해당 enemy를 erase하는 형식으로 코드 수정
            // 수정 결과, 삭제 한 뒤, i--를 해주면 정상적으로 동작하는 것으로 확인됨.
            if (bulletRect.intersects(enemyRect) == true) {
                enemies.erase(enemies.begin() + i);
                i--;
            }
        }
    }
    void checkDestroyedAll(MyText& myText) {

        if (enemies.size() > 0) {
            return;
        }

        myText.stage = Clear;
        myText.setText("Clear!");
    }
};

int main(void) {

    RenderWindow window(VideoMode(1280, 720), "Shooting Game Example");
    window.setFramerateLimit(30);

    // 만약 생성자가 parameter를 받지 않는다면,
    // ()를 붙이지 않고 생성해야 오류없이 실행된다?
    Gun gun;
    Enemies enemies;
    MyText myText;
    myText.setText("Start!");

    while (window.isOpen()) {

        Event globalEvent;

        while (window.pollEvent(globalEvent)) {

            switch (globalEvent.type) {
            case Event::Closed:
                window.close();
                break;
            default:
                break;
            }
        }

        if (myText.stage == Start) {
            //myText.updateMousePos();
            //myText.checkRectPressed();

            if (Mouse::isButtonPressed(Mouse::Left) == true) {
                myText.stage = Playing;
            }
        }

        if (myText.stage == Playing) {
            if (Keyboard::isKeyPressed(Keyboard::Left) == true) {
                gun.moveLeft();
            }
            if (Keyboard::isKeyPressed(Keyboard::Right) == true) {
                gun.moveRight();
            }
            if (Keyboard::isKeyPressed(Keyboard::Space) == true) {
                gun.fireBullet();
            }

            enemies.checkHit(총);
            gun.moveBullet();
            enemies.move();
        }

        window.clear();

        switch (myText.stage) {
        case Start:
            myText.draw(window);
            break;
        case Playing:
            gun.draw(window);
            enemies.draw(window);
            enemies.checkDestroyedAll(myText);
            break;
        case Clear:
            myText.draw(window);
            break;
        default:
            break;
        }

        window.display();
    }

    return 0;
}