echo off
cls
echo Build server...
mkdir .\bin
g++ -std=c++20 %cd%\Server\*.cpp %cd%\Server\Handler\*.cpp %cd%\Server\LinuxServer\*.cpp %cd%\Server\WinServer\*.cpp %cd%\Misc\*.cpp %cd%\DB\*.cpp %cd%\DB\Structs\*.cpp -lws2_32 -o %cd%\bin\server.exe
echo Build srever complete
pause