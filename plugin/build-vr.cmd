@echo off
rem Usage: build-vr.cmd <flat|vr|all> [Release|RelWithDebInfo|Debug]   (VS 2022 Build Tools + %USERPROFILE%\vcpkg)
setlocal
set RT=%1
if "%RT%"=="" set RT=vr
set CONFIG=%2
if "%CONFIG%"=="" set CONFIG=Release
call "C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat" >nul || exit /b 1
set VCPKG_ROOT=%USERPROFILE%\vcpkg
cd /d "%~dp0"
cmake -S . -B build\%RT%-%CONFIG% -G Ninja -DCMAKE_BUILD_TYPE=%CONFIG% -DDELIRIUM_RUNTIMES=%RT% "-DCMAKE_SHARED_LINKER_FLAGS=/MAP" "-DCMAKE_TOOLCHAIN_FILE=%VCPKG_ROOT%\scripts\buildsystems\vcpkg.cmake" -DVCPKG_TARGET_TRIPLET=x64-windows-static-md "-DVCPKG_INSTALLED_DIR=%~dp0build\vcpkg_installed" || exit /b 1
cmake --build build\%RT%-%CONFIG% -- -k 0 || exit /b 1
