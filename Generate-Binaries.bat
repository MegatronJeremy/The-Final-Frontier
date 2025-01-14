@echo off

REM Configuration
set PROJECT_NAME=TheFinalFrontier
set BUILD_DIR=build
set CMAKE_GENERATOR="Unix Makefiles"
set BUILD_TYPE=Debug

REM Step 1: Check if CMake is installed
cmake --version >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo CMake is not installed. Please install CMake to continue.
    pause
    exit /b 1
)

REM Step 2: Check if GCC is installed
gcc --version >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo GCC is not installed or not in PATH. Please install GCC to continue.
    pause
    exit /b 1
)

REM Step 3: Create the build directory if it doesn't exist
if not exist %BUILD_DIR% (
    mkdir %BUILD_DIR%
)

REM Step 4: Run CMake to generate the project
echo Generating project files with CMake...
cmake -S . -B %BUILD_DIR% -G %CMAKE_GENERATOR% -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DCMAKE_C_COMPILER=gcc -DCMAKE_CXX_COMPILER=g++
if %ERRORLEVEL% NEQ 0 (
    echo Failed to generate the project files.
    pause
    exit /b 1
)

REM Step 5: Build the project
echo Building the project...
cd %BUILD_DIR%
make
if %ERRORLEVEL% NEQ 0 (
    echo Build failed.
    cd ..
    pause
    exit /b 1
)
cd ..

REM Step 6: Locate the executable
echo Locating the executable...
for /r %BUILD_DIR% %%f in (%PROJECT_NAME%*) do (
    set EXECUTABLE_PATH=%%f
    goto ExecutableFound
)

:ExecutableNotFound
echo Error: Executable not found in the build directory.
pause
exit /b 1

:ExecutableFound
echo Found executable at %EXECUTABLE_PATH%.

REM Step 7: Move the executable to the root folder
move "%EXECUTABLE_PATH%" .\%PROJECT_NAME%.exe
if %ERRORLEVEL% NEQ 0 (
    echo Failed to move the executable.
    pause
    exit /b 1
)

REM Step 8: Delete the build directory
echo Cleaning up...
rmdir /s /q %BUILD_DIR%

echo Build completed successfully. Executable is located at .\%PROJECT_NAME%.exe
pause
