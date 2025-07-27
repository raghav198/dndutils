OBJS = main.o dice.o skills.o
APP = dnd

DBG = -g
REL = -O3
CFLAGS = -std=c11 -Wall -Wextra --pedantic -Iinclude/
LDFLAGS = 

OUTDIR = bin
DEPDIR := $(OUTDIR)/.deps
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.d

CFLAGS += $(DEPFLAGS)
CFLAGS += $(DBG)

all: $(APP)

# submodules:
# 	git submodule update --init --recursive --depth 1

# include/raylib.h: submodules
# 	mkdir -p include
# 	cp vendor/raylib/src/raylib.h include/raylib.h

# include/raymath.h: submodules
# 	mkdir -p include
# 	cp vendor/raylib/src/raymath.h include/raymath.h

# lib/libraylib.a: submodules
# 	cd vendor/raylib/src; make PLATFORM=PLATFORM_DESKTOP
# 	mkdir -p lib
# 	cp vendor/raylib/src/libraylib.a lib/

$(OUTDIR)/%.o: src/%.c

$(OUTDIR)/%.o: src/%.c $(DEPDIR)/%.d | $(DEPDIR)
	$(CC) -c $< -o $@ $(CFLAGS)

$(DEPDIR): ; @mkdir -p $@

DEPFILES := $(OBJS:%.o=$(DEPDIR)/%.d)
$(DEPFILES):

include $(wildcard $(DEPFILES))

$(APP): $(addprefix bin/, $(OBJS))
	$(CC) bin/*.o $(LDFLAGS) -o $(APP)

clean:
	/bin/rm -rf $(OUTDIR)
	/bin/rm -rf $(APP)