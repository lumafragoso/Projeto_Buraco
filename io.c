#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "definitions.h"
#include "funcs.h"

Cartas* tratamento_leitura(FILE* log, char line[MAX_LINE], Cartas* mao){
  char* pedaco;
  int tam=1;
  fprintf(log, "Eu li: ");
  //-> PARA TIRAR O PRIMEIRO ESPAÇO
  pedaco = strtok(line, " ");
  //-> PARA TIRAR O COLCHETE E O SEGUNDO ESPAÇO
  pedaco = strtok(NULL, " ");
  for(int i=0; i < 11; i++){
    sscanf(pedaco, "%c", &mao[i].value[0]);
    mao = realloc(mao, sizeof(Cartas) * (tam + 1));
    tam++;
    //-> SE O VALOR DA CARTA TENHA APENAS UM CARACTERE
    if(mao[i].value[0] != '1'){
      sscanf(pedaco, "%c%s", &mao[i].value[0], mao[i].suit);
      fprintf(log, "%c%s ", mao[i].value[0], mao[i].suit);
    }
    //-> CASO O VALOR DA CARTA TENHA MAIS DE UM CARACTERE
    else if(mao[i].value[0] == '1'){
      sscanf(pedaco, "%c%c%s", &mao[i].value[0], &mao[i].value[1], mao[i].suit);
      fprintf(log, "%c%c%s ", mao[i].value[0], mao[i].value[1], mao[i].suit);
    }
    //-> ATUALIZAÇÃO DO LOOP
    pedaco = strtok(NULL, " ");
  }
  fprintf(log, "\n");
  return mao;
}

void print_mao(Cartas* mao, FILE* log, int tamanho){
  for(int i = 0; i < tamanho; i++){
    if(mao[i].value[0] == '1'){
      fprintf(log, "%c%c%s ", mao[i].value[0], mao[i].value[1], mao[i].suit);
    }
    else{
      fprintf(log, "%c%s ", mao[i].value[0], mao[i].suit);
    }
  }
  fprintf(log, "\n");
}

void add_lixo(Cartas* lixo, FILE* log, char line[MAX_LINE], int cont){
  sscanf(line, "%c", &lixo[cont-1].value[0]);
  //-> SE O VALOR DA CARTA TENHA APENAS UM CARACTERE
  if(lixo[cont-1].value[0] != '1'){
    sscanf(line, "%c%s", &lixo[cont-1].value[0], lixo[cont-1].suit);
    fprintf(log, "LIXO:%c%s\n", lixo[cont-1].value[0], lixo[cont-1].suit);
  }
  //-> CASO O VALOR DA CARTA TENHA MAIS DE UM CARACTERE
  else if(lixo[cont-1].value[0] == '1'){
    sscanf(line, "%c%c%s", &lixo[cont-1].value[0], &lixo[cont-1].value[1], lixo[cont-1].suit);
    fprintf(log, "LIXO:%c%c%s\n", lixo[cont-1].value[0], lixo[cont-1].value[1], lixo[cont-1].suit);
  }
}

void print_GET_DISCARD(Cartas* mao, int j, FILE* log){
  //-> CASO NAO TENHA 10
  if(mao[j].value[0] != '1' && mao[j+1].value[0] != '1'){
    printf("GET_DISCARD [ %c%s %c%s ]\n", mao[j].value[0], mao[j].suit, mao[j+1].value[0], mao[j+1].suit);
    fprintf(log,"GET_DISCARD [ %c%s %c%s ]\n", mao[j].value[0], mao[j].suit, mao[j+1].value[0], mao[j+1].suit);
  }
  //-> CASO A PRIMEIRA SEJA 10
  else if(mao[j].value[0] == '1' && mao[j+1].value[0] != '1'){
    printf("GET_DISCARD [ %c%c%s %c%s ]\n", mao[j].value[0], mao[j].value[1], mao[j].suit, mao[j+1].value[0], mao[j+1].suit);
    fprintf(log,"GET_DISCARD [ %c%c%s %c%s ]\n", mao[j].value[0], mao[j].value[1], mao[j].suit, mao[j+1].value[0], mao[j+1].suit);
  }
  //-> CASO A ULTIMA SEJA 10
  else if(mao[j].value[0] != '1' && mao[j+1].value[0] == '1'){
    printf("GET_DISCARD [ %c%s %c%c%s ]\n", mao[j].value[0], mao[j].suit, mao[j+1].value[0],mao[j+1].value[1], mao[j+1].suit);
    fprintf(log,"GET_DISCARD [ %c%s %c%c%s ]\n", mao[j].value[0], mao[j].suit, mao[j+1].value[0],mao[j+1].value[1], mao[j+1].suit);
  }
}

