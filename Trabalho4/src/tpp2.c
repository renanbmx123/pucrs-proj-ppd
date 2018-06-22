#include <stdio.h>
#include "mpi.h"
#include <stdlib.h>
#include <string.h>

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

int *interleaving3(int vetor[], int tam, int offset1, int offset2, int offset3){
  int *vetor_auxiliar;
  int i1, i2, i3, i_aux;
  vetor_auxiliar = (int *)malloc(sizeof(int) * tam);
  i1 = offset1;
  i2 = offset2;
  i3 = offset3;
  for (i_aux = 0; i_aux < tam; i_aux++) {
    if (((vetor[i1] <= vetor[i2]) && (i1 < i2) && (vetor[i1] <= vetor[i3])) || ((i2 == offset3) && (i3 == tam))){
      vetor_auxiliar[i_aux] = vetor[i1++];
    }else if (((vetor[i2] <= vetor[i1]) && (i2 < i3) && (vetor[i2] <= vetor[i3])) || ((i1 == offset2) && (i3 == tam))){
      vetor_auxiliar[i_aux] = vetor[i2++];
    }else
      if (i3 != tam){
        vetor_auxiliar[i_aux] = vetor[i3++];
      }else{
        if (((vetor[i1] <= vetor[i2]) && (i1 < i2)))
          vetor_auxiliar[i_aux] = vetor[i1++];
        else
          vetor_auxiliar[i_aux] = vetor[i2++];
      }
  }
  return vetor_auxiliar;
}

int *interleaving(int vetor[], int tam)
{
        int *vetor_auxiliar;
        int i1, i2, i_aux;

        vetor_auxiliar = (int *)malloc(sizeof(int) * tam);

        i1 = 0;
        i2 = tam / 2;

        for (i_aux = 0; i_aux < tam; i_aux++) {
                if (((vetor[i1] <= vetor[i2]) && (i1 < (tam / 2)))
                    || (i2 == tam))
                        vetor_auxiliar[i_aux] = vetor[i1++];
                else
                        vetor_auxiliar[i_aux] = vetor[i2++];
        }

        return vetor_auxiliar;
}

int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

#define VETOR_SIZE 1000000
#define delta 70000
#define LOCAL 1

/*              7       15      31      63      127     255
normal          250000  125000  62500   31250   15625   8000    
local           150000  70000   34000   16000   8000    4000
*/

void main(int argc, char** argv){
  int i, tam_vetor, tam_local, tam_divide;
  int my_rank, rank_pai;       // Identificador deste processo
  int proc_n;        // Numero de processos disparados pelo usuario na linha de comando (np)  
  int message;       // Buffer para as mensagens
  MPI_Status status; // estrutura que guarda o estado de retorno          
  int (*vetor) = malloc(VETOR_SIZE * sizeof *vetor);
  double t1, t2;

  MPI_Init(&argc , &argv); // funcao que inicializa o MPI, todo o codigo paralelo esta abaixo

  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); // pega o numero do processo atual (rank)
  MPI_Comm_size(MPI_COMM_WORLD, &proc_n);

  if ( my_rank != 0 ){
        MPI_Recv(vetor, VETOR_SIZE, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);  // não sou a raiz, tenho pai
        rank_pai = status.MPI_SOURCE;
        MPI_Get_count(&status, MPI_INT, &tam_vetor);  // descubro tamanho da mensagem recebida
  }
  else{
        t1 = MPI_Wtime();
        tam_vetor = VETOR_SIZE;       // defino tamanho inicial do vetor
        for(i = 0 ; i < tam_vetor ; i++)      // sou a raiz e portanto gero o vetor - ordem reversa
                vetor[i] = VETOR_SIZE - i;
  }
  if( tam_vetor <= delta ){
        //qsort(vetor, tam_vetor, sizeof(int), cmpfunc);
        bubble_sort(vetor, tam_vetor);
  }
   else{
        float tam_filhos;
        if(LOCAL){      //Se for local, ordena 40% do vetor e divide 60%
                tam_local = delta;
                tam_divide = tam_vetor-delta;
        }else{          //Se nao for local, manda todo
                tam_local = 0;
                tam_divide = tam_vetor;
        }
        tam_filhos = tam_divide/2.0;
        if(tam_filhos != (int)(tam_filhos+0.5)){		//se for um tamanho de vetor com casa decimal, manda tam_vetor/2 e tam_vetor/2+1
                MPI_Send(&vetor[tam_local], (int)(tam_divide/2), MPI_INT, 2*my_rank+1, 0, MPI_COMM_WORLD);  // mando metade inicial do vetor
                MPI_Send(&vetor[tam_local + (int)(tam_divide/2)], (int)(tam_divide/2+1), MPI_INT, 2*my_rank+2, 0, MPI_COMM_WORLD);  // mando metade final 
        }
        else{							//se não for um tamanho de vetor com casa decimal
                MPI_Send(&vetor[tam_local], tam_divide/2, MPI_INT, 2*my_rank+1, 0, MPI_COMM_WORLD);  // mando metade inicial do vetor
                MPI_Send(&vetor[tam_local + tam_divide/2], tam_divide/2, MPI_INT, 2*my_rank+2, 0, MPI_COMM_WORLD);  // mando metade final 
        }
        if(LOCAL){
                //qsort(vetor, tam_local, sizeof(int), cmpfunc);
                bubble_sort(vetor, tam_local);
        }
        if(tam_filhos != (int)(tam_filhos+0.5)){		//se for um tamanho de vetor com casa decimal, manda tam_vetor/2 e tam_vetor/2+1
                MPI_Recv(&vetor[tam_local], (int)(tam_divide/2), MPI_INT, 2*my_rank+1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);   //recebo metade inicial             
                MPI_Recv(&vetor[tam_local + (int)(tam_divide/2)], (int)(tam_divide/2+1), MPI_INT, 2*my_rank+2, MPI_ANY_TAG, MPI_COMM_WORLD, &status); //recebo metade final
        }
        else{							//se não for um tamanho de vetor com casa decimal
                MPI_Recv(&vetor[tam_local], tam_divide/2, MPI_INT, 2*my_rank+1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);  //recebo metade inicial             
                MPI_Recv(&vetor[tam_local + tam_divide/2], tam_divide/2, MPI_INT, 2*my_rank+2, MPI_ANY_TAG, MPI_COMM_WORLD, &status); //recebo metade final
        }
        if(LOCAL) vetor = interleaving3(&vetor[0], tam_vetor, 0, tam_local, tam_local + tam_divide/2); //ordena para local
        else vetor = interleaving(vetor, tam_vetor);    //ordena para 2 vetores
  }
  if ( my_rank !=0 ){
        MPI_Send(vetor, tam_vetor, MPI_INT, rank_pai, 0, MPI_COMM_WORLD);  // tenho pai, retorno vetor ordenado pra ele
  }
  else  {
        t2 = MPI_Wtime();
        printf("Tempo de execucao: %f\n", t2-t1);
        //print_line(VETOR_SIZE, vetor); 
  }
  MPI_Finalize();
}


