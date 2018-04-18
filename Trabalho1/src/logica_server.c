/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "logica.h"
#include "list.h"

List contas;
List codigos;
int FAIL_FLAG = FALSE;

int valida_codigo(int codigo){
	int result = 0;
	//Percorre a lista 'codigos' procurando 'codigo'
	Node *n = codigos->head;
	while(n != NULL){
		if(*((int*)(n->value)) == codigo){
			result = 1;
			break;
		}
		n = n->next;
	}

	return result;
}

int desativa_codigo(int codigo){
	int result = 0;

	Node *n = codigos->head;
	while(n != NULL){
		if(*((int*)(n->value)) == codigo){
			free(n->value);
			n->value == NULL;
			result = 1;
			break;
		}
		n = n->next;
	}

	return result;
}

int *
solicita_codigo_100_svc(void *argp, struct svc_req *rqstp)
{
	static int  result;
	static int codigo = 0;

	list_push(codigos, &codigo, sizeof(codigo));

	result = codigo++;

	return &result;
}

int *
solicita_abertura_100_svc(int *argp, struct svc_req *rqstp)
{
	static int  result;
	result = -1;

	if(valida_codigo(*argp)){

	}else{
		printf("NÃO FOI POSSÍVEL VALIDAR O CÓDIGO %d\n", *argp);
	}
	if(!desativa_codigo(*argp)){
		printf("NÃO FOI POSSÍVEL DESATIVAR O CÓDIGO %d\n", *argp);
	}
	if(FAIL_FLAG){ result = NULL; }
	return &result;
}

int *
solicita_autenticacao_100_svc(transacao *argp, struct svc_req *rqstp)
{
	static int  result;
	result = 0;

	if(valida_codigo(argp->codigo)){
		Node *n = contas->head;
		while(n != NULL){
			conta *conta_cliente = (conta*)(n->value);
			if(conta_cliente != NULL){
				if(conta_cliente->ID == argp->ID){
					result = 1;
					break;
				}
			}
			n = n->next;
		}
		if(!result){
			printf("NÃO FOI POSSÍVEL AUTENTICAR A CONTA %d\n", argp->ID);
		}
	}else{
		printf("NÃO FOI POSSÍVEL VALIDAR O CÓDIGO %d\n", argp->codigo);
	}
	if(!desativa_codigo(argp->codigo)){
		printf("NÃO FOI POSSÍVEL DESATIVAR O CÓDIGO %d\n", argp->codigo);
	}
	if(FAIL_FLAG){ result = NULL; }
	return &result;
}

int *
solicita_fechamento_100_svc(transacao *argp, struct svc_req *rqstp)
{
	static int  result;
	result = 0;

	if(valida_codigo(argp->codigo)){

	}else{
		printf("NÃO FOI POSSÍVEL VALIDAR O CÓDIGO %d\n", argp->codigo);
	}
	if(!desativa_codigo(argp->codigo)){
		printf("NÃO FOI POSSÍVEL DESATIVAR O CÓDIGO %d\n", argp->codigo);
	}
	if(FAIL_FLAG){ result = NULL; }
	return &result;
}

int *
solicita_deposito_100_svc(transacao *argp, struct svc_req *rqstp)
{
	static int  result;
	result = 0;

	if(valida_codigo(argp->codigo)){
		Node *n = contas->head;
		while(n != NULL){
			conta *conta_cliente = (conta*)(n->value);
			if(conta_cliente != NULL){
				if(conta_cliente->ID == argp->ID){
					conta_cliente->Saldo += argp->valor;
					result = 1;
					break;
				}
			}
			n = n->next;
		}
		if(!result){
			printf("NÃO FOI POSSÍVEL DEPOSITAR %.2f NA CONTA %d\n", argp->valor, argp->ID);
		}
	}else{
		printf("NÃO FOI POSSÍVEL VALIDAR O CÓDIGO %d\n", argp->codigo);
	}
	if(!desativa_codigo(argp->codigo)){
		printf("NÃO FOI POSSÍVEL DESATIVAR O CÓDIGO %d\n", argp->codigo);
	}
	if(FAIL_FLAG){ result = NULL; }
	return &result;
}

int *
solicita_retirada_100_svc(transacao *argp, struct svc_req *rqstp)
{
	static int  result;
	result = 0;

	if(valida_codigo(argp->codigo)){
		Node *n = contas->head;
		while(n != NULL){
			conta *conta_cliente = (conta*)(n->value);
			if(conta_cliente != NULL){
				if(conta_cliente->ID == argp->ID){
					conta_cliente->Saldo -= argp->valor;
					result = 1;
					break;
				}
			}
			n = n->next;
		}
		if(!result){
			printf("NÃO FOI POSSÍVEL RETIRAR %.2f DA CONTA %d\n", argp->valor, argp->ID);
		}
	}else{
		printf("NÃO FOI POSSÍVEL VALIDAR O CÓDIGO %d\n", argp->codigo);
	}
	if(!desativa_codigo(argp->codigo)){
		printf("NÃO FOI POSSÍVEL DESATIVAR O CÓDIGO %d\n", argp->codigo);
	}
	if(FAIL_FLAG){ result = NULL; }
	return &result;
}

//FEITO
//OK?
conta *
solicita_consulta_100_svc(transacao *argp, struct svc_req *rqstp)
{
	static conta result;
	conta conta_vazia;
	result = conta_vazia;

	if(valida_codigo(argp->codigo)){
		int sucesso = 0;
		Node *n = contas->head;
		while(n != NULL){
			conta *conta_cliente = (conta*)(n->value);
			if(conta_cliente != NULL){
				if(conta_cliente->ID == argp->ID){
					result = *conta_cliente;
					sucesso = 1;
					break;
				}
			}
			n = n->next;
		}
		if(!sucesso){
			printf("NÃO FOI POSSÍVEL CONSULTAR A CONTA %d\n", argp->ID);
		}
	}else{
		printf("NÃO FOI POSSÍVEL VALIDAR O CÓDIGO %d\n", argp->codigo);
	}
	if(!desativa_codigo(argp->codigo)){
		printf("NÃO FOI POSSÍVEL DESATIVAR O CÓDIGO %d\n", argp->codigo);
	}
	if(FAIL_FLAG){ result = NULL; }
	return &result;
}
