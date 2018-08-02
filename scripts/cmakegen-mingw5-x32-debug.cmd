@echo off

:: Configure build variables.
set BUILD_CONFIGURATION=debug
set BUILD_ARCH=x32
set BUILD_DIR=..\build\mingw5-%BUILD_ARCH%-%BUILD_CONFIGURATION%

echo Creating a build directory...
if not exist %BUILD_DIR% mkdir %BUILD_DIR%
pushd %BUILD_DIR%

:: Run conan.
:: build_type option requires first letter of value
:: to be capital.
echo [*] Installing dependencies...
conan ^
	install ^
	..\.. ^
	-s build_type=Debug ^
	-s compiler="gcc" ^
	-s os=Windows ^
	--build=missing
if errorlevel 1 goto :error_conan

:: Run CMake.
echo [CMake] Generating project files...
cmake ^
	-G "MinGW Makefiles" ^
	-DCMAKE_BUILD_TYPE:STRING=%BUILD_CONFIGURATION% ^
	-DENABLE_TESTING:STRING=ON ^
	..\.. 
if errorlevel 1 goto :error_cmake

echo [*] Project generation succeded!
echo [*] --------------------------------------------------

:: Back to previous working directory.
popd

goto :eof

:error_cmake
echo [!] ERROR: Failed to generate project files - see above and correct.
popd
exit /b 1

:error_conan
echo [!] ERROR: Failed to install dependencies - see above and correct.
popd
exit /b 2
