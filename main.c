#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//struct para la creacion de muestro bloque
typedef struct{
	char part_status;
	char part_type;
	char part_fit;
	int part_start;
	int part_size;
	char part_name[16];
	}TParticion;
	typedef struct{
	char part_status;
	char part_fit;
	int part_start;
	int part_size;
	int part_next;
	char part_name[16];
	}TParticionEBR;
///MBR****************************+
typedef struct{
	int mbr_tamao;
	char* mbr_fecha_creacion;
	int  mbr_disk_signature;
	TParticion part_primaria[4];//struct con infornacion de particiones primarias
	TParticionEBR part_ext;//struct con informcion de particion extendida
	}MBR;
void comandofdisk(char comando[300]){
char *token;
	int size;
	char unit;
	char* path;
	char type;
	char *fit;
	char *ddelete;
	char *nombre;
	int add;
	int tipoaccion=0;//0 crear particion, 1.add ,2.delete
	unit='k';
	type='p';
    fit="wf\0";
	ddelete="fast\0";
	add=0;
	int bandera;
	bandera=0;
	 token=strtok(comando," \n\\");
	 token=strtok(NULL, " \n\\");

	 while(token!=NULL){
		 ///**************bandera
 	 if(bandera==1){
	  strcat(path," ");
		 if(strstr(token,"\"")==NULL){
			 bandera=1;
			 strcat(path,token);
			 }else{
				 bandera=0;
				 strncat(path,token,strlen(token)-1);
				 }}
		//************siz
	else if(strstr(token,"-size::")!=NULL){
		 int len=strlen(token);
		 int i=7;
		 char* temp;
		  int j=0;
		   temp=(char*) malloc (6);
		  for(;i<len;i++)
		   {temp[j]=token[i];
		   j++;
		   }temp[j]='\0';
		   size=atoi(temp);

		 }
		 ///********************unit
	else if(strstr(token,"+unit::")!=NULL){
			 int le=strlen(token);
			 unit=token[le-1];


		 }
		 else if(strstr(token,"+type::")!=NULL){
			 int le=strlen(token);
			 type=token[le-1];


		 }
		 else if(strstr(token,"+fit::")!=NULL){
			 int le=strlen(token);
			 fit[0]=token[le-2];
            fit[1]=token[le-1];
		 }
		 else if(strstr(token,"+delete::")!=NULL){
			 int le=strlen(token);
			 ddelete=(char*) malloc (1000);
			 int i=10;
			 int j=0;
			 for(; i<le;i++){
				 ddelete[j]=token[i];
				 j++;
				 }

            tipoaccion=2;
		 }
		 else if(strstr(token,"-name::")!=NULL){
			 int le=strlen(token);
			  nombre=(char*) malloc (1000);
			 int i=8;
			 int j=0;
			 for(; i<le-1;i++){
				 nombre[j]=token[i];
				 j++;
				 }


		 }
		 else if(strstr(token,"+add::")!=NULL){
		 int len=strlen(token);
		 int i=7;
		 char* temp;
		  int j=0;
		   temp=(char*) malloc (6);
		  for(;i<len;i++)
		   {temp[j]=token[i];
		   j++;
		   }temp[j]='\0';
		   add=atoi(temp);
        tipoaccion=1;

		 }
		 ///****************************
		 else if(strstr(token,"-path::")!=NULL){
			 int len=strlen(token);

			   path=(char*) malloc (1000);
			  if(strstr(token,"\"")==NULL)
			 {  bandera=0;

				 int i=7;
			 int j=0;
			 for(;i<len;i++){
				 path[j]=token[i];
				 j++;
				 }
				 }
			 else{
			 bandera=1;
			 int i=8;
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

		  token=strtok(NULL, " \n\\");

		}
		printf("%s\n",path);
		printf("%s\n",nombre);
		printf("%s\n",ddelete);
		printf("%d\n",size);
		printf("%d\n",add);
		printf("%c\n",unit);
		printf("%c\n",type);
		printf("%s\n",fit);
}
void comandomkdisk(char comando[300]){
//identificando valores

    int size;
    char unidad;
    char *path;
    char *token;
    char *nombre;
//valor por default
unidad='m';
size=-1;
path=NULL;
nombre=NULL;
token=strtok(comando," \n\\");
	 token=strtok(NULL, " \n\\");
	 int bandera=0;//la bandera nos ayuda con el path con comillas
	 while(token!=NULL){

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
	else if(strstr(token,"-size")!=NULL){

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
        if((unidad!='k')&&(unidad!='m'))
        printf("error :el valor de la unidad no es valida\n");

		 }
		 else if(strstr(token,"-name::")!=NULL){
			int le=strlen(token);
			  nombre=(char*) malloc (1000);
			 int i=7;
			 int j=0;
			 for(; i<le;i++){
				 nombre[j]=token[i];
				 j++;
                }
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
			 printf(" %s error: instruccion no reconocido\n",token);
			 }

		  token=strtok(NULL, " \n\\");

		}
		if(size!=-1 && path!=NULL && nombre!=NULL){
//printf("%c\n",unidad);  printf("%d\n",size);  printf("%s\n",path);printf("%s\n",nombre);
//crear el tamaño de archivo que se indico
int sizetotal;
int numerobyt;
       int s=250;//un byte
		sizetotal=1;
        s*=size;
		if(unidad=='k')
			{sizetotal=size*1024; numerobyt=1;
			}
		  else {sizetotal=size*1024*1024;
		  s*=100;
		  numerobyt=10;
			  }

//creamos el archivo
FILE *fichero;
strcat(path,nombre);
//debemos verificaar si el disco ya existe
fichero = fopen(path, "r" );

   if( fichero )
     { printf( " EL DISCO YA EXISTE\n" );
      fclose(fichero);}
   else
   {//no existe entonces procedemos a crearlo
 fichero=fopen(path, "w+b"); //binario read write
      if(fichero==NULL){
        printf("%s",path);
      return ;
  }else{

int mibyte[s];
  int i=0;
  int j=0;
char nulo='\0';
        for (;j<sizetotal;j++){
        for (;i<s;i++){
         fwrite(&nulo, sizeof(mibyte[i]),numerobyt,fichero);
        }

}
//ya creado el archivo creamos el master record
time_t tt;
tt=time(NULL);

			  MBR master;
			  master.mbr_tamao=sizetotal;
			  master.mbr_fecha_creacion=asctime(localtime(&tt));
			  master.mbr_disk_signature=rand();
              master.part_primaria[0].part_status=0;
              master.part_primaria[1].part_status=0;
              master.part_primaria[2].part_status=0;
              master.part_primaria[3].part_status=0;
              master.part_ext.part_status=0;
              fseek(fichero,0,SEEK_SET);
      fwrite(&master, sizeof(master),1,fichero);
      printf("disco creado con exito\n");
 fclose(fichero);

}}
}else{
printf("VALORES SIN DEFINIR\n");
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

		strcat(comando,temporal);
		fgets(temporal, 200, stdin);
		}
		strcat(comando,temporal);

   int i=0;


   for(;i<300; i++){
       comando[i]=tolower(comando[i]);
   }
   ///identificar el comando

   if(strstr(comando,"mkdisk")!=NULL){
        comandomkdisk(comando);
   }else if(strstr(comando,"fdisk")!=NULL){
        comandofdisk(comando);
   }

}

int main()
{
int seguir;
printf("¿Desea ingresar comandos? 1)si 2)no\n");
scanf("%d",&seguir);
getchar();
if(seguir==1){
     comandos();
     }
    return 0;
}
