#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define NOME_ARQUIVO_SAIDA "output.txt"


char* leEntrada(char* nomeArquivo);
void criaMatriz(char* chave);
int letraNova(char letra);
void decriptar(char *inString,char *outString);
void newDecipher(char* decipheredText, char* cipheredText, char* key);

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

  char *nomeArquivo, *stringOriginal, *chave;
  char *stringSaida;
	int i = 0, num;

  if (argc > 2) {
    nomeArquivo = argv[1];
		chave = argv[2];
	
	  stringOriginal = leEntrada(nomeArquivo);
	  stringSaida = calloc(2*strlen(stringOriginal), sizeof(char));
	  
  	newDecipher(stringSaida, stringOriginal, chave);
  	
  	printf("StringOriginal: %s \n", stringOriginal );
   	printf("stringSaida   : %s \n", stringSaida);
  
  }  	
   

  return 0;
}


void newDecipher(char* decipheredText, char* cipheredText, char* key){
	criaMatriz(key);
	decriptar(cipheredText, decipheredText);

}


void decriptar(char *inString,char *outString){
	
	int i, letra1, letra2, x, y;
	int letra1x, letra1y, letra2x, letra2y, cont;
	
	cont = strlen(inString);

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

    exit(1);
  }

  fseek(descritorArquivo, 0, SEEK_END);
  tamanhoArquivo = ftell(descritorArquivo)-1;
  rewind(descritorArquivo);
  conteudoArquivo = calloc(1, tamanhoArquivo);
  fread(conteudoArquivo, 1, tamanhoArquivo, descritorArquivo);
  fclose(descritorArquivo);

  return conteudoArquivo;
}


