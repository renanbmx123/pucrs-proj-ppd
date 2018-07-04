#include <stdio.h>
#include "mpi.h"
#include <stdlib.h>
#include <string.h>

#define VETOR_SIZE 1000000
#define TAM_TROCA ((int)(VETOR_SIZE*0.015))
#define VERDADEIRO 1
#define FALSO 0

void print_line(int c, int* line){
         int i;
         for(i = 0 ; i < c ; i++)
                 printf("%d ", line[i]);
         printf("\n");
}

void bubble_sort (int *a, int n) {
    int i, t, j = n, s = 1;
    while (s) {
        s = 0;
        for (i = 1; i < j; i++) {
            if (a[i] < a[i - 1]) {
                t = a[i];
                a[i] = a[i - 1];
                a[i - 1] = t;
                s = 1;
            }
        }
        j--;
    }
}

int verifica_pronto(int *v, int tam){
    int i;
    for(i = 0; i < tam; i++){
        if(v[i] == 0) return FALSO;
    }
    return VERDADEIRO;
}

void main(int argc, char** argv){
  int i, tam_vetor, comp;
  int my_rank, rank_pai;       // Identificador deste processo
  int proc_n;        // Numero de processos disparados pelo usuario na linha de comando (np)  
  int message;       // Buffer para as mensagens
  MPI_Status status; // estrutura que guarda o estado de retorno          
  double t1, t2;
  int pronto;

  MPI_Init(&argc , &argv); // funcao que inicializa o MPI, todo o codigo paralelo esta abaixo

  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); // pega o numero do processo atual (rank)
  MPI_Comm_size(MPI_COMM_WORLD, &proc_n);

  tam_vetor = VETOR_SIZE/proc_n;	//tamanho do vetor local
  if(my_rank == 0) t1 = MPI_Wtime();
  int vet_local[tam_vetor + TAM_TROCA];	//vetor local com espaços a mais para troca
  for(i = 0; i < tam_vetor;i++ )
    vet_local[i] = VETOR_SIZE - tam_vetor * my_rank - i; //pior caso, ordem decrescente
  int vet_pronto[proc_n];	//vetor com estados de pronto dos processos
  vet_pronto[0] = 1;
  for(i = 1 ; i < proc_n ; i++)
    vet_pronto[i] = 0;		//todas posicoes com 0 exceto a primeira
  pronto = FALSO;
  while ( !pronto ){
    // ordeno vetor local
    bubble_sort(vet_local, tam_vetor);
    // verifico condição de parada
      // se nao for np-1, mando o meu maior elemento para a direita
    if(my_rank != (proc_n - 1)){
        MPI_Send(&vet_local[tam_vetor - 1], 1, MPI_INT, my_rank + 1, 0, MPI_COMM_WORLD);
    }
      // se nao for 0, recebo o maior elemento da esquerda
     if(my_rank != 0){
        MPI_Recv(&comp, 1, MPI_INT, my_rank - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      // comparo se o meu menor elemento eh maior do que o maior elemento recebido (se sim, estou ordenado em relação ao meu vizinho)
        if(comp < vet_local[0]){
                vet_pronto[my_rank] = 1;
        }
    }
      // compartilho o meu estado com todos os processos
    for(i = 0; i < proc_n; i++){
        MPI_Bcast(&vet_pronto[i], 1, MPI_INT, i, MPI_COMM_WORLD);
    }
      // se todos estiverem ordenados com seus vizinhos, a ordenaçao do vetor global está pronta ( pronto = TRUE, break)
    pronto = verifica_pronto(vet_pronto, proc_n);
    if(pronto) break;
      // senao continuo
    // troco valores para convergir
      //usa as posicoes a mais no vet_local pra receber, ordena com a parte maior, devolve os maiores dps
      // se nao for o 0, mando os menores valores do meu vetor para a esquerda
    if(my_rank != 0)
        MPI_Send(&vet_local[0], TAM_TROCA, MPI_INT, my_rank - 1, 0, MPI_COMM_WORLD);
      // se nao for np-1, recebo os menores valores da direita
    if(my_rank != (proc_n - 1)){
        MPI_Recv(&vet_local[tam_vetor], TAM_TROCA, MPI_INT, my_rank+1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
         // ordeno estes valores com a parte mais alta do meu vetor local
        bubble_sort(&vet_local[tam_vetor-TAM_TROCA], 2*TAM_TROCA);
         // devolvo os valores que recebi para a direita
        MPI_Send(&vet_local[tam_vetor], TAM_TROCA, MPI_INT, my_rank+1, 0, MPI_COMM_WORLD);
    }
      // se nao for o 0, recebo de volta os maiores valores da esquerda
    if(my_rank!=0)
      MPI_Recv(&vet_local[0], TAM_TROCA, MPI_INT, my_rank - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
   }
  if(my_rank==0){
    t2 = MPI_Wtime();
    printf("tempo: %f\n", t2-t1);
  }
  //printf("PROCESSO: %d, first: %d, last: %d\n", my_rank, vet_local[0], vet_local[tam_vetor-1]);
  MPI_Finalize();
}                                      
