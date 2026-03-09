# Compiler and Flags
CC = gcc
CFLAGS = -I./ -I./lvgl -O3 -Wall
LDFLAGS = -lSDL2 -lm

# Find all LVGL source files automatically
LVGL_SOURCES = $(shell find lvgl/src -name "*.c")
# Add your main file
SOURCES = main.c
# Convert .c names to .o names in a 'build' folder
LVGL_OBJECTS = $(patsubst %.c, build/%.o, $(LVGL_SOURCES))
APP_OBJECTS = $(patsubst %.c, build/%.o, $(SOURCES))

TARGET = my_app
LIBLVGL = build/liblvgl.a

all: $(LIBLVGL) $(TARGET)

# Build the application by linking with liblvgl.a
$(TARGET): $(APP_OBJECTS) $(LIBLVGL)
	$(CC) -o $@ $(APP_OBJECTS) $(LIBLVGL) $(LDFLAGS)

# Create the static library
$(LIBLVGL): $(LVGL_OBJECTS)
	@mkdir -p $(dir $@)
	ar rcs $@ $^

# Rule to compile every .c file into a .o file
build/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf build $(TARGET)
