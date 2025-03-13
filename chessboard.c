#include <stdio.h>
#include <string.h>
#include "raylib.h"

#define OUTPUT_DIR "output"

const int width = 400;
const int height = 400;

bool is_white(int x, int y) {
	int x_square = x / (width  / 8);
    int y_square = y / (height / 8);
    return (x_square + y_square) % 2 == 0;
}

int main(void) {
	Image image = GenImageColor(width, height, BLACK);

	for(int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			if (is_white(x, y)) {
				ImageDrawPixel(&image, x, y, WHITE);
			}
		}
	}

	bool success = ExportImage(image, OUTPUT_DIR "/chessboard.png");
	UnloadImage(image);
	printf("%s\n", success ? "successfully saved the image" : "error saving the image");
	return 0;
}
