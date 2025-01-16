#include "raylib.h"
#include "raymath.h"

#define MAX_BULLETS 128

typedef Vector2 vec2_t;
typedef Color color_t;

typedef struct {
  vec2_t position;
  vec2_t velocity;
  vec2_t facing;
  float size;
  color_t color;
  float max_speed; // pixels / second
} player_t;

typedef struct {
  float lifetime; // seconds
  vec2_t position;
  vec2_t velocity;
  float size;
  color_t color;
} bullet_t;

bullet_t bullets[MAX_BULLETS];

void bullet_spawn(vec2_t origin, vec2_t direction) {
  for (int i = 0; i < MAX_BULLETS; i++) {
    if (bullets[i].lifetime <= 0.0f) {
      bullets[i] = (bullet_t){
          .position = origin,
          .velocity = Vector2Scale(Vector2Normalize(direction), 1000.0f),
          .lifetime = 2.5f,
          .size = 8,
          .color = RED};
      return;
    }
  }
}

void bullets_update(float delta_time) {
  for (int i = 0; i < MAX_BULLETS; i++) {
    if (bullets[i].lifetime <= 0.0f) {
      continue;
    }
    bullets[i].lifetime -= delta_time;
    if (bullets[i].lifetime < 0.0f) {
      bullets[i].lifetime = 0.0f;
    }
    bullets[i].position = Vector2Add(
        bullets[i].position, Vector2Scale(bullets[i].velocity, delta_time));
  }
}

void bullets_draw() {
  for (int i = 0; i < MAX_BULLETS; i++) {
    if (bullets[i].lifetime <= 0.0f) {
      continue;
    }
    DrawCircleV(bullets[i].position, bullets[i].size, bullets[i].color);
  }
}

int main() {

  player_t player = {.position = (vec2_t){400, 300},
                     .size = 32,
                     .color = GREEN,
                     .max_speed = 300};

  InitWindow(800, 600, "Shooter");
  while (!WindowShouldClose()) {

    float delta_time = GetFrameTime();
    vec2_t mouse = GetMousePosition();

    player.velocity = (vec2_t){0.0f, 0.0f};

    if (IsKeyDown(KEY_W)) {
      player.velocity.y = -1;
    } else if (IsKeyDown(KEY_S)) {
      player.velocity.y = 1;
    }

    if (IsKeyDown(KEY_A)) {
      player.velocity.x = -1;
    } else if (IsKeyDown(KEY_D)) {
      player.velocity.x = 1;
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      bullet_spawn(player.position, player.facing);
    }

    player.velocity = Vector2Scale(Vector2Normalize(player.velocity),
                                   player.max_speed * delta_time);

    player.position = Vector2Add(player.position, player.velocity);

    player.facing = Vector2Subtract(mouse, player.position);

    bullets_update(delta_time);

    BeginDrawing();
    ClearBackground(MAGENTA);
    DrawCircle(player.position.x, player.position.y, player.size, player.color);

    DrawLine(player.position.x, player.position.y,
             player.position.x + player.facing.x,
             player.position.y + player.facing.y, WHITE);

    bullets_draw();

    EndDrawing();
  }
  return 0;
}