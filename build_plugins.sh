#!/bin/bash
set -e

echo "=== ClassicJems Build System ==="
echo "Detecting package manager and installing dependencies..."

# Install dependencies if root/sudo is available
if [ "$EUID" -ne 0 ]; then
  SUDO="sudo"
else
  SUDO=""
fi

if command -v apt-get &> /dev/null; then
  echo "Debian/Ubuntu detected. Installing dependencies..."
  $SUDO apt-get update -y
  $SUDO apt-get install -y cmake build-essential \
    qt6-base-dev qt6-declarative-dev qt6-base-private-dev qtbase5-dev qtdeclarative5-dev qtbase5-private-dev extra-cmake-modules \
    liblxqt1-dev libkf5plasma-dev libxfce4panel-2.0-dev libsdl2-dev
elif command -v pacman &> /dev/null; then
  echo "Arch Linux detected. Installing dependencies..."
  $SUDO pacman -Sy --noconfirm cmake gcc make \
    qt6-base qt6-declarative extra-cmake-modules \
    liblxqt libkf6plasma libxfce4panel sdl2
elif command -v dnf &> /dev/null; then
  echo "Fedora detected. Installing dependencies..."
  $SUDO dnf install -y cmake gcc-c++ make \
    qt6-qtbase-devel qt6-qtdeclarative-devel extra-cmake-modules \
    liblxqt-devel kf6-plasma-devel libxfce4panel-devel SDL2-devel
else
  echo "Unknown package manager. Please ensure Qt6, SDL2, LXQt-Panel, KDE Plasma, and XFCE4-Panel dev libraries are installed."
fi

echo "=== Configuring build directory ==="
mkdir -p build
cd build

echo "=== Running CMake ==="
cmake ..

echo "=== Compiling plugins ==="
cmake --build . --config Release

echo "=== Build Complete! ==="
