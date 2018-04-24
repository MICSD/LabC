#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#define MAX_SIZE 1000

int login();
int menu_cliente();
int menu();

FILE *usuario;

int login()
{
	char  user[20], path_user[80], pass[20], pass1[20],passtemp[20];
	char passaux[]="password";
  
	printf("Utilizador: "); //nome de utilizador
	scanf("%s", user);
	
	sprintf(path_user, "../aulas/labc/trabalho/Utilizadores/%s" ,user);
	printf("%s\n", path_user);
	
	usuario = fopen(path_user,"r+");
	if(usuario==NULL){
		printf("User não encontrado\n");
		system("clear");
		menu();
	}
	else {
		printf("Password:"); //Password do user
		scanf("%s", pass);
	
		fscanf(usuario, "%s", pass1); //abre o ficheiro com a pass
		if(strcmp(pass1,pass)==0){    //compara a pass do ficheiro com a pass inserida
			menu_cliente();
		}
		else{
			printf("Palavra passe errada, tente outra vez\n");  
			menu();
		}
	}
}

int menu_cliente(){
	printf("Bem vindo\n");
	printf("\n");
	printf("[1] Criar Novos Tópicos\n");
	printf("[2] Gerir Tópicos/Utilizadores\n");
	printf("[3] Ver estatísticas\n");
	printf("[4] Logout\n");
}


int menu()
{
	
  printf("**Menu de Registo**\n1) Login/Autenticação\n2) Pedidio de registo de novo utilizador\n3) Sair\n");
  int opcao;
  printf("Escolha uma opção: ");
  scanf("%d", &opcao);
  
  if (opcao==1){
	  system("clear");
	  login();
  }
  
  if (opcao==2){
	  system("clear");
	  exit(0);
  }
  
  if( opcao !=1 && opcao !=2){
	  system("clear");
	  printf("Escolha uma opção válida!\n");
  }
  
  
 return 0; 
}


int main(int argc, char const *argv[]) {
	menu();
}