Cartas* add_lixo_mao(Cartas* mao, Cartas* lixo, FILE* log, int tam_lixo, int tam_mao, int j){
  int aux_lixo=0;
  print_GET_DISCARD(mao, j, log);
  //-> REALOCA A MÃO PARA O TAMANHO LIXO + MAO
  mao = realloc(mao, sizeof(Cartas) * (tam_lixo + tam_mao + 1));
  for(int c = tam_mao; c < (tam_mao + tam_lixo); c++){
    mao[c] = lixo[aux_lixo];
    aux_lixo++;
  }
  return  mao;
}

void fuc_nossa_mesa(Cartas **nossa_mesa, Cartas* mao, FILE* log, int quant_jogo_nosso, int quant_cartas_nosso, int a){
  if (quant_cartas_nosso == 0){
    nossa_mesa[quant_jogo_nosso] = NULL;
  }
  //-> REALOCA O VETOR DE CARTAS
  nossa_mesa[quant_jogo_nosso] = realloc(nossa_mesa[quant_jogo_nosso], sizeof(Cartas) * (quant_cartas_nosso+1));

  //-> ATRIBUI A CARTA AO VETOR DE CARTAS DA MESA
  nossa_mesa[quant_jogo_nosso][quant_cartas_nosso] = mao[a];
  
  //-> SE O VALOR DA CARTA TENHA APENAS UM CARACTERE
  if(nossa_mesa[quant_jogo_nosso][quant_cartas_nosso].value[0] != '1'){
    fprintf(log, "NOSSA MESA:%c%s\n", nossa_mesa[quant_jogo_nosso][quant_cartas_nosso].value[0], nossa_mesa[quant_jogo_nosso][quant_cartas_nosso].suit);
  }
  //-> CASO O VALOR DA CARTA TENHA MAIS DE UM CARACTERE
  else if(nossa_mesa[quant_jogo_nosso][quant_cartas_nosso].value[0] == '1'){
    fprintf(log, "NOSSA MESA:%c%c%s\n", nossa_mesa[quant_jogo_nosso][quant_cartas_nosso].value[0], nossa_mesa[quant_jogo_nosso][quant_cartas_nosso].value[1], nossa_mesa[quant_jogo_nosso][quant_cartas_nosso].suit);
  }
}

Cartas** abaixa_mesa(Cartas **nossa_mesa, Cartas* mao, FILE* log, int quant_jogo_nosso, int quant_cartas_nosso, int* cartas_mesa_nossa, int a, int b, int c){
  //-> REALOCA AS POSIÇÕES DA MESA DE ACORDO COM A QUANTIDADE DE JOGOS
  nossa_mesa = realloc(nossa_mesa, sizeof(Cartas*) * (quant_jogo_nosso + 1));

  //-> PASSA A PRIMEIRA CARTA
  fuc_nossa_mesa(nossa_mesa, mao, log, quant_jogo_nosso, quant_cartas_nosso, a);
  quant_cartas_nosso++;
  mao[a].value[0] = 'x'; 

  //-> PASSA A SEGUNDA CARTA
  fuc_nossa_mesa(nossa_mesa, mao, log, quant_jogo_nosso, quant_cartas_nosso, b);
  quant_cartas_nosso++;
  mao[b].value[0] = 'x'; 

  //-> PASSA A TERCEIRA CARTA
  fuc_nossa_mesa(nossa_mesa, mao, log, quant_jogo_nosso, quant_cartas_nosso, c);
  mao[c].value[0] = 'x'; 

  //-> VETOR COM A QUANTIDADE DE CARTAS EM CADA JOGO
  cartas_mesa_nossa[quant_jogo_nosso] = quant_cartas_nosso;
  
  return nossa_mesa;
}

