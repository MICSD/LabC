#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
#include <dirent.h>
//#include <curses.h>
#define MAX_SIZE 1000
#define ANSI_COLOR_GREEN    	 "\x1b[32m"
#define ANSI_COLOR_RED       	 "\x1b[31m"
#define ANSI_COLOR_YELLOW  		 "\x1b[33m"
#define ANSI_COLOR_BLUE			 "\x1b[34m"
#define ANSI_COLOR_BRIGHT_CYAN   "\x1b[96m"
#define ANSI_COLOR_MAGENTA  	 "\x1b[35m"
#define ANSI_COLOR_CYAN     	 "\x1b[36m"
#define ANSI_COLOR_RESET   		 "\x1b[0m"

typedef char bool;
#define true 1
#define false 0
//funções a corrigir
bool user_exists(); //verifica se um utilizador já existe ou não
void topMaisAti(); //procurar tópicos mais ativos
void subTopico(); //subscrever tópicos

//funções acabadas
void menu(); //menu inicial
void user_pass(); //lê o utilizador e a senha do stdin e usa-os como argumentos para a funçõa login
void login(); //autenticação
void menu_cliente(); //menu de opções dentro da conta
void pedido(); //guarda as informações sobre um novo utilizador no ficheiro pedidos
void strCompact(char* str); //limpa caracteres brancos do inicio e do fim de uma string
bool strIsOnlySpaces(const char* str); //vê se uma string tem só white spaces
void topicos(); //ver topicos e respetivas mensagens e likes

//funções por fazer
void feed(); //mostrar o feed
void pubTopico(); //publicar tópicos
void gerirListaSub(); //gerir lista de subscrições
void estatisticas(); //ver estatisticas
void gerirConta(); //ver e/ou mudar os dados do utilizador 



typedef struct {
    char nome[200];
    char senha[200];
} utilizador;

char user_usado[50];


bool strIsOnlySpaces(const char* str) {		
	size_t n=str?strlen(str):0;
	for(size_t i=0 ; i<n ; i++)
		if(!isspace(str[i] && isprint(str[i])))
			return false;
	return true;
}

void strCompact(char* str) {
	size_t len;
	if(!str || !(len=strlen(str)))
		return;
	size_t nInicio=0;
	while(str[nInicio] && isspace(str[nInicio]))
		nInicio++;
	if(nInicio>=len) {
		str[0]=0;
	} else {
		memmove(str,str+nInicio,len-nInicio+1);
		for(len=strlen(str) ; len>0 && isspace(str[len-1]) ; len--)
			str[len-1]=0;
	}
}

bool user_exists(const char* new_username) {
	FILE *fc;
	char *user2=NULL, *pass2=NULL, *line=NULL;
	fc = fopen("Login/Users/utilizadores","r");
	//if (fc) {
		size_t n_u, n_p;
		while(getline(&user2, &n_u, fc)!=-1) {
			if(strlen)
			user2[strlen(user2)-1]='\0';	//tira o \n do fim
			/*if(getline(&pass2, &n_p, f)==-1) {					//Se o ficheiro tiver um formato errado sai da função
				fprintf(stderr, "Ficheiro com formato errado\n");
				free(user2);
				free(pass2);
				return;
			}*/
			getline(&pass2, &n_p, fc);
			pass2[strlen(pass2)-1]='\0';	//tira o \n do fim
			if(!strcmp(new_username,user2)) {		//Se o user2 for igual ao user a ser testado E se a pass2 for igual à pass, então o conjunto user-pass é válido
				//printf("Utilizador já existente!");
				free(user2);
				free(pass2);
				return true;
			}
		
        }
    //}
   
	/*else {
		fprintf(stderr, "Ficheiro de utilizadores não pôde ser aberto\n");
		return;
	}*/

	return false;
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
		printf(ANSI_COLOR_RED "Escolha inválida! Tente novamente!\n" ANSI_COLOR_RESET);
		sleep(2);
		system("clear");
		menu();
  }  
}

FILE *f;

