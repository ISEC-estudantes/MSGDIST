GESTOROBS = gestor.o verificar.o utils.o
GESTOROUT = gestor

CLIENTEOBS = cliente.o
CLIENTEOUT = cliente

all: $(GESTOROUT)  $(CLIENTEOUT)

$(CLIENTEOUT):$(CLIENTEOBS)
	$(CC) $(CLIENTEOBS) -o $(CLIENTEOUT) $(CFLAGS)

$(GESTOROUT):$(GESTOROBS)
	$(CC) $(GESTOROBS) -o $(GESTOROUT) $(CFLAGS)


%.o:%.c
	$(CC) -o $@ -c $< $(CFLAGS)


debug: CFLAGS += -Wall -g
debug: all

clear: *.o $(CLIENTEOUT) $(GESTOROUT)
	rm -f *.o $(CLIENTEOUT) $(GESTOROUT)