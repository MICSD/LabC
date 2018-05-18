#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
#define MAX_SIZE 1000
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

//funções a corrigir
void pedido();

//funções acabadas
void menu(); //menu inicial
void user_pass(); //lê o utilizador e a senha do stdin e usa-os como argumentos para a funçõa login
void login(); //autenticação
void menu_cliente(); //menu de opções dentro da conta

//funções por fazer
void feed();
void topicos();
void topMaisAti();
void subTopico();
void pubTopico();
void gerirListaSub();
void estatisticas();
void gerirConta();



typedef struct {
    char nome[200];
    char senha[200];
} utilizador;

typedef char bool;
#define true 1
#define false 0

bool strIsOnlySpaces(const char* str) {
	size_t n=str?strlen(str):0;
	for(size_t i=0 ; i<n ; i++)
		if(!isspace(str[i] && isprint(str[i])))
			return false;
	return true;
}

void menu() {
	
  printf("\033[22;34m**Menu de Registo**\033[0m\n\033[22;34m1)\033[0m Login/Autenticação\n\033[22;34m2)\033[0m Pedido de registo de novo utilizador\n\033[22;34m3)\033[0m Sair\n");
  int opcao;
  printf("Escolha uma opção: ");
  scanf("%d", &opcao);
  switch(opcao) {
  	case 1:
  		system("clear");
		user_pass();
		break;
	case 2:
		system("clear");
		pedido();
		break;
	case 3:
		system("clear");
		exit(0);
	default:
		system("clear");
		menu();
  }  
}

FILE *f;

void pedido() {
	FILE *pedidos;
	char new_username[30], new_password[20], email[100], data[9];
	pedidos = fopen("Login/Users/pedidos","w+");
	
	printf("\033[22;34mUsername:\033[0m ");
	do {
		fgets(new_username,30,stdin);
	} while(strIsOnlySpaces(new_username)); 
	fgets(new_username,30,stdin);	
	fprintf(pedidos,"%s", new_username);
	
	printf("\033[22;34mPassword:\033[0m ");
	do {
		fgets(new_password,20,stdin);
	} while(strIsOnlySpaces(new_password)); 
	fgets(new_password,20,stdin);
	fprintf(pedidos,"%s", new_password);

	printf("\033[22;34mEmail:\033[0m");
	do {
		fgets(email,100,stdin);
	} while(strIsOnlySpaces(email));
	fprintf(pedidos,"%s", email);
	
	printf("\033[22;34mData de nascimento (DD/MM/AA):\033[0m ");
	do {
		fgets(data,9,stdin);
	} while(strIsOnlySpaces(data));
	fgets(data,9,stdin);
	fprintf(pedidos,"%s", data);

	//ADICIONAR E CRIAR UM DOCUMENTO PARA PEDIDOS COM AS INFORMAÇÕES FORNECIDAS (fwrite)
	printf("\nA carregar... Por favor aguarde.\n");
	usleep(2000000);
	printf(ANSI_COLOR_GREEN "Pedido registado com sucesso! Volte mais tarde.\n" ANSI_COLOR_RESET);
	usleep(2000000);
	system("clear");
	menu();
}

void user_pass() {
	char user[30], pass[30];
    int select3;

    printf("\033[22;34mUtilizador:\033[0m ");
    scanf("%s", user);
    printf("\033[22;34mSenha:\033[0m ");
    scanf("%s", pass);

    login(user, pass);
}

void login(char user[30], char pass[30]) {
	FILE *file;
    char *user2=NULL, *pass2=NULL, *line=NULL;
    file = fopen("Login/Users/utilizadores", "r+");
	if (file) {
		size_t n_u, n_p;
		while(getline(&user2, &n_u, file)!=-1) {
			if(strlen)
			user2[strlen(user2)-1]='\0';	//tira o \n do fim
			if(getline(&pass2, &n_p, file)==-1) {					//Se o ficheiro tiver um formato errado sai da função
				fprintf(stderr, "Ficheiro com formato errado\n");
				free(user2);
				free(pass2);
				return;
			}
			pass2[strlen(pass2)-1]='\0';	//tira o \n do fim
			if(!strcmp(user2,user) && !strcmp(pass2,pass)) {		//Se o user2 for igual ao user a ser testado E se a pass2 for igual à pass, então o conjunto user-pass é válido
				printf("Login com sucesso! Bem vindo(a), %s\n", user);
				free(user2);
				free(pass2);
				system("clear");
				menu_cliente(); //Vai para o menu_cliente
				return;
			}
		}
		printf("Conjunto username-pass inválido! Tente novamente!\n");
		user_pass();
		return;
    	while(fgets(line, sizeof line, file)) {
        	if(sscanf(line, "%29s %29s", user2, pass2) == 2) {
            	if(!strcmp(user, user2) && !strcmp(pass,pass2)) {
            		system("clear");
    				fclose(file);
            		menu_cliente();
            	}
            	else {
            		printf("Conjunto username-pass inválido! Tente novamente!\n");
            		usleep(200000);
            		system("clear");
    				fclose(file);
            		user_pass();
            	}
        	}
   		}
	} else {
		fprintf(stderr, "Ficheiro de utilizadores não pôde ser aberto\n");
		return;
	}
}

