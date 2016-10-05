# get Microsoft Visual Studio command from registery
for /f "tokens=3*" %%i in ('reg query "HKEY_CLASSES_ROOT\Applications\WDExpress.exe\shell\open\command" /ve') do set VC_PATH_TMP=%%i %%j

# filter argument placeholder
set VC_PATH_LOCAL=%VC_PATH_TMP:"%1"=%

set WORKING_DIR=%CD%

mkdir cmake
cd cmake
cmake.exe -G "Visual Studio 14" -A "x64" ..

if errorlevel 1 (
	echo "cmake failed to generate the project files"
	echo "Note: The windows version of cmake is required and NOT the cygwin version!"
	echo "The cygwin version doesn't have any Visual Studio generators compiled in!"
	pause
	exit 1
)

if exist %VC_PATH_LOCAL% (
	start "" %VC_PATH_LOCAL% %WORKING_DIR%/cmake/btreetest.sln
)
