FROM fedora:latest

RUN dnf update -y
RUN dnf upgrade -y

RUN dnf install -y gcc-c++ make cmake mingw64-gcc mingw64-gcc-c++ clang wine git libstdc++-static glibc-static ninja-build

RUN git clone https://github.com/libsdl-org/SDL && \
cd SDL &&\
mkdir build &&\
cd build &&\
cmake .. -DSDL_STATIC_PIC=ON -DSDL_SHARED=ON -DSDL_STATIC=ON &&\
cmake --build . &&\
cmake --install .
