CC = clang
CFLAGS = -Wall -Wextra -Iinclude -std=c11

SRC = src/main.c src/logger.c
OUT = hyperxhid

all:
	$(CC) $(CFLAGS) -framework IOKit -framework CoreFoundation $(SRC) -o $(OUT)

clean:
	rm -f $(OUT)
