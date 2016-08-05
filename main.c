#include <stdio.h>
#include <stdlib.h>


void comandos(){
   char comando[300];
    char temporal[200];
    char *ptr;
comando[0]='\0';
    printf("Introduzca comando\n");

    fgets(temporal, 200, stdin);

       while(strstr(temporal,"\\")!=NULL)
    {
		int largo=strlen(temporal);
		strncat(comando,temporal,(largo-2));
		fgets(temporal, 200, stdin);
		}
		strcat(comando,temporal);

   int i=0;


   for(;i<300; i++){
       comando[i]=tolower(comando[i]);
   }
   printf("%s",comando);
}

int main()
{
     comandos();
    return 0;
}
