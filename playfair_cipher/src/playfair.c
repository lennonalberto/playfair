#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define NOME_ARQUIVO_SAIDA "output.txt"


char* leEntrada(char* nomeArquivo);
void salvaArquivo(char* stringS);
void criaMatriz(char* chave);
int letraNova(char letra);
int limpaTexto(char *inString, char *outLimpa);
void encriptar(char *inString,char *outString, int cont);
void decriptar(char *inString,char *outString, int cont);
void modoUso();

char alfabeto[] = "abcdefghiklmnopqrstuvwxyz";

char matriz[7][7] ={
  {'0','0','0','0','0','0','0'},
	{'0','0','0','0','0','0','0'},
	{'0','0','0','0','0','0','0'},
	{'0','0','0','0','0','0','0'},
	{'0','0','0','0','0','0','0'},
	{'0','0','0','0','0','0','0'},
	{'0','0','0','0','0','0','0'}
};

int main(int argc, char**argv) {

  char *nomeArquivo, *stringOriginal, *stringLimpa, *semCifra, *chave;
  char *stringSaida, *modo;
	int i = 0, num;

  if (argc > 3) {
    nomeArquivo = argv[1];
		chave = argv[2];
		modo = argv[3];
		criaMatriz(chave);
    stringOriginal = leEntrada(nomeArquivo);
    stringLimpa = calloc(2*strlen(stringOriginal), sizeof(char));
    stringSaida = calloc(2*strlen(stringOriginal), sizeof(char));
    num = limpaTexto(stringOriginal, stringLimpa);
    if(modo[0] == '0'){
    	encriptar(stringLimpa, stringSaida, num);
    	printf("Texto Claro  : ");
   		puts(stringLimpa);
   		printf("Texto Cifrado: ");
   		puts(stringSaida);
    	
    }
    else {
    	decriptar(stringLimpa, stringSaida, num);
     	printf("Texto Cifrado  : ");
   		puts(stringLimpa);
   		printf("Texto Decifrado: ");
   		puts(stringSaida);
   	}
   	salvaArquivo(stringSaida);
	}
  else {
  modoUso();
  return 1;
	}

  return 0;
}

void encriptar(char *inString,char *outString, int cont){
	
	int i, letra1, letra2, x, y;
	int letra1x, letra1y, letra2x, letra2y;

	for(i = 0; i < cont; i=i+2){
		
		letra1 = inString[i];
		for(x = 1; x < 6; x++)
			for(y = 1; y < 6; y++)
				if(letra1 == matriz[x][y]){
					letra1x = x;
					letra1y = y;
				}
			
		letra2 = inString[i+1];
		for(x = 1; x < 6; x++)
			for(y = 1; y < 6; y++)
				if(letra2 == matriz[x][y]){
					letra2x = x;
					letra2y = y;
				}		
		
		if(letra1x == letra2x){
			outString[i]=matriz[letra1x][letra1y+1];
			outString[i+1]=matriz[letra2x][letra2y+1];
		}	
		else if(letra1y == letra2y){
			
			outString[i]=matriz[letra1x+1][letra1y];
			outString[i+1]=matriz[letra2x+1][letra2y];
		}	
		else{
			outString[i]=matriz[letra1x][letra2y];
			outString[i+1]=matriz[letra2x][letra1y];
		
		}
	
	}
	
}

void decriptar(char *inString,char *outString, int cont){
	
	int i, letra1, letra2, x, y;
	int letra1x, letra1y, letra2x, letra2y, cont2;
	
	cont2 = strlen(inString);

	for(i = 0; i < cont2; i=i+2){
		
		letra1 = inString[i];
		for(x = 1; x < 6; x++)
			for(y = 1; y < 6; y++)
				if(letra1 == matriz[x][y]){
					letra1x = x;
					letra1y = y;
				}
			
		letra2 = inString[i+1];
		for(x = 1; x < 6; x++)
			for(y = 1; y < 6; y++)
				if(letra2 == matriz[x][y]){
					letra2x = x;
					letra2y = y;
				}		
		
		if(letra1x == letra2x){
			outString[i]=matriz[letra1x][letra1y-1];
			outString[i+1]=matriz[letra2x][letra2y-1];
		}	
		else if(letra1y == letra2y){
			
			outString[i]=matriz[letra1x-1][letra1y];
			outString[i+1]=matriz[letra2x-1][letra2y];
		}	
		else{
			outString[i]=matriz[letra1x][letra2y];
			outString[i+1]=matriz[letra2x][letra1y];
		
		}
	
	}
	
}

