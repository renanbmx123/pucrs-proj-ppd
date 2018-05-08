#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

enum {OPERACIONAL, CADASTRANDO_CANDIDATOS, ESPERANDO_ELEICAO} estado;

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
  MPI_Status status; // estrutura que guarda o estado de retorno          

    if(argc < 3){
        printf("Usage: ./%s <processo que detecta a falha> <coordenador>\n", argv[0]);
        exit(0);
    }    

  MPI_Init(&argc , &argv); // funcao que inicializa o MPI, todo o codigo paralelo estah abaixo

  MPI_Comm_rank(MPI_COMM_WORLD, &ID); // pega pega o numero do processo atual (rank)
  MPI_Comm_size(MPI_COMM_WORLD, &proc_n);  // pega informacao do numero de processos (quantidade total)
{
  //Inicializa vetor de candidatos
  candidatos = malloc(sizeof(int) * (proc_n + 1));
  memset(candidatos, -1, proc_n + 1);

  //Solicita eleicao se detecta desconexao com o coordenador
  if(ID == atoi(argv[1]){
    printf("Processo %d solicitando eleicao!", ID);              
    solicitar_eleicao(ID, candidatos, proc_n + 1);
    cadastrar_candidato(ID, candidatos);

    //Envia adiante o pedido de eleição
    MPI_Send(candidatos, proc_n + 1, MPI_INT, ((ID + 1) % proc_n), 1, MPI_COMM_WORLD); 
    //Aguarda conclusão do cadastramento dos candidatos
    MPI_Recv(candidatos, proc_n + 1, MPI_INT, ID - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);//E se for o primeiro?
    
    //Realiza eleição
    coordenador = escolher_candidato(candidatos, proc_n + 1);
    printf("Eleito como coordenador o processo %d!", coordenador);
    //Propaga a escolha do coordenador eleito
    MPI_Send(&coordenador, 1, MPI_INT, ((ID + 1) % proc_n), 1, MPI_COMM_WORLD);
  }

  //Espera pedido de eleição
  MPI_Recv(candidatos, proc_n + 1, MPI_INT, ID - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);//E se for o primeiro?
  cadastrar_candidato(ID, candidatos);
  MPI_Send(candidatos, proc_n + 1, MPI_INT, ((ID + 1) % proc_n), 1, MPI_COMM_WORLD);
  //Espera escolha do novo coordenador
  MPI_Recv(&coordenador, 1, MPI_INT, ID - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);//E se for o primeiro?
}
  MPI_Finalize();
  free(candidatos);
}

void solicitar_eleicao(int ID, int* candidatos, int n){
  memset(candidatos, -1, n);
  candidatos[0] = 1;
}

void cadastrar_candidato(int ID, int* candidatos){
  int i = candidatos[0];
  candidatos[i] = ID;
  candidatos[0] = ++i;
}

int escolher_candidato(int* candidatos, int n){
  int i, melhor_candidato = candidatos[1];
  for(i = 1; i < n; i++){
    melhor_candidato = (melhor_candidato > candidatos[i]) ? melhor_candidato : candidatos[i];
  }
  return melhor_candidato;
}
