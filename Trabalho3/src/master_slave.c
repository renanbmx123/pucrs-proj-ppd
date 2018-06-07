#include <stdio.h>
#include "mpi.h"
#include <stdlib.h>
#include <string.h>

#define KILLTAG 1
#define WORKTAG 0 

int cmpfunc (const void *a, const void *b) {
   return ( *(int*)a - *(int*)b );
}

void print_line(int c, int *line){
	int i;
	for(i = 0; i < c; i++)
		printf("%4d ", line[i]);
	printf("\n");
}

main(int argc, char **argv)
{
	int i, j, prox_tarefa, pos;
	int l=1000, c = 100000;
	int ID;       // Identificador deste processo
	int proc_n;        // Numero de processos disparados pelo usuario na linha de comando (np)           
	double timestamp_inicial, timestamp_final;
	MPI_Status status; // estrutura que guarda o estado de retorno          
	int (*m)[c] = malloc (l * sizeof *m);
	int (*message) = malloc(c * sizeof(int));
	int *controle;

	//Inicializa saco
	for(i = 0; i < l; i++)
		for(j = 0; j < c; j++)
			m[i][j]= c - j;

	MPI_Init(&argc , &argv); // funcao que inicializa o MPI, todo o codigo paralelo esta abaixo
	MPI_Comm_rank(MPI_COMM_WORLD, &ID); // pega o numero do processo atual (rank)
	MPI_Comm_size(MPI_COMM_WORLD, &proc_n);
	//Vetor de controle de processos
	controle = calloc(proc_n - 1, sizeof *controle);

	if(ID == 0){
		timestamp_inicial = MPI_Wtime();	
		for(i = 1; i < proc_n; i++){
			controle[i-1] = i; 
			MPI_Send(m[i-1], c, MPI_INT, i, WORKTAG, MPI_COMM_WORLD);
			print_line(proc_n-1, controle);
		}
		prox_tarefa = (proc_n-1)-1;
		while(++prox_tarefa < l){
			MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			pos = controle[status.MPI_SOURCE-1];
			MPI_Recv(m[pos], c, MPI_INT, status.MPI_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		
			controle[status.MPI_SOURCE-1] = prox_tarefa;
			MPI_Send(m[prox_tarefa], c, MPI_INT, status.MPI_SOURCE, WORKTAG, MPI_COMM_WORLD);
			print_line(proc_n-1, controle);
		}
		for(i = 0; i < proc_n - 1; i++){
			if(controle[i] != 0){
				pos = controle[i];
				MPI_Recv(m[pos], c, MPI_INT, i+1, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
				controle[i] = 0;
			}
			MPI_Send(m[0], c, MPI_INT, i+1, KILLTAG, MPI_COMM_WORLD);
		}
		timestamp_final = MPI_Wtime();
		printf("Tempo de execucao: %f\n", timestamp_final - timestamp_inicial);
	}
	else{
		while(1){
			MPI_Recv(message, c, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			if(status.MPI_TAG == KILLTAG) break;
			qsort(message, c, sizeof(int), cmpfunc);
			MPI_Ssend(message, c, MPI_INT, 0, WORKTAG, MPI_COMM_WORLD);
		}
	} 
	MPI_Finalize();
	free(m);
}