void print_total_mesa(Cartas **nossa_mesa, FILE* log, int quant_jogo_nosso, int quant_cartas_nosso,int *cartas_mesa_nossa){
  fprintf(log, "====== TOTAL NOSSA MESA ======:\n");
    for(int a = 0; a < quant_jogo_nosso; ++a){
      for(int b = 0; b < cartas_mesa_nossa[a]+1; ++b){
        if(nossa_mesa[a][b].value[0] != '1'){
          fprintf(log, "%c%s ", nossa_mesa[a][b].value[0], nossa_mesa[a][b].suit);
        }
        else if(nossa_mesa[a][b].value[0] == '1'){
          fprintf(log, "%c%c%s ", nossa_mesa[a][b].value[0], nossa_mesa[a][b].value[1], nossa_mesa[a][b].suit);
        }
      }
      fprintf(log, "\n");
    }
  fprintf(log, "\n");
}

void print_GET_STOCK(Cartas puxada[1], FILE* log, char line[MAX_LINE]){
  //-> ENVIA A AÇÃO PARA PUXAR UMA CARTA
  printf("GET_STOCK\n");
  fprintf(log,"GET_STOCK\n");
  //-> RECEBE A CARTA QUE PUXOU
  readline(line);
      
  if(line[0] != '1'){
    sscanf(line, "%c%s", &puxada[0].value[0], puxada[0].suit);
    fprintf(log, "PUXADA:%c%s\n", puxada[0].value[0], puxada[0].suit);
  }
  //-> CASO O VALOR DA CARTA TENHA MAIS DE UM CARACTERE
  else if(line[0] == '1'){
    sscanf(line, "%c%c%s", &puxada[0].value[0], &puxada[0].value[1], puxada[0].suit);
    fprintf(log, "PUXADA:%c%c%s\n", puxada[0].value[0], puxada[0].value[1], puxada[0].suit);
    }
}

