consolechat: miscLib
	g++ -o _Make/consolechat main.cpp DB/*.cpp DB/**/*.cpp Chat/*.cpp Chat/**/*.cpp -L. -l:_Make/miscLib.a

miscLib: Misc/sha1.cpp Misc/DateTime.cpp Misc/Stream.cpp
	mkdir _Make
	g++ -o _Make/sha1.o Misc/sha1.cpp -c
	g++ -o _Make/DateTime.o Misc/DateTime.cpp -c
	g++ -o _Make/Stream.o Misc/Stream.cpp -c
	ar rc _Make/miscLib.a _Make/sha1.o _Make/DateTime.o _Make/Stream.o
	
clean:
	rm -rf _Make/*.o _Make/*.a