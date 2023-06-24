echo off
cls
echo Build client...
mkdir .\bin
g++ -std=c++20 %cd%/Client/*.cpp %cd%/Client/Handler/*.cpp %cd%/Client/WinClient/*.cpp %cd%/Client/LinuxClient/*.cpp %cd%\Misc\*.cpp -lws2_32 -o %cd%\bin\client.exe
echo Build client complete
pause