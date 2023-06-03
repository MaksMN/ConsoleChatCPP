consolechat: miscLib
	g++ -o _Make/consolechat main.cpp DB/*.cpp DB/**/*.cpp Chat/*.cpp Chat/**/*.cpp -L. -l:_Make/miscLib.a

miscLib: Misc/sha1.cpp Misc/DateTime.cpp
	g++ -o _Make/sha1.o Misc/sha1.cpp -c
	g++ -o _Make/DateTime.o Misc/DateTime.cpp -c
	ar rc _Make/miscLib.a _Make/sha1.o _Make/DateTime.o
	
clean:
	rm -rf _Make/*.o _Make/*.a