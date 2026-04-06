#Este archivo de make es para el programa Elkubo.c

appKubo.exe: appKubo.o funGTKkubo.o funCkubo.o
	gcc appKubo.o funGTKkubo.o funCkubo.o -o appKubo.exe `pkg-config --cflags --libs gtk+-2.0`

appKubo.o: appKubo.c
	gcc -c appKubo.c `pkg-config --cflags --libs gtk+-2.0`

funGTKkubo.o: funGTKkubo.c
	gcc -c funGTKkubo.c `pkg-config --cflags --libs gtk+-2.0`
	
funCkubo.o: funCkubo.c
	gcc -c funCkubo.c `pkg-config --cflags --libs gtk+-2.0`