void print_MELD_NEW(Cartas* mao, FILE* log, int a, int b, int c){
  //-> CASO O PRIMEIRO TENHA 10 E OS OUTROS NÃO
  if(mao[a].value[0] == '1' && mao[b].value[0] != '1' && mao[c].value[0] != '1'){
    printf("MELD_NEW [ %c%c%s %c%s %c%s ]\n", mao[a].value[0], mao[a].value[1], mao[a].suit, mao[b].value[0], mao[b].suit, mao[c].value[0], mao[c].suit);
    fprintf(log, "MELD_NEW [ %c%c%s %c%s %c%s ]\n", mao[a].value[0], mao[a].value[1], mao[a].suit, mao[b].value[0], mao[b].suit, mao[c].value[0], mao[c].suit);
  }
  //-> CASO O SEGUNDO TENHA 10 E OS OUTROS NÃO
  else if(mao[a].value[0] != '1' && mao[b].value[0] == '1' && mao[c].value[0] != '1'){
    printf("MELD_NEW [ %c%s %c%c%s %c%s ]\n", mao[a].value[0], mao[a].suit, mao[b].value[0], mao[b].value[1], mao[b].suit, mao[c].value[0], mao[c].suit);
    fprintf(log,"MELD_NEW [ %c%s %c%c%s %c%s ]\n", mao[a].value[0], mao[a].suit, mao[b].value[0], mao[b].value[1], mao[b].suit, mao[c].value[0], mao[c].suit);
  }
  //-> CASO O TERCEIRO TENHA 10 E OS OUTROS NÃO
  else if(mao[a].value[0] != '1' && mao[b].value[0] != '1' && mao[c].value[0] == '1'){
    printf("MELD_NEW [ %c%s %c%s %c%c%s ]\n", mao[a].value[0], mao[a].suit, mao[b].value[0], mao[b].suit, mao[c].value[0], mao[c].value[1], mao[c].suit);
    fprintf(log,"MELD_NEW [ %c%s %c%s %c%c%s ]\n", mao[a].value[0], mao[a].suit, mao[b].value[0], mao[b].suit, mao[c].value[0], mao[c].value[1], mao[c].suit);
  }
  //-> CASO O PRIMEIRO E SEGUNDO TENHAM 10 E O TERCEIRO NÃO
  else if(mao[a].value[0] == '1' && mao[b].value[0] == '1' && mao[c].value[0] != '1'){
    printf("MELD_NEW [ %c%c%s %c%c%s %c%s ]\n", mao[a].value[0], mao[a].value[1], mao[a].suit, mao[b].value[0], mao[b].value[1], mao[b].suit, mao[c].value[0], mao[c].suit);
    fprintf(log,"MELD_NEW [ %c%c%s %c%c%s %c%s ]\n", mao[a].value[0], mao[a].value[1], mao[a].suit, mao[b].value[0], mao[b].value[1], mao[b].suit, mao[c].value[0], mao[c].suit);
  }
  //-> CASO O SEGUNDO E TERCEIRO TENHAM 10 E O PRIMEIRO NÃO
  else if(mao[a].value[0] != '1' && mao[b].value[0] == '1' && mao[c].value[0] == '1'){
    printf("MELD_NEW [ %c%s %c%c%s %c%c%s ]\n", mao[a].value[0], mao[a].suit, mao[b].value[0], mao[b].value[1], mao[b].suit, mao[c].value[0], mao[c].value[1], mao[c].suit);
    fprintf(log,"MELD_NEW [ %c%s %c%c%s %c%c%s ]\n", mao[a].value[0], mao[a].suit, mao[b].value[0], mao[b].value[1], mao[b].suit, mao[c].value[0], mao[c].value[1], mao[c].suit);
  }
  //-> CASO O PRIMEIRO E TERCEIRO TENHAM 10 E O SEGUNDO NÃO
  else if(mao[a].value[0] == '1' && mao[b].value[0] != '1' && mao[c].value[0] == '1'){
    printf("MELD_NEW [ %c%c%s %c%s %c%c%s ]\n", mao[a].value[0], mao[a].value[1], mao[a].suit, mao[b].value[0], mao[b].suit, mao[c].value[0], mao[c].value[1], mao[c].suit);
    fprintf(log,"MELD_NEW [ %c%c%s %c%s %c%c%s ]\n", mao[a].value[0], mao[a].value[1], mao[a].suit, mao[b].value[0], mao[b].suit, mao[c].value[0], mao[c].value[1], mao[c].suit);
  }
  //-> CASO TODOS COM 10
  else if(mao[a].value[0] == '1' && mao[b].value[0] == '1' && mao[c].value[0] == '1'){
    printf("MELD_NEW [ %c%c%s %c%c%s %c%c%s ]\n", mao[a].value[0], mao[a].value[1], mao[a].suit, mao[b].value[0], mao[b].value[1], mao[b].suit, mao[c].value[0], mao[c].value[1], mao[c].suit);
    fprintf(log,"MELD_NEW [ %c%c%s %c%c%s %c%c%s ]\n", mao[a].value[0], mao[a].value[1], mao[a].suit, mao[b].value[0], mao[b].value[1], mao[b].suit, mao[c].value[0], mao[c].value[1], mao[c].suit);
  }
  //-> CASO COM NENHUM 10
  else if(mao[a].value[0] != '1' && mao[b].value[0] != '1' && mao[c].value[0] != '1'){
    printf("MELD_NEW [ %c%s %c%s %c%s ]\n", mao[a].value[0], mao[a].suit, mao[b].value[0], mao[b].suit, mao[c].value[0], mao[c].suit);
    fprintf(log,"MELD_NEW [ %c%s %c%s %c%s ]\n", mao[a].value[0], mao[a].suit, mao[b].value[0], mao[b].suit, mao[c].value[0], mao[c].suit);
  }
}

