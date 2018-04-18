/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "logica.h"
#include "list.h"

#define CRED     "\x1b[31m"
#define CGREEN   "\x1b[32m"
#define CYELLOW  "\x1b[33m"
#define CBLUE    "\x1b[34m"
#define CMAGENTA "\x1b[35m"
#define CCYAN    "\x1b[36m"
#define CRESET   "\x1b[0m"

List contas;
List codigos;
int FAIL_FLAG = FALSE;

int valida_codigo(int codigo){
	int result = 0;

	Node *n = codigos.head;
	while(n != NULL){
		if(*((int*)(n->value)) == codigo){
			printf("CGREEN[CODIGO %02d VALIDADO]CRESET\n", codigo);
			result = 1;
			break;
		}
		n = n->next;
	}

	return result;
}

//Complicado
int desativa_codigo(int codigo){
	Node *n = codigos.head;
	if(list_size(&codigos)==1){
		codigos.head = NULL;
		printf(CGREEN);
		printf("[CODIGO %02d DESATIVADO]", codigo);
		puts(CRESET);
		return 1;
	}
	else if (list_size(&codigos) == 2)
	{
		codigos.head = codigos.head->next;
		printf("CGREEN[CODIGO %02d DESATIVADO]CRESET\n", codigo);
		return 1;
	}
	while(n->next != NULL){
		Node *aux = n->next;
		if(*((int*)(aux->value)) == codigo){
			n->next = aux->next;
			free(aux);
			printf("CGREEN[CODIGO %02d DESATIVADO]CRESET\n", codigo);
			return 1;
			break;
		}
		n = n->next;
	}
	return 0;
}

conta *procura_conta(int ID){
	conta *conta_cliente = NULL;
	Node *n = contas.head;
	while (n != NULL)
	{
		conta_cliente = (conta *)(n->value);
		if (conta_cliente != NULL)
		{
			if (conta_cliente->ID == ID)
			{
				break;
			}
		}
		n = n->next;
	}
	return conta_cliente;
}

int *
solicita_codigo_100_svc(void *argp, struct svc_req *rqstp)
{
	static int  result;
	static int codigo = 0;

	list_push(&codigos, &codigo, sizeof(codigo));
	printf("CYELLOW[CODIGO %02d SOLICITADO]CRESET\n", codigo);
	result = codigo++;

	return &result;
}

int *
solicita_abertura_100_svc(int *argp, struct svc_req *rqstp)
{
	static int  result;
	result = 0;

	if(valida_codigo(*argp)){
		conta nova_conta;
		if(contas.tail == NULL){
			nova_conta.ID = 1;
		}
		else{
			conta *ultima_conta = (conta *)(contas.tail);
			nova_conta.ID = ultima_conta->ID + 1;
		}
		nova_conta.Saldo = 0;
		list_push(&contas, (void *)&nova_conta, sizeof(nova_conta));
		result = nova_conta.ID;
	}
	else
	{
		printf("CRED[NÃO FOI POSSÍVEL VALIDAR O CÓDIGO %02d]CRESET\n", *argp);
	}
	if(!desativa_codigo(*argp)){
		printf("CRED[NÃO FOI POSSÍVEL DESATIVAR O CÓDIGO %02d]CRESET\n", *argp);
	}
	if(FAIL_FLAG){ return NULL; }
	return &result;
}

int *
solicita_autenticacao_100_svc(transacao *argp, struct svc_req *rqstp)
{
	static int  result;
	result = 0;

	if(valida_codigo(argp->codigo)){
		if(procura_conta(argp->ID) != NULL){
			result = 1;
		}else{
			printf("CRED[NÃO FOI POSSÍVEL AUTENTICAR A CONTA %02d]CRESET\n", argp->ID);
		}
	}else{
		printf("CRED[NÃO FOI POSSÍVEL VALIDAR O CÓDIGO %02d]CRESET\n", argp->codigo);
	}
	if(!desativa_codigo(argp->codigo)){
		printf("CRED[NÃO FOI POSSÍVEL DESATIVAR O CÓDIGO %02d]CRESET\n", argp->codigo);
	}
	if(FAIL_FLAG){ return NULL; }
	return &result;
}

