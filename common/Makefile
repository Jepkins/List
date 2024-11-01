CURRENT_DIR = $(curr)
CC = $(comp)
CFLAGS = $(flags)
BUILD_DIR = $(build)/$(CURRENT_DIR)

LIBS =

SUBDIRS = $(wildcard */)

SOURCES = $(wildcard $(addsuffix *.cpp, $(SUBDIRS)))
OBJECTS = $(addprefix $(BUILD_DIR)/, $(SOURCES:.cpp=.o))
DEPS = $(addprefix ../, $(addsuffix .d, $(basename $(OBJECTS))))

LOCAL_INCLUDES = $(addprefix -I, $(SUBDIRS))
GLOBAL_INCLUDES = $(addprefix $(CURRENT_DIR)/, $(SUBDIRS))
CFLAGS += $(LOCAL_INCLUDES)

all: $(addprefix ../, $(OBJECTS))
	@echo $(CURRENT_DIR): done

-include $(DEPS)

../$(BUILD_DIR)/%.o: %.cpp
	@echo --making $(@:../%=%)--
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) $< -o $@

echo_objects:
	@echo $(OBJECTS)

echo_includes:
	@echo $(GLOBAL_INCLUDES)

echo_libs:
	@echo $(LIB_INCS)







