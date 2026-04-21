#include "raylib.h"

#define SCREEN_WIDTH 720
#define SCREEN_HEIGHT 720
#define TEXT_SIZE 180
#define FPS 60

bool timerActive = true;
const int TIME = 1500;

double pauseOffset = 0.0;
double pauseStart = 0.0;

double timeInMinutes;
double timeInSeconds;
int timeLeft = TIME;

void HandleInput() {
	if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_MIDDLE_RIGHT) && IsGamepadButtonPressed(0, GAMEPAD_BUTTON_MIDDLE_LEFT)) {
		CloseWindow();
	}
	if (IsKeyPressed(KEY_SPACE) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) {
		timerActive = !timerActive;
		if (!timerActive) {
		pauseStart = GetTime();
		} else {
			pauseOffset += GetTime() - pauseStart;
		}
	}
}

void CountDown() {
	
	if (timerActive) {
		/*
		TimePassed += ((double)(1.0 / 60) * (double)(GetFrameTime() * 60));
		TimePassed += (double)(GetTime() / GetTime()) / FPS;
		timeInMinutes = (TIME - (int)TimePassed) / 60;
		timeInSeconds = (TIME - (int)TimePassed) % 60;
		timeLeft = TIME - (int)TimePassed;
		*/
		double elapsed = GetTime() - pauseOffset;
		timeLeft = TIME -  (int)elapsed;
		timeInMinutes = timeLeft / 60;
		timeInSeconds = timeLeft % 60;
	}
}

void RenderTimer() {
	if (timeLeft > 0) {
		int xBufferSpace = 20;
			if (timeInMinutes < 10 && timeInSeconds < 10) {
				DrawText(TextFormat("0%.0f:0%.0f", timeInMinutes, timeInSeconds), SCREEN_WIDTH / 2 - TEXT_SIZE - xBufferSpace - 15, SCREEN_HEIGHT / 2 - xBufferSpace - 20, TEXT_SIZE, BLACK);
			}
			else if (timeInMinutes < 10) {
				DrawText(TextFormat("0%.0f:%.0f", timeInMinutes, timeInSeconds), SCREEN_WIDTH / 2 - TEXT_SIZE - xBufferSpace- 15, SCREEN_HEIGHT / 2 - xBufferSpace - 20, TEXT_SIZE, BLACK);
			}
			else if (timeInSeconds < 10) {
				DrawText(TextFormat("%.0f:0%.0f", timeInMinutes, timeInSeconds), SCREEN_WIDTH / 2 - TEXT_SIZE - xBufferSpace- 15, SCREEN_HEIGHT / 2 - xBufferSpace - 20, TEXT_SIZE, BLACK);
			}
			else {
				DrawText(TextFormat("%.0f:%.0f", timeInMinutes, timeInSeconds), SCREEN_WIDTH / 2 - TEXT_SIZE - xBufferSpace- 15, SCREEN_HEIGHT / 2 - xBufferSpace - 20, TEXT_SIZE, BLACK);
			}
		} else {
			DrawText("00:00", SCREEN_WIDTH / 2 - TEXT_SIZE, SCREEN_HEIGHT / 2, TEXT_SIZE, RED);
		}
}


int main() {
  	InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Pomodoro Timer");
  	SetTargetFPS(60);
  
	while (!WindowShouldClose()) {
		HandleInput();
		CountDown();
		
		BeginDrawing();
			ClearBackground(LIGHTGRAY);
			RenderTimer();
		EndDrawing();
	}

	CloseWindow();
	return 0;
}