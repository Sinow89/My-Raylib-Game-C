#include "raylib.h"
#include "raymath.h"

//Created 2025-01-15 by Christoffer Rozenbachs

/* Use this command in powershell to make it compile. gcc cant find
zig cc -I"C:/raylib/include" -L"C:/raylib/lib" -o "MyGame.exe" main.c -lraylib -lm
*/

typedef struct{
    float ball_xpos;
    float ball_ypos;
    float ball_velocity;
    float ball_radius;
} ball;

typedef struct{
    float player_xpos;
    float player_ypos;
    float player_speed;
} player;

Vector2 AddVector2(Vector2 v1, Vector2 v2) {
    return (Vector2){v1.x + v2.x, v1.y + v2.y};
}

int main(void) {
    // Initialize the window
    const int screenWidth = 800;
    const int screenHeight = 600;
    Vector2 velocity = {5, 3};
    SetTargetFPS(60);
    InitWindow(screenWidth, screenHeight, "Raylib - Hello World");

    // Initialize the player and ball
    player game_player = {400, 300, 5};
    ball game_ball = {200, 200, 2, 10};

    while (!WindowShouldClose()) {

        BeginDrawing();
        ClearBackground(BLACK);
        DrawCircle(game_ball.ball_xpos, game_ball.ball_ypos, game_ball.ball_radius, BLUE);
        DrawRectangle(game_player.player_xpos, game_player.player_ypos, 75, 25, WHITE);

        //The Rectangle defines the Retangle and sets the position and size.
        Rectangle rec = {game_player.player_xpos, game_player.player_ypos , 75, 25};

        //This Vector2 is the center of the ball.
        Vector2 center = {game_ball.ball_xpos, game_ball.ball_ypos};
        Vector2 newPosition = AddVector2(center, velocity);

        game_ball.ball_xpos = newPosition.x;
        game_ball.ball_ypos = newPosition.y;

        //Making the ball "bounce" when hiting the walls.
        if (game_ball.ball_xpos - game_ball.ball_radius < 0 || game_ball.ball_xpos + game_ball.ball_radius > screenWidth) {
            velocity.x = -velocity.x; // Reverse x direction
        }
        if (game_ball.ball_ypos - game_ball.ball_radius < 0 || game_ball.ball_ypos + game_ball.ball_radius > screenHeight) {
            velocity.y = -velocity.y; // Reverse y direction
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

        /*-------------------------------------------------------*/
        /*--------------------End-of-Controls--------------------*/
        /*-------------------------------------------------------*/

        DrawFPS(700, 500);
        EndDrawing();
    }

    // Close window and OpenGL context
    CloseWindow();
    return 0;
}