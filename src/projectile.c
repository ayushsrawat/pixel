#include <stdio.h>
#include <math.h>
#include "raylib.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

int x_len = 500;
int y_len = 500;
int neg_len = 50;
int radius = 10;

Color tsoding = {
    .r = 0x18,
    .g = 0x18,
    .b = 0x18,
    .a = 0xFF,
};

Vector2 origin = {
	.x = 100,
	.y = 530,
};

Vector2 velocity = {
	.x = 100.0f, 
	.y = -210.0f,
};

Vector2 path;
float gravity = 9.8f * 10;

int main(void) {
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Projectile Motion");
	path = origin;
	SetTargetFPS(60);
	while(!WindowShouldClose()) {
		BeginDrawing();
			ClearBackground(tsoding);
			DrawText("Projectile Motion", WINDOW_WIDTH/3, 20, 30, RAYWHITE);
			DrawLineEx((Vector2){origin.x - neg_len, origin.y}, (Vector2){origin.x + x_len - neg_len, origin.y}, 2, WHITE); // x axis
			DrawLineEx((Vector2){origin.x, origin.y + neg_len}, (Vector2){origin.x, origin.y - (y_len - neg_len)}, 2, WHITE); // y axis
			DrawCircleV(path, radius, BLUE);
			float time = GetTime();
			printf("pos at [%f]: {%f, %f}\n", time,  path.x, path.y);
			float nx = origin.x + velocity.x * time;
			float ny = origin.y + velocity.y * time + (0.5f * gravity * time * time);
			path.x = nx;
			path.y = ny;
			if (path.y >= origin.y) {
				path.y = origin.y;
			}
		EndDrawing();
	}
	printf("thanks for watching projectile motion :)\n");
	CloseWindow();
	return 0;
}
