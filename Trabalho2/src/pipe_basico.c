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

  MPI_Init(&argc , &argv); // funcao que inicializa o MPI, todo o codigo paralelo estah abaixo

  MPI_Comm_rank(MPI_COMM_WORLD, &ID); // pega pega o numero do processo atual (rank)
  MPI_Comm_size(MPI_COMM_WORLD, &proc_n);  // pega informacao do numero de processos (quantidade total)
{
  //Inicializa vetor de candidatos
  candidatos = malloc(sizeof(int) * (proc_n + 1));
  memset(candidatos, -1, proc_n + 1);

  //Solicita eleicao se detecta desconexao com o coordenador
  if(/* testar falha */){
    solicitar_eleicao(ID, candidatos, proc_n + 1);
    cadastrar_candidato(ID, candidatos);

    MPI_Send(candidatos, proc_n + 1, MPI_INT, ID + 1, 1, MPI_COMM_WORLD); //E se for o ultimo?
    MPI_Recv(candidatos, proc_n + 1, MPI_INT, ID - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);//E se for o primeiro?
    
    coordenador = escolher_candidato(candidatos, proc_n + 1);
    MPI_Send(&coordenador, 1, MPI_INT, ID + 1, 1, MPI_COMM_WORLD); //E se for o ultimo?
  }

  //Espera pedido de eleição
  MPI_Recv(candidatos, proc_n + 1, MPI_INT, ID - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);//E se for o primeiro?
  cadastrar_candidato(ID, candidatos);
  MPI_Send(candidatos, proc_n + 1, MPI_INT, ID + 1, 1, MPI_COMM_WORLD);//E se for o ultimo?
  //Espera escolha do novo coordenador
  MPI_Recv(&coordenador, 1, MPI_INT, ID - 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);//E se for o primeiro?
}



  // receber da esquerda

  if ( ID == 0 ) // sou o primeiro?
     message = 5;     // sim, sou o primeiro, crio a mensagem sem receber
  else
     MPI_Recv(&message, 1, MPI_INT, ID-1, MPI_ANY_TAG, MPI_COMM_WORLD, &status); // recebo da esquerda
     
  // processo mensagem

  printf("Pid: %d, mensagem %d\n", ID, message); // mostro mensagem na tela 
  message +=1; // incremento um na mensagem recebida

  // enviar para a direita

  if ( ID == proc_n-1 ) // sou o utlimo?
     printf("Pid: %d, sou o ultimo!\n", ID); // mostro mensagem na tela pois sou o ultimo
  else
     MPI_Send(&message, 1, MPI_INT, ID+1, 1, MPI_COMM_WORLD); // envio para a direita

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