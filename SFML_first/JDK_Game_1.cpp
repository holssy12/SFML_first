#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

#define BAR_HEIGHT 500.0f

using namespace std;
using namespace sf;

class Ball {
public:
    CircleShape ball;
    Vector2f ball_move;

    Ball() {
        ball = CircleShape(10.0f);
        ball.setFillColor(Color::Magenta);
        ball.setPosition(250.0f - ball.getRadius(), 250.0f);

        ball_move = Vector2f(0.0f, 3.0f);
    }
    void draw(RenderWindow& _window) {
        _window.draw(ball);
    }
    void update(RenderWindow& _window) {

        // 화면 위로 나가는 것을 막음
        if (ball.getPosition().y <= 0) {
            ball_move.y = -ball_move.y;
        }

        // 화면 오른쪽으로 나가는 것을 막음
        if (ball.getPosition().x + ball.getRadius() >= 500) {
            ball_move.x = -ball_move.x;
        }

        // 화면 왼쪽으로 나가는 것을 막음
        if (ball.getPosition().x <= 0) {
            ball_move.x = -ball_move.x;
        }

        // 수직으로 떨어질 때, 오른쪽으로 치우치게 만듦 -> 알고리즘 개선 필요.
        if (ball_move.y >= 0 && ball_move.x == 0) {
            ball_move.x = 1.0f;
        }

        if (ball.getPosition().y >= 500.0f) {
            _window.close();
        }

        ball.move(ball_move);
    }

};

class Brick {
public:
    RectangleShape brick;

    Brick(Vector2f brick_pos) {
        brick = RectangleShape(Vector2f(38.0f, 20.0f));
        brick.setFillColor(Color::Red);
        brick.setPosition(brick_pos);
    }
    void draw(RenderWindow& _window) {
        _window.draw(brick);
    }

};

class BrickSet {
public:
    vector<Brick> bricks;

    BrickSet() {

        for (int i = 0; i < 3; i++) {

            for (int j = 0; j < 11; j++) {

                Vector2f brick_pos;
                brick_pos.x = 6.0f + 45 * j;
                brick_pos.y = 100.0f + 25 * i;

                Brick brick(brick_pos);

                bricks.push_back(brick);
            }
        }
    }
    void draw(RenderWindow& _window) {
        for (auto& brick : bricks) {
            brick.draw(_window);
        }
    }
};

class Bar {
public:
    RectangleShape bar;

    Bar() {
        bar = RectangleShape(Vector2f(50.0f, 20.0f));
        bar.setFillColor(Color::Green);
        bar.setPosition(250.0f - 25.0f, BAR_HEIGHT);
    }
    void draw(RenderWindow& _window) {
        _window.draw(bar);
    }
    void update(RenderWindow& _window) {
        Vector2i mouse_pos = Mouse::getPosition(_window);

        if (mouse_pos.x <= 0) { // 마우스 위치가 window의 왼쪽으로 나갈 경우
            cout << "mouse_pos.x <= 0\n";
            bar.setPosition(0.0f, BAR_HEIGHT - 20.0f);
            return;
        }
        else if (mouse_pos.x >= 500.0f - bar.getSize().x) { // 마우스의 위치가 window의 오른쪽으로 나갈 경우
            bar.setPosition(500.0f - bar.getSize().x, BAR_HEIGHT - 20.0f);
            return;
        }

        // 나가지 않았다면, bar의 위치를 갱신해준다.

        Vector2f bar_position;
        bar_position.x = mouse_pos.x - bar.getSize().x / 2;
        bar_position.y = BAR_HEIGHT - 20.0f;

        bar.setPosition(bar_position);
    }
};

class Game {
public:
    Bar bar;
    BrickSet brickSet;
    Ball ball;
    int prevent_cnt = 0;
    bool recently_intersected = false;

public:
    void ball_intersects_bar() {
        FloatRect ball_area = ball.ball.getGlobalBounds();
        FloatRect bar_area = bar.bar.getGlobalBounds();

        // 공이 바와 부딪히면, 공의 y축 진행 방향을 바꿈.

        if (recently_intersected == true) {
            prevent_cnt_on();
            return;
        }

        if (ball_area.intersects(bar_area)) {
            recently_intersected = true;

            cout << "intersects!\n";
            Vector2f ball_pos = ball.ball.getPosition();
            ball_pos.x += ball.ball.getRadius();
            ball_pos.y += ball.ball.getRadius();

            Vector2f bar_pos = bar.bar.getPosition();

            // 맞는 방향에 따라서 꺾이는 각도가 달라지도록 만들어야 한다.
            // 문제 발생, 옆에 맞는 경우에, intersects가 여러 차례 발생해서
            // 의도치 않은 동작이 발생하게 된다.

            // 왼쪽에 맞는 경우
            if (ball_pos.x < bar_pos.x) {
                ball.ball_move.x = -3.0f;
                cout << "1\n" << endl;
            }

            // 오른쪽에 맞는 경우
            else if (ball_pos.x > bar_pos.x + bar.bar.getSize().x) {
                ball.ball_move.x = 3.0f;
                cout << "2\n" << endl;

            }

            // 위에 맞는 경우
            else if (ball_pos.x >= bar_pos.x && ball_pos.x <= bar_pos.x + bar.bar.getSize().x) {

                // 위에 맞았으므로 진행방향을 바꿔줌
                if (ball_pos.x < 0) {
                    ball_pos.x = -1.0f;
                    cout << "3\n" << endl;

                }
                else {
                    ball_pos.x = 1.0f;
                    cout << "4\n" << endl;

                }
            }

            ball.ball_move.y = -ball.ball_move.y;
        }
    }
    void ball_intersects_bricks() {
        FloatRect ball_area = ball.ball.getGlobalBounds();

        for (int i = 0; i < brickSet.bricks.size(); i++) {

            FloatRect brick_area = brickSet.bricks[i].brick.getGlobalBounds();

            if (ball_area.intersects(brick_area)) {
                ball.ball_move.y = -ball.ball_move.y;

                brickSet.bricks.erase(brickSet.bricks.begin() + i);
            }
        }
    }
    // 한번 bar와 intersect가 발생한다면, 일정 시간동안 다시 intersect가 일어나지 않도록 만들어줌.
    void prevent_cnt_on() {

        cout << prevent_cnt << endl;
        if (prevent_cnt == 100 && recently_intersected == true) {
            prevent_cnt = 0;
            recently_intersected = false;
        }
        else if (prevent_cnt < 100 && recently_intersected == true) {
            prevent_cnt++;
        }
    }
};

int main(void) {

    RenderWindow window(VideoMode(500, 500), "Split!");
    window.setFramerateLimit(120);

    Game game;

    while (window.isOpen()) {

        Event event;

        while (window.pollEvent(event)) {

            if (event.type == Event::Closed) {
                window.close();
            }
        }

        game.bar.update(window);
        game.ball.update(window);

        game.ball_intersects_bar();
        game.ball_intersects_bricks();

        window.clear();
        game.bar.draw(window);
        game.ball.draw(window);
        game.brickSet.draw(window);
        window.display();

    }

    return 0;
}