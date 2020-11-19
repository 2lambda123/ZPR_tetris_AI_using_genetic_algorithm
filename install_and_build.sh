sudo apt install libboost-test1.67-dev libsfml-dev -y
BUILD_DIR="build"
[ ! -d "$BUILD_DIR" ] && mkdir "$BUILD_DIR"
cd "$BUILD_DIR"
cmake ..
make -j4
