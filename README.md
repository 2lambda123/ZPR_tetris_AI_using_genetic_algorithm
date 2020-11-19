# Genetic Tetris

## Installing dependencies

### Boost.Test
#### Linux
```sh
sudo apt install libboost-test1.67-dev
```
#### Windows

### SFML
#### Linux
```sh
sudo apt install libsfml-dev
```

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
