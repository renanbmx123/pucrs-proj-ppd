#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mpi.h"

enum {OPERACIONAL, CADASTRANDO_CANDIDATOS, ESPERANDO_ELEICAO} estado;

void imprimir_candidatos(int, int *);
void solicitar_eleicao(int, int*, int);
void cadastrar_candidato(int, int*);
int escolher_candidato(int*, int);

//DETECTAR PERDA
//SOLICITAR ELEICAO
//CADASTRAR CANDIDATO

main(int argc, char** argv)
  {
  int ID;       // Identificador deste processo
  int proc_n;        // Numero de processos disparados pelo usuario na linha de comando (np)  
  int* candidatos;
  int coordenador;            
  int ID_detecta_falha;
  MPI_Status status; // estrutura que guarda o estado de retorno          

    /*if(argc < 3){
        printf("Usage: ./%s <processo que detecta a falha> <coordenador>\n", argv[0]);
        exit(0);
    }*/    

  MPI_Init(&argc , &argv); // funcao que inicializa o MPI, todo o codigo paralelo estah abaixo

  MPI_Comm_rank(MPI_COMM_WORLD, &ID); // pega pega o numero do processo atual (rank)
  MPI_Comm_size(MPI_COMM_WORLD, &proc_n);  // pega informacao do numero de processos (quantidade total)
{

  //Inicializa vetor de candidatos
  candidatos = malloc(sizeof(int) * (proc_n + 1));
  memset(candidatos, -1, proc_n + 1);
    
  if(ID == 0){
    srand(time(NULL));
    ID_detecta_falha = rand() % proc_n;
    printf("Processo [%2d] detecta a falha.\n", ID_detecta_falha);
    MPI_Send(&ID_detecta_falha, 1, MPI_INT, 1, 1, MPI_COMM_WORLD);
  }else if(ID < (proc_n - 1)){
    MPI_Recv(&ID_detecta_falha, 1, MPI_INT, ID - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    MPI_Send(&ID_detecta_falha, 1, MPI_INT, ID + 1, 1, MPI_COMM_WORLD);
    if(ID == ID_detecta_falha){
        MPI_Recv(&ID_detecta_falha, 1, MPI_INT, proc_n - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    }
  }else{
        MPI_Recv(&ID_detecta_falha, 1, MPI_INT, ID - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    MPI_Send(&ID_detecta_falha, 1, MPI_INT, ID_detecta_falha, 1, MPI_COMM_WORLD);
  }

  //Solicita eleicao se detecta desconexao com o coordenador
  if(ID == ID_detecta_falha){
    printf("Processo %d solicitando eleicao!\n", ID);              
    
    solicitar_eleicao(ID, candidatos, proc_n + 1);
    cadastrar_candidato(ID, candidatos);

    //Envia adiante o pedido de eleição
    MPI_Send(candidatos, proc_n + 1, MPI_INT, ((ID + 1) % proc_n), 1, MPI_COMM_WORLD); 
    //Aguarda conclusão do cadastramento dos candidatos
    printf("[%d] Esperando receber de [%d]\n", ID, (ID + proc_n - 1) % proc_n);
    MPI_Recv(candidatos, proc_n + 1, MPI_INT, (ID + proc_n - 1) % proc_n, MPI_ANY_TAG, MPI_COMM_WORLD, &status);//E se for o primeiro?
    printf("Recebido!\n"); 
    //Realiza eleição
    coordenador = escolher_candidato(candidatos, proc_n + 1);
    printf("Eleito como coordenador o processo %d!\n", coordenador);
    //Propaga a escolha do coordenador eleito
    MPI_Send(&coordenador, 1, MPI_INT, ((ID + 1) % proc_n), 1, MPI_COMM_WORLD);
  }else{
    //Espera pedido de eleição
    MPI_Recv(candidatos, proc_n + 1, MPI_INT, (ID + proc_n - 1) % proc_n, MPI_ANY_TAG, MPI_COMM_WORLD, &status);//E se for o primeiro?
    cadastrar_candidato(ID, candidatos);
    MPI_Send(candidatos, proc_n + 1, MPI_INT, ((ID + 1) % proc_n), 1, MPI_COMM_WORLD);
    //Espera escolha do novo coordenador
    MPI_Recv(&coordenador, 1, MPI_INT, (ID + proc_n - 1) % proc_n, MPI_ANY_TAG, MPI_COMM_WORLD, &status);//E se for o primeiro?
    printf("[%2d] Reconheceu [%2d] como novo coordenador!\n", ID, coordenador);
    MPI_Send(&coordenador, 1, MPI_INT, ((ID + 1) % proc_n), 1, MPI_COMM_WORLD);
    }
}
  MPI_Finalize();
  free(candidatos);
}

void solicitar_eleicao(int ID, int* candidatos, int n){
  memset(candidatos, -1, n);
  candidatos[0] = 1;
  //printf("SE ");
  //imprimir_candidatos(ID, candidatos);
}

void cadastrar_candidato(int ID, int* candidatos){
  int i = candidatos[0];
  candidatos[i] = ID;
  candidatos[0] = ++i;
  //printf("CC ");
  //imprimir_candidatos(ID, candidatos);
}

int escolher_candidato(int* candidatos, int n){
  int i, melhor_candidato = candidatos[1];
  for(i = 1; i < n; i++){
    melhor_candidato = (melhor_candidato > candidatos[i]) ? melhor_candidato : candidatos[i];
  }
  return melhor_candidato;
}

void imprimir_candidatos(int ID, int* candidatos){
    int i, n = candidatos[0];

    printf("ID=[%2d] N=[%2d] ", ID, n);
    for(i = 1; i < n; i++){
        printf("%2d ", candidatos[i]);
    }
    printf("\n");
}
