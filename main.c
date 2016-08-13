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
//para menejar las particiones montadas
char listaletras[27]={'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s',
                't','u','v','w','x','y','z'};
typedef struct{
char identificador[300];
char idcaracter[10];

}celda;
celda cabezeras[10];
celda tablaparticion[10][10];

void clasificar(char comando[300]){
if(comando[0]!='\0'){
   if(strstr(comando,"mkdisk")!=NULL){
        comandomkdisk(comando);
        getchar();
   }else if(strstr(comando,"fdisk")!=NULL){
      {  comandofdisk(comando);getchar();}
   }else if(strstr(comando,"rep")!=NULL){
    if(comando[0]=='r'&& comando[1]=='e'&& comando[2]=='p'){
            crear_reporte(comando);
            getchar();}

   }else if(strstr(comando,"mount")!=NULL){
     montar(comando);getchar();

   }else if(strstr(comando,"exec")!=NULL){
        exe(comando);getchar();

   }else if(strstr(comando,"#")!=NULL){
       //comentario

   }else{printf(" %s comando no definido\n", comando);}
}
}
void exe(char comando[300]){
char *path;
    char *token;
    char temporal[300];
    char comandof[300];
    path=NULL;
    temporal[0]='\0';
    comandof[0]='\0';
token=strtok(comando," \n\\");
	 token=strtok(NULL, " \n\\");
int bandera=0;//la bandera nos ayuda con el path con comillas

		 path=(char*)malloc(300);
		 if(token!=NULL)
		{ strcpy(path,token);
            FILE *archivo;
            archivo=fopen(path,"r");
            if(archivo){
            while (feof(archivo) == 0)
            {temporal[0]='\0';

                fgets(temporal, 300, archivo);
    if(bandera){
    strcat(comandof,temporal);

    }else
    strcpy(comandof,temporal);
    if(strstr(temporal,"\\")!=NULL){

        bandera=1;

    }else bandera=0;


   int i=0;


   for(;i<strlen(comandof); i++){
       comandof[i]=tolower(comandof[i]);
   }
   if(bandera==0)
  { printf("%s\n",comandof);
    clasificar(comandof);
  }






   }fclose(archivo);
            }else printf("no se pudo abrir el archivo\n");
        }
        else
        printf("error en exec\n");
}
void montar(char comando[300]){
char *path;
    char *token;
    char *nombre;
//valor por default

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
				 path[strlen(token)]='\0';

				 }}
				  else if(strstr(token,"-name::")!=NULL){
			int le=strlen(token);
			  nombre=(char*) malloc (16);

			 int i=8;
			 int j=0;
			 for(; i<le-1;i++){

				 nombre[j]=token[i];
				 j++;
                }
                for(;j<16;j++){

                nombre[j]='\0';
                }
		 }
		 ///****************************
		 else if(strstr(token,"-path::")!=NULL){
			 int len=strlen(token);

			   path=(char*) malloc (1000);//inicializamor el char para path
			  if(token[len-1]=='\"')//no es path con comillas
			 {  bandera=0;

				 int i=8;
			 int j=0;
			 for(;i<len-1;i++){
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
				 path[len]='\0';


			 }


		 }
		 else{
			 printf(" %s error: instruccion no reconocido\n",token);
			 }

		  token=strtok(NULL, " \n\\");

		}
		if(path!=NULL && nombre!=NULL){
		//buscar el path en la lista de cabezeras
		printf("%s ",nombre);
		int j=0;
		int encontrado=0;
		char sdv[5]="vd";
		int esta=0;
    FILE *fichero;
    fichero=fopen(path,"rb");
    if(!fichero){
        printf("el disco no existe\n");
    }else{
        MBR aux;
        fseek (fichero, 0, SEEK_SET);
        fread (&aux, sizeof(aux), 1, fichero);
        int i=0;

        for(;i<4;i++){
        if(aux.part_primaria[i].part_status==1){
            if(strstr(aux.part_primaria[i].part_name,nombre)
            )
              {  esta=1;
                i=4;

            break;
            }


        }
        }
        if(esta==0){
        printf("la particion no existe en el disco\n");
        }
    fclose(fichero);
    }
    if(esta==1){
		while(encontrado!=1 && j<10){

		if(cabezeras[j].identificador[0]=='\0'){

        strcpy(cabezeras[j].identificador,path);
        //sprintf(cabezeras[j].idcaracter,"%d",j+1);
       sdv[2]=listaletras[j];
       sdv[3]='\0';
    strcpy(cabezeras[j].idcaracter,sdv);//se agrego cabezera el path
	//	printf("%s %s\n",cabezeras[j].idcaracter,cabezeras[j].identificador);

        ///agregar valores a la tabla de control de particiones
        sdv[2]=listaletras[j];
        sdv[3]='1';
        sdv[5]='\0';
       strcpy(tablaparticion[0][j].identificador,nombre);
       strcpy(tablaparticion[0][j].idcaracter,sdv);
    printf("%s %s\n",cabezeras[j].identificador,tablaparticion[0][j].idcaracter );
		encontrado=1;


		}else if(strcmp(cabezeras[j].identificador,path)==0){
		//solo agregarmos a tabla de particiones
		int i=0;
        for(;i<10;i++){
        if(tablaparticion[i][j].identificador[0]=='\0'){
            strcpy(tablaparticion[i][j].identificador,nombre);
             sdv[2]=listaletras[j];
             char num[5];

            sprintf(num,"%d",i+1);
            strcat(sdv,num);
             strcpy(tablaparticion[i][j].idcaracter,sdv);
             printf("tabla %s\n",tablaparticion[i][j].idcaracter);
            i=10;
        }

        }
		encontrado=1;
		}


        j++;
		}

}
		}
