#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void comandomkdisk(char comando[300]){
//identificando valores

    int size;
    char unidad;
    char *path;
    char *token;
    char *nombre;
//valor por default
unidad='m';
path=NULL;
token=strtok(comando," \n");
	 token=strtok(NULL, " \n");
	 int bandera=0;//la bandera nos ayuda con el path con comillas
	 while(token!=NULL){
	 printf("%s\n",token);
		 ///**************bandera
 	 if(bandera==1){
	  strcat(path," ");//agrega espacio en blanco al path
		 if(strstr(token,"\"")==NULL){
			 bandera=1;//continua concatenando el path
			 strcat(path,token);
			 }else{

				 bandera=0;//se encontro cierre de comillas
				 strncat(path,token,strlen(token)-1);//concatena el ultimo token del path sin comillas
				 }}
		//************siz
	else if(strstr(token,"-size::")!=NULL){
		 int len=strlen(token);
		 int i=7;//tamaño del token -size::
		 char* temp;//captura el valor de size
		  int j=0;
		   temp=(char*) malloc (6);
		  for(;i<len;i++)
		   {temp[j]=token[i];
		   j++;
		   }temp[j]='\0';
		   size=atoi(temp);//castea valor size

		 }
		 ///********************unit
	else if(strstr(token,"+unit::")!=NULL){
			 int le=strlen(token);//tamaño total del token +unit:NUMERO
			 unidad=token[le-1];


		 }
		 ///****************************
		 else if(strstr(token,"-path::")!=NULL){
			 int len=strlen(token);

			   path=(char*) malloc (1000);//inicializamor el char para path
			  if(strstr(token,"\"")==NULL)//no es path con comillas
			 {  bandera=0;

				 int i=7;
			 int j=0;
			 for(;i<len;i++){
				 path[j]=token[i];//guarda el valor del path
				 j++;
				 }
				 }
			 else{//si tiene comillas
			 bandera=1;//se activa la bandera para concatenar lo siguiente
			 int i=8;//-path::
			 int j=0;
			 for(;i<len;i++){
				 path[j]=token[i];
				 j++;
				 }


			 }


		 }
		 else{
			 printf("error: instruccion no reconocido\n");
			 }

		  token=strtok(NULL, " \n");

		}
}
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
   ///identificar el comando
printf("%s", comando);
   if(strstr(comando,"mkdisk")!=NULL){
        comandomkdisk(comando);
   }

}

int main()
{
     comandos();
    return 0;
}
