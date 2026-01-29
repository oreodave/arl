CC=cc

DIST=build
OUT=$(DIST)/arl.out

MODULES=. lib lexer
UNITS=main lib/vec lib/sv lexer/token lexer/lexer
OBJECTS:=$(patsubst %,$(DIST)/%.o, $(UNITS))

LDFLAGS=
GFLAGS=-Wall -Wextra -Wpedantic -std=c23 -I./include/
DFLAGS=-ggdb -fsanitize=address -fsanitize=undefined -DVERBOSE_LOGS=1
RFLAGS=-O3

MODE=release
ifeq ($(MODE), release)
CFLAGS=$(GFLAGS) $(RFLAGS)
else
CFLAGS=$(GFLAGS) $(DFLAGS)
endif

# Dependency generation
DEPFLAGS=-MT $@ -MMD -MP -MF
DEPDIR=$(DIST)/deps

$(OUT): $(OBJECTS) | $(DIST)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(DIST)/%.o: src/%.c | $(DIST) $(DEPDIR)
	$(CC) $(CFLAGS) $(DEPFLAGS) $(DEPDIR)/$*.d -c -o $@ $<

$(DIST):
	mkdir -p $(patsubst %,$(DIST)/%, $(MODULES))

$(DEPDIR):
	mkdir -p $(patsubst %,$(DEPDIR)/%, $(MODULES))

clangd: compile_commands.json
compile_commands.json: Makefile
	bear -- $(MAKE) -B MODE=debug

.PHONY: run clean
ARGS=
run: $(OUT)
	./$^ $(ARGS)

clean:
	rm -rf $(DIST)

DEPS:=$(patsubst %,$(DEPDIR)/%.d, $(UNITS))
include $(wildcard $(DEPS))
