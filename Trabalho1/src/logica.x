struct conta {
	int ID;
	float Saldo; };
struct transacao {
	conta conta_cliente;
	int codigo;
	double valor; };
program PROG {
	version VERSAO {
	int solicita_codigo() = 1;
	int solicita_abertura(transacao) = 2;
	int solicita_autenticacao(transacao) = 3;
	int solicita_fechamento(transacao) = 4;
	int solicita_deposito(transacao) = 5;
	int solicita_retirada(transacao) = 6;
	int solicita_consulta(transacao) = 7;
	} = 100;
} = 1048576;