void pedido() {
	FILE *pedidos;
	char nome[50], new_username[30], new_password[20], email[100], data[20];
	pedidos = fopen("Login/Users/pedidos","a");
	

	printf("\033[22;34mNome:\033[0m ");
	do {
		fgets(nome,50,stdin);		//lê o nome do utilizador
		strCompact(nome);
	} while(strIsOnlySpaces(nome));
	fprintf(pedidos,"%s\n", nome);

	printf("\033[22;34mUsername:\033[0m ");
	do {
		fgets(new_username,30,stdin);
		strCompact(new_username);
	} while(strIsOnlySpaces(new_username));
	fprintf(pedidos,"%s\n", new_username);

	if(user_exists(new_username)) {
		printf("Este utilizador já existe!\n");
		pedido();
		return;
	}
	
	printf("\033[22;34mPassword:\033[0m ");
	do {
		fgets(new_password,20,stdin);
		strCompact(new_password);
	} while(strIsOnlySpaces(new_password)); 
	fprintf(pedidos,"%s\n", new_password);

	printf("\033[22;34mEmail:\033[0m");
	do {
		fgets(email,100,stdin);
		strCompact(email);
	} while(strIsOnlySpaces(email));
	fprintf(pedidos,"%s\n", email);
	
	printf("\033[22;34mData de nascimento (DD/MM/AAAA):\033[0m ");
	do {
		fgets(data,20,stdin);
		strCompact(data);
	} while(strIsOnlySpaces(data));
	fflush(stdin);
	fprintf(pedidos,"%s\n", data);

	printf("\nA carregar... Por favor aguarde.\n");
	sleep(1);
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
				printf("Ficheiro com formato errado\n");
				free(user2);
				free(pass2);
				return;
			}
			pass2[strlen(pass2)-1]='\0';	//tira o \n do fim
			if(!strcmp(user2,user) && !strcmp(pass2,pass)) {		//Se o user2 for igual ao user a ser testado E se a pass2 for igual à pass, então o conjunto user-pass é válido
				strcpy(user_usado, user);
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
	} else {
		printf("Ficheiro de utilizadores não pôde ser aberto\n");
	}
}

void menu_cliente(){
	system("clear");
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
	getchar(); //ler o \n
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
	//topicos e respetivas mensagens

	printf("Feed\n");
}

void topicos() {
	//MOSTRAR TOPICOS
	struct dirent *conteudoDir;
	DIR *diretorio;
	diretorio = opendir("Topicos");
	printf(ANSI_COLOR_BRIGHT_CYAN "Tópicos existentes:\n" ANSI_COLOR_RESET);
	while((conteudoDir=readdir(diretorio))!=NULL) {
		if(!strcmp(conteudoDir->d_name, "." ) || !strcmp(conteudoDir->d_name, "..") || !strcmp(conteudoDir->d_name, "lista_topicos_sub")){
			continue;
		}
		else{
			printf(ANSI_COLOR_BRIGHT_CYAN "→ " ANSI_COLOR_RESET "%s\n", conteudoDir -> d_name);
		}
	}
	char a;
	printf("Para voltar ao menu prima qualquer tecla.\n");
	scanf("%c", &a);
	switch(a) {
		case 'a':
		menu_cliente();
		break;
		default:
		menu_cliente();
		break;
	}
	
}

void topMaisAti() {
	//PROCURAR TÓPICOS MAIS ATIVOS
	printf("Pretende filtrar a sua pesquisa num período de tempo (Y/n)? ");
	char a;
	if(!isalpha(a)) {
		printf("Opção inválida! Tente novamente!\n");
		usleep(2000000);
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
	//ver os topicos existentes
	struct dirent *conteudoDir;
	DIR *diretorio;
	FILE *lista;
	FILE *topico;
	diretorio = opendir("Topicos");
	int a;
	char path_topico[40] = "Topicos/";
	char nome_topico[100] = "Topicos/";
	
	printf("1) Subscrever tópicos\n");
	printf("2) Voltar\n");
	printf("Insira uma opção: ");
	scanf("%d", &a);
	system("clear");
	switch(a) {
		case 1:
			printf(ANSI_COLOR_BRIGHT_CYAN "Tópicos existentes:\n" ANSI_COLOR_RESET);
			while((conteudoDir=readdir(diretorio))!=NULL) {
				if(!strcmp(conteudoDir->d_name, "." ) || !strcmp(conteudoDir->d_name, "..") || !strcmp(conteudoDir->d_name, "lista_topicos_sub")){
					continue;
				}
				else{
					printf(ANSI_COLOR_BRIGHT_CYAN "→ " ANSI_COLOR_RESET "%s\n", conteudoDir -> d_name);
				}
			}
			printf("Insira o nome do tópico que quer subscrever: ");
			scanf("%s", nome_topico+8);
			//printf("%s\n",nome_topico);
			if((topico = fopen(nome_topico,"r"))==NULL) {
					//printf("0\n");
					printf("O tópico não existe.\n");
					sleep(1);
					system("clear");
					subTopico();
			}
			else {
				//printf("1\n");
				lista = fopen("Topicos/lista_topicos_sub", "a");
				fprintf(lista, "%s:\n", user_usado);
				fprintf(lista, "→ %s\n", nome_topico+8);
			}
			break;
		case 2:
			system("clear");
			menu_cliente();
			break;
		default:
			printf("Opção inválida! Tente novamente!\n");
			sleep(2);
			system("clear");
			subTopico();
			break;
	}
	
	closedir(diretorio);
	//TO DO: agrupar os topicos subscritos em cada pessoa
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