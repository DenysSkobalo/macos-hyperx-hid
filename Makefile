CC = clang
CFLAGS = -Wall -Wextra -Iinclude -std=c11

SRC = src/main.c src/hid_utils.c
OUT = hyperxhid
LOGS = log.csv

all:
	$(CC) $(CFLAGS) -framework IOKit -framework CoreFoundation $(SRC) -o $(OUT)

clean:
	rm -f $(OUT) $(LOGS)
