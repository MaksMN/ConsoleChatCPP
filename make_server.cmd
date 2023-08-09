echo off
mkdir .\bin
cls
echo Build server...
g++ -std=c++20 %cd%/Server/*.cpp %cd%/Server/DBclient/*.cpp %cd%/Server/ChatCore/*.cpp %cd%/Server/Socket/Handler/*.cpp %cd%/Server/Socket/WinServer/*.cpp %cd%/Misc/*.cpp %cd%/Server/ChatPages/*.cpp "-Ic:\Program Files\MySQL\MySQL Server 8.0\include" "-Lc:\Program Files\MySQL\MySQL Server 8.0\lib" -llibmysql -lws2_32 -lodbc32 -o %cd%\bin\server.exe
echo Build server complete
pause