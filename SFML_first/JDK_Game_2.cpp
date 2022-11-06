#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 500
#define TO_RIGHT 1
#define TO_LEFT -1

using namespace sf;
using namespace std;

enum Game_Stage {
    START = 0, PLAYING = 1, END = 2
};

class Bullet {
public:
    CircleShape bullet;

public:
    Bullet() {
        bullet = CircleShape(5.0f, 3);
        bullet.setFillColor(Color::Yellow);
    }
    void draw(RenderWindow& _window) {
        _window.draw(bullet);
    }
    void update() {
        bullet.move(0.0f, -10.0f);
    }
    void fire(Vector2f bullet_pos) {
        bullet.setPosition(bullet_pos);
    }
};

class Gun {
public:
    CircleShape gun;
    vector<Bullet> bullets;

public:
    Gun() {
        gun = CircleShape(10.0f);
        gun.setFillColor(Color::Green);
    }
    void draw(RenderWindow& _window) {
        _window.draw(총);

        for (auto& bullet : bullets) {
            bullet.draw(_window);
        }
    }
    void update(RenderWindow& _window) {

        Vector2i mouse_pos = Mouse::getPosition(_window);

        // mouse가 화면 밖으로 벗어나는 것을 block 해준다.
        if (mouse_pos.x <= 0) {
            gun.setPosition(0.0f, WINDOW_HEIGHT - gun.getRadius());
            return;
        }
        if (mouse_pos.x >= 500 - 2 * gun.getRadius()) {
            gun.setPosition(WINDOW_WIDTH - 2 * gun.getRadius(), WINDOW_HEIGHT - gun.getRadius());
            return;
        }

        gun.setPosition(mouse_pos.x, WINDOW_HEIGHT - gun.getRadius());

        // 
        for (int i = 0; i < bullets.size(); i++) {
            if (bullets[i].bullet.getPosition().y <= 0) {
                bullets.erase(bullets.begin() + i);
                i--;
            }
        }

        for (auto& bullet : bullets) {
            bullet.update();
        }
    }
    void fire() {

        if (bullets.size() >= 3) {
            return;
        }

        // 현재 bullet이 발사된 상태가 아니므로, bullet을 발사합니다.
        Vector2f bullet_pos;
        bullet_pos.x = gun.getPosition().x - gun.getRadius() + 10.0f;
        bullet_pos.y = gun.getPosition().y + 2 * 10.0f;

        Bullet bullet;
        bullet.fire(bullet_pos);
        bullets.push_back(bullet);
    }
};

class Enemy {
public:
    RectangleShape enemy;
    int direction = TO_RIGHT;

public:
    Enemy(Vector2f enemy_pos) {
        enemy = RectangleShape(Vector2f(25.0f, 5.0f));
        enemy.setFillColor(Color::Red);
        enemy.setPosition(enemy_pos);
    }
    void draw(RenderWindow& _window) {
        _window.draw(enemy);
    }
    void update() {

        if (enemy.getPosition().x >= WINDOW_WIDTH - enemy.getSize().x && direction == TO_RIGHT) {
            direction = TO_LEFT;
        }
        else if (enemy.getPosition().x <= 0 && direction == TO_LEFT) {
            direction = TO_RIGHT;
        }

        enemy.move(direction * 5.0f, 0.0f);
    }
};

class Enemies {
public:
    vector<Enemy> enemies;

public:
    Enemies() {
        for (int i = 0; i < 20; i++) {
            Vector2f enemy_pos = Vector2f(10.0f + 30 * i, 10.0f + 5.0f * i);
            Enemy enemy(enemy_pos);
            enemies.push_back(enemy);
        }
    }
    void draw(RenderWindow& _window) {
        for (auto& enemy : enemies) {
            enemy.draw(_window);
        }
    }
    void update() {
        for (int i = 0; i < enemies.size(); i++) {
            enemies[i].update();
        }
    }

};

class Stage {
public:
    int stage;

public:
    Stage() {
        stage = START;
    }
};

class Game {
public:
    Gun gun;
    Enemies enemies;
    Stage stage;

    Font font;
    Text text;
    RectangleShape background;

public:
    Game() {
        font.loadFromFile("SFML/fonts/NotoSans-Bold.ttf");
        text = Text("Left Click to Start!", font);
        text.setStyle(Text::Regular);
        text.setCharacterSize(25);
        text.setFillColor(Color::White);
        text.setPosition(130.0f, WINDOW_HEIGHT / 2 - 30.0f);
        text.setOutlineColor(Color::Black);
        text.setOutlineThickness(2.0f);

        background = RectangleShape(Vector2f(250.0f, 50.0f));
        background.setFillColor(Color::Red);
        background.setPosition(120.0f, 210.0f);
    }
    void draw(RenderWindow& _window) {
        gun.draw(_window);
        enemies.draw(_window);
    }
    void update(RenderWindow& _window) {
        gun.update(_window);

        // enemy를 update하기 전에, bullet과 enemy가 부딪혔는지 체크하고, 제거한다.
        check_hit();
        enemies.update();
    }
    void start_stage(RenderWindow& _window) {

        if (Mouse::isButtonPressed(Mouse::Left)) {

            Vector2i mouse_pos = Mouse::getPosition(_window);

            if (mouse_pos.x >= 120.0f &&
                mouse_pos.x <= 120.0f + background.getSize().x &&
                mouse_pos.y >= 210.0f &&
                mouse_pos.y <= 210.0f + background.getSize().y)

            {
                this->stage.stage = PLAYING;
            }
        }
        else {
            _window.draw(background);
            _window.draw(text);
        }
    }
    void check_end() {
        if (enemies.enemies.size() == 0) {
            stage.stage = END;
            text.setString("CLEAR!");
            text.setPosition(210.0f, 230.0f);
            text.setFillColor(Color::Green);
        }
    }
    void end_stage(RenderWindow& _window) {
        _window.draw(text);
    }

    // enemy와 bullet을 모두 담고있는 game class에서
    // intersect가 일어난 bullet과 enemy를 모두 제거해준다.
    void check_hit() {
        for (int i = 0; i < enemies.enemies.size(); i++) {

            FloatRect enemy_area = enemies.enemies[i].enemy.getGlobalBounds();

            for (int j = 0; j < gun.bullets.size(); j++) {
                FloatRect bullet_area = gun.bullets[j].bullet.getGlobalBounds();

                if (enemy_area.intersects(bullet_area) == true) {
                    enemies.enemies.erase(enemies.enemies.begin() + i);
                    i--;

                    gun.bullets.erase(gun.bullets.begin() + j);
                    break;
                }
            }
        }
    }
};

int main(void) {

    RenderWindow window(VideoMode(500, 500), "Shooting Game");
    window.setFramerateLimit(60);

    Game game;

    while (window.isOpen()) {

        Event event;

        while (window.pollEvent(event)) {

            if (event.type == Event::Closed) {
                window.close();
            }

            // 왜 pollEvent에서 fire를 check하지 않으면 끊어서 발사가 불가능할까?
            if (Mouse::isButtonPressed(Mouse::Left) == true) {
                game.gun.fire();
            }
        }

        window.clear();

        // stage 구분하는 if statements
        if (game.stage.stage == START) {
            game.start_stage(window);
        }
        else if (game.stage.stage == PLAYING) {

            game.update(window);
            game.draw(window);
            game.check_end();
        }
        else if (game.stage.stage == END) {
            game.end_stage(window);
        }

        window.display();
    }

    return 0;
}