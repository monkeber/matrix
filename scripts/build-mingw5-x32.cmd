@echo off

:: Configure build variables.
set BUILD_CONFIGURATION=release
set BUILD_ARCH=x32
set BUILD_DIR=..\build\mingw5-%BUILD_ARCH%
set ENABLE_TESTS=TRUE

echo [*] Creating a build directory...
if not exist %BUILD_DIR% mkdir %BUILD_DIR%
pushd %BUILD_DIR%

:: Run conan.
:: build_type option requires first letter of value
:: to be capital.
echo [*] Installing dependencies...
conan ^
	install ^
	..\.. ^
	-s build_type=Release ^
	-s compiler="gcc" ^
	-s os=Windows ^
	--build=missing
if errorlevel 1 goto :error_conan

:: Run CMake.
echo [CMake] Generating project files...
cmake ^
	-G "MinGW Makefiles" ^
	-DCMAKE_BUILD_TYPE:STRING=%BUILD_CONFIGURATION% ^
	-DENABLE_TESTING:BOOL=%ENABLE_TESTS% ^
	..\.. 
if errorlevel 1 goto :error_cmake

echo [*] Project generation succeded!
echo [*] --------------------------------------------------

echo [*] Building Release configuration...
echo [*] --------------------------------------------------
cmake --build . --config %BUILD_CONFIGURATION%
if errorlevel 1 goto :error_build

echo [*] Building succeded!
echo [*] --------------------------------------------------

if %ENABLE_TESTS% == TRUE (
	echo [*] Running tests...
	echo [*] --------------------------------------------------

	ctest --extra-verbose --parallel %NUMBER_OF_PROCESSORS%
	if errorlevel 1 goto :error_tests

	echo [*] Testing succed!
	echo [*] --------------------------------------------------
)

:: Back to previous working directory.
popd

goto :eof

:error_build
echo [!] ERROR: Failed to build project - see above and correct.
popd
exit /b 1

:error_cmake
echo [!] ERROR: Failed to generate project files - see above and correct.
popd
exit /b 2

:error_conan
echo [!] ERROR: Failed to install dependencies - see above and correct.
popd
exit /b 3

:error_tests
echo [!] ERROR: Failed to pass tests - see above and correct.
popd
exit /b 4