#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

using namespace std;
using namespace sf;

class ENEMY
{
public:
    RectangleShape rect;
    float offset_x;
    bool enemy_alive;

    ENEMY();
    void draw(RenderWindow& _window);
    void update();
};
ENEMY::ENEMY()
{
    enemy_alive = true;

    rect = RectangleShape(Vector2f(80.0f, 20.0f));
    rect.setFillColor(Color::Magenta);
    rect.setPosition(30, 30);

    offset_x = 2.0f;


}
void ENEMY::draw(RenderWindow& _window)
{
    if (enemy_alive == false) {
        return;
    }
    _window.draw(rect);
}
void ENEMY::update()
{
    if (enemy_alive == false) {
        return;
    }
    // rect 객체가 화면의 좌/우측 가장자리에 도달하면 offset_x값의
    // 부호를 반대로 해준다.

    // 1. rect 객체의 위치정보를 얻기.
    Vector2f rect_pos = rect.getPosition();

    // 2. 화면 가장자리에 도달했는지 판단.
    if (rect_pos.x <= 0) { // 화면의 좌측 가장자리 도달.
        offset_x = 2.0f;
    }
    else if (rect_pos.x + rect.getSize().x >= 640) // 화면의 우측 가장자리 도달.
    {
        offset_x = -2.0f;
    }

    rect.move(offset_x, 0);
}

class GUN
{
public:
    CircleShape body;
    CircleShape bullet;
    bool bullet_alive;

    GUN();

    void update(RenderWindow& _window);
    void draw(RenderWindow& _window);
    void fire();
};
GUN::GUN()
{
    body = CircleShape(50);
    body.setFillColor(Color::Green);
    body.setPosition(640 / 2 - body.getRadius(), 480 - body.getRadius()); // 화면의 하단부 중심.

    bullet = CircleShape(10, 3);
    bullet.setFillColor(Color::White);
    bullet.setPosition(999, 999); // 화면 밖으로 빼놓기.

    bullet_alive = false;
}
void GUN::update(RenderWindow& _window)
{
    // 원 객체가 마우스 커서를 따라오도록 구현.
    Vector2i mouse_pos = Mouse::getPosition(_window);
    body.setPosition(mouse_pos.x - body.getRadius(), body.getPosition().y);

    // 원 객체가 화면 밖을 벗어나지 않도록 구현.
    // 
    // 1. 원 객체의 위치를 얻고
    Vector2f body_pos = body.getPosition();
    // 2. 그 위치정보를 기반으로 화면밖으로 나갔는지 판단.
    if (body_pos.x <= 0) { // 화면 좌측 침범.
        body.setPosition(0, body_pos.y);
    }
    else if (body_pos.x + 2 * body.getRadius() >= 640) // 화면 우측 침범.
    {
        body.setPosition(640 - 2 * body.getRadius(), body_pos.y);
    }

    // 총알을 위로 움직이기
    if (bullet_alive == true) {
        bullet.move(0, -5.0f);
    }

    // 총알의 window의 가장 상단부를 찍었지를 판단.
    // 총알 객체의 위치
    Vector2f bullet_pos = bullet.getPosition();
    if (bullet_pos.y <= 0) { // 총알이 화면의 상단부를 찍었을 때.
        bullet_alive = false;
    }
}
void GUN::draw(RenderWindow& _window)
{
    _window.draw(body);
    if (bullet_alive == true) {
        _window.draw(bullet);
    }
}
void GUN::fire()
{
    if (bullet_alive == true) {
        return;
    }

    bullet_alive = true;

    // 총알을 body의 중앙 상단부로 가져오기.
    Vector2f body_pos = body.getPosition();
    bullet.setPosition(body_pos.x + body.getRadius() - bullet.getRadius(), body_pos.y);
}

class GAME
{
public:
    // 적 객체
    ENEMY enemy;
    // 원 객체
    GUN gun;

    void fire();
    void update(RenderWindow& _window);
    void draw(RenderWindow& _window);
};
void GAME::fire()
{
    gun.fire();
}
void GAME::update(RenderWindow& _window)
{
    enemy.update();
    gun.update(_window);
    // enemy.rect
    // gun.bullet
    // 위 두 객체가 서로 충돌 했는지 검사.

    FloatRect enemy_area = enemy.rect.getGlobalBounds();
    FloatRect bullet_area = gun.bullet.getGlobalBounds();
    if (enemy_area.intersects(bullet_area) == true && enemy.enemy_alive == true && gun.bullet_alive == true) { // 충돌
        gun.bullet_alive = false;
        enemy.enemy_alive = false;
    }

}
void GAME::draw(RenderWindow& _window)
{
    enemy.draw(_window);
    gun.draw(_window);
}

int main()
{
    RenderWindow window(VideoMode(640, 480), "shoot");
    window.setFramerateLimit(120);

    GAME game;

    while (window.isOpen())
    {
        // 마우스의 좌클릭 event를 감지.

        Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case Event::Closed:
                window.close();
                break;
            case Event::MouseButtonPressed:
                // 마우스의 좌클릭으로 총알 발사.
                if (Mouse::isButtonPressed(Mouse::Left) == true) {
                    // 총알을 발사.
                    game.fire();
                }

                break;

            default:
                break;
            }
        }

        /* update */
        game.update(window);

        /* clear */
        window.clear();

        /* draw */
        game.draw(window);

        /* display */
        window.display();
    }



    return 0;
}