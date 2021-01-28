program: derle calistir

derle:
	g++ -I ./include/ -o ./lib/Pizza.o -c ./src/Pizza.cpp
	g++ -I ./include/ -o ./lib/Dusman.o -c ./src/Dusman.cpp
	g++ -I ./include/ -o ./lib/Kontrol.o -c ./src/Kontrol.cpp
	g++ -I ./include/ -o ./bin/program ./lib/Pizza.o ./lib/Dusman.o ./lib/Kontrol.o ./src/program.cpp

calistir:
	./bin/program