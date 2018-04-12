all:		servidor cliente

cliente:	cliente.c const.h
		@gcc -o cliente cliente.c

servidor:	servidor.c const.h
		@gcc -o servidor servidor.c

clean:
		@rm -f cliente servidor *~

info:
		@echo "(c) Roland Teodorowitsch (30 ago. 2013 - 31 mar. 2017)"
