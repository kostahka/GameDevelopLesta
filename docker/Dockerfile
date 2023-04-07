FROM fedora:latest

RUN dnf update -y
RUN dnf upgrade -y

RUN dnf install -y gcc-c++ make cmake mingw64-gcc mingw64-gcc-c++ clang wine git libstdc++-static glibc-static ninja-build

RUN git clone https://github.com/libsdl-org/SDL
WORKDIR SDL
RUN mkdir build
WORKDIR build
RUN cmake ..
RUN cmake --build .
RUN cmake --install .
