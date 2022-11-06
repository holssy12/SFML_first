#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <algorithm>

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
    // rect ��ü�� ȭ���� ��/���� �����ڸ��� �����ϸ� offset_x����
    // ��ȣ�� �ݴ�� ���ش�.

    // 1. rect ��ü�� ��ġ������ ���.
    Vector2f rect_pos = rect.getPosition();

    // 2. ȭ�� �����ڸ��� �����ߴ��� �Ǵ�.
    if (rect_pos.x <= 0) { // ȭ���� ���� �����ڸ� ����.
        offset_x = 2.0f;
    }
    else if (rect_pos.x + rect.getSize().x >= 640) // ȭ���� ���� �����ڸ� ����.
    {
        offset_x = -2.0f;
    }

    rect.move(offset_x, 0);
}

//
class ENEMY_SET {
public:
    vector<ENEMY> vec;

    ENEMY_SET(int num = 4);
    void draw(RenderWindow& _window);
    void update();
};

// defualt parameter�� ������Ÿ�� or ���� �Լ� ����
// �� �߿� �� ������ �־�� ������ �߻����� �ʴ´�.
ENEMY_SET::ENEMY_SET(int num) {
    for (int i = 0; i < num; i++) {
        ENEMY buf;
        buf.rect.setPosition(i * 30, i * 30);
        vec.push_back(buf);
    }
}

void ENEMY_SET::draw(RenderWindow& _window) {
    for (int i = 0; i < vec.size(); i++) {
        vec[i].draw(_window);
    }
}

