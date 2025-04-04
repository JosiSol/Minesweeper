# 💣 Minesweeper in C++ with Raylib

A simple clone of the classic **Minesweeper** game built using **C++** and **[Raylib](https://www.raylib.com/)**. Play the game, flag the mines, and try to win!!  

![Minesweeper](https://github.com/user-attachments/assets/55da7a13-9856-4e62-89a8-55198cb88a93)

## 🎮 Features

- Classic 10x10 Minesweeper grid
- Left-click to reveal cells
- Right-click to place or remove flags
- Automatic flood-fill on empty cells
- Game win/loss detection
- Restart game with `Enter` key

## 🛠️ Tech Stack

- **Language**: C++
- **Graphics Library**: [Raylib](https://www.raylib.com/)
- **Build Tool**: Makefile

## 📸 Screenshots
| Gameplay | Game Over | Victory |
|----------|-----------|---------|
| ![Gameplay](https://github.com/user-attachments/assets/2d52d343-93ad-4f8d-bafb-188e3d094b32) | ![Game Over](https://github.com/user-attachments/assets/a2ee93ea-8a8c-4c08-ba78-3813897d90fa) | ![Win](https://github.com/user-attachments/assets/f396dec5-df28-4c5f-95f5-a067121f9448) |

## 🚀 Getting Started

### 1. Clone the Repository

```bash
git clone https://github.com/yourusername/minesweeper-raylib.git
cd minesweeper-raylib
```

### 2. Compile the Game
```bash
g++ src/main.cpp -o minesweeper -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
```
> 🧠 **Note**: The flags above work on **Linux**.  
> If you're on **macOS** or **Windows**, you'll need the appropriate Raylib setup.  
> Check out the [Raylib official installation guide](https://github.com/raysan5/raylib/wiki) for instructions tailored to your OS.

### 3. Run the Game
```bash
./main
```
