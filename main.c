#include "raylib.h"
#include "raymath.h"
#include "time.h"
#include "stdio.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 480
#define TEXT_SIZE 180
#define FPS 60

bool timerActive = false;
bool realTime = false;
bool timerPause = false;
int TIME = 1800;

double pauseOffset = 0.0;
double pauseStart = 0.0;

double timeInMinutes;
double timeInSeconds;
int timeLeft = 1800;

Vector2 startPos = { 0 };
Vector2 lastPos = { 0 };  // add this
bool isTouching = false;
const float minDistance = 50.0f;


Vector2 TransformTouch(Vector2 raw) {
    // Physical screen is 480x800, logical is 800x480
    // For 90° clockwise rotation:
    return (Vector2){ raw.y, SCREEN_HEIGHT - raw.x };
}

void HandleInput() { 
    // Keyboard/gamepad
    if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_MIDDLE_RIGHT) && IsGamepadButtonPressed(0, GAMEPAD_BUTTON_MIDDLE_LEFT))
        CloseWindow();

    if (IsKeyPressed(KEY_SPACE)  || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN))
 	{
        timerActive = !timerActive;
        if (!timerActive) pauseStart = GetTime();
        else pauseOffset += GetTime() - pauseStart;
    }

    // Touch
    int touchCount = GetTouchPointCount();

    if (touchCount > 0) {
        Vector2 raw = GetTouchPosition(0);
        Vector2 pos = TransformTouch(raw);  // transform first!

        if (!isTouching) {
            startPos = pos;
            isTouching = true;
        }
        lastPos = pos;  // keep updating last known position
    }

    if (touchCount == 0 && isTouching) {
        Vector2 delta = Vector2Subtract(lastPos, startPos);
        float len = Vector2Length(delta);

        if (len < 20.0f && !realTime) {
            // It's a tap — toggle timer
            timerActive = !timerActive;
            if (!timerActive) pauseStart = GetTime();
            else pauseOffset += GetTime() - pauseStart;
        } else if (len > minDistance) {
            if (fabsf(delta.x) > fabsf(delta.y)) {
                // Horizontal swipe
                if (delta.x > 0) { 
					realTime = false;
				}
                else { 
					realTime = true;
				}
            } else {
                // Vertical swipe
                if (delta.y < 0) {
					TIME += 60;
					double elapsed = GetTime() - pauseOffset;
					timeLeft = TIME -  (int)elapsed;
					timeInMinutes = timeLeft / 60;

				} 
                else TIME -= 60;              
                if (TIME < 60) {
					TIME = 60; 
					double elapsed = GetTime() - pauseOffset;
					timeLeft = TIME -  (int)elapsed;
					timeInMinutes = timeLeft / 60;

				}
            }
        }
        isTouching = false;
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
	if (timeLeft <= 0 && timerPause != true) {
		timerPause = true;
		timeLeft = 300;
		TIME = 300;
	} else if (timeLeft <= 0){
		timerPause = false;
		timeLeft = 1800;
		TIME = 1800;
	}
}