void menu_cliente(){
	printf("\033[22;34m**Menu**\n1)\x1b[0m Ver feed\n\033[22;34m2)\x1b[0m Ver tópicos\n");
	printf("\033[22;34m3)\x1b[0m Procurar tópicos mais ativos\n");
	printf("\033[22;34m4)\x1b[0m Subscrever tópico\n");
	printf("\033[22;34m5)\x1b[0m Publicar num tópico\n");
	printf("\033[22;34m6)\x1b[0m Gerir Lista de Subscrições\n");
	printf("\033[22;34m7)\x1b[0m Ver Estatísticas\n");
	printf("\033[22;34m8)\x1b[0m Gerir conta\n");
	printf("\033[22;34m9)\x1b[0m Logout\n\nEscolha uma opção: ");
	int opcao;
	scanf("%d", &opcao);
	switch(opcao) {
		case 1:
			feed();
			break;
		case 2:
			topicos();
			break;
		case 3:
			topMaisAti();
			break;
		case 4:
			subTopico();
			break;
		case 5:
			pubTopico();
			break;
		case 6:
			gerirListaSub();
			break;
		case 7:
			estatisticas();
			break;
		case 8:
			gerirConta();
			break;
		case 9:
			printf("A carregar...\n");
			usleep(2000000);
			system("clear");
			menu();
			break;
		default:
			printf("Opção inválida! Tente novamente.\n");
			menu_cliente();
	}
}


void feed() {
	//MOSTRAR O FEED
	printf("Feed\n");
}

void topicos() {
	//MOSTRAR TOPICOS
}

void topMaisAti() {
	//PROCURAR TÓPICOS MAIS ATIVOS
	printf("Pretende filtrar a sua pesquisa num período de tempo (Y/n)? ");
	char a;
	if(!isalpha(a)) {
		printf("Opção inválida! Tente novamente!\n");
		usleep(4000000);
		system("clear");
		topMaisAti();
	}
	else {
		if(isupper(a))
			tolower(a);
	}

	switch(a) {
		case 'n':
			//MOSTRAR O FEED TODO
			break;
		case 'y':
			printf("1) última hora\n2) hoje\n3) esta semana\n4) desde o último login\n5) este mês");
			int b;
			printf("\n\nEscolha uma opção: ");
			scanf("%d", &b);
			switch(b) {
				case 1:
					//MOSTRAR A ULTIMA HORA
					break;
				case 2:
					//MOSTRAR O FEED DE HOJE
					break;
				case 3:
					//MOSTRAR O FEED DESTA SEMANA
					break;
				case 4:
					//MOSTRAR O FEED DESDE O ULTIMO LOGIN
					break;
				case 5:
					//MOSTRAR O FEED DESTE MÊS
					break;
				default:
					printf("Opção inválida! Tente novamente!");
					usleep(3000000);
					system("clear");
					topMaisAti();
					break;
			}
			break;
		default:
			printf("Opção inválida! Tente novamente!\n");
			topMaisAti();
			break;

	}
	if(a=='y') {
		
	}
}

void subTopico() {
	//SUBSCREVER TÓPICO
}

void pubTopico() {
	//PUBLICAR TÓPICO
}

void gerirListaSub() {
	//GERIR LISTA DE SUBSCRIÇÕES
}

void estatisticas() {
	//VER ESTATISTICAS
}

void gerirConta() {
	//MUDAR A SENHA, NOME, DATA DE NASCIMENTO, EMAIL, ...

}


int main(int argc, char const *argv[]) {
	system("clear");
	menu();
}