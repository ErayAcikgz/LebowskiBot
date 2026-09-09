# Lebowski

Lebowski is a 6-DOF programmable robotic arm graduation project.

## Current Status

This repository currently contains only the initial Qt Widgets application skeleton. Robot control, communication, ROS2, motion planning, and hardware integration are not implemented yet.

## Build

Requirements: C++17, Qt 6 Widgets, CMake 3.21 or newer, and Ninja.

```sh
cmake -S . -B build -G Ninja
cmake --build build
```

## Run

```sh
./build/Lebowski
```