Cartas** condicoes_MELD_NEW(Cartas* mao, Cartas **nossa_mesa, int cartas_mesa_nossa[50], FILE* log, int tam_mao, int quant_cartas_nosso){
  for(int n = 0; n < tam_mao; n++){
    //-> SE AS CARTAS SÃO DO MESMO NAIPE
    if((n+2 < tam_mao) && (strcmp(mao[n].suit, mao[n+1].suit)==0) && (strcmp(mao[n+1].suit, mao[n+2].suit)==0)){
      //-> SE AS CARTAS ESTÃO EM SEQUENCIA
      if(muda_int(mao[n].value[0])+1 == muda_int(mao[n+1].value[0]) && muda_int(mao[n+1].value[0])+1 == muda_int(mao[n+2].value[0])){
        print_MELD_NEW(mao, log, n, n+1, n+2);

        nossa_mesa = abaixa_mesa(nossa_mesa, mao, log, quant_jogo_nosso, quant_cartas_nosso, cartas_mesa_nossa, n, n+1, n+2);
    
        quant_jogo_nosso++;
        quant_cartas_nosso=0;
        break;
      }
      else if(muda_int(mao[n].value[0]) == as){
        mao[n].value[0] = 'Z';
        if(muda_int(mao[n].value[0])+1 == muda_int(mao[n+1].value[0]) && muda_int(mao[n+1].value[0])+1 == muda_int(mao[n+2].value[0])){
          mao[n].value[0] = 'A';
          print_MELD_NEW(mao, log, n, n+1, n+2);

          nossa_mesa = abaixa_mesa(nossa_mesa, mao, log, quant_jogo_nosso, quant_cartas_nosso, cartas_mesa_nossa, n, n+1, n+2);
    
          quant_jogo_nosso++;
          quant_cartas_nosso=0;
          break;
        }
        if(mao[n].value[0] == 'Z'){
          mao[n].value[0] = 'A';
        }
      }
    }
  }
  return nossa_mesa;
}

void print_MELD_JOIN(Cartas* mao, FILE* log, int a, int ind){
  if(mao[a].value[0] != '1'){
    printf("MELD_JOIN %d [ %c%s ]\n", ind, mao[a].value[0], mao[a].suit);
    fprintf(log, "MELD_JOIN %d [ %c%s ]\n", ind, mao[a].value[0], mao[a].suit);
  }
  else if(mao[a].value[0] == '1'){
    printf("MELD_JOIN %d [ %c%c%s ]\n", ind, mao[a].value[0], mao[a].value[1], mao[a].suit);
    fprintf(log, "MELD_JOIN %d [ %c%c%s ]\n", ind, mao[a].value[0], mao[a].value[1], mao[a].suit);
  }
}

