CC = g++
CFLAGS = -c -Wall -D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual\
-Wchar-subscripts -Wconversion -Wctor-dtor-privacy -Wempty-body -Wformat-nonliteral -Wformat-security -Wformat=2 -Winline\
-Wnon-virtual-dtor -Woverloaded-virtual -Wpacked -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo\
-Wstrict-overflow=2 -Wsuggest-override -Wswitch-default -Wundef -Wunreachable-code -Wunused -Wvariadic-macros\
-Wno-missing-field-initializers -Wno-narrowing -Wno-old-style-cast -Wno-varargs -Wsuggest-override\
-Wlong-long -fopenmp -fcheck-new -fsized-deallocation -fstack-protector -fstrict-overflow -fno-omit-frame-pointer\
-Wlarger-than=8192 -fPIE -Werror=vla -MP -MMD
LDFLAGS =
BUILD_DIR = build

LIST_DIR = list
COMMON_DIR = common

COMMON_OBJECTS = $(shell $(MAKE) -s -C $(COMMON_DIR) curr=$(COMMON_DIR) build='$(BUILD_DIR)' echo_objects)
LIST_OBJECTS = $(shell $(MAKE) -s -C $(LIST_DIR) curr=$(LIST_DIR) build='$(BUILD_DIR)' echo_objects)
COMMON_INCLUDES = $(shell $(MAKE) -s -C $(COMMON_DIR) curr=$(COMMON_DIR) echo_includes)

LDFLAGS += $(shell $(MAKE) -s -C $(COMMON_DIR) curr=$(COMMON_DIR) echo_libs)
EXECUTABLE_DIR = $(BUILD_DIR)/executable

LIST_EXEC = $(EXECUTABLE_DIR)/list.exe

.PHONY: all list common run

all: list

list: $(LIST_EXEC)
	@echo Successfully remade $<

common: $(COMMON_OBJECTS)

$(LIST_EXEC): make_common make_list
	@echo --making $@--
	@mkdir -p $(@D)
	@sleep 1
	@$(CC) $(COMMON_OBJECTS) $(LIST_OBJECTS) -o $@ $(LDFLAGS)

make_common:
	@echo $(shell $(MAKE) -s -C $(COMMON_DIR) curr=$(COMMON_DIR) comp='$(CC)' flags='$(CFLAGS)' build='$(BUILD_DIR)')

make_list:
	@echo $(shell $(MAKE) -s -C $(LIST_DIR) curr=$(LIST_DIR) comp='$(CC)' flags='$(CFLAGS)' build='$(BUILD_DIR)' common_incs='$(COMMON_INCLUDES)')

run:
	./$(LIST_EXEC)



clean:
	@rm -rf -d $(BUILD_DIR)
