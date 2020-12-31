# Genetic Tetris
![projectState](/res/state.png)
## Installing dependencies
### For clean Ubuntu 20.04 installation
```sh
sudo add-apt-repository universe
sudo apt update
sudo apt install cmake
sudo apt install doxygen
sudo apt install g++
sudo apt install libboost-test1.67-dev
sudo apt install libsfml-dev
```

## Dependencies
### Boost.Test
**Linux**
```sh
sudo apt install libboost-test1.67-dev
```
**Windows**
Download and install prebuilt binary (version 1.67 or higher) <br>
https://sourceforge.net/projects/boost/files/boost-binaries/

### CMake
**Linux**
```sh
sudo apt install cmake
```
**Windows**
https://cmake.org/download/

### Doxygen
**Linux**
```sh
sudo apt install doxygen
```
**Windows**
https://www.doxygen.nl/download.html

### SFML
**Linux**
```sh
sudo apt install libsfml-dev
```
**Windows**
Download and put in lib/win <br>
https://www.sfml-dev.org/files/SFML-2.5.1-windows-vc15-64-bit.zip


## Project compilation
### Linux
```sh
mkdir build
cd ./build
cmake ..
make -j4
```
`./app` to run main app (use arrow keys to play, **DOWN** to force drop, **UP** to rotate) <br>
`./tests/main` to run tests

### Windows
This will generate Visual Studio solution
```sh
mkdir build
cd build
cmake .. -A x64
```


## Generating code documentation
Go to *docs/* directory
From *docs/*
```sh
doxygen config
```
This generates *html/* and *latex/* directories
To view documentation in html open *html/index.html*
To generate *pdf* go to *latex/* directory and run:
```sh
make pdf
```


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
