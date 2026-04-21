#include "raylib.h"

#define SCREEN_WIDTH 720
#define SCREEN_HEIGHT 720

int main() {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pomodoro Timer");
  SetTargetFPS(60);
  
  
  while (!WindowShouldClose()) {
	BeginDrawing();
	ClearBackground(WHITE);
    EndDrawing();
  }

  CloseWindow();
  return 0;
}