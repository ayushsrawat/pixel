CC = cc
SRC = src
BUILD = build
RAY_HOME=/opt/thirdparty/raylib/5.5
RAY_LIB = ${RAY_HOME}/lib
RAY_INCLUDE = ${RAY_HOME}/include

CFLAGS = -Wall -Wextra -pedantic -I"${RAY_INCLUDE}"
LDFLAGS = -L"${RAY_LIB}" -lraylib -Wl,-rpath,"${RAY_LIB}"
#LDFLAGS = -L"${RAY_LIB}" -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreAudio -framework CoreVideo -Wl,-rpath,"${RAY_LIB}"

.PHONY: all
all: chessboard projectile

$(BUILD):
	mkdir -p $(BUILD)

chessboard: $(BUILD) $(SRC)/chessboard.c
	$(CC) $(CFLAGS) -o $(BUILD)/chessboard $(SRC)/chessboard.c $(LDFLAGS)

projectile: $(BUILD) $(SRC)/projectile.c
	$(CC) $(CFLAGS) -o $(BUILD)/projectile $(SRC)/projectile.c $(LDFLAGS)

clean:
	rm -rf $(BUILD)
