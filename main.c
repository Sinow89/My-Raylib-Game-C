#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "raylib.h"
#include "raymath.h"

//Created 2025-01-15 by Christoffer Rozenbachs

/* Use this command in powershell to make it compile. gcc cant find
zig cc -I"C:/raylib/include" -L"C:/raylib/lib" -o "MyGame.exe" main.c -lraylib -lm
*/

//------------------------TO-DO---------------------------------------------//
//1. Creat AABB collision on blocks.
//2. Refactor and create methods for reset states for cleared level and game over instead.
//3. Fix so when rows decrement also the randomize powerups blocks also decrements.
//4. Make header files of the game logic instead.
//5. Create a "real" game menu.

typedef struct{
    Vector2 position;
    Vector2 size;
    int lives;
    bool active;
} block_t;

typedef struct{
    Vector2 position;
    Vector2 velocity;
    float speed;
    Vector2 size;
    int lives;
} player_t;

typedef struct{
    Vector2 position;
    Vector2 velocity;
    float radius;
    float speed;
} ball_t;

Vector2 add_vector2(Vector2 v1, Vector2 v2) {
    return (Vector2){v1.x + v2.x, v1.y + v2.y};
}

int read_high_score_file(){
    FILE* file;
    int highscore;
    file = fopen("./highscore.txt", "r");
    fscanf(file, "%d", &highscore);
    fclose(file);
    return highscore;
}

void write_new_high_score(int score){
    int current_high_score = read_high_score_file();
        if (score > current_high_score){
            FILE* file = fopen("./highscore.txt", "w");
            fprintf(file, "%d", score);
            fclose(file);
        }
}

void randomize_powerup_blocks(int *RED_ROWS, int *RED_COLS, int *BLUE_ROWS, int *BLUE_COLS){
        for (int i = 0; i < 4; i++) { 
        *RED_ROWS = (rand() % 4) + 1; 
        *BLUE_ROWS = (rand() % 4) + 1;
    }
        for (int i = 0; i < 9; i++) {
        *RED_COLS = (rand() % 9) + 1;
        *BLUE_COLS = (rand() % 9) + 1;
    }
}

