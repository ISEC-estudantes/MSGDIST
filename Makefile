CC=gcc
CFLAGS=-Wall --std=c11

all: debug

debug:clean gestor verificador cliente



verificador: verificador.c
	$(CC) $(CFLAGS) verificador.c -o verificador -g


cliente: cliente.o
	 $(CC) $(CFLAGS) cliente.o -o cliente -g 

cliente.o: cliente.c
	$(CC) $(CFLAGS) -c cliente.c -g 


gestor.o: gestor.c
	$(CC) $(CFLAGS) -c gestor.c -o gestor -o gestor.o -g 

verificar.o: verificar.c
	$(CC) $(CFLAGS) -c verificar.c -o verificar.o -g 

utils.o: utils.c
	$(CC) $(CFLAGS) -c utils.c -g

cmd.o: cmd.c
	$(CC) $(CFLAGS) -c cmd.c -g 

gestor: gestor.o verificar.o utils.o cmd.o
	$(CC) $(CFLAGS) gestor.o verificar.o utils.o cmd.o -o gestor -g 


stable:clean verificador.c cliente.c gestor.c verificar.c utils.c cmd.c
	$(CC) $(CFLAGS) verificador.c -o verificador 
	$(CC) $(CFLAGS) cliente.c -o cliente
	$(CC) $(CFLAGS) gestor.c verificar.c utils.c cmd.c -o gestor


clean:
	rm -vfr *~ msgdist
