SV_SRC = Server/*.cpp Server/Handler/*.cpp Server/LinuxServer/*.cpp Server/WinServer/*.cpp Misc/*.cpp DB/*.cpp DB/Structs/*.cpp
CL_SRC = Client/*.cpp Client/Handler/*.cpp Client/WinClient/*.cpp Client/LinuxClient/*.cpp Misc/*.cpp
SV_TARGET = chatserver
CL_TARGET = chatclient
PREFIX = /usr/local/bin

consolechat:
	mkdir -p bin
	g++ -std=c++20 -o bin/$(SV_TARGET) $(SV_SRC)
	g++ -std=c++20 -o bin/$(CL_TARGET) $(CL_SRC)

install:
	install bin/$(SV_TARGET) $(PREFIX)
	install bin/$(CL_TARGET) $(PREFIX)
	
uninstall:
	rm -rf $(PREFIX)/$(SV_TARGET)
	rm -rf $(PREFIX)/$(CL_TARGET)