#all: main


main: main.o ini_parser.o  
	g++ -g main.o ini_parser.o -o  main  

ini_parser.o: ini_parser.cc
	g++ -g -c ini_parser.cc

main.o: main.cc
	g++ -g -c main.cc


  
clean:  
	-rm main *.o  
