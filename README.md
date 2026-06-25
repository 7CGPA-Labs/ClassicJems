# RetroFuse: C++/Qt J2ME & Symbian Desktop Emulator

RetroFuse (Antigravity) is a high-performance desktop emulation platform for J2ME and Symbian mobile application archives, built natively using **C++17** and the **Qt6 framework**. By combining UI view tracking and execution engines inside a single runtime ecosystem, it provides native instruction interpretation loops without garbage collection or intermediate translation lags.

---

## 📁 1. Project Directory Structure

Layout of the folders and files in the repository:

```text
retrofuse-qt/
├── CMakeLists.txt              # Unified build rules for Windows and Linux
├── README.md                   # Setup guide and compilation workflows
├── src/                        # Core C++ Application Source
│   ├── main.cpp                # App entry point initializing Qt
│   ├── mainwindow.h            # Main UI controller definitions
│   ├── mainwindow.cpp          # Stacked screen router & keyboard interceptor logic
│   ├── gamelibrarywidget.h     # Game Library grid view definitions
│   └── gamelibrarywidget.cpp   # Game Library drag-and-drop & search logic
└── third_party/                # Engine Submodule Repositories
    ├── eka2l1/                 # Symbian C++ emulation core (Submodule)
    └── freej2me/               # J2ME execution core (Submodule)
```

---

## 🚀 2. Features

- **Retro ROM Dashboard**: A modern, dark-themed grid dashboard presenting available game titles grouped by platform.
- **Drag-and-Drop Loader**: Import games instantly by dragging `.jar` (J2ME) or `.sis` / `.sisx` (Symbian) files directly into the emulator dashboard.
- **Incremental Search Indexer**: Quickly filter game collections by typing in the search bar.
- **Viewport Stack Routing**: Switches viewports instantly from the dashboard to a fixed aspect ratio (`450x800`) OpenGL rendering viewport.
- **Zero-Latency Keyboard Interception**: Bypasses OS key-repeat delays to capture instantaneous button press and release states.

---

## 🎮 3. Input Mappings & Controls

When playing games, your desktop system keyboard functions as a virtual mechanical keypad array mapped according to the following matrix rules:

* **W / A / S / D (or Arrow Keys)** -> Directional Navigational D-Pad (Up=1, Down=2, Left=3, Right=4)
* **Z / Enter** -> Primary Select / Center Trigger Key (Button A / KeyCode 5)
* **X / Escape** -> Clear Frame / Return Navigation (Button B / KeyCode 8)
* **Q / E** -> Left Soft Key (LSK = -6) / Right Soft Key (RSK = -7)
* **Top Row 0 - 9 Keys** -> Standard Numeric T9 input matrix overrides (0-9)
* **F1 Key** -> Exit active emulation and return to the Game Library launcher dashboard

---

## 🛠️ 4. Compilation & Local Setup

### 🐧 Linux Environment Setup (Ubuntu/Debian)

Install core developer tool chains and the required library components directly via your system package manager:

```bash
sudo apt update
sudo apt install build-essential cmake qt6-base-dev qt6-declarative-dev libsdl2-dev
```

### 🪟 Windows Environment Setup

1. Download and install **Visual Studio 2022 Community Edition** (Ensure **"Desktop development with C++"** workspace tracking is checked).
2. Install **CMake** and verify it is mapped onto your global Windows environment system PATH variables.
3. Fetch dependencies cleanly via **vcpkg**:
```powershell
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
./bootstrap-vcpkg.bat
./vcpkg install qt6-base sdl2
```

---

## 🚀 5. Execution Pipelines

To configure, build, and launch the executable workspace binary from your command terminal shell, run the following unified compile sequence:

```bash
# Generate build workspace tracking folder
mkdir build
cd build

# Configure project make vectors via CMake
# (Windows users: Append your target -DCMAKE_TOOLCHAIN_FILE variable pointing to vcpkg script)
cmake ..

# Compile the final native application executable payload
cmake --build . --config Release
```