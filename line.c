#include <stdio.h>
#include <math.h>
#include "raylib.h"

#define EPSILON 1e-3
#define OUTPUT_DIR "output"

const int width  = 400;
const int height = 400;

int main(void) {
	Image image = GenImageColor(width, height, RAYWHITE);

	Vector2 p1 = {2,2};
	Vector2 p2 = {300, 120};

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			// if point c belongs to line P1P2 ? if tan0 of both are same
			Vector2 p = {x, y};
			if (fabs((p.y - p1.y) * (p2.x - p1.x) == (p.x - p1.x) * (p2.y - p1.y)) < EPSILON) {
				printf("%d, %d\n", x, y);
				ImageDrawPixelV(&image, p, BLACK);
			}
		}
	}

	bool success = ExportImage(image, OUTPUT_DIR"/line.png");
	UnloadImage(image);
	printf("%s\n", success ? "successfully saved the image" : "error saving the image");
	return 0;
}