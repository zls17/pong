#include "raylib.h"
#include <cassert>
#include <iostream>

typedef enum GameScreen {TITLE, GAMEPLAY, ENDING} GameScreen;

void resetBars(Vector2& leftBar, Vector2& rightBar) {
    leftBar = {10, 200};
    rightBar = {880, 200};
}

void resetBalls(Vector2& BallPosition, Vector2& BallSpeed) {
    BallPosition = {(float)GetScreenHeight()/2.0f, (float)GetScreenWidth()/2.0f};
    BallSpeed = {5.0f, 2.0f};
 
}

int main() {

    constexpr int height {500};
    constexpr int width {900};
    const char* title {"Pong"};
    
    Vector2 BallPosition {height/2.0, width/2.0};
    Vector2 BallSpeed {5.0f, 2.0f};
    Vector2 LeftBarPosition {10, 200};
    Vector2 LeftBarSize {10, 100};
    Vector2 RightBarPosition {880, 200};
    Vector2 RightBarSize {10, 100};
    constexpr int BallRadius {20};
    // bool gameOver = false;
    int frameCounter {};
    int winner {};
    GameScreen currentScreen = TITLE;
    InitWindow(width, height, title);
    SetTargetFPS(60);
    InitAudioDevice();
    Music music = LoadMusicStream("resources/mini1111.xm");
    while (!WindowShouldClose()) {
        // Update Code
        UpdateMusicStream(music);
        switch (currentScreen) {
            case TITLE:
                {
                if (IsKeyDown(KEY_SPACE)) currentScreen = GAMEPLAY;
                break;
                }
             case GAMEPLAY: 
                {
                PlayMusicStream(music);
                BallPosition.x += BallSpeed.x;
                BallPosition.y += BallSpeed.y;
          

                bool colCond1 { (BallPosition.x <= 20 + BallRadius) };
                bool colCond2 { (BallPosition.y >= LeftBarPosition.y) && (BallPosition.y <= (LeftBarPosition.y + 100)) };

                bool colCond3 { (BallPosition.x >= (880 - BallRadius)) };
                bool colCond4 { (BallPosition.y >= RightBarPosition.y) && (BallPosition.y <= (RightBarPosition.y + 100)) };
         
                if (colCond1 && colCond2) BallSpeed.x *= -1.0f;
                if (colCond3 && colCond4) BallSpeed.x *= -1.0f;
                

                if (((BallPosition.y) >= ((float)GetScreenHeight() - BallRadius)) || (BallPosition.y <= BallRadius))  BallSpeed.y *= -1.0f;
          
                // Check for collision of Bar and Wall
        HERE:
                if (LeftBarPosition.y <= 0) {
                    LeftBarPosition.y += 11;
                    if (IsKeyDown(KEY_A)) goto HERE;
                } 
                else if (LeftBarPosition.y >= 400) {
                    LeftBarPosition.y -= 10;
                    if (IsKeyDown(KEY_D)) goto HERE;
                }
                else if (RightBarPosition.y <= 0) {
                    RightBarPosition.y += 1;
                    if (IsKeyDown(KEY_UP)) goto HERE;
                }
                else if (RightBarPosition.y >= 400) {
                    RightBarPosition.y -= 1;
                    if (IsKeyDown(KEY_DOWN)) goto HERE;
                }
                else {
                    if (IsKeyDown(KEY_A)) LeftBarPosition.y -= 5.0f;
                    if (IsKeyDown(KEY_D)) LeftBarPosition.y += 5.0f;
                    if (IsKeyDown(KEY_UP)) RightBarPosition.y -= 5.0f;
                    if (IsKeyDown(KEY_DOWN)) RightBarPosition.y += 5.0f;
                }

                 
                if (frameCounter % 200 == 0) {
                    BallSpeed.x *= 1.1f;
                    BallSpeed.y *= 1.1f;
                } 
                frameCounter++;
                if (BallPosition.x < 0 || BallPosition.x > 900) currentScreen = ENDING;
                break;
            }

            case ENDING: 
                {
                StopMusicStream(music);
                if (BallPosition.x < 0 || BallPosition.x > 900) {
                    if (BallPosition.x < 0) {
                        winner = 2;
                    }
                    if (BallPosition.x > 900) {
                        winner = 1;
                    }
                    if (IsKeyDown(KEY_SPACE)) {
                        // gameOver = false;
                        BallPosition.x = height / 2.0f;
                        BallPosition.y = width / 2.0f;
                        resetBars(LeftBarPosition, RightBarPosition);
                        resetBalls(BallPosition, BallSpeed);
                        currentScreen = GAMEPLAY;
                    }
                    break;
                }
                }


        }

        // Draw Code
        BeginDrawing();
        
        
        switch (currentScreen) {
            case TITLE:
                {
                    ClearBackground(BLACK);
                    DrawText("Pong", GetScreenWidth()/2-100, GetScreenHeight()/3, 90, WHITE);
                    DrawText("Player 1", (int)LeftBarPosition.x + 20, (int)LeftBarPosition.y - 150, 50, BLUE);
                    DrawText("Player 2", (int)RightBarPosition.x - 220, (int)RightBarPosition.y - 150, 50, BLUE);
                    DrawRectangleV(LeftBarPosition, LeftBarSize, GREEN);
                    DrawRectangleV(RightBarPosition, RightBarSize, GREEN);                    
                    break;
                }

            case GAMEPLAY:
                {
                    ClearBackground(BLACK);
                    DrawCircleV(BallPosition, BallRadius, RED);
                    DrawRectangleV(LeftBarPosition, LeftBarSize, GREEN);
                    DrawRectangleV(RightBarPosition, RightBarSize, GREEN);
                    break;
                }
            case ENDING:
                {
                    ClearBackground(BLACK);
                    if (winner == 1) DrawText("Player 1 wins!", (GetScreenWidth()/2)-200, (GetScreenHeight()/2)-100, 60, YELLOW);
                    if (winner == 2) DrawText("Player 2 wins!", (GetScreenWidth()/2)-200, (GetScreenHeight()/2)-100, 60, YELLOW);
                    DrawText("Press [Space] to restart", (GetScreenWidth()/2)-200, GetScreenHeight()/2+50, 30, PINK);
                    break;
                }

        }


        EndDrawing();

    }

    UnloadMusicStream(music);
    CloseAudioDevice();
    CloseWindow();
    
    return 0;
}