void ENEMY_SET::update() {
    for (int i = 0; i < vec.size(); i++) {
        vec[i].update();
    }
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
    body.setPosition(640 / 2 - body.getRadius(), 480 - body.getRadius()); // ȭ���� �ϴܺ� �߽�.

    bullet = CircleShape(10, 3);
    bullet.setFillColor(Color::White);
    bullet.setPosition(999, 999); // ȭ�� ������ ������.

    bullet_alive = false;
}
void GUN::update(RenderWindow& _window)
{
    // �� ��ü�� ���콺 Ŀ���� ��������� ����.
    Vector2i mouse_pos = Mouse::getPosition(_window);
    body.setPosition(mouse_pos.x - body.getRadius(), body.getPosition().y);

    // �� ��ü�� ȭ�� ���� ����� �ʵ��� ����.
    // 
    // 1. �� ��ü�� ��ġ�� ���
    Vector2f body_pos = body.getPosition();
    // 2. �� ��ġ������ ������� ȭ������� �������� �Ǵ�.
    if (body_pos.x <= 0) { // ȭ�� ���� ħ��.
        body.setPosition(0, body_pos.y);
    }
    else if (body_pos.x + 2 * body.getRadius() >= 640) // ȭ�� ���� ħ��.
    {
        body.setPosition(640 - 2 * body.getRadius(), body_pos.y);
    }

    // �Ѿ��� ���� �����̱�
    if (bullet_alive == true) {
        bullet.move(0, -5.0f);
    }

    // �Ѿ��� window�� ���� ��ܺθ� ������� �Ǵ�.
    // �Ѿ� ��ü�� ��ġ
    Vector2f bullet_pos = bullet.getPosition();
    if (bullet_pos.y <= 0) { // �Ѿ��� ȭ���� ��ܺθ� ����� ��.
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

    // �Ѿ��� body�� �߾� ��ܺη� ��������.
    Vector2f body_pos = body.getPosition();
    bullet.setPosition(body_pos.x + body.getRadius() - bullet.getRadius(), body_pos.y);
}

class GAME
{
public:
    // �� ��ü
    ENEMY_SET enemy_set;
    // �� ��ü
    GUN gun;

    Text alive_enemy_num_text;
    Font font;

    GAME();

    void fire();
    void update(RenderWindow& _window);
    void draw(RenderWindow& _window);
    void init_end(RenderWindow& _window, int status = 1);
};
GAME::GAME() {
    font.loadFromFile("images/OpenSans-Bold.ttf");
    alive_enemy_num_text.setFont(font);
    alive_enemy_num_text.setCharacterSize(30);
    alive_enemy_num_text.setFillColor(Color::Magenta);
    alive_enemy_num_text.setStyle(Text::Bold);
}

void GAME::fire()
{
    gun.fire();
}
void GAME::update(RenderWindow& _window)
{
    enemy_set.update();
    gun.update(_window);
    // enemy.rect
    // gun.bullet
    // �� �� ��ü�� ���� �浹 �ߴ��� �˻�.

    FloatRect bullet_area = gun.bullet.getGlobalBounds();

    for (int i = 0; i < enemy_set.vec.size(); i++) {
        FloatRect enemy_area = enemy_set.vec[i].rect.getGlobalBounds();
        if (enemy_area.intersects(bullet_area) == true && enemy_set.vec[i].enemy_alive == true && gun.bullet_alive == true) { // �浹
            gun.bullet_alive = false;
            enemy_set.vec[i].enemy_alive = false;
        }
    }

    // ����ִ� ENEMY�� ���� ����, �� ���� �ؽ�Ʈ ������ �Ἥ body ���� display
    // 1. ����ִ� ENEMY�� ���� ����.
    int alive_enemy_num = 0;
    alive_enemy_num = count_if(enemy_set.vec.begin(), enemy_set.vec.end(),
        [](ENEMY& e) {
            return (e.enemy_alive == true);
        });

    alive_enemy_num_text.setString(to_string(alive_enemy_num));

    Vector2f body_pos = gun.body.getPosition();

    alive_enemy_num_text.setPosition(body_pos.x + 45, body_pos.y + 15);

    // ������ ������ ���� �˻�.
    // ENEMY�� �� ������,
    // ENEMY.enemy_alive�� ���� false.

    for (int i = 0; i < enemy_set.vec.size(); i++) {
        if (enemy_set.vec[i].enemy_alive == true) return;
    }

    // ��� ������ paramer�� 0�� �ְ� init_end ȣ��.
    init_end(_window, 0);

}
void GAME::draw(RenderWindow& _window)
{
    enemy_set.draw(_window);
    gun.draw(_window);
    _window.draw(alive_enemy_num_text);
}

void GAME::init_end(RenderWindow& _window, int status) {

    RectangleShape startBtn(Vector2f(300.0f, 150.0f));
    startBtn.setPosition(640 / 2 - 150, 480 / 2 - 75);

    Texture startBtnTexture;
    if (status == 1) {
        startBtnTexture.loadFromFile("images/start.png");
    }
    else if (status == 0) {
        startBtnTexture.loadFromFile("images/clear.png");
    }
    // RectangleShape�� setTexture�� ����, Texture�� �ּҸ� �Ѱ��־�� �Ѵ�.
    startBtn.setTexture(&startBtnTexture);

    // ��ư�� ����ȭ���̱� ������, ��� ������Ʈ ���� �ʿ䰡 ����.
    _window.clear();
    _window.draw(startBtn);
    _window.display();

    bool signal = false;

    while (_window.isOpen())
    {
        // ���콺�� ��Ŭ�� event�� ����.

        Event event;
        while (_window.pollEvent(event))
        {
            switch (event.type)
            {
            case Event::Closed:
                _window.close();
                break;
            case Event::MouseButtonPressed:
                // ���콺�� ��Ŭ������ ���� ��ư �������� �˻�.
                if (Mouse::isButtonPressed(Mouse::Left) == true) {
                    // ���콺 Ŀ���� ��ġ�� startBtn ���� �ִ��� �˻�.
                    Vector2i mouse_pos = Mouse::getPosition(_window);

                    Vector2f btn_pos = startBtn.getPosition();
                    Vector2f btn_size = startBtn.getSize();

                    if (mouse_pos.x >= btn_pos.x && mouse_pos.x <= btn_pos.x + btn_size.x) {
                        if (mouse_pos.y >= btn_pos.y && mouse_pos.y <= btn_pos.y + btn_size.y) {
                            signal = true;
                        }
                    }
                }

                break;

            default:
                break;
            }
        }

        if (signal == true) {
            break;
        }
    }
}

int main()
{
    RenderWindow window(VideoMode(640, 480), "shoot");
    window.setFramerateLimit(120);

    GAME game;

    game.init_end(window);

    while (window.isOpen())
    {
        // ���콺�� ��Ŭ�� event�� ����.

        Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case Event::Closed:
                window.close();
                break;
            case Event::MouseButtonPressed:
                // ���콺�� ��Ŭ������ �Ѿ� �߻�.
                if (Mouse::isButtonPressed(Mouse::Left) == true) {
                    // �Ѿ��� �߻�.
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