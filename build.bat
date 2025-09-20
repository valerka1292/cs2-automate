@echo off
echo Creating build directory...
if not exist build mkdir build

echo Entering build directory...
cd build

echo Running CMake...
cmake ..

echo Building project...
cmake --build . --config Release

echo Build completed!
pause