int main(void) {
    // Initialize start up variabels.
    srand(time(NULL));
    const int screen_width = 800;
    const int screen_height = 600;
    bool start_menu = true;
    bool debug_menu = false; 
    bool pause = true;
    bool lost_life = false;
    bool debuff_active = false;
    int score = 0;
    int start_rows = 5;
    int start_cols = 10;
    int rows = 5; 
    int cols = 10;
    int active_blocks = rows * cols;
    float duration = 3.0f;
    float elapsed_time = 0.0f;
    float debuff_time = 0.0f;
    SetTargetFPS(60);
    InitWindow(screen_width, screen_height, "RaylibGame");

    int RED_ROWS, RED_COLS, BLUE_ROWS, BLUE_COLS;

    randomize_powerup_blocks(&RED_ROWS, &RED_COLS, &BLUE_ROWS, &BLUE_COLS);

    // Initialize the player and ball.
    player_t player = {350, 500, 5, 5, 1000, 75, 25, 3};
    ball_t ball = {200, 200, -5, -5, 10, 8};
    block_t block = {100, 100, 75,25, 1, true};

    block_t blocks[rows][cols];
    
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            blocks[i][j].position.x = j * block.size.x; // Set x position
            blocks[i][j].position.y = i * block.size.y; // Set y position
            blocks[i][j].active = true; // Set block as active
            blocks[i][j].lives = 1; // Set initial lives
        }
    }

    while (!WindowShouldClose()) {

        if(start_menu){
            while (true){ 
                BeginDrawing();
                ClearBackground(BLACK);

                DrawText("Welcome to my Raylib Breakout game!", 200, 250, 30, WHITE);
                DrawText("Made by Christoffer Rozenbachs", 200, 300, 20, WHITE);
                DrawText("Press SPACE to start", 200, 350, 20, WHITE);

                if (IsKeyPressed(KEY_SPACE)){
                    start_menu = false;
                    break;
                }
                EndDrawing();
            }
        }
        float delta_time = GetFrameTime();
        Rectangle rec = { player.position.x - player.size.x/2, player.position.y - player.size.y/2, player.size.x, player.size.y};

        int high_score = read_high_score_file();
        write_new_high_score(score);

        /*-------------------------------------------------------*/
        /*----------------------Game-logic-----------------------*/
        /*-------------------------------------------------------*/

        if (pause == false)
        {
            ball.position.x += ball.velocity.x;
            ball.position.y += ball.velocity.y; 
            ball.velocity = Vector2Scale(Vector2Normalize(ball.velocity), ball.speed);
            ball.position = add_vector2(ball.position, Vector2Scale(ball.velocity, delta_time));
        }

        //Lose a life mechanics
        if (ball.position.x > 800 || ball.position.y > 575)
        {
            --player.lives;
            ball.position.x = 200;
            ball.position.y = 200;
            player.position.x = 350;
            player.position.y = 500;
            lost_life= true;
        }

        //Making the ball "bounce" when hiting the walls.
        if (ball.position.x - ball.radius < 0 || ball.position.x + ball.radius > screen_width) {
            ball.velocity.x = -ball.velocity.x; // Reverse x direction
        }
        if (ball.position.y - ball.radius < 0 || ball.position.y + ball.radius > screen_height) {
            ball.velocity.y = -ball.velocity.y; // Reverse y direction
        }

        //Collision logic ball with player
        if (CheckCollisionCircleRec(ball.position, ball.radius, rec))
        {
            if (ball.velocity.y > 0){
                ball.velocity.y = -ball.velocity.y;
                ball.velocity.x = (ball.position.x - player.position.x)/(player.size.x/2)*5;
            }
        }

        //Ball collision logic with blocks and red and blue powerup block
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (blocks[i][j].active) {
                    if (i == RED_ROWS && j == RED_COLS){
                    
                    Rectangle red_block = {blocks[RED_ROWS][RED_COLS].position.x+30, blocks[RED_ROWS][RED_COLS].position.y+30, block.size.x, block.size.y};
                            if (CheckCollisionCircleRec(ball.position, ball.radius, red_block)) {
                                ball.velocity.y = -ball.velocity.y;
                                blocks[RED_ROWS][RED_COLS].lives--;
                            
                            if (blocks[RED_ROWS][RED_COLS].lives == 0) {
                                blocks[RED_ROWS][RED_COLS].active = false;
                                score = score + 100;
                                --active_blocks;
                                ++player.lives;
                            }
                         }
                    }
                    if (i == BLUE_ROWS && j == BLUE_COLS){
                    
                    Rectangle blue_block = {blocks[BLUE_ROWS][BLUE_COLS].position.x+30, blocks[BLUE_ROWS][BLUE_COLS].position.y+30, block.size.x, block.size.y};
                            if (CheckCollisionCircleRec(ball.position, ball.radius, blue_block)) {
                                ball.velocity.y = -ball.velocity.y;
                                blocks[BLUE_ROWS][BLUE_COLS].lives--;
                            
                            if (blocks[BLUE_ROWS][BLUE_COLS].lives == 0) {
                                blocks[BLUE_ROWS][BLUE_COLS].active = false;
                                score = score + 100;
                                --active_blocks;
                                debuff_time += delta_time;
                                debuff_active = true;
                            }
                         }
                    }

                    else{
                    Rectangle rec_block = {blocks[i][j].position.x+30, blocks[i][j].position.y+30, block.size.x, block.size.y};
                    
                        if (CheckCollisionCircleRec(ball.position, ball.radius, rec_block)) {
                            ball.velocity.y = -ball.velocity.y;
                            blocks[i][j].lives--;
                            
                            if (blocks[i][j].lives == 0) {
                                blocks[i][j].active = false;
                                score = score+100;
                                --active_blocks;
                            }
                        }
                    }
                }
            }
        }

        /*-------------------------------------------------------*/
        /*----------------------Controls-------------------------*/
        /*-------------------------------------------------------*/
        
        if (IsKeyPressed(KEY_SPACE))
        {
            pause = !pause;
        }  

        // if(IsKeyDown(KEY_W) && player.position.y >= 0)
        // {
        //     player.position.y = player.position.y - player.speed*delta_time;
        // }

        if(IsKeyDown(KEY_A) && player.position.x > 46 && pause == false)
        {
            player.position.x = player.position.x - player.speed*delta_time;
        }

        // if(IsKeyDown(KEY_S) && player.position.y < (screen_height-32))
        // {
        //     player.position.y = player.position.y + player.speed*delta_time;
        // }

        if(IsKeyDown(KEY_D) && player.position.x < screen_width-45 && pause == false)
        {
            player.position.x = player.position.x + player.speed*delta_time;
        }

        /*-------------------------------------------------------*/
        /*--------------------Debugging--------------------------*/
        /*-------------------------------------------------------*/

        if (IsKeyPressed(KEY_U)) {
            debug_menu = !debug_menu;  // Toggle the menu visibility
        }

        if (debug_menu) {
                    char debugText[256];

        // Format the debug information into a string
        sprintf(debugText, "Velocity: (%.2f, %.2f)", ball.velocity.x, ball.velocity.y);
        DrawText(debugText, 10, 10, 20, DARKGRAY);

        sprintf(debugText, "Position: (%.2f, %.2f)", ball.position.x, ball.position.y);
        DrawText(debugText, 10, 40, 20, DARKGRAY);

        sprintf(debugText, "Speed: %.2f, Delta Time: %.5f", ball.speed, delta_time);
        DrawText(debugText, 10, 70, 20, DARKGRAY);

        // Add block lives to debug output
        char test[21];
        sprintf(test, "active_blocks: %d", active_blocks);
        DrawText(test, 10, 350, 20, DARKGRAY);
        sprintf(test, "debuff_active: %d", debuff_active);
        DrawText(test, 10, 400, 20, DARKGRAY);
        sprintf(test, "blue blocks: %d", blocks[BLUE_ROWS][BLUE_COLS].active);
        DrawText(test, 10, 450, 20, DARKGRAY);
        sprintf(test, "blue lives: %d", blocks[BLUE_ROWS][BLUE_COLS].lives);
        DrawText(test, 10, 500, 20, DARKGRAY);

        DrawFPS(700, 500);
        } 

        /*-------------------------------------------------------*/
        /*----------------------Drawing--------------------------*/
        /*-------------------------------------------------------*/

        BeginDrawing();
        ClearBackground(BLACK);

        //Start/Pause text
        if(pause == true){
            DrawText("Press Space to begin and pause the game", 200, 300, 20, WHITE);
        }

        if (player.lives == 0)
        {
            while (true){ 
                BeginDrawing();
                ClearBackground(BLACK);

                DrawText("Game Over!", 200, 300, 20, WHITE);
                DrawText("Press SPACE to Restart", 200, 350, 20, WHITE);

                if (IsKeyPressed(KEY_SPACE)) {
                    // Reset the game or exit the game over state
                    player.lives = 3; // Example: Reset lives
                    score = 0;
                    ball.position.x = 200;
                    ball.position.y = 200;
                    player.position.x = 350;
                    player.position.y = 500;
                    rows = start_rows;
                    cols = start_cols;
                    ball.speed = 8;
                    active_blocks = rows * cols;
                    randomize_powerup_blocks(&RED_ROWS, &RED_COLS, &BLUE_ROWS, &BLUE_COLS);
                    break; // Exit the loop
                    
                }
                for (int i = 0; i < rows; i++) {
                    for (int j = 0; j < cols; j++) {
                        blocks[i][j].position.x = j * block.size.x; // Set x position
                        blocks[i][j].position.y = i * block.size.y; // Set y position
                        blocks[i][j].active = true; // Set block as active
                        blocks[i][j].lives = 1; // Set initial lives
                    }
                }
                EndDrawing();
            }
        }

        if (active_blocks == 0)
        {   
            write_new_high_score(score);
            while (true){ 
                BeginDrawing();
                ClearBackground(BLACK); // Optional: Clear the screen for better visibility

                DrawText("You Won! Congratulations!", 200, 300, 20, WHITE);
                DrawText("Press SPACE to Restart", 200, 350, 20, WHITE);

                if (IsKeyPressed(KEY_SPACE)) {
                    // Reset the game or exit the game over state
                    ball.position.x = 200;
                    ball.position.y = 200;
                    player.position.x = 350;
                    player.position.y = 500;
                    randomize_powerup_blocks(&RED_ROWS, &RED_COLS, &BLUE_ROWS, &BLUE_COLS);
                    --rows;
                    active_blocks = rows * cols;
                    ball.speed++;
                    if(rows == 0){
                        rows = start_rows;
                        cols = start_cols;
                        active_blocks = rows * cols;
                    }
                    break; // Exit the loop
                    
                }
                for (int i = 0; i < rows; i++) {
                    for (int j = 0; j < cols; j++) {
                        blocks[i][j].position.x = j * block.size.x; // Set x position
                        blocks[i][j].position.y = i * block.size.y; // Set y position
                        blocks[i][j].active = true; // Set block as active
                        blocks[i][j].lives = 1; // Set initial lives
                    }
                }
                EndDrawing();
            }
        }

        while (lost_life == true){
                BeginDrawing();
                ClearBackground(BLACK); // Optional: Clear the screen for better visibility

                DrawText("You lost a life", 200, 300, 20, WHITE);
                DrawText("Press SPACE to continue", 200, 350, 20, WHITE);

                if (IsKeyPressed(KEY_SPACE)) {
                    ball.position.x = 200;
                    ball.position.y = 200;
                    player.position.x = 350;
                    player.position.y = 500;
                    lost_life = false;
                    break;
                }
                EndDrawing();
        }

        //Score points
        char score_point[9];
        sprintf(score_point, "Score: %d",score);
        DrawText(score_point, 10, 550, 20, DARKGRAY);

        //Lives points
        char lives_point[9];
        sprintf(lives_point, "Lives: %d",player.lives);
        DrawText(lives_point, 350, 550, 20, DARKGRAY);

        //High Score
        char high_point[25];
        sprintf(high_point, "High Score: %d", high_score);
        DrawText(high_point, 585, 550, 20, DARKGRAY);


        //Drawing of active Rectangles
        for (int i = 0; i < rows; i++){
            for (int j = 0; j < cols; j++){
                if (blocks[i][j].active){
                    if (i == RED_ROWS && j == RED_COLS){
                        DrawRectangle(blocks[RED_ROWS][RED_COLS].position.x+30, blocks[RED_ROWS][RED_COLS].position.y+30, 70, 20, RED);
                    }
                    else if (i == BLUE_ROWS && j == BLUE_COLS){
                        DrawRectangle(blocks[BLUE_ROWS][BLUE_COLS].position.x+30, blocks[BLUE_ROWS][BLUE_COLS].position.y+30, 70, 20, BLUE);
                    }
                    else
                    DrawRectangle(blocks[i][j].position.x+30, blocks[i][j].position.y+30, 70, 20, GRAY);
                }
            }
        }

        //The Rectangle defines the Retangle and sets the position and size.
         DrawRectangle(player.position.x - player.size.x/2, player.position.y - player.size.y/2, player.size.x, player.size.y, WHITE);
        // DrawRectangle(player.position.x, player.position.y, player.width, player.height, WHITE);

        //Draw the ball and set the speed of ball.
        DrawCircleV(ball.position, ball.radius, BLUE);

        //Debuff system
        if (debuff_active) {
            debuff_time += delta_time;
            ball.speed = 2;
            if (debuff_time >= duration) {
                ball.speed = 8;
                debuff_active = false;
                debuff_time = 0;
            }
        }

        if (debuff_active){
        char duration_time_array[21];
        float time_left = duration - debuff_time ;
        sprintf(duration_time_array, "Slow down time: %.2f", time_left);
        DrawText(duration_time_array, 300, 400, 20, WHITE); // Show "TEST" during the debuff
        }

        EndDrawing();
    }

    // Close window and OpenGL context.
    CloseWindow();
    return 0;
}
