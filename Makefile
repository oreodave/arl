CC=cc

DIST=build
OUT=$(DIST)/main.out

LDFLAGS=
GFLAGS=-Wall -Wextra -Wpedantic -std=c23
DFLAGS=-ggdb -fsanitize=address -fsanitize=undefined
RFLAGS=-O3

MODE=release
ifeq ($(MODE), release)
CFLAGS=$(GFLAGS) $(RFLAGS)
else
CFLAGS=$(GFLAGS) $(DFLAGS)
endif

$(OUT): $(DIST)/main.o | $(DIST)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(DIST)/main.o: main.c | $(DIST)
	$(CC) $(CFLAGS) -c -o $@ $^

$(DIST):
	mkdir -p $(DIST)

.PHONY: run clean
ARGS=
run: $(OUT)
	./$^ $(ARGS)

clean:
	rm -rf $(DIST)
