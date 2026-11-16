# Inspired by https://github.com/tilir/teaching/blob/master/problem_lc/makefile.v3.mak

ifeq ($(origin CC), default)
CC = gcc
endif

CFLAGS ?= -Wall -Wextra -Wdeclaration-after-statement -std=c99 -pedantic -Og -g
override CFLAGS += -I$(INC_DIR)

SRC_DIR = src
INC_DIR = inc
BUILD_DIR ?= build

EXECUTABLE ?= $(BUILD_DIR)/bitcarpet
SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/bitcarpet.c
OBJS = $(addprefix $(BUILD_DIR)/,$(SRCS:.c=.o))
DEPS = $(OBJS:.o=.d)

# Targets which we have no need to recollect deps
NO_DEP_TARGETS = clean

.PHONY: all run clean

all: $(EXECUTABLE)

# Static pattern rule to not redefine generic one
$(OBJS): $(BUILD_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -o $@ $<

# Or should i use -MMD -MP?
$(DEPS): $(BUILD_DIR)/%.d: %.c
	@mkdir -p $(@D)
	$(CC) -E $(CFLAGS) -MM -MT $(@:.d=.o) $< > $@

ifeq (0, $(words $(findstring $(MAKECMDGOALS), $(NO_DEP_TARGETS))))
include $(DEPS)
endif

# Condition to get rid of two spaces when LDFLAGS is empty
$(EXECUTABLE): $(OBJS)
ifneq ($(LDFLAGS),)
	$(CC) $(LDFLAGS) -o $@ $^
else
	$(CC) -o $@ $^
endif

run: $(EXECUTABLE)
	./$(EXECUTABLE)

clean:
	rm -rf $(OBJS) $(DEPS) $(EXECUTABLE)