int limpaTexto(char *inString, char *outString)
{
	int i = 0, numChars = 0;
	char letra;
	
	while(i<strlen(inString)-1){
	
		letra = tolower((unsigned char) inString[i]);
		if(letra == 'j')
			letra = 'i';
		if (outString[numChars-1] == letra && numChars%2 != 0){
			outString[numChars] = 'x';
			numChars++;
		}
		
		else{
			if (letra >= 'a' && letra <= 'z'){
				outString[numChars] = letra;
				numChars++;				
			}
			i++;
		}
	}
	
	if(numChars%2 != 0)
		outString[numChars++] = 'x';
	
	
	//printf("\nnumChars=%d\n",numChars);
	
	return(numChars);
}


void criaMatriz(char* chave){
		
	int x, y, i;
	
	for(i = 0; i < strlen(chave); i++){
	 chave[i] = tolower((unsigned char) chave[i]);
	}
		
	i = 0;
	for(x = 1; x < 6; x++){
		for(y = 1; y < 6; y++){
			aqui:
			if(letraNova(chave[i])){
				matriz[x][y] = chave[i];
				i++;
			}
			else{
			 i++;
			 if(i < strlen(chave))
				 goto aqui;
			}
			if(i >= strlen(chave))
			break;
		}
		if(i >= strlen(chave))
		break;
	}
	
	
	i=0;
	for(x = 1; x < 6; x++){
		for(y = 1; y < 6; y++){
				if(matriz[x][y] == '0')
				aqui2:
				if(letraNova(alfabeto[i])){
					matriz[x][y] = alfabeto[i];
					i++;
				}
				else{		
					i++;	
					goto aqui2;
				}
		}
	}	
	
	for(y = 1; y < 6; y++)
		matriz[6][y] = matriz[1][y];
	
	for(x = 1; x < 6; x++)
		matriz[x][6] = matriz[x][1];
		
	for(y = 1; y < 6; y++)
		matriz[0][y] = matriz[5][y];
	
	for(x = 1; x < 6; x++)
		matriz[x][0] = matriz[x][5];
		
	
	
	for(x = 1; x < 6; x++){
		for(y = 1; y < 6; y++){
				printf("%c",matriz[x][y]); 
		}
		printf("\n"); 
	}
}



int letraNova(char letra){
	int x, y;
	for(x = 1; x < 6; x++)
		for(y = 1; y < 6; y++)
			if(matriz[x][y] == letra)
					return(0);
		
	return(1);
}




char* leEntrada(char* nomeArquivo) {
  FILE* descritorArquivo;
  char* conteudoArquivo;
  long tamanhoArquivo;

  descritorArquivo = fopen(nomeArquivo, "r");

  if (descritorArquivo == NULL ) {
    printf("Erro na abertura de arquivo %s\n", nomeArquivo);
    modoUso();
    exit(1);
  }

  fseek(descritorArquivo, 0, SEEK_END);
  tamanhoArquivo = ftell(descritorArquivo);
  rewind(descritorArquivo);
  conteudoArquivo = calloc(1, tamanhoArquivo);
  fread(conteudoArquivo, 1, tamanhoArquivo, descritorArquivo);
  fclose(descritorArquivo);

  return conteudoArquivo;
}

void salvaArquivo(char* stringS) {

	FILE* arquivoSaida;
	char *nomeArquivoSaida;
 
	nomeArquivoSaida = NOME_ARQUIVO_SAIDA;
	arquivoSaida = fopen(nomeArquivoSaida, "w");
  fprintf(arquivoSaida, "%s", stringS);
	fclose(arquivoSaida);

}

void modoUso() {
  printf("\nCertifique-se de que utilizou da seguinte maneira:\n");
  printf("$ ./playfair input.txt chave modo\n");
  printf("onde \'input.txt\' é um arquivo com a mensagem original.\n");
  printf("modo: 0 - Encript; 1 - Decript.\n\n");
  /*printf(
      "Além disso, no diretório corrente ao executável\ndeve existir um arquivo chamado \'%s\'\n",
      NOME_ARQUIVO_COMPARACAO);
  printf("com a mensagem original para fins de comparação.\n");*/

}
