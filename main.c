#include "raylib.h"

#define SCREEN_WIDTH 720
#define SCREEN_HEIGHT 720
#define TEXT_SIZE 120

const int TIME = 20;
bool timerActive = true;

double TimePassed;
double timeInMinutes;
double timeInSeconds;
int timeLeft = TIME;

void HandleInput() {
	if (IsKeyPressed(KEY_SPACE)) {
		timerActive = !timerActive;
	}
}

void CountDown() {
	if (timerActive) {
		
		TimePassed += ((double)(1.0 / 60) * (double)(GetFrameTime() * 60));
		timeInMinutes = (TIME - (int)TimePassed) / 60;
		timeInSeconds = (TIME - (int)TimePassed) % 60;
		timeLeft = TIME - (int)TimePassed;
	}
}


int main() {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pomodoro Timer");
  SetTargetFPS(60);
  
  while (!WindowShouldClose()) {
	HandleInput();
	CountDown();
	
    BeginDrawing();
    	ClearBackground(RAYWHITE);
		if (timeLeft > 0) {
			if (timeInMinutes < 10 && timeInSeconds < 10) {
				DrawText(TextFormat("0%.0f:0%.0f", timeInMinutes, timeInSeconds), SCREEN_WIDTH / 2 - TEXT_SIZE, SCREEN_HEIGHT / 2, TEXT_SIZE, BLACK);
			}
			else if (timeInMinutes < 10) {
				DrawText(TextFormat("0%.0f:%.0f", timeInMinutes, timeInSeconds), SCREEN_WIDTH / 2 - TEXT_SIZE, SCREEN_HEIGHT / 2, TEXT_SIZE, BLACK);
			}
			else if (timeInSeconds < 10) {
				DrawText(TextFormat("%.0f:0%.0f", timeInMinutes, timeInSeconds), SCREEN_WIDTH / 2 - TEXT_SIZE, SCREEN_HEIGHT / 2, TEXT_SIZE, BLACK);
			}
			else {
				DrawText(TextFormat("%.0f:%.0f", timeInMinutes, timeInSeconds), SCREEN_WIDTH / 2 - TEXT_SIZE, SCREEN_HEIGHT / 2, TEXT_SIZE, BLACK);
			}
		} else {
			DrawText("00:00", SCREEN_WIDTH / 2 - TEXT_SIZE, SCREEN_HEIGHT / 2, TEXT_SIZE, RED);
		}

    EndDrawing();
  }

  CloseWindow();
  return 0;
}