CC = gcc
BUILD = build
RAY_HOME=/opt/homebrew/Cellar/raylib/5.0
RAY_LIB = ${RAY_HOME}/lib
RAY_INCLUDE = ${RAY_HOME}/include

CFLAGS = -Wall -Wextra -pedantic -I"${RAY_INCLUDE}"
LDFLAGS = -L"${RAY_LIB}" -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreAudio -framework CoreVideo -Wl,-rpath,"${RAY_LIB}"

.PHONY: all
all: chessboard

$(BUILD):
	mkdir -p $(BUILD)

chessboard: $(BUILD) chessboard.c
	$(CC) $(CFLAGS) -o $(BUILD)/chessboard chessboard.c $(LDFLAGS)

clean:
	rm -rf $(BUILD)