void RenderTimer() {
	if (timerActive) {
		if (timeLeft > 0) {
			char timeStr[9];
				int xBufferSpace = 20;
					if (timeInMinutes < 10 && timeInSeconds < 10) {
						sprintf(timeStr, "0%.0f:0%.0f", timeInMinutes, timeInSeconds);
						int margin = MeasureText(timeStr, TEXT_SIZE);
						DrawText(TextFormat("0%.0f:0%.0f", timeInMinutes, timeInSeconds), SCREEN_WIDTH / 2 - margin / 2, SCREEN_HEIGHT / 2 - xBufferSpace - 45, TEXT_SIZE, BLACK);
					}
					else if (timeInMinutes < 10) {
						sprintf(timeStr, "0%.0f:%.0f", timeInMinutes, timeInSeconds);
						int margin = MeasureText(timeStr, TEXT_SIZE);
						DrawText(TextFormat("0%.0f:%.0f", timeInMinutes, timeInSeconds), SCREEN_WIDTH / 2 - margin / 2, SCREEN_HEIGHT / 2 - xBufferSpace - 45, TEXT_SIZE, BLACK);
					}
					else if (timeInSeconds < 10) {
						sprintf(timeStr, "%.0f:0%.0f", timeInMinutes, timeInSeconds);
						int margin = MeasureText(timeStr, TEXT_SIZE);
						DrawText(TextFormat("%.0f:0%.0f", timeInMinutes, timeInSeconds), SCREEN_WIDTH / 2 - margin / 2, SCREEN_HEIGHT / 2 - xBufferSpace - 45, TEXT_SIZE, BLACK);
					}
					else {
						sprintf(timeStr, "%.0f:%.0f", timeInMinutes, timeInSeconds);
						int margin = MeasureText(timeStr, TEXT_SIZE);
						DrawText(TextFormat("%.0f:%.0f", timeInMinutes, timeInSeconds), SCREEN_WIDTH / 2 - margin / 2, SCREEN_HEIGHT / 2 - xBufferSpace - 45, TEXT_SIZE, BLACK);
					}
				} else {
					int margin = MeasureText("00:00", TEXT_SIZE);
					DrawText("00:00", SCREEN_WIDTH / 2 - TEXT_SIZE - margin / 2, SCREEN_HEIGHT / 2, TEXT_SIZE, RED);
				}
	} else {
		int x = SCREEN_WIDTH / 2 - 50;
		int y = SCREEN_HEIGHT / 2 - 65;

		char mins[8], secs[8];
		sprintf(mins, (int)timeInMinutes < 10 ? "0%.0f" : "%.0f", timeInMinutes);
		sprintf(secs, (int)timeInSeconds < 10 ? "0%.0f" : "%.0f", timeInSeconds);

		int minsWidth = MeasureText(mins, TEXT_SIZE) + 18;
		int colonWidth = MeasureText(":", TEXT_SIZE) + 18;

		DrawText(mins, x / 2, y, TEXT_SIZE, BLACK);

		// Only draw colon when timer is active OR on even seconds (blink when paused)
		if (timerActive || (int)GetTime() % 2 == 0) {
			DrawText(":", x / 2 + minsWidth, y, TEXT_SIZE, BLACK);
		} else {
			DrawText(":", x  / 2 + minsWidth, y, TEXT_SIZE, GRAY);
		}

		DrawText(secs, x / 2 + minsWidth + colonWidth, y, TEXT_SIZE, BLACK);
	}
			
	// Debug
	int touches = GetTouchPointCount();  // how many fingers currently touching

	for (int i = 0; i < touches; i++)
	{
		Vector2 raw = GetTouchPosition(i);
		Vector2 pos = TransformTouch(raw);
		DrawCircle(pos.x, pos.y, 50, RED);
	}	
}


void RenderLocalTime() {
    time_t rawtime;
    struct tm *t;
    time(&rawtime);
    t = localtime(&rawtime);

    char timeStr[9];
    sprintf(timeStr, "%02d:%02d:%02d", t->tm_hour, t->tm_min, t->tm_sec);

	int margin = MeasureText(timeStr, TEXT_SIZE);

    DrawText(timeStr, SCREEN_WIDTH / 2 - margin / 2, SCREEN_HEIGHT / 2 - 65, TEXT_SIZE, BLACK);
}


int main() {
	timeLeft = TIME;
	timeInMinutes = timeLeft / 60;
	timeInSeconds = timeLeft % 60;
	SetGesturesEnabled(GESTURE_TAP | GESTURE_SWIPE_UP | GESTURE_SWIPE_DOWN | GESTURE_SWIPE_RIGHT | GESTURE_SWIPE_LEFT);
  	SetConfigFlags(FLAG_FULLSCREEN_MODE);
	InitWindow(0, 0, "Pomodoro Timer");
  	SetTargetFPS(60);
	RenderTexture target = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);


	while (!WindowShouldClose()) {
		HandleInput();
		CountDown();
		
		BeginDrawing();
			ClearBackground(BLACK);
			BeginTextureMode(target);
				ClearBackground(LIGHTGRAY);
				if (realTime) {
					RenderLocalTime();
				} else {
					RenderTimer();
				}	
			EndTextureMode();

			DrawTexturePro(target.texture, (Rectangle){ 0, 0, target.texture.width, -target.texture.height }, (Rectangle){ SCREEN_HEIGHT, 0, SCREEN_WIDTH, SCREEN_HEIGHT}, (Vector2){0, 0}, 90.0f, WHITE);
		EndDrawing();
	}

	CloseWindow();
	return 0;
}
