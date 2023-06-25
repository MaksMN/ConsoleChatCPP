echo off
mkdir .\bin
cls
echo Build server...
g++ -std=c++20 %cd%\Server\*.cpp %cd%\Server\Handler\*.cpp %cd%\Server\LinuxServer\*.cpp %cd%\Server\WinServer\*.cpp %cd%\Misc\*.cpp %cd%\DB\*.cpp %cd%\DB\Structs\*.cpp -lws2_32 -o %cd%\bin\server.exe
echo Build srever complete
echo Build client...
g++ -std=c++20 %cd%/Client/*.cpp %cd%/Client/Handler/*.cpp %cd%/Client/WinClient/*.cpp %cd%/Client/LinuxClient/*.cpp %cd%\Misc\*.cpp -lws2_32 -o %cd%\bin\client.exe
echo Build client complete
pause