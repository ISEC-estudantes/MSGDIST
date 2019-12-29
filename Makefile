GESTOROBS = ./project/gestor.o ./project/verificar.o ./project/utils.o ./project/comand.o ./project/gfrt.o ./project/cltusr.c
GESTOROUT = gestor

CLIENTEOBS = ./project/cliente.o ./project/utils.o ./project/cltusr.c
CLIENTEOUT = cliente

CFLAGS += -lncurses -lpthread

default: debug

all: $(GESTOROUT)  $(CLIENTEOUT) verificador

verificador: ./project/verificador.c
	$(CC) ./project/verificador.c -o verificador $(CFLAGS)

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
	rm -f ./project/*.o $(CLIENTEOUT) $(GESTOROUT) verificador
	find . -type p -delete
