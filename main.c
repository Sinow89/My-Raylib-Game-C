#include "raylib.h"

//Created 2025-01-15 by Christoffer Rozenbachs

/* Use this command in powershell to make it compile. gcc cant find
zig cc -I"C:/raylib/include" -L"C:/raylib/lib" -o "MyGame.exe" main.c -lraylib -lm
*/

typedef struct{
    float ball_xpos;
    float ball_ypos;
    float ball_speed;
    float ball_radius;
} ball;

typedef struct{
    float player_xpos;
    float player_ypos;
    float player_speed;
} player;

int main(void) {
    // Initialize the window
    InitWindow(800, 600, "Raylib - Hello World");
    SetTargetFPS(60);

    player game_player = {400, 300, 5};
    ball game_ball = {200, 200, 2, 10};

    while (!WindowShouldClose()) {

        BeginDrawing();
        ClearBackground(BLACK);
        DrawCircle(game_ball.ball_xpos, game_ball.ball_ypos, game_ball.ball_radius, BLUE);
        DrawRectangle(game_player.player_xpos, game_player.player_ypos, 75, 25, WHITE);

        //This Vector2 is the center of the ball.
        //The Rectangle defines the Retangle and sets the position and size.
        Vector2 center = {game_ball.ball_xpos , game_ball.ball_ypos};
        Rectangle rec = {game_player.player_xpos , game_player.player_ypos  , 75, 25};

        while (1)
        {
            game_ball.ball_ypos = game_ball.ball_ypos + game_ball.ball_speed;
            game_ball.ball_xpos = game_ball.ball_xpos + game_ball.ball_speed;

            if(CheckCollisionCircleRec(center, 10.0f, rec))
            {
            game_ball.ball_ypos = game_ball.ball_ypos - game_ball.ball_speed;
            game_ball.ball_xpos = game_ball.ball_xpos - game_ball.ball_speed;
            }

            break;
        }

        /*-------------------------------------------------------*/
        /*----------------------Controls-------------------------*/
        /*-------------------------------------------------------*/

        if(IsKeyDown(KEY_W) && game_player.player_ypos > 0)
        {
            game_player.player_ypos = game_player.player_ypos-game_player.player_speed;
        }

        if(IsKeyDown(KEY_A) && game_player.player_xpos > 0)
        {
            game_player.player_xpos = game_player.player_xpos-game_player.player_speed;
        }

        if(IsKeyDown(KEY_S) && game_player.player_ypos < 575)
        {
            game_player.player_ypos = game_player.player_ypos+game_player.player_speed;
        }

        if(IsKeyDown(KEY_D) && game_player.player_xpos < 725)
        {
            game_player.player_xpos = game_player.player_xpos+game_player.player_speed;
        }

        DrawFPS(700, 500);
        EndDrawing();
    }

    // Close window and OpenGL context
    CloseWindow();
    return 0;
}