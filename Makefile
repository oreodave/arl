CC=cc

DIST=build
OUT=$(DIST)/arl.out
MODULES=main lib/vec lib/sv parser/ast parser/parser
OBJECTS:=$(patsubst %,$(DIST)/%.o, $(MODULES))

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

# Dependency generation
DEPFLAGS=-MT $@ -MMD -MP -MF
DEPDIR=$(DIST)/deps

$(OUT): $(OBJECTS) | $(DIST)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(DIST)/%.o: src/arl/%.c | $(DIST) $(DEPDIR)
	$(CC) $(CFLAGS) $(DEPFLAGS) $(DEPDIR)/$*.d -c -o $@ $<

$(DIST):
	mkdir -p $(DIST)
	mkdir -p $(DIST)/lib
	mkdir -p $(DIST)/parser

$(DEPDIR):
	mkdir -p $(DEPDIR)
	mkdir -p $(DEPDIR)/lib
	mkdir -p $(DEPDIR)/parser

.PHONY: run clean
ARGS=
run: $(OUT)
	./$^ $(ARGS)

clean:
	rm -rf $(DIST)

DEPS:=$(patsubst %,$(DEPDIR)/%.d, $(MODULES))
include $(wildcard $(DEPS))
