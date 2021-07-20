
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "definitions.h"

void free_vetor(Cartas* libera){
  free(libera->value);
  free(libera->suit);
  free(libera);
}

void free_matriz(Cartas** matriz){
  for(int y=0; y < quant_jogo_nosso; y++){
    free(matriz[y]);
  }
  free(matriz);
}

//-> LÊ A ENTRADA E REMOVE O "\n" E COLOCA "\0" NO FINAL DA STRING
void readline(char *line) {
  fgets(line, MAX_LINE, stdin);
  int l = strlen(line) - 1;
  if(line[l] == '\n') {
    line[l] = '\0';
  }
}

int muda_int(char string){
  int v=0;
  switch(string){
    case 'Z': v = um; break;
		case '2': v = dois; break;
		case '3': v = tres; break;
		case '4': v = quatro; break;
		case '5': v = cinco; break;
    case '6': v = seis; break;
    case '7': v = sete; break;
    case '8': v = oito; break;
    case '9': v = nove; break;
    case '1': v = dez; break;
    case 'J': v = valete; break;
    case 'Q': v = rainha; break;
    case 'K': v = rei; break;
    case 'A': v = as; break;
	}
  return v;
}

void ordenar(Cartas* mao, int t){
  Cartas aux;
  //-> VARIÁVEL DE CONTROLE DO LOOP
  int contagem=0;
  //-> VARIÁVEL PARA CONTROLAR AS POSIÇÕES DO VETOR
  int i = 0, anterior = 0;
  //-> VETOR DOS NAIPES
  char *vet[] = {"♥", "♦", "♣", "♠"};
  //-> ORDENA POR NAIPE
  for(int a=0; a < 4; a++){
    while(contagem < t){
      if(strcmp(mao[contagem].suit, vet[a]) == 0){
        aux = mao[contagem];
        mao[contagem] = mao[i];
        mao[i] = aux;
        //-> PRÓXIMA POSIÇÃO DO VETOR
        i++;
      }
      //-> PRÓXIMA CARTA
      contagem++;
    }
    //-> ORDENA POR NÚMERO
    int val1, val2;
    //-> FUNÇÃO PARA TRANSFORMAR DE STRING PARA INTEIRO
    for(int b = anterior; b < i; b++){
      //-> CARTA PARA COMPARAR
      val1 = muda_int(mao[b].value[0]);
      for(int c = b+1; c < i; c++){
        //-> COMPARAR COM O RESTO DAS CARTAS DO JOGO
        val2 = muda_int(mao[c].value[0]);
        if(val2 < val1){
          aux = mao[b];
          mao[b] = mao[c];
          mao[c] = aux;
					val1 = val2;
        }
      }
    }
    //-> COMO AS PRIMEIRAS CARTAS JÁ FORAM COLOCADAS CORRETAMENTE, É PARA COMECAR DE ONDE AONDE NÃO FOI COLOCADO
    anterior = i;
    //-> COMO AS PRIMEIRAS CARTAS JÁ FORAM COLOCADAS CORRETAMENTE, É PARA COMECAR DE ONDE AONDE NÃO FOI COLOCADO
    contagem = i;
  }
}

void fuc_mesa_adv(Cartas** mesa_adversario, int quant_jogo, int quant, char* separa, char line[MAX_LINE], int* cartas_mesa_adv){ 
  while(separa != NULL){
    //-> LÊ O NÚMERO DA CARTA
    sscanf(separa, "%c", &mesa_adversario[quant_jogo][quant].value[0]);
    //-> REALOCA O VETOR DE CARTAS
    mesa_adversario[quant_jogo] = realloc(mesa_adversario[quant_jogo], sizeof(Cartas) * (quant + 1)); 
    //-> SE O VALOR DA CARTA TENHA APENAS UM CARACTERE
    if(mesa_adversario[quant_jogo][quant].value[0] != '1'){
      sscanf(separa, "%c%s", &mesa_adversario[quant_jogo][quant].value[0], mesa_adversario[quant_jogo][quant].suit);
    }
    //-> CASO O VALOR DA CARTA TENHA MAIS DE UM CARACTERE
    else if(mesa_adversario[quant_jogo][quant].value[0] == '1'){
      sscanf(line, "%c%c%s", &mesa_adversario[quant_jogo][quant].value[0], &mesa_adversario[quant_jogo][quant].value[1], mesa_adversario[quant_jogo][quant].suit);
    }
    //-> ATUALIZAÇÃO DO LOOP
    separa = strtok(NULL, " ");
    //-> PRÓXIMA CARTA
    quant++;
    //-> SE CHEGAR NO "]" DAR UM BREAK
    if(strlen(separa) == 1){
      break;
    }
  }
  //-> GUARDAR O TAMANHO DO QUANT PARA SABER A QUANTIDADE DE CARTAS EM CASA JOGO
  cartas_mesa_adv[quant_jogo] = quant;
}

Cartas* tira_carta_mao(Cartas* mao, int tam_mao, int cont_aux_mao){
  Cartas *aux_mao = malloc(sizeof(Cartas));
  int ind=0;

  aux_mao = realloc(aux_mao, sizeof(Cartas) * ((tam_mao - cont_aux_mao) + 1));

  for(int h=0; h < tam_mao; h++){
    if(mao[h].value[0] != 'x'){
      aux_mao[ind] = mao[h];
      ind++;
    }
  }
  
  for(int i=0; i < ind; i++){
    mao[i] = aux_mao[i];
  }
  mao = realloc(mao, sizeof(Cartas) * ((tam_mao - cont_aux_mao) + 1));

  return mao;
}

int contagem_x(Cartas* mao, int tam_mao, int cont_aux_mao){
  for(int g=0; g < tam_mao; g++){
    if(mao[g].value[0] == 'x'){
      cont_aux_mao++;
    }
  }
  return cont_aux_mao;
}
