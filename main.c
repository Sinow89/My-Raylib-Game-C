#include "raylib.h"
#include "raymath.h"

//Created 2025-01-15 by Christoffer Rozenbachs

/* Use this command in powershell to make it compile. gcc cant find
zig cc -I"C:/raylib/include" -L"C:/raylib/lib" -o "MyGame.exe" main.c -lraylib -lm
*/

//------------------------TO-DO---------------------------------------------//
//1. Fix player struct player_speed is not use. Should be player_velocity.
//2. Fix ball collsion
//

typedef struct{
    Vector2 position;
    Vector2 velocity;
    float speed;
} player_t;

typedef struct{
    Vector2 position;
    Vector2 velocity;
    float radius;
    float speed;
} ball_t;

Vector2 AddVector2(Vector2 v1, Vector2 v2) {
    return (Vector2){v1.x + v2.x, v1.y + v2.y};
}

int main(void) {
    // Initialize the window.
    const int screen_width = 800;
    const int screen_height = 600;
    SetTargetFPS(60);
    InitWindow(screen_width, screen_height, "RaylibGame");

    // Initialize the player and ball.
    player_t player = {400, 300, 5, 5, 5};
    ball_t ball = {200, 200, 10, 10, 10, 2};

    //Set the Velocity outside the while loop.
    Vector2 velocity = {5, 3};
    Vector2 player_velocity = {10, 6};

    while (!WindowShouldClose()) {

        float delta_time = GetFrameTime();

        BeginDrawing();
        ClearBackground(BLACK);
        DrawRectangle(player.position.x, player.position.y, 75, 25, WHITE);
        DrawCircle(ball.position.x, ball.position.y, ball.radius, BLUE);

        //The Rectangle defines the Retangle and sets the position and size.
        Rectangle rec = {player.position.x, player.position.y , 75, 25};

        //This Vector2 is the center of the ball.
        // Vector2 center = {ball.position.x, ball.position.y};
        // Vector2 newPosition = AddVector2(center, velocity);

        // ball.position.x = newPosition.x;
        // ball.position.y = newPosition.y;

        //Vector2 of the player
        Vector2 center_player = {player.position.x, player.position.y};
        Vector2 newPosition_player = AddVector2(center_player, player_velocity);

        //Making the ball "bounce" when hiting the walls.
        // if (ball.position.x - ball.radius < 0 || ball.position.x + ball.radius > screen_width) {
        //     velocity.x = -velocity.x; // Reverse x direction
        // }
        // if (ball.position.y - ball.radius < 0 || ball.position.y + ball.radius > screen_height) {
        //     velocity.y = -velocity.y; // Reverse y direction
        // }
        // if (CheckCollisionCircleRec(center, 10.0f, rec)){
        //     velocity.y = -velocity.y;
        // }
        // if (CheckCollisionCircleRec(center, 10.0f, rec)){
        //     velocity.x = -velocity.x;
        // }

        /*-------------------------------------------------------*/
        /*----------------------Controls-------------------------*/
        /*-------------------------------------------------------*/
        
        /*
        player.velocity = Vector2Normalize(player.velocity);                // 1. make length = 1
        player.velocity = Vector2Scale(player.velocity, player.speed);      // 2. make length = speed
        player.velocity = Vector2Scale(player.velocity, delta_time);        // 3. make length proportional to one frame
        player.position = Vector2Add(player.position, player.velocity);     // 4. apply velocity vector to position vector
        */


        if(IsKeyDown(KEY_W) && player.position.y > 0)
        {
            // player.position.y = -newPosition_player.y;
            player.position.y = player.position.y-player.speed;
        }

        if(IsKeyDown(KEY_A) && player.position.x > 0)
        {
            // game_player.pl
            player.position.x = player.position.x - player.speed;
        }

        if(IsKeyDown(KEY_S) && player.position.y < 575)
        {
            player.position.y = player.position.y+player.speed;
        }

        if(IsKeyDown(KEY_D) && player.position.x < 725)
        {
            player.position.x = player.position.x+player.speed;
        }

        /*-------------------------------------------------------*/
        /*--------------------End-of-Controls--------------------*/
        /*-------------------------------------------------------*/

        DrawFPS(700, 500);
        EndDrawing();
    }

    // Close window and OpenGL context.
    CloseWindow();
    return 0;
}