int *
solicita_fechamento_100_svc(transacao *argp, struct svc_req *rqstp)
{
	static int  result;
	result = 0;
	//Funcao mais complicada de todas
	if(valida_codigo(argp->codigo)){
		conta *conta_cliente;
		Node *n = contas.head;
		if(list_size(&contas)==1){
			contas.head = NULL;
		}else if(list_size(&contas)==2){
			contas.head = contas.head->next;
		}
		while(n->next != NULL){
			Node *aux = n->next;
			conta_cliente = (conta *)(aux->value);
			if (conta_cliente != NULL){
				if(conta_cliente->ID == argp->ID){
					n->next = aux->next;
					free(conta_cliente);
					free(aux);
					result = 1;
					break;
				}
			}
		}
		n = n->next;
	}else{
		printf("CRED[NÃO FOI POSSÍVEL VALIDAR O CÓDIGO %02d]CRESET\n", argp->codigo);
	}
	if(!desativa_codigo(argp->codigo)){
		printf("CRED[NÃO FOI POSSÍVEL DESATIVAR O CÓDIGO %02d]CRESET\n", argp->codigo);
	}
	if(FAIL_FLAG){ return NULL; }
	return &result;
}

int *
solicita_deposito_100_svc(transacao *argp, struct svc_req *rqstp)
{
	static int  result;
	result = 0;

	if(valida_codigo(argp->codigo)){
		conta *conta_cliente;
		if((conta_cliente = procura_conta(argp->ID)) != NULL){
			conta_cliente->Saldo += argp->valor;
			result = 1;
		}else{
			printf("CRED[NÃO FOI POSSÍVEL DEPOSITAR %.2f NA CONTA %d\n]CRESET", argp->valor, argp->ID);
		}
	}else{
		printf("CRED[NÃO FOI POSSÍVEL VALIDAR O CÓDIGO %02d]CRESET\n", argp->codigo);
	}
	if(!desativa_codigo(argp->codigo)){
		printf("CRED[NÃO FOI POSSÍVEL DESATIVAR O CÓDIGO %02d]CRESET\n", argp->codigo);
	}
	if(FAIL_FLAG){ return NULL; }
	return &result;
}

int *
solicita_retirada_100_svc(transacao *argp, struct svc_req *rqstp)
{
	static int  result;
	result = 0;

	if(valida_codigo(argp->codigo)){
		conta *conta_cliente;
		if((conta_cliente = procura_conta(argp->ID)) != NULL){
			conta_cliente->Saldo -= argp->valor;
			result = 1;
		}else{
			printf("CRED[NÃO FOI POSSÍVEL RETIRAR %.2f DA CONTA %d]CRESET\n", argp->valor, argp->ID);
		}
	}else{
		printf("CRED[NÃO FOI POSSÍVEL VALIDAR O CÓDIGO %02d]CRESET\n", argp->codigo);
	}
	if(!desativa_codigo(argp->codigo)){
		printf("CRED[NÃO FOI POSSÍVEL DESATIVAR O CÓDIGO %02d]CRESET\n", argp->codigo);
	}
	if(FAIL_FLAG){ return NULL; }
	return &result;
}

conta *
solicita_consulta_100_svc(transacao *argp, struct svc_req *rqstp)
{
	static conta result;
	conta conta_vazia;
	result = conta_vazia;

	if(valida_codigo(argp->codigo)){
		conta *conta_cliente;
		if((conta_cliente = procura_conta(argp->ID)) != NULL){
			result = *conta_cliente;
		}else{
			printf("CRED[NÃO FOI POSSÍVEL CONSULTAR A CONTA %02d]CRESET\n", argp->ID);
		}
	}else{
		printf("CRED[NÃO FOI POSSÍVEL VALIDAR O CÓDIGO %02d]CRESET\n", argp->codigo);
	}
	if(!desativa_codigo(argp->codigo)){
		printf("CRED[NÃO FOI POSSÍVEL DESATIVAR O CÓDIGO %02d]CRESET\n", argp->codigo);
	}
	if(FAIL_FLAG){ return NULL; }
	return &result;
}
