CC=cc

DIST=build
OUT=$(DIST)/arl.out

LDFLAGS=
GFLAGS=-Wall -Wextra -Wpedantic -std=c23 -I./src/
DFLAGS=-ggdb -fsanitize=address -fsanitize=undefined
RFLAGS=-O3

MODE=release
ifeq ($(MODE), release)
CFLAGS=$(GFLAGS) $(RFLAGS)
else
CFLAGS=$(GFLAGS) $(DFLAGS)
endif

HEADERS=$(shell find "src" -type 'f' -name '*.h')
MODULES=main lib/vec lib/sv parser/ast parser/parser
OBJECTS=$(patsubst %,$(DIST)/%.o, $(MODULES))

$(OUT): $(OBJECTS) | $(DIST)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(DIST)/%.o: src/arl/%.c $(HEADERS) | $(DIST)
	$(CC) $(CFLAGS) -c -o $@ $<

$(DIST)/%.o: src/arl/parser/%.c $(HEADERS) | $(DIST)
	$(CC) $(CFLAGS) -c -o $@ $<

$(DIST)/%.o: src/arl/lib/%.c $(HEADERS) | $(DIST)
	$(CC) $(CFLAGS) -c -o $@ $<

$(DIST):
	mkdir -p $(DIST)
	mkdir -p $(DIST)/lib
	mkdir -p $(DIST)/parser

.PHONY: run clean
ARGS=
run: $(OUT)
	./$^ $(ARGS)

clean:
	rm -rf $(DIST)
