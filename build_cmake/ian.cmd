@echo off
mkdir build_debugger_vs2019
pushd build_debugger_vs2019
"C:\Program Files\CMake\bin\cmake" ../.. -G "Visual Studio 16 2019" -A x64 -DUSE_QT=on -DBUILD_PSFPLAYER=on -DDEBUGGER_INCLUDED=on -DCMAKE_PREFIX_PATH="C:\Qt\5.13.0\msvc2017_64"
popd
