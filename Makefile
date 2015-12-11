gunshot : GPS.o client.o util.o impulse.o libportaudio.a
	gcc -lrt -lasound -lpthread -lm -o gunshot impulse.o GPS.o client.o util.o libportaudio.a

impulse.o : impulse.c
	gcc -c impulse.c

GPS.o : GPS.c GPS.h
	gcc -c GPS.c

client.o : client.c client.h
	gcc -c client.c

util.o : util.c util.h
	gcc -c util.c

clean :
	rm gunshot impulse.o client.o GPS.o util.o

