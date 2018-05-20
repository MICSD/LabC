#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
#include <dirent.h>
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
void login(char user[], char pass[]); //autenticação
void menu_cliente(); //menu de opções dentro da conta
void pedido(); //guarda as informações sobre um novo utilizador no ficheiro pedidos
void strCompact(char* str); //limpa caracteres brancos do inicio e do fim de uma string
bool strIsOnlySpaces(const char* str); //vê se uma string tem só white spaces
void topicos(); //ver topicos e respetivas mensagens e likes
size_t getFileSize(FILE* f);
void insertInFile(FILE* f, const char* bytes, size_t nBytes, size_t startPos, size_t bytesToReplace);

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
	char *user2=NULL, *pass2=NULL;
	fc = fopen("Login/Users/utilizadores","r");
	if(!fc) {
		printf("Erro a abir ficheiro de utilizadores!\n");
		return false;
	}
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
			fclose(fc);
			return true;
		}
    }
    if(user2)
    	free(user2);
    if(pass2)
    	free(pass2);
    fclose(fc);
	return false;
}

void menu() {
	system("clear");
	printf("\033[22;34m===============================\n\tMenu de Registo\n===============================\n\033[0m\n\033[22;34m1)\033[0m Login/Autenticação\n\033[22;34m2)\033[0m Pedido de registo de novo utilizador\n\033[22;34m3)\033[0m Sair\n");
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

void pedido() {
	FILE *pedidos;
	char nome[50], new_username[30], new_password[20], email[100], data[20];
	pedidos = fopen("Login/Users/pedidos","a");
	if(!pedidos) {
		printf("Erro a abir o ficheiro de pedidos!\n");
		return;
	}
	

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
		fclose(pedidos);
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
	fclose(pedidos);
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


void login(char user[], char pass[]) {
	FILE *file;
    char *user2=NULL, *pass2=NULL, *line=NULL;
    if((file = fopen("Login/Users/utilizadores", "r+"))==NULL) {
    	printf("O ficheiro \"utilizadores\" não existe!\n");
    	return;
    }
	if (file) {
		size_t n_u, n_p;
		while(getline(&user2, &n_u, file)!=-1) {
			if(strlen)
			user2[strlen(user2)-1]='\0';	//tira o \n do fim
			if(getline(&pass2, &n_p, file)==-1) {					//Se o ficheiro tiver um formato errado sai da função
				printf(ANSI_COLOR_RED"Ficheiro com formato errado\n"ANSI_COLOR_RESET);
				free(user2);
				free(pass2);
				fclose(file);
				return;
			}
			pass2[strlen(pass2)-1]='\0';	//tira o \n do fim
			if(!strcmp(user2,user) && !strcmp(pass2,pass)) {		//Se o user2 for igual ao user a ser testado E se a pass2 for igual à pass, então o conjunto user-pass é válido
				strcpy(user_usado, user);
				printf(ANSI_COLOR_GREEN"Login com sucesso!\n"ANSI_COLOR_RESET, user);
				free(user2);
				free(pass2);
				fclose(file);
				sleep(2);
				system("clear");
				printf(ANSI_COLOR_BLUE "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t----------------------------\n\t\t\t\t\t\t     Bem vindo(a) %s!\n\t\t\t\t\t\t----------------------------\n" ANSI_COLOR_RESET, user_usado);
				sleep(2);
				menu_cliente(); //Vai para o menu_cliente
				return;
			}
		}
		printf("Conjunto username-pass inválido! Tente novamente!\n");
		if(user2)
			free(user2);
		if(pass2)
			free(pass2);
		fclose(file);
		user_pass();
	} else {
		printf("Ficheiro de utilizadores não pôde ser aberto\n");
	}
}

void menu_cliente(){
	system("clear");
	printf("\033[22;34m====================\n\tMenu\n====================\n\n1)\x1b[0m Ver feed\n\033[22;34m2)\x1b[0m Ver tópicos\n");
	printf("\033[22;34m3)\x1b[0m Procurar tópicos mais ativos\n");
	printf("\033[22;34m4)\x1b[0m Subscrever tópico\n");
	printf("\033[22;34m5)\x1b[0m Publicar num tópico\n");
	printf("\033[22;34m6)\x1b[0m Gerir Lista de Subscrições\n");
	printf("\033[22;34m7)\x1b[0m Ver Estatísticas\n");
	printf("\033[22;34m8)\x1b[0m Gerir conta\n");
	printf("\033[22;34m9)\x1b[0m Logout\n\n"ANSI_COLOR_CYAN"---------------------\nEscolha uma opção: ");
	int opcao;
	scanf("%d", &opcao);
	printf(ANSI_COLOR_RESET);
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
			usleep(1000000);
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
	system("clear");
	struct dirent *conteudoDir;
	DIR *diretorio;
	diretorio = opendir("Topicos");
	printf(ANSI_COLOR_BRIGHT_CYAN "===========================\n     Tópicos existentes\n===========================\n\n" ANSI_COLOR_RESET);
	while((conteudoDir=readdir(diretorio))!=NULL) {
		if(!strcmp(conteudoDir->d_name, "." ) || !strcmp(conteudoDir->d_name, "..") || !strcmp(conteudoDir->d_name, "lista_topicos_sub")){
			continue;
		}
		else{
			printf(ANSI_COLOR_BRIGHT_CYAN "- " ANSI_COLOR_RESET "%s\n", conteudoDir -> d_name);
		}
	}
	char a;
	printf("Para voltar ao menu prima a tecla enter.\n");
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
	printf("Função incompleta.\n");
	getchar();
	menu_cliente();
	//PROCURAR TÓPICOS MAIS ATIVOS
	/*printf("Pretende filtrar a sua pesquisa num período de tempo (Y/n)? ");
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
		
	}*/
}

size_t getFileSize(FILE* f) {
	size_t curr_pos = ftell(f);
	fseek(f,0,SEEK_END);
	size_t n_bytes=ftell(f);
	fseek(f,curr_pos,SEEK_SET);
	return n_bytes;
}

void subTopico() {
	//ver os topicos existentes
	struct dirent *conteudoDir;
	DIR *diretorio;
	FILE *lista;
	FILE *topico;
	diretorio = opendir("Topicos");
	int a;
	char nome_topico[100] = "Topicos/";
	system("clear");
	printf(ANSI_COLOR_BRIGHT_CYAN "===========================\n     Tópicos existentes\n===========================\n\n" ANSI_COLOR_RESET);
	while((conteudoDir=readdir(diretorio))!=NULL) {
		if(!strcmp(conteudoDir->d_name, "." ) || !strcmp(conteudoDir->d_name, "..") || !strcmp(conteudoDir->d_name, "lista_topicos_sub")){
			continue;
		}
		else{
			printf(ANSI_COLOR_BRIGHT_CYAN "- " ANSI_COLOR_RESET "%s\n", conteudoDir -> d_name);
		}
	}
	printf(ANSI_COLOR_BRIGHT_CYAN "1)" ANSI_COLOR_RESET " Subscrever tópicos\n");
	printf(ANSI_COLOR_BRIGHT_CYAN "2)" ANSI_COLOR_RESET " Voltar\n");
	printf("\nInsira uma opção: ");
	scanf("%d", &a);
	switch(a) {
		case 1:
		{
			printf("Insira o nome do tópico que quer subscrever: ");
			scanf("%s", nome_topico+8);
				//printf("%s\n",nome_topico);
			if((topico = fopen(nome_topico,"r"))==NULL) {
				printf(ANSI_COLOR_RED "O tópico não existe. Tente novamente.\n" ANSI_COLOR_RESET);
				sleep(1);
				system("clear");
				closedir(diretorio);
				subTopico();
				return;
			}
			fclose(topico);
			if((lista = fopen("Topicos/lista_topicos_sub", "r+"))==NULL) {
				printf(ANSI_COLOR_RED "A lista de tópicos não existe.\n" ANSI_COLOR_RESET);
				sleep(1);
				system("clear");
				closedir(diretorio);
				subTopico();
				return;
			}
			char *line=NULL;
			size_t line_buffer_size=0;
			ssize_t n_read;
			fseek(lista,0,SEEK_SET);
			while((n_read=getline(&line,&line_buffer_size,lista))>=0) {
				if(strIsOnlySpaces(line))
					continue;
				strCompact(line);
				if(line[0]=='-')
					continue;
				if(!strcmp(line,user_usado))
					break;
			}
			if(n_read<=0) { //chegou ao fim do ficheiro e não encontrou o utilizador
				fprintf(lista, "%s\n- %s\n", user_usado, nome_topico+8);
			} else { //econtrou o utilizador e vai escrever o novo tópico no meio do ficheiro, se ainda não tiver sido subscrito
				bool insere=false;
				size_t curr_pos = ftell(lista);
				while((n_read=getline(&line,&line_buffer_size,lista))>=0) {
					if(strIsOnlySpaces(line))
						continue;
					strCompact(line);
					if(line[0]!='-') { //chegámos ao fim dos tópicos do utilizador e não encontrámos o tópico
						//o tópico não tinha sido subscrito ainda
						fseek(lista,curr_pos,SEEK_SET);
						insere=true;
						break;
					}
					if(!strcmp(nome_topico+8,line+2)) { //encontrou o tópico na lista dos tópicos desse utilizador
						printf(ANSI_COLOR_YELLOW "Esse tópico já foi subscrito por si!\n" ANSI_COLOR_RESET);
						break;
					}
				}
				if(insere || n_read<=0) { //se o tópico ainda não foi subscrito, subscreve
					curr_pos = ftell(lista);
					size_t n_bytes_lista;
					n_bytes_lista=getFileSize(lista);
					void* p=NULL;
					if(n_bytes_lista-curr_pos>0) {
						p = malloc(n_bytes_lista-curr_pos);
						fread(p, n_bytes_lista-curr_pos, 1, lista);
						fseek(lista,curr_pos,SEEK_SET);
					}
					fprintf(lista, "- %s\n", nome_topico+8);
					if(n_bytes_lista-curr_pos>0) {
						fwrite(p, n_bytes_lista-curr_pos, 1, lista);
						free(p);
					}
					printf(ANSI_COLOR_GREEN "Tópico \"%s\" subscrito com sucesso!\n" ANSI_COLOR_RESET, nome_topico+8);
				}
			}
			sleep(2);
			menu_cliente();
		}
			break;
		case 2:
			system("clear");
			menu_cliente();
			break;
		default:
			printf("Opção inválida! Tente novamente!\n");
			system("clear");
			subTopico();
	}
	
	closedir(diretorio);
}
void pubTopico() { //PUBLICAR NUM TÓPICO
	system("clear");
	struct dirent *conteudoDir;
	DIR *diretorio;
	FILE *lista;
	FILE *topico;
	char nome_topico[100] = "Topicos/";
	diretorio = opendir("Topicos");
	printf(ANSI_COLOR_BRIGHT_CYAN "===========================\n     Tópicos existentes\n===========================\n\n" ANSI_COLOR_RESET);
	while((conteudoDir=readdir(diretorio))!=NULL) {
		if(!strcmp(conteudoDir->d_name, "." ) || !strcmp(conteudoDir->d_name, "..") || !strcmp(conteudoDir->d_name, "lista_topicos_sub")){
			continue;
		}
		else{
			printf(ANSI_COLOR_BRIGHT_CYAN "- " ANSI_COLOR_RESET "%s\n", conteudoDir -> d_name);
		}
	}

	printf("Em qual dos tópicos pretende adicionar uma mensagem? ");
	scanf("%s", nome_topico+8);
	if((topico = fopen(nome_topico,"r"))==NULL) {
		printf(ANSI_COLOR_RED "O tópico não existe! Tente novamente.\n" ANSI_COLOR_RESET);
		sleep(1);
		pubTopico();
	}
	else {
		lista = fopen("Topicos/lista_topicos_sub", "a");
		fprintf(lista, "%s:\n", user_usado);
		fprintf(lista, "- %s\n", nome_topico+8);
		printf(ANSI_COLOR_GREEN "Tópico \"%s\" subscrito com sucesso!\n" ANSI_COLOR_RESET, nome_topico+8);
		sleep(2);
	}
}

void gerirListaSub() {
	//GERIR LISTA DE SUBSCRIÇÕES
}

void estatisticas() {
	//VER ESTATISTICAS
}

void insertInFile(FILE* f, const char* bytes, size_t nBytes, size_t startPos, size_t bytesToReplace) {
	if(!f || !bytes)
		return;
	size_t file_size;
	fseek(f,0,SEEK_END);
	file_size=ftell(f);
	if(startPos+bytesToReplace>file_size)
		bytesToReplace=file_size-startPos;

	if(file_size<=startPos) { //append to the file
		fwrite(bytes,nBytes,1,f);
	} else { //insert in the middle
		if(startPos+bytesToReplace>=file_size) { //don't need to save anything
			fseek(f,startPos,SEEK_SET);
			fwrite(bytes,nBytes,1,f);
		} else {
			fseek(f,startPos+bytesToReplace,SEEK_SET);
			void* temp=malloc(file_size-startPos-bytesToReplace);
			fread(temp,file_size-startPos-bytesToReplace,1,f);

			fseek(f,startPos,SEEK_SET);
			fwrite(bytes,nBytes,1,f);
			fwrite(temp ,file_size-startPos-bytesToReplace,1,f);
			free(temp);
		}
	}

	if(bytesToReplace>nBytes) { //file will shrink
		size_t new_size;
		new_size = file_size-bytesToReplace+nBytes;
		fflush(f);
		ftruncate(fileno(f),new_size);
	}
}

void gerirConta() {
	//MUDAR A SENHA, NOME, DATA DE NASCIMENTO, EMAIL, ...
	printf("Olá %s!\n", user_usado);
	printf("Os seus dados são os seguintes: \n");

    FILE *info;
 
    char filename[100] = "Login/Users/info_utilizadores";
    char *line = NULL;
    size_t s = 0,c;
    // Open file
    info = fopen(filename, "r+");
    if (info == NULL)
    {
        printf("Cannot open file \n");
        menu_cliente();
    }
 	
 	size_t n_bytes=getFileSize(info), user_start_byte=n_bytes;
 	fseek(info,0,SEEK_SET);
    // Lê o conteúdo do ficheiro
 	bool reachedTheUser=false;
    char *line1 = NULL, *novo_username=NULL, *nova_pass=NULL, *novo_nome=NULL, *novo_email=NULL, *nova_data=NULL;
    size_t username_line_size=0, pass_line_size=0, nome_line_size=0, email_line_size=0, data_line_size=0;
    for (int count = 0 ; (c = getline(&line,&s,info))!=EOF ; count=count>=4?0:(count+1))
    {
    	size_t line_size=strlen(line);
    	strCompact(line);
    	if(count==0) {
    		reachedTheUser=!strcmp(line,user_usado);
    		user_start_byte=ftell(info)-c;
    	}
    	if(reachedTheUser) {
    		if      (count==0) {
	    		printf("Username: ");
	    		novo_username=strdup(line);
	    		username_line_size = line_size;
    		}
	    	else if (count==1) {
	    		printf("Pass: ");
	    		nova_pass=strdup(line);
	    		pass_line_size = line_size;
	    	}else if(count==2) {
	    		printf("Nome: ");
	    		novo_nome=strdup(line);
	    		nome_line_size = line_size;
	    	}else if(count==3) {
	    		printf("Email: ");
	    		novo_email=strdup(line);
	    		email_line_size = line_size;
	    	}
	    	else if(count==4) {
	    		printf("Data de nascimento: %s\n", line);
	    		nova_data=strdup(line);
	    		data_line_size = line_size;
	    		break;
	    	}
	    	else
	    		break;
	        printf ("%s\n", line);
    	}
    }
    if(!reachedTheUser && c<0) {
    	printf("Utilizador sem informações\n");
    }

    printf("1) Alterar a pass\n");
    printf("2) Alterar nome\n");
    printf("3) Alterar o email\n");
    printf("4) Alterar a data de nascimento\n");
    printf("5) voltar para o menu\n");
    int a;
    while(scanf("%d", &a)!=1);
    getchar();
    size_t size, t_s=s;
    switch(a) {
    	case 1: {
	    	//alterar pass
	    	printf("Nova pass: ");
	    	s=strlen(nova_pass)+1;
	    	getline(&nova_pass,&s,stdin);
	    	strCompact(nova_pass);
	    	printf("Alterou para '%s'\n", nova_pass);
	    	insertInFile(info,nova_pass,strlen(nova_pass),user_start_byte+username_line_size,pass_line_size-1);

	   		FILE* login_f=fopen("Login/Users/utilizadores","r+");
	   		if(!login_f) {
	   			printf("Não foi possível abrir o ficheiro de login!\n");
	   			break;
	   		}

	   		user_start_byte=0;
	   		while((c=getline(&line,&s,login_f)!=-1)) {
	   			user_start_byte=ftell(login_f)-c;
	   			size_t username_size=c;
	   			strCompact(line);
	   			novo_email=strdup(line);
	   			if((c=getline(&line,&s,login_f))==-1) {
	   				break;
	   			}
	   			strCompact(line);
	   			//printf("user: '%s' | user_usado: '%s' | line:'%s'\n",novo_email,user_usado,line);
	   			if(!strcmp(novo_email,user_usado)) {
	   				//printf("ENCONTRADO %lu \n", user_start_byte+username_size);
	   				insertInFile(login_f,line,strlen(line),user_start_byte+username_size,c-1);
	   			}
	   		}
	   		fclose(login_f);
    	}
    	break;
    	case 2:
    	//alterar nome
    	printf("Novo nome: ");
    	s=strlen(novo_nome)+1;
    	getline(&novo_nome,&s,stdin);
    	strCompact(novo_nome);
    	printf("Alterou para '%s'\n", novo_nome);
    	insertInFile(info,novo_nome,strlen(novo_nome),user_start_byte+username_line_size+pass_line_size,nome_line_size-1);
    	break;
    	case 3:
    	//alterar email
    	printf("Novo email: ");
    	s=strlen(novo_email)+1;
    	getline(&novo_email,&s,stdin);
    	strCompact(novo_email);
    	printf("Alterou para '%s'\n", novo_email);
    	insertInFile(info,novo_email,strlen(novo_email),user_start_byte+username_line_size+pass_line_size+nome_line_size,email_line_size-1);
    	break;
    	case 4:
    	//alterar data de nascimento
    	printf("Nova data de nascimento: ");
    	s=strlen(nova_data)+1;
    	getline(&nova_data,&s,stdin);
    	strCompact(nova_data);
    	printf("Alterou para '%s'\n", nova_data);
    	insertInFile(info,nova_data,strlen(nova_data),user_start_byte+username_line_size+pass_line_size+nome_line_size+email_line_size,data_line_size-1);
    	break;
    	case 5:

    	break;
    	default:
    	printf(ANSI_COLOR_RED"Opção inválida! Tente novamente!\n"ANSI_COLOR_RESET);
    	gerirConta();
   	}
 	free(line);
 	if(novo_username)
 		free(novo_username);
 	if(nova_pass)
 		free(nova_pass);
 	if(novo_nome)
 		free(novo_nome);
 	if(novo_email)
 		free(novo_email);
 	if(nova_data)
 		free(nova_data);
    fclose(info);
    printf("Prima ENTER para passar ao próximo ecrã...\n");
	getchar();
	menu_cliente();
}


int main(int argc, char const *argv[]) {
	system("clear");
	menu();
}