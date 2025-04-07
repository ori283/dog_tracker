@echo off
echo Configuring CMake...
cmake -G "MinGW Makefiles" -B build -S .

echo.
echo Building project...
cmake --build build

echo.
echo Done! The executable is in bin/ 