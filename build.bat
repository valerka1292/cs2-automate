@echo off
echo Cleaning old build directory if it exists...
if exist build (
    rmdir /s /q build
)

echo Creating build directory...
mkdir build

echo Entering build directory...
cd build

echo Running CMake...
cmake .. -G "Visual Studio 17 2022" -A x64

if %errorlevel% neq 0 (
    echo [ERROR] CMake configure failed!
    pause
    exit /b %errorlevel%
)

echo Building project...
cmake --build . --config Release

if %errorlevel% neq 0 (
    echo [ERROR] Build failed!
    pause
    exit /b %errorlevel%
)

echo Build completed successfully!
pause
