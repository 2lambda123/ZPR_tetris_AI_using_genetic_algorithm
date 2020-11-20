# Genetic Tetris

## Installing dependencies

### Boost.Test
#### Linux
```sh
sudo apt install libboost-test1.67-dev
```
#### Windows
Download and install prebuilt binary (version 1.67 or higher)
https://sourceforge.net/projects/boost/files/boost-binaries/

### SFML
#### Linux
```sh
sudo apt install libsfml-dev
```
#### Windows
Download and put in lib/win <br>
https://www.sfml-dev.org/files/SFML-2.5.1-windows-vc15-64-bit.zip

### Doxygen
https://www.doxygen.nl/download.html

## How to generate code documentation
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

## Project compilation
### Linux
```sh
mkdir build
cd build
cmake ..
make -j4
```

### Windows
This will generate Visual Studio solution
```sh
mkdir build
cd build
cmake .. -A x64
```
