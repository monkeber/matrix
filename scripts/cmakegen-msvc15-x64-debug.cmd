@echo off

:: Check build parent directory
set BUILD_CONFIGURATION=debug
set BUILD_ARCH=x64
set BUILD_DIR=..\build\msvc15-%BUILD_ARCH%-%BUILD_CONFIGURATION%

set VS_VERSION=15
set VS_YEAR=2017
set VS_ARCH=Win64

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
	-s compiler="Visual Studio" ^
	-s compiler.runtime=MTd ^
	-s compiler.version=15 ^
	-s os=Windows ^
	--build=missing
if errorlevel 1 goto :error_conan

:: Run CMake.
echo [*] Running Cmake
cmake ^
	-G "Visual Studio %VS_VERSION% %VS_YEAR% %VS_ARCH%" ^
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