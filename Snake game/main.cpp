#include <iostream>
#include <conio.h> // For _kbhit() and _getch()
#include <windows.h> // For Sleep() and system("cls")
#include <vector>
#include <cstdlib> // For rand()
#include <ctime>   // For time()

using namespace std;

bool gameOver;
const int width = 20;
const int height = 20;
int x, y, fruitX, fruitY, score;
vector<pair<int, int>> snake; // Stores snake body segments (x, y)
enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };
Direction dir;

void Setup() {
    gameOver = false;
    dir = STOP;
    x = width / 2;
    y = height / 2;
    snake.push_back({x, y}); // Initial snake head
    srand(time(0));
    fruitX = rand() % width;
    fruitY = rand() % height;
    score = 0;
}

void Draw() {
    system("cls"); // Clear console (Windows-specific)
    
    // Draw top border
    for (int i = 0; i < width + 2; i++)
        cout << "#";
    cout << endl;

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (j == 0) cout << "#"; // Left border

            // Draw snake head or body
            bool isSnakeSegment = false;
            for (const auto& segment : snake) {
                if (segment.first == j && segment.second == i) {
                    cout << "O";
                    isSnakeSegment = true;
                    break;
                }
            }

            // Draw fruit
            if (j == fruitX && i == fruitY && !isSnakeSegment)
                cout << "F";
            else if (!isSnakeSegment)
                cout << " ";

            if (j == width - 1) cout << "#"; // Right border
        }
        cout << endl;
    }

    // Draw bottom border
    for (int i = 0; i < width + 2; i++)
        cout << "#";
    cout << endl;

    cout << "Score: " << score << endl;
}

void Input() {
    if (_kbhit()) { // Check if key is pressed
        switch (_getch()) {
            case 'a': dir = LEFT;  break;
            case 'd': dir = RIGHT; break;
            case 'w': dir = UP;    break;
            case 's': dir = DOWN;  break;
            case 'x': gameOver = true; break;
        }
    }
}

void Logic() {
    // Move snake body (from tail to head)
    for (int i = snake.size() - 1; i > 0; i--) {
        snake[i] = snake[i - 1];
    }

    // Update head position
    switch (dir) {
        case LEFT:  snake[0].first--;  break;
        case RIGHT: snake[0].first++;  break;
        case UP:    snake[0].second--; break;
        case DOWN:  snake[0].second++; break;
    }

    // Check wall collision
    if (snake[0].first < 0 || snake[0].first >= width || 
        snake[0].second < 0 || snake[0].second >= height) {
        gameOver = true;
    }

    // Check self-collision
    for (int i = 1; i < snake.size(); i++) {
        if (snake[0].first == snake[i].first && snake[0].second == snake[i].second) {
            gameOver = true;
        }
    }

    // Check fruit collision
    if (snake[0].first == fruitX && snake[0].second == fruitY) {
        score += 10;
        fruitX = rand() % width;
        fruitY = rand() % height;
        snake.push_back({-1, -1}); // Grow snake (position updated next frame)
    }
}

int main() {
    Setup();
    while (!gameOver) {
        Draw();
        Input();
        Logic();
        Sleep(100); // Delay for smoother gameplay
    }
    cout << "Game Over! Final Score: " << score << endl;
    return 0;
}
