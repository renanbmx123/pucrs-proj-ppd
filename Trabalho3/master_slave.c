#include <stdio.h>
#include "mpi.h"
#include <stdlib.h>
#include <string.h>

int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

void print_line(int c, int* line){
        int i;
        for(i=0;i<c;i++)
                printf("%d ", line[i]);
        printf("\n");
}

main(int argc, char** argv)
  {
  int i, j, next, pos, countKill=0;
  int my_rank;       // Identificador deste processo
  int proc_n;        // Numero de processos disparados pelo usuario na linha de comando (np)           
  double t1, t2;
  MPI_Status status; // estrutura que guarda o estado de retorno          
  int l=10, c=10;
  int (*m)[c] = malloc (l * sizeof *m);
  int (*message) = malloc(c * sizeof *message);
  for(i=0; i<l;i++)
        for(j=0;j<c;j++)
                m[i][j]=c-j;
  MPI_Init(&argc , &argv); // funcao que inicializa o MPI, todo o codigo paralelo esta abaixo

  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); // pega o numero do processo atual (rank)
  MPI_Comm_size(MPI_COMM_WORLD, &proc_n);
  int v[proc_n-1];
  if(my_rank==0){
	t1 = MPI_Wtime();
        for(i=1;i<proc_n;i++){
                v[i-1] = i-1;
                MPI_Send(m[i-1], c, MPI_INT, i, 1, MPI_COMM_WORLD);
        }
        next = proc_n-2;
        while(countKill!=(proc_n-1)){
                MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
                pos = v[status.MPI_SOURCE-1];
                MPI_Recv(m[pos], c, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
                next++;
                if(next < l){
                        v[status.MPI_SOURCE-1] = next;
                        MPI_Send(m[next], c, MPI_INT, status.MPI_SOURCE, 1, MPI_COMM_WORLD);
                }
                else{
                        countKill++;
                        MPI_Send(m[next-1], c, MPI_INT, status.MPI_SOURCE, 0, MPI_COMM_WORLD);
                }
                //for(i=0;i<l;i++){
                //printf("%d: ", i);
                //print_line(c, m[i]);}
        }
        t2 = MPI_Wtime();
        printf("Tempo de execucao: %f\n", t2-t1);
  }
  else{
        do{
                MPI_Recv(message, c, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
                if(status.MPI_TAG!=0){
                        qsort(message, c, sizeof(int), cmpfunc);
                        MPI_Send(message, c, MPI_INT, 0, 1, MPI_COMM_WORLD);
                }
        }while(status.MPI_TAG!=0);
  }
  MPI_Finalize();
  free(m);
}
    
