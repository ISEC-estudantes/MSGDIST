GESTOROBS = ./project/gestor.o ./project/verificar.o ./project/utils.o ./project/comand.o ./project/gfrt.o ./project/cltusr.o ./project/mensagem.o 
GESTOROUT = gestor

CLIENTEOBS = ./project/cliente.o ./project/utils.o ./project/cltusr.o  ./project/editor.o  ./project/cfrt.o ./project/mensagem.o

CLIENTEOUT = cliente

CFLAGS += -lncurses -lpthread

default: debug

all: $(GESTOROUT)  $(CLIENTEOUT) verificador

verificador: ./project/verificador.c
	$(CC) ./project/verificador.c -o verificador $(CFLAGS)

cliente:$(CLIENTEOBS)
	$(CC) $(CLIENTEOBS) -o $(CLIENTEOUT) $(CFLAGS)

gestor:$(GESTOROBS)
	$(CC) $(GESTOROBS) -o $(GESTOROUT) $(CFLAGS)

testncurses: ./project/testncurses.c ./project/utils.o ./project/editor.o 



	$(CC) ./project/testncurses.c ./project/utils.o ./project/editor.o -o testncurses -lncurses -g

%.o:%.c
	$(CC) -o $@ -c $< $(CFLAGS)


debug: CFLAGS += -Wall -g -DDEBUG
	#-fsanitize=address
debug: all


clear:
	rm -f ./project/*.o $(CLIENTEOUT) $(GESTOROUT) verificador
	find . -type p -delete
