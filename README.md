# Simple SDL2 + LVGL App

This repository contains a minimal application that demonstrates how to use [LVGL](https://github.com/lvgl/lvgl) with [SDL2](https://www.libsdl.org/) for graphics rendering.  
LVGL is included as a Git submodule (see `.gitmodules`).

---

## 📦 Project Setup

### Step 1: Clone the project
git clone https://github.com/peakhunt/sdl_lvgl.git
cd yourproject

### Step 2: Initialize the LVGL submodule
git submodule update --init --recursive

---

## ⚙️ Dependencies
- SDL2 (development libraries must be installed)
- LVGL (added as a Git submodule)
- C compiler (e.g., GCC or Clang)
- Make (build system)

On Linux (Debian/Ubuntu):
sudo apt-get install libsdl2-dev make

On Windows:
Install SDL2 development libraries from the official site and use MinGW or a similar environment with `make`.

---

## 🚀 Build & Run
Build using the provided Makefile:
make

Run the app:
./simple_app

---

## 📂 Project Structure
doc/          # Documentation and screenshots  
lvgl/         # LVGL submodule  
README.md     # Project documentation  

---

## 🖼️ Screenshot
Here’s what the app looks like:

![App Screenshot](doc/michael%20slop.png)
