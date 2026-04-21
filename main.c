#include "raylib.h"

#define SCREEN_WIDTH 720
#define SCREEN_HEIGHT 720

int main() {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pomodoro Timer");
  SetTargetFPS(60);

  const int TIME = 60;
  
  
  while (!WindowShouldClose()) {
	double timeInMinutes = (TIME - (int)GetTime()) / 60;
	double timeInSeconds = (TIME - (int)GetTime()) % 60;
	int timeLeft = TIME - (int)GetTime();
	
    BeginDrawing();
    	ClearBackground(RAYWHITE);
		if (timeLeft > 0) {
			DrawText(TextFormat("%.0f : %.0f", timeInMinutes, timeInSeconds), SCREEN_WIDTH / 2 - 60, SCREEN_HEIGHT / 2, 60, BLACK);
		} else {
			DrawText("TIME RUN OUT", SCREEN_WIDTH / 2 - 60, SCREEN_HEIGHT / 2, 60, RED);
		}

    EndDrawing();
  }

  CloseWindow();
  return 0;
}