else{
int j=0;
int i=0;
for(;j<10;j++){
for(;i<10;i++){
if(cabezeras[j].identificador[0]!='\0')
   {
    if(tablaparticion[i][j].identificador[0]!='\0'){
    printf("id:: %s path::%s nombre:: %s\n",tablaparticion[i][j].idcaracter,cabezeras[j].identificador,tablaparticion[i][j].identificador);
    }
    }
}
}


}


}
void crear_reporte(char comando[300]){

    char *id;
    char *ruta;
    char *path;
    char *token;
    char *nombre;
//valor por default
id=NULL;
ruta=NULL;
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
	else if(strstr(token,"-id")!=NULL){

		 int len=strlen(token);
		 int i=5;//tamaño del token -size::
		 char* temp;//captura el valor de size
		  int j=0;
		   id=(char*) malloc (6);
		  for(;i<len;i++)
		   {id[j]=token[i];
		   j++;
		   }id[j]='\0';




		 }
		 ///********************unit
	else if(strstr(token,"-ruta::")!=NULL){
			 int len=strlen(token);//tamaño total del token +unit:NUMERO
			 int i=7;//tamaño del token -size::
		 char* temp;//captura el valor de size
		  int j=0;
		   ruta=(char*) malloc (100);
		  for(;i<len;i++)
		   {ruta[j]=token[i];
		   j++;
		   }ruta[j]='\0';

		 }
		 else if(strstr(token,"-name::")!=NULL){
			int le=strlen(token);
			  nombre=(char*) malloc (16);
			 int i=7;
			 int j=0;
			 for(; i<le;i++){
				 nombre[j]=token[i];
				 j++;
                }
                for(;j<16;j++){

                nombre[j]='\0';
                }
		 }
		 ///****************************
		 else if(strstr(token,"-path::")!=NULL){
			 int len=strlen(token);

			   path=(char*) malloc (1000);//inicializamor el char para path
			  if(token[len-1]=='\"')//no es path con comillas
			 {  bandera=0;

				 int i=8;
			 int j=0;
			 for(;i<len-1;i++){
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

 //printf("%s\n",path);printf("%s\n",nombre);
 if(strcmp(nombre,"mbr")==0){

 //buscar en tabla de mount

 char chardisco=id[2];

 int j=0;
int columana=-1;
 for(;j<27;j++){
 if(listaletras[j]==chardisco)
    {columana=j;
        j=27;

 }
 }

char dire[100];
dire[0]='\0';
char idsd[16];
idsd[0]='\0';
 if(columana>=0){
 int i=0;
 for(;i<10;i++){
    if(strcmp(tablaparticion[i][columana].idcaracter,id)==0){
    strcpy(dire,cabezeras[columana].identificador);
    strcpy(idsd,tablaparticion[i][columana].identificador);
    }

 }

 }
 if(dire[0]!='\0' && idsd[0]!='\0'){
    FILE *fichero;
    fichero=fopen(dire,"rb");
    if(fichero){
    MBR auxiliar;
     fread (&auxiliar, sizeof(auxiliar), 1, fichero);
    //printf("%d",auxiliar.mbr_tamao);
    fclose(fichero);
    fichero=fopen("/home/emilia/midot.dot","w+");
    fprintf(fichero, "%s",  "digraph G { node [shape=plaintext] a [label=<<table border=\"0\" cellborder=\"1\" cellspacing=\"0\">");
    fprintf(fichero,"%s %d %s ", "<tr><td>",auxiliar.mbr_disk_signature,"</td></tr>");
    fprintf(fichero,"%s %c %s ", "<tr><td>",auxiliar.part_primaria[0].part_type,"</td></tr>");
    fprintf(fichero,"%s %d %s ", "<tr><td>",auxiliar.mbr_tamao,"</td></tr>");
    fprintf(fichero,"%s ", " </table>>];}");
    fclose(fichero);
    system("dot /home/emilia/midot.dot -o /home/emilia/midot.png -Tpng ");
    }
 }else
    printf("error datos no se encuentran montados\n");

 }

}
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
	path=NULL;
	size=0;
	nombre=NULL;
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
			  nombre=(char*) malloc (16);
			 int i=8;
			 int j=0;
			 for(; i<le-1;i++){
				 nombre[j]=token[i];
				 j++;
				 }
for(;j<16;j++){

                nombre[j]='\0';
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
			   if(token[len-1]=='\"')//no es path con comillas
			 {  bandera=0;

				 int i=8;
			 int j=0;
			 for(;i<len-1;i++){
				 path[j]=token[i];//guarda el valor del path
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

	/*	printf("%s\n",path);
		printf("%s\n",nombre);
		printf("%s\n",ddelete);
		printf("%d\n",size);
		printf("%d\n",add);
		printf("%c\n",unit);
		printf("%c\n",type);
		printf("%s\n",fit);*/

	int sizetotal;
int numerobyt=1;
       int s=250;//un byte
		sizetotal=1;
        s*=size;
		if(unit=='k')
			{sizetotal=size*1024; numerobyt=1;
			}
		  else if(unit=='m'){sizetotal=size*1024*1024;
		  s*=100;
		  numerobyt=10;
			  }

		if(tipoaccion==0){
		if(path!=NULL && size!=0 && nombre!=NULL){
		FILE *fichero;

 int nlibres=0;
		fichero=fopen(path,"rb+");
		if(fichero){
		MBR temporal;
		fseek (fichero, 0, SEEK_SET);
		long int posicion;
		  fread (&temporal, sizeof(temporal), 1, fichero);
		  int posmbrf=ftell(fichero)+sizeof(int);
          fseek (fichero, 0, SEEK_SET);

int mibyte[s];
  int i=0;
  int j=0;
  int fin;
char nulo='\0';
        for (;j<sizetotal;j++){
        for (;i<s;i++){

          fseek(fichero,numerobyt*sizeof(mibyte[i]),SEEK_CUR);
        }

}
fin=ftell(fichero);




        i=0;

        for(;i<4;i++){
        if(temporal.part_primaria[i].part_status==0)
        { nlibres++;

        }else{
        printf("Xi=%d,Xf=%d\n",temporal.part_primaria[i].part_start ,temporal.part_primaria[i].part_start+temporal.part_primaria[i].part_size);
        }
        }
        printf("tamaño total=%d\n",temporal.mbr_tamao);
        if(type=='p'){

        if(nlibres==0)
        printf("No se puede crear particion  particones.Primarias=4\n");
        else if(nlibres==1 && temporal.part_ext.part_status==1)
        printf("YA existen 3 primarias y una extendida\n");
        else //buscar posicon
        {


            if(nlibres==4 && temporal.part_ext.part_status!=1){//no existe ingunga particion


                temporal.part_primaria[0].part_status=1;
                temporal.part_primaria[0].part_start=posmbrf;
                temporal.part_primaria[0].part_fit=fit;
                strcpy( temporal.part_primaria[0].part_name,nombre);
                temporal.part_primaria[0].part_size=fin;
                temporal.part_primaria[0].part_type=type;
                 fseek(fichero,0,SEEK_SET);//actualizo mbr
                 fwrite(&temporal, sizeof(temporal),1,fichero);

                 printf("se creo particion en posicion 1 con Exito\n");
            }
            else {
                 int encontrado=0;
             //recorrer todo
             //primero
             if(temporal.part_primaria[0].part_status!=1){//se puede colocar en la primera posicion
             //buscar limite final
                int posf=0;
                int posi=posmbrf;


             if(temporal.part_primaria[1].part_status==1)
             posf=temporal.part_primaria[1].part_start;
             else if(temporal.part_primaria[2].part_status==1)
             posf=temporal.part_primaria[2].part_start;
                 else if(temporal.part_primaria[3].part_status==1)
             posf=temporal.part_primaria[3].part_start;
             else
              {fseek(fichero,0,SEEK_END);
              posf=ftell(fichero);
              }

             if(temporal.part_ext.part_status==1)
                if(temporal.part_ext.part_start<posf)
              posf=temporal.part_ext.part_start+temporal.part_ext.part_size+sizeof(int);

                //calcular si el espacio es suficiente
                if(posi+fin<posf){

                temporal.part_primaria[0].part_status=1;
                temporal.part_primaria[0].part_start=posmbrf;
                temporal.part_primaria[0].part_fit=fit;
                strcmp(temporal.part_primaria[0].part_name,nombre);
                temporal.part_primaria[0].part_size=fin;
                temporal.part_primaria[0].part_type=type;
                 fseek(fichero,0,SEEK_SET);//actualizo mbr
                 fwrite(&temporal, sizeof(temporal),1,fichero);
                printf("se creo particion en posicion 1 con Exito\n");

                encontrado=1;

                }else
                encontrado=0;

             }if(temporal.part_primaria[1].part_status!=1 && encontrado==0){//se puede colocar en la primera posicion
             //buscar limite final

                int posf=0;
                int posi=temporal.part_primaria[0].part_start+temporal.part_primaria[0].part_size+sizeof(int);


             if(temporal.part_primaria[2].part_status==1)
             posf=temporal.part_primaria[2].part_start;
             else if(temporal.part_primaria[3].part_status==1)
             posf=temporal.part_primaria[3].part_start;

             else
              {fseek(fichero,0,SEEK_END);

              posf=ftell(fichero);
              }

             if(temporal.part_ext.part_status==1)
                if(temporal.part_ext.part_start<posf)
                posf=temporal.part_ext.part_start+temporal.part_ext.part_size+sizeof(int);

                //calcular si el espacio es suficiente
                if(posi+fin<posf){

                temporal.part_primaria[1].part_status=1;
                temporal.part_primaria[1].part_start=posi;
                temporal.part_primaria[1].part_fit=fit;
                strcpy(temporal.part_primaria[1].part_name,nombre);
                temporal.part_primaria[1].part_size=fin;
                temporal.part_primaria[1].part_type=type;

                 fseek(fichero,0,SEEK_SET);//actualizo mbr
                 fwrite(&temporal, sizeof(temporal),1,fichero);

                 printf("se creo particion en posicion 2 con Exito\n");
                encontrado=1;

                }else{
                encontrado=0;

             }



            }if(temporal.part_primaria[2].part_status!=1 && encontrado==0){//se puede colocar en la primera posicion
             //buscar limite final

                int posf=0;
                int posi=temporal.part_primaria[1].part_start+temporal.part_primaria[1].part_size+sizeof(int);


             if(temporal.part_primaria[3].part_status==1)
             posf=temporal.part_primaria[3].part_start;


             else
              {fseek(fichero,0,SEEK_END);

              posf=ftell(fichero);
              }

             if(temporal.part_ext.part_status==1)
                if(temporal.part_ext.part_start<posf)
                posf=temporal.part_ext.part_start+temporal.part_ext.part_size+sizeof(int);

                //calcular si el espacio es suficiente
                if(posi+fin<posf){

                temporal.part_primaria[2].part_status=1;
                temporal.part_primaria[2].part_start=posi;
                temporal.part_primaria[2].part_fit=fit;
                strcpy(temporal.part_primaria[1].part_name,nombre);
                temporal.part_primaria[2].part_size=fin;
                temporal.part_primaria[2].part_type=type;

                 fseek(fichero,0,SEEK_SET);//actualizo mbr
                 fwrite(&temporal, sizeof(temporal),1,fichero);

                 printf("se creo particion en posicion 3 con Exito\n");
                encontrado=1;

                }else{
                encontrado=0;

             }



            }
 ////___ultima posicion______________________________
        if(temporal.part_primaria[3].part_status!=1 && encontrado==0){//se puede colocar en la primera posicion
             //buscar limite final

                int posf=0;
                int posi=temporal.part_primaria[2].part_start+temporal.part_primaria[2].part_size+sizeof(int);

            fseek(fichero,0,SEEK_END);

              posf=ftell(fichero);


             if(temporal.part_ext.part_status==1)
                if(temporal.part_ext.part_start<posf)
                posf=temporal.part_ext.part_start+temporal.part_ext.part_size+sizeof(int);

                //calcular si el espacio es suficiente
                if(posi+fin<posf){

                temporal.part_primaria[3].part_status=1;
                temporal.part_primaria[3].part_start=posi;
                temporal.part_primaria[3].part_fit=fit;
                strcpy(temporal.part_primaria[3].part_name,nombre);
                temporal.part_primaria[3].part_size=fin;
                temporal.part_primaria[3].part_type=type;

                 fseek(fichero,0,SEEK_SET);//actualizo mbr
                 fwrite(&temporal, sizeof(temporal),1,fichero);

                 printf("se creo particion en posicion 4 con Exito\n");
                encontrado=1;

                }else{
                encontrado=0;

                printf(" La particion primaria no se pudo crear no hay espacio suficiente\n");
             }



            }
        }


        }
        }



        }

		fclose(fichero);
		}

		}

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
			  nombre=(char*) malloc (16);
			 int i=7;
			 int j=0;
			 for(; i<le;i++){
				 nombre[j]=token[i];
				 j++;
                }
                for(;j<16;j++){

                nombre[j]='\0';
                }
		 }
		 ///****************************
		 else if(strstr(token,"-path::")!=NULL){
			 int len=strlen(token);

			   path=(char*) malloc (1000);//inicializamor el char para path
			  if(token[len-1]=='\"')//no es path con comillas
			 {  bandera=0;

				 int i=8;
			 int j=0;
			 for(;i<len-1;i++){
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
printf("%c\n",unidad);  printf("%d\n",size);  printf("%s\n",path);printf("%s\n",nombre);
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
			 // master.mbr_fecha_creacion=asctime(localtime(&tt));
			  strcpy(master.mbr_fecha_creacion,asctime(localtime(&tt)));
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
//getchar();
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
   }else if(strstr(comando,"rep")!=NULL){
    if(comando[0]=='r'&& comando[1]=='e'&& comando[2]=='p'){
            crear_reporte(comando);}

   }else if(strstr(comando,"mount")!=NULL){
     montar(comando);

   }else if(strstr(comando,"exec")!=NULL){
        exe(comando);

   }else if(strstr(comando,"#")!=NULL){
       //comentario

   }else{printf("comando no definido\n");}


}

int main()
{
int i,j;
i=j=0;
for(;i<10;i++){
for(;j<10;j++){

tablaparticion[i][j].idcaracter[0]='\0';
tablaparticion[i][j].identificador[0]='\0';
}
}
i=0;
for(;j<10;j++){

cabezeras[i].idcaracter[0]='\0';
cabezeras[i].identificador[0]='\0';
}
int seguir=1;
while(seguir==1){

if(seguir==1){
     comandos();
     }
printf("¿Desea ingresar comandos? 1)si 2)no\n");
scanf("%d",&seguir);
getchar();
     }
    return 0;
}
