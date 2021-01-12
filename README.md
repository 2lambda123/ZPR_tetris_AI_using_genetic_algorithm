# Genetic Tetris
![projectState](/res/state.png)

## Installation for Linux
**For clean Ubuntu 20.04 installation**
```sh
sudo add-apt-repository universe
sudo apt update
sudo apt install cmake doxygen g++ libboost-test1.67-dev libsfml-dev

mkdir build
cd ./build
cmake ..
make -j4
```
`./app` to run main app <br>
Tests are in `./tests/`
### Generating code documentation
Go to `docs/` directory <br>
From `docs/`
```sh
doxygen config
```
This generates `html/` and `latex/` directories <br>
To view documentation in html open `html/index.html` <br>
To generate `pdf` go to `latex/` directory and run:
```sh
make pdf
```

## Installation for Windows
**For clean Windows 10 20H2 installation**
1. Boost <br>
   Download and install prebuilt binary (version 1.67 or higher) <br>
   https://sourceforge.net/projects/boost/files/boost-binaries/
2. CMake <br>
   https://cmake.org/download/
3. Doxygen <br>
   https://www.doxygen.nl/download.html
4. SFML <br>
   Download and put in `lib/win` (in project root) <br>
   https://www.sfml-dev.org/files/SFML-2.5.1-windows-vc15-64-bit.zip
5. Microsoft Visual C++ <br>
   https://visualstudio.microsoft.com/pl/vs/features/cplusplus/
```sh
mkdir build
cd build
cmake .. -A x64
./app
```
   
## Dependencies
* Boost
* CMake
* Doxygen
* SFML

## Controls
* **Up** arrow and **X** are to rotate 90° clockwise.
* **Space** to hard drop.
* **Ctrl** and **Z** are to rotate 90° counterclockwise.
* **Down**, **Left**, **Right** arrows perform soft drop, left shift, and right shift respectively.

Numpad controls:
* **8**, **4**, **6**, and **2** are hard drop, left shift, right shift, and soft drop respectively.
* **1**, **5**, and **9** are to rotate 90° clockwise.
* **3** and **7** are to rotate 90° counterclockwise.

## Scoring
| Action    | Point Value       |
| --------- | ----------------- |
| Single    | 100 * level       |
| Double    | 300 * level       |
| Triple    | 500 * level       |
| Tetris    | 800 * level       |
| Soft drop | 1 point per cell  |
| Hard drop | 2 points per cell |

## Coding style
We use clang-format to ensure that everyone is using the same formatting rules. <br>
We use coding style based on *Google C++ Style* <br>
But we make following modifications <br>
```c++
const int kBufferSize = 128; // Google
const int BUFFER_SIZE = 128; // our style

enum class UrlTableError { // Google
  kOk = 0,
  kOutOfMemory,
  kMalformedInput,
};

enum class UrlTableError { // our style
  OK = 0,
  OUT_OF_MEMORY,
  MALFORMED_INPUT,
};

class MyClass { // Google
 public:
  ...
  void FooBar();
};

class MyClass { // our style
public: // access specifier -4 spaces
    ...
    void fooBar(); // tab is 4 spaces wide
};
```
**We use 100 column lines** <br>
**We use .hpp and .cpp file extensions** <br>
**We name our files using snake_case format** <br>
