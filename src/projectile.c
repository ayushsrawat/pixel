#include <stdio.h>
#include <math.h>
#include "raylib.h"
#define STB_DS_IMPLEMENTATION
#include "../thirdparty/stb_ds.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
float gravity = 9.8f * 10;

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
Vector2* paths = NULL;

int main(void) {
	InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Projectile Motion");
	path = origin;
	SetTargetFPS(60);
	while(!WindowShouldClose()) {
		BeginDrawing();
			ClearBackground(tsoding);
			DrawText("Projectile Motion", WINDOW_WIDTH/3, 20, 30, RAYWHITE);
			DrawFPS(WINDOW_WIDTH - 100, 5);
			DrawLineEx((Vector2){origin.x - neg_len, origin.y}, (Vector2){origin.x + x_len - neg_len, origin.y}, 2, WHITE); // x axis
			DrawText("x", origin.x + x_len - 100, origin.y + 5, 4, WHITE);
			DrawLineEx((Vector2){origin.x, origin.y + neg_len}, (Vector2){origin.x, origin.y - (y_len - neg_len)}, 2, WHITE); // y axis
			DrawText("y", origin.x + 5, origin.y - y_len + 100, 4, WHITE);
			
			DrawCircleV(path, radius, BLUE);
			DrawLineEx((Vector2){origin.x, origin.y}, (Vector2) {path.x, origin.y}, 3, GREEN);
			DrawLineEx((Vector2){origin.x, origin.y}, (Vector2) {origin.x, path.y}, 3, GREEN);
			DrawLineEx((Vector2){path.x, origin.y}, (Vector2) {path.x, path.y}, 1, PURPLE);
			DrawLineEx((Vector2){origin.x, path.y}, (Vector2) {path.x, path.y}, 1, PURPLE);
			arrput(paths, path);
			for (int i = 0; i < arrlen(paths); i++) {
				DrawCircleV(paths[i], 1, RED);
			}
			float time = GetTime();
			printf("pos at time [%f]: {%f, %f}; arrlen(paths): [%td]\n", time,  path.x, path.y, arrlen(paths));
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
	arrfree(paths);
	CloseWindow();
	return 0;
}
