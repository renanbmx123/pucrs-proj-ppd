#include <stdio.h>
#include "mpi.h"
#include <stdlib.h>
#include <string.h>

#define VETOR_SIZE 40
#define TAM_TROCA 2
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

//16    62500
//32    31250

void main(int argc, char** argv){
  int i, tam_vetor, comp;
  int my_rank, rank_pai;       // Identificador deste processo
  int proc_n;        // Numero de processos disparados pelo usuario na linha de comando (np)  
  int message;       // Buffer para as mensagens
  MPI_Status status; // estrutura que guarda o estado de retorno          
  int (*vetor) = malloc(VETOR_SIZE * sizeof *vetor);
  double t1, t2;
  int pronto;
  for(i = 0 ; i < tam_vetor ; i++)
                vetor[i] = VETOR_SIZE - i;

  MPI_Init(&argc , &argv); // funcao que inicializa o MPI, todo o codigo paralelo esta abaixo

  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); // pega o numero do processo atual (rank)
  MPI_Comm_size(MPI_COMM_WORLD, &proc_n);

  tam_vetor = VETOR_SIZE/proc_n;

  int vet_local[proc_n + TAM_TROCA];
  vet_local = &vetor[tam_vetor * my_rank];

  printf("Process: %d, vetor local: ", my_rank); print_line(tam_vetor, vet_local);
  int vet_pronto[proc_n];
  vet_pronto[0] = 1;
  for(i = 1 ; i < proc_n ; i++)
                vet_pronto[i] = 0;

  pronto = FALSO;

  while ( !pronto ){

   // ordeno vetor local

        bubble_sort(vet_local, tam_vetor);
        printf("Process: %d, vetor local: ", my_rank); print_line(tam_vetor, vet_local);
   // verifico condição de parada

      // se não for np-1, mando o meu maior elemento para a direita
        if(my_rank != (proc_n - 1)){
                MPI_Send(&vet_local[tam_vetor - 1], 1, MPI_INT, my_rank + 1, 0, MPI_COMM_WORLD);
        }
      // se não for 0, recebo o maior elemento da esquerda
        if(my_rank != 0){
                MPI_Recv(comp, 1, MPI_INT, my_rank - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        printf("process: %d, compara: %d com %d\n", my_rank, comp, vet_local[0]);
      // comparo se o meu menor elemento é maior do que o maior elemento recebido (se sim, estou ordenado em relação ao meu vizinho)
                if(comp < vet_local[0]){
                        vet_pronto[my_rank] = 1;
                }
        }
      // compartilho o meu estado com todos os processos
        for(i = 0; i < proc_n; i++){
                MPI_Bcast(&vet_pronto[i], 1, MPI_INT, i, MPI_COMM_WORLD);
        }
        printf("Process: %d, vetor pronto: ", my_rank); print_line(proc_n, vet_pronto);
      // se todos estiverem ordenados com seus vizinhos, a ordenação do vetor global está pronta ( pronto = TRUE, break)
        pronto = verifica_pronto(vet_pronto, proc_n);
        if(pronto) break;
      // senão continuo

   // troco valores para convergir


        //usa as posicoes a mais no vet_local pra receber, ordena com a parte maior, devolve os maiores dps

      // se não for o 0, mando os menores valores do meu vetor para a esquerda
        if(my_rank != 0)
                MPI_Send(&vet_local[0], TAM_TROCA, MPI_INT, my_rank - 1, 0, MPI_COMM_WORLD);
      // se não for np-1, recebo os menores valores da direita
        if(my_rank != (proc_n - 1))
                MPI_Recv(&vet_local[tam_vetor], TAM_TROCA, MPI_INT, my_rank+1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
         // ordeno estes valores com a parte mais alta do meu vetor local
        printf("Process: %d, vetor local dps que recebi: ", my_rank); print_line(tam_vetor + TAM_TROCA, vet_local);
        bubble_sort(&vet_local[tam_vetor-TAM_TROCA], 2*TAM_TROCA);
        printf("Process: %d, vetor local dps que ordenei: ", my_rank); print_line(tam_vetor + TAM_TROCA, vet_local);
         // devolvo os valores que recebi para a direita
        if(my_rank!=(proc_n-1))
                MPI_Send(&vet_local[tam_vetor], TAM_TROCA, MPI_INT, my_rank+1, 0, MPI_COMM_WORLD);

      // se não for o 0, recebo de volta os maiores valores da esquerda
        if(my_rank!=0)
                MPI_Recv(&vet_local[0], TAM_TROCA, MPI_INT, my_rank-1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

   }

  MPI_Finalize();
  free(vetor);
}
                                                                                                                                    132,1         Bo