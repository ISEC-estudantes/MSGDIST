GESTOROBS = gestor.o verificar.o utils.o comand.o threads.o
GESTOROUT = gestor

CLIENTEOBS = cliente.o
CLIENTEOUT = cliente

CFLAGS += -lncurses -lpthread

default: debug

all: $(GESTOROUT)  $(CLIENTEOUT) verificador

verificador: verificador.c
	$(CC) verificador.c -o verificador $(CFLAGS)

$(CLIENTEOUT):$(CLIENTEOBS)
	$(CC) $(CLIENTEOBS) -o $(CLIENTEOUT) $(CFLAGS)

$(GESTOROUT):$(GESTOROBS)
	$(CC) $(GESTOROBS) -o $(GESTOROUT) $(CFLAGS)


%.o:%.c
	$(CC) -o $@ -c $< $(CFLAGS)


debug: CFLAGS += -Wall -g -DDEBUG
	#-fsanitize=address
debug: all


clear:
	rm -f *.o $(CLIENTEOUT) $(GESTOROUT) verificador
	./rmfifos 1
