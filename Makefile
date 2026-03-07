# Compiler and Flags
CC = gcc
CFLAGS = -I./ -I./lvgl -O3 -Wall
LDFLAGS = -lSDL2 -lm

# Find all LVGL source files automatically
LVGL_SOURCES = $(shell find lvgl/src -name "*.c")
# Add your main file
SOURCES = main.c $(LVGL_SOURCES)
# Convert .c names to .o names in a 'build' folder
OBJECTS = $(patsubst %.c, build/%.o, $(SOURCES))

TARGET = my_app

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) -o $@ $^ $(LDFLAGS)

# Rule to compile every .c file into a .o file
build/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf build $(TARGET)

