EXEC = arcanoid
SRC_FILES = arcanoid.c ball.c brick.c game.c level.c media.c player.c

CC = cc

LDLIBS = -lm -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf

O_FILES = $(SRC_FILES:%.c=%.o)


all: $(EXEC)


$(EXEC): $(O_FILES)

clean:
	rm -f $(EXEC)
	rm -f *.o
