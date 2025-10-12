#include <stdio.h>
#include "raylib.h"

#define OUTPUT_DIR "output"

const int width = 400;
const int height = 400;

void draw_triangle_by_points(Image *image, Vector2 p1, Vector2 p2, Vector2 p3, Color color) {
	ImageDrawLineV(image, p1, p2, color);
	ImageDrawLineV(image, p1, p3, color);
	ImageDrawLineV(image, p2, p3, color);
}

// given a base line, draw a triangle by appending smaller lines below that
// this idea is flawed in case p1.y and p2.y aren't same
/*void draw_triangle_by_lines(Image *image, Vector2 p1, Vector2 p2, Color color) {
	bool is_merged = false;
	while(!is_merged) {
		if (p1.x == p2.x) is_merged = true;
		printf("p1.x : %f, p2.x: %f\n", p1.x, p2.x);
		ImageDrawLineV(image, p1, p2, color);

		p1.x += 1;
		p2.x -= 1;
		p1.y += 1;
		p2.y += 1;
	}
}*/

int main(void) {

	Image image = GenImageColor(width, height, RAYWHITE);

	Vector2 p1 = {width/4, height/4};
	Vector2 p2 = {width - (width/4), height/4};
	Vector2 p3 = {width/2, (height/2) + 12};
	draw_triangle_by_points(&image, p1, p2, p3, RED);

	// Vector2 p1 = {width/4, height/4};
	// Vector2 p2 = {width - (width/4), height/4};
	// draw_triangle_by_lines(&image, p1, p2, RED);

	bool success = ExportImage(image, OUTPUT_DIR "/triangle.png");
	UnloadImage(image);
	printf("%s\n", success ? "successfully saved the image" : "error saving the image");
	return 0;
}