#!/bin/bash

rm -rf build            # Deletes build folder 
cmake -B build          # configure once
cmake --build build -j  # -j uses all available cores

./app

# mkdir -p build

# # Compile
# for file in $(find . -name "*.cpp"); do
#     g++ -std=c++17 -I. -c "$file" -o "build/$(basename ${file%.cpp}.o)"
# done

# # Link
# g++ build/*.o -o app

# # Run
# ./app

# #!/bin/bash

# # Compile all .cpp files into object files
# g++ -c *.cpp
# # g++ -c JSONLoader.cpp
# # g++ -c Prompt.cpp
# # g++ -c User.cpp
# # g++ -c Auth.cpp
# # g++ -c main.cpp

# # Link all object files into executable
# g++ *.o -o app

# # Optional: clean object files
# rm *.o

# # Run
# ./app
# 

# To build for older versions
# 
# export PATH=$PWD/x86_64-linux-musl-cross/bin:$PATH



# cmake -B build -S . \

  # -DCMAKE_C_COMPILER=musl-gcc \

  # -DCMAKE_CXX_COMPILER=musl-g++


                         