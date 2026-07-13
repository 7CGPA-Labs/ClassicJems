# ClassicJems: Modular Linux Panel Plugins & Emulator Player (J2ME & Symbian)

ClassicJems is a high-performance desktop emulation platform for J2ME and Symbian mobile application archives, redesigned as modular taskbar panel widgets and desktop applets for **LXQt Panel**, **KDE Plasma (Plasmoid)**, and **XFCE Panel**, accompanied by a standalone core player executable.

---

## 📁 1. Project Directory Structure

Layout of the folders and files in the repository:

```text
ClassicJems/
├── CMakeLists.txt              # Multi-Qt project CMake build definition
├── README.md                   # Setup guide and compilation workflows
├── build_plugins.sh            # Linux package installer and build runner
├── src/                        # Core C++ Application Source
│   ├── emulatorwindow.h        # Shared emulator OpenGL viewport window
│   ├── emulatorwindow.cpp      # Aspect ratio locked (450x800) canvas & key translation
│   ├── gamelibrarywidget.h     # Launcher game grid view definitions
│   ├── gamelibrarywidget.cpp   # ROM drag-and-drop, grid, and search logic
│   ├── player/                 # Standalone bootstrapper executable
│   │   └── main.cpp            # Launches the EmulatorWindow out-of-process
│   ├── lxqt-plugin/            # LXQt panel integration plugin (Qt-based)
│   │   ├── plugin-classicjems.h/.cpp
│   │   └── classicjems.desktop.in
│   ├── kde-plasmoid/           # KDE Plasma desktop widget (QML & C++ extension)
│   │   ├── classicjems_qmlplugin.h/.cpp
│   │   ├── metadata.json
│   │   └── contents/ui/main.qml
│   └── xfce-plugin/            # XFCE taskbar plugin (GTK-based C applet)
│       ├── xfce_plugin.c
│       └── classicjems-xfce.desktop.in
└── third_party/                # Engine Submodule Repositories
    ├── eka2l1/                 # Symbian C++ emulation core (Submodule)
    └── freej2me/               # J2ME execution core (Submodule)
```

---

## 🚀 2. Features

- **LXQt Taskbar Widget**: Integrates a toolbar dropdown presenting the ROM library grid. Launching a game instantiates the player window.
- **KDE Plasma Plasmoid**: A QML-based desktop or panel widget offering quick list ROM access. Double-clicking starts the player.
- **XFCE Panel Applet**: A GTK-based panel button that launches a file explorer to select `.jar` or `.sis` games, starting them out-of-process.
- **Standalone Emulator Player**: A dedicated runtime wrapper (`classicjems-player`) linking to EKA2L1 and FreeJ2ME to render game frames on a locked aspect ratio `450x800` OpenGL window.
- **Zero-Latency Keyboard Interception**: Captures physical keypress and release events to map desktop inputs directly into J2ME/Symbian T9 controls.

---

## 🎮 3. Input Mappings & Controls

When playing games, your desktop system keyboard functions as a virtual keypad mapped according to the following rules:

* **W / A / S / D (or Arrow Keys)** -> Directional Navigational D-Pad (Up=1, Down=2, Left=3, Right=4)
* **Z / Enter** -> Primary Select / Center Trigger Key (Button A / KeyCode 5)
* **X / Escape** -> Clear Frame / Return Navigation (Button B / KeyCode 8)
* **Q / E** -> Left Soft Key (LSK = -6) / Right Soft Key (RSK = -7)
* **Top Row 0 - 9 Keys** -> Standard Numeric T9 input matrix overrides (0-9)
* **F1 Key** -> Exit active emulation and close player window

---

## 🛠️ 4. Compilation & Local Setup

### 🐧 Automatic Build Script
You can compile the player and all three desktop panel plugins automatically using the helper script:

```bash
chmod +x build_plugins.sh
./build_plugins.sh
```
This script detects your distribution (Ubuntu, Arch, Fedora), installs all required dependencies (including Qt5, Qt6, GTK3, XFCE4, and LXQt header files), and builds the project.

### 📦 Manual Dependencies Installation (Debian/Ubuntu 24.04 LTS)

Install the core development packages and libraries:
```bash
sudo apt-get update
sudo apt-get install -y cmake build-essential \
  qt6-base-dev qt6-declarative-dev qt6-base-private-dev \
  qtbase5-dev qtdeclarative5-dev qtbase5-private-dev \
  extra-cmake-modules liblxqt1-dev libkf5plasma-dev libxfce4panel-2.0-dev libsdl2-dev xvfb
```

---

## 🚀 5. Manual Build Pipelines

To build and compile the workspace manually, execute:

```bash
mkdir build
cd build

# Configure build system
cmake ..

# Compile all plugins and player
cmake --build . --config Release
```

---

## 🧪 6. Testing & CI Pipeline

The project features a comprehensive GitHub Actions CI pipeline:
- **Headless GUI Testing**: Employs `xvfb-run` to run the constructed `classicjems-player` in a simulated X11 screen to verify that the GUI framework and OpenGL startup pathways load successfully without crash.
- **Symbol Linkage Verification**: Runs `ldd -r` on all generated shared libraries (`.so` plugin files) to ensure there are no unresolved symbols or linking errors.
- **Artifact Upload**: Packages the runnable player and panel plugins, uploading them as downloadable assets (`classicjems-linux-plugins`) directly to the Actions run summary page.