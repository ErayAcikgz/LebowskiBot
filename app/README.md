# Lebowski Desktop Application

This directory contains the desktop Qt application and user interface for the Lebowski robot project. It is the application component of the `LebowskiBot` repository, not a standalone repository.

## Current Purpose

The application currently provides the UI shell and an initial overview screen. The overview displays static/disconnected robot, joint, power, and system status values. Control, program, teach, AI, monitor, and settings pages are currently placeholder views; no application backend or robot integration is implemented.

## Technologies

- C++17
- Qt 6 Widgets
- CMake 3.21 or newer
- Qt automatic MOC, UIC, and RCC processing

## Layout

- `CMakeLists.txt`: Application build configuration and Qt Widgets target definition.
- `src/main.cpp`: Application entry point.
- `src/ui/`: Main window, top bar, navigation, status bar, and reusable widgets.
- `src/pages/`: Overview and placeholder application pages.
- `resources/resources.qrc`: Qt resource collection.
- `resources/styles/app.qss`: Application stylesheet.

## Build and Run

From the repository root, with Qt 6 Widgets and CMake available:

```sh
cmake -S app -B app/build
cmake --build app/build
./app/build/Lebowski
```