void condicoes_MELD_JOIN(Cartas* mao, FILE* log, Cartas **nossa_mesa, int cartas_mesa_nossa[50], int tam_mao, int quant_jogo_nosso){
  Cartas aux[15];
  int a=0;
  for(int i = 0; i < tam_mao; i++){
    for(int j = 0; j < quant_jogo_nosso; j++){
      if(strcmp(mao[i].suit, nossa_mesa[j][0].suit)==0){  
        //-> PARA ENTRAR NO INICIO DO JOGO
        if((j+1 < quant_jogo_nosso) && muda_int(nossa_mesa[j][0].value[0]) - 1 == muda_int(mao[i].value[0])  && muda_int(nossa_mesa[j+1][0].value[0]) - 2 == muda_int(mao[i].value[0])){
          //-> PASSA OS VALORES JÁ EXISTENTES NO JOGO PARA UM VETOR AUXILIAR
          for(int k = 0; k < cartas_mesa_nossa[j] +1; k++){
            aux[k] = nossa_mesa[j][k];
          }
          //-> AUMENTA A QUANTIDADE DE CARTAS
          cartas_mesa_nossa[j]++;
          //-> REALOCA A QUANTIDADE DE CARTAS DO JOGO NA MESA
          nossa_mesa[j] = realloc(nossa_mesa[j], sizeof(Cartas) * (cartas_mesa_nossa[j] + 1));
          //-> ATRIBUI A PRIMEIRA POSIÇÃO AO VALOR DA CARTA DA MÃO
          nossa_mesa[j][0] = mao[i];
          //-> COLOCA O RESTO DAS CARTAS DO JOGO NA MESA NOVAMENTE
          for(int l = 1; l < cartas_mesa_nossa[j]+1; l++){
            nossa_mesa[j][l]= aux[a];
            a++;
          }
          //-> MARCA A CARTA DA MÃO QUE FOI ABAIXADA NA MESA
          print_MELD_JOIN(mao, log, i, j);
          mao[i].value[0] = 'x';
          a=0;
        } 
        //-> PARA ENTRAR NO FINAL DO JOGO
        else if(muda_int(nossa_mesa[j][cartas_mesa_nossa[j]].value[0]) + 1 == muda_int(mao[i].value[0])){
          //-> AUMENTA A QUANTIDADE DE CARTAS
          cartas_mesa_nossa[j]++;
          //-> REALOCA A QUANTIDADE DE CARTAS DO JOGO NA MESA
          nossa_mesa[j] = realloc(nossa_mesa[j], sizeof(Cartas) * (cartas_mesa_nossa[j] + 2));
          //-> ATRIBUI A PRIMEIRA POSIÇÃO AO VALOR DA CARTA DA MÃO
          nossa_mesa[j][cartas_mesa_nossa[j]] = mao[i];
          //-> MARCA A CARTA DA MÃO QUE FOI ABAIXADA NA MESA
          print_MELD_JOIN(mao, log, i, j);
          mao[i].value[0] = 'x';
        }
        else if(muda_int(mao[i].value[0]) == as){
          mao[i].value[0] = 'Z';
          fprintf(log, "----(3) CARTAS MELD JOIN %c: %d\n",mao[i].value[0], muda_int(mao[i].value[0]));
          if(muda_int(nossa_mesa[j][0].value[0]) - 1 == muda_int(mao[i].value[0])){
            mao[i].value[0] = 'A';
            //-> PASSA OS VALORES JÁ EXISTENTES NO JOGO PARA UM VETOR AUXILIAR
            for(int k = 0; k < cartas_mesa_nossa[j] +1; k++){
              aux[k] = nossa_mesa[j][k];
            }
            //-> AUMENTA A QUANTIDADE DE CARTAS
            cartas_mesa_nossa[j]++;
            //-> REALOCA A QUANTIDADE DE CARTAS DO JOGO NA MESA
            nossa_mesa[j] = realloc(nossa_mesa[j], sizeof(Cartas) * (cartas_mesa_nossa[j] + 1));
            //-> ATRIBUI A PRIMEIRA POSIÇÃO AO VALOR DA CARTA DA MÃO
            nossa_mesa[j][0] = mao[i];
            //-> COLOCA O RESTO DAS CARTAS DO JOGO NA MESA NOVAMENTE
            for(int l = 1; l < cartas_mesa_nossa[j]+1; l++){
              nossa_mesa[j][l]= aux[a];
              a++;
            }
            //-> MARCA A CARTA DA MÃO QUE FOI ABAIXADA NA MESA
            print_MELD_JOIN(mao, log, i, j);
            mao[i].value[0] = 'x';
            a=0;
          } 
          if(mao[i].value[0] == 'Z'){
            mao[i].value[0] = 'A';
          }
        }
      }
    }
  }
}

void print_DISCARD(Cartas* mao, FILE* log, int a){
  if(mao[a].value[0] != '1'){
    printf("DISCARD %c%s\n", mao[a].value[0], mao[a].suit);
    fprintf(log, "DISCARD %c%s\n", mao[a].value[0], mao[a].suit);
  }
  else if(mao[a].value[0] == '1'){
    printf("DISCARD %c%c%s\n", mao[a].value[0], mao[a].value[1], mao[a].suit);
    fprintf(log, "DISCARD %c%c%s\n", mao[a].value[0], mao[a].value[1], mao[a].suit);
  }
}

void condicoes_DISCARD(Cartas* mao, Cartas puxada[1], FILE* log, int tam_mao, int tam_lixo, Cartas* lixo){
  int control=0, index = 0, maior=0;
  Cartas aux[1];
  aux[0].value[0] = mao[0].value[0];
  for(int l = 0; l < tam_mao; l++){
    //-> CARTA SOZINHA DO NAIPE
    if(((l-1 >= 0) && (l+1 < tam_mao)) && (strcmp(mao[l].suit, mao[l-1].suit) != 0) && (strcmp(mao[l].suit, mao[l+1].suit) != 0)){
      lixo[tam_lixo] = mao[l];
      print_DISCARD(mao, log, l);
      control = 1;
      mao[l] = puxada[0];
      break;
    }
  }
  //-> VC COMPARA UMA CARTA J E SE A J+1 NÃO FOR IGUAL AO VALOR DE J ACRESCENTADO UMA UNIDADE, OU DUAS UNIDADES COLOCA A MAIOR CARTA DO VETOR AUXILIAR E VAI PERCORRENDO A MÃO TODA
  if(control == 0){
    int vetor[100];
    int y=0;
    for(int l = 0; l < tam_mao; l++){
      if((l+1 < tam_mao) && ((muda_int(mao[l].value[0]) != muda_int(mao[l+1].value[0])-1) || (muda_int(mao[l].value[0]) != muda_int(mao[l+1].value[0])-2)) && (strcmp(mao[l].suit, mao[l+1].suit) == 0)){
        vetor[y] = l+1;
        y++;
        aux[0].value[0] = mao[l+1].value[0];
      }
      if(l+1 == tam_mao){
          for(int m = 0; m < y; m++){
            if((muda_int(mao[vetor[m]].value[0]) >= muda_int(aux[0].value[0]))){
              aux[0] = mao[vetor[m]];
              index = vetor[m];
            }
          }
          lixo[tam_lixo] = aux[0];
          print_DISCARD(aux, log, 0);
          mao[index] = puxada[0];
          control = 1;
          break;
        }
    }
  }
  //-> ÚLTIMO CASO SENDO A MAIOR DA MÃO
  if(control == 0){
    for(int l = 0; l < tam_mao; l++){
      if((muda_int(mao[l].value[0]) > muda_int(aux[0].value[0]))){
        aux[0] = mao[l];
        maior = l;
        if(l+1==tam_mao){
          if(muda_int(aux[0].value[0]) > muda_int(puxada[0].value[0])){
            lixo[tam_lixo] = aux[0];
            print_DISCARD(aux, log, 0);
            mao[maior] = puxada[0];
            break;
          }
          else{
            lixo[tam_lixo] = puxada[0];
            print_DISCARD(puxada, log, 0);
            break;
          }
        }
      }
    }
  }
}