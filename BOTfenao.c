//NEYLANE E LUMA :)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "definitions.h"
#include "io.h"
#include "funcs.h"

int main(){
  char line[MAX_LINE];
  //-> IDENTIFICADOR DO BOT EM QUESTÃO
  char myId[ID_SIZE];
  //-> ARQUIVO DE LOG PARA LER MELHOR
  FILE* log = fopen("simulacao", "w");
  //-> STDIN, STDOUT E STDERRR N TERÃO BUFFERS, NADA É GUARDADO TEMPORARIAMENTE
  setbuf(stdin, NULL);
  setbuf(stdout, NULL);
  setbuf(stderr, NULL);
  //-> SETAR O BUFFER PARA O ARQUIVO
  setbuf(stderr, NULL);
  setbuf(log, NULL);
  //-> DADOS DO INÍCIO DA PARTIDA
  //-> LÊ OS IDENTIFICADORES DOS JOGADORES
  readline(line);
  //-> LÊ O ID DO BOT
  readline(line);
  //-> GUARDA PARA USO FUTURO
  strncpy(myId, line, ID_SIZE);
  //-> PRINTAR NO ARQUIVO O ID
  fprintf(log, "Meu id:%s\n", myId);
  //-> LÊ AS CARTAS QUE O BOT TEM NA MÃO
  readline(line);

  //-> PRINTAR NO ARQUIVO A MÃO
  fprintf(log, "Minha mão:%s\n", line);
  Cartas *mao = malloc(sizeof(Cartas));
  //-> CHAMA A FUNÇÃO PARA LER AS CARTAS DA MÃO
  mao = tratamento_leitura(log, line, mao);
  //-> ORDENAR AS CARTAS POR NAIPE
  static int tam_mao=11;
  ordenar(mao, tam_mao);
  fprintf(log, "ordena:\n");
  print_mao(mao, log, tam_mao);

  //-> LÊ A CARTA INICIAL DA PILHA DE DESCARTE
  readline(line);

  //-> INICIAR O LIXO (ALOCAR)
  Cartas *lixo = malloc(sizeof(Cartas));
  //-> SABER QUANTAS CARTAS TEM NO LIXO
  static int tam_lixo = 1;
  add_lixo(lixo, log, line, tam_lixo);

  //-> VETOR PARA SABER QUANTAS CARTAS TEM EM CADA JOGO ABAIXADO NA MESA
  int cartas_mesa_adv[50];
  //-> INICIAR MESA ADVERSÁRIO
  Cartas **mesa_adversario = malloc(sizeof(Cartas *));
  mesa_adversario[0] = malloc(sizeof(Cartas) * 1);
  char* separa;
  
  //-> INICIAR A NOSSA MESA
  Cartas **nossa_mesa = malloc(sizeof(Cartas*));
  nossa_mesa[0] = malloc(sizeof(Cartas) * 1);
  int quant_cartas_nosso=0;
  int cartas_mesa_nossa[50];

  int cont_GET_STOCK = 81;
  int cont_aux_mao = 0;
  static int rodada =0;
  
  while(1){
    /*============================== VEZ DO ADVERSÁRIO ==============================*/
    do{
      readline(line);
			fprintf(log, "Linha: %s\n", line);
      //-> SEPARA DAS CARTAS GET_DISCARD, MELD_NEW, MELD_JOIN, DISCARD
      separa = strtok(line, " ");
      
      if(strcmp(separa,"GET_STOCK") == 0){
        cont_GET_STOCK --;
        fprintf(log, "OPONENTE DEU GET STOCK: %d\n", cont_GET_STOCK);
      }
      //-> CASO GET_DISCARD ADVERSÁRIO
      else if (strcmp(separa,"GET_DISCARD") == 0){
        //-> REALOCA O VETOR LIXO QUANDO ESTÁ VAZIO
        lixo = malloc(sizeof(Cartas) * 1);
        //-> ZERA O VETOR LIXO
        tam_lixo = 0;
      }
      //-> CASO DISCARD c
      else if(strcmp(separa,"DISCARD") == 0){
        //-> LER O TEXTO PÓS "DISCARD", ISTO É A CARTA E O NAIPE
        separa = strtok(NULL, "");
        //-> QUANTIFICADOR DE CARTAS
				tam_lixo++;
        //-> CHAMA A FUNÇÃO DE LER O LIXO E ADICIONAR MAIS CARTAS NELE
        lixo = realloc(lixo, sizeof(Cartas) * (tam_lixo + 2));
        //-> FUNÇAO DE ADICIONAR A CARTA AO LIXO
        add_lixo(lixo, log, separa, tam_lixo);
      }
      //-> MESA ADVERSÁRIO
      else if(strcmp(separa,"MELD_NEW") == 0){
        fprintf(log, "NOVO JOGO:\n");
        //-> QUANTIFICADOR DE CARTAS
        int quant = 0;
        //-> CONTAR QUANTAS VEZES CRIA UM JOGO NOVO
        static int quant_jogo = 0;
        //-> PARA TIRAR O COLCHETE E O SEGUNDO ESPAÇO
        separa = strtok(NULL, " ");
        separa = strtok(NULL, " ");
        //-> REALOCA AS POSIÇÕES DA MESA DE ACORDO COM A QUANTIDADE DE JOGOS
        mesa_adversario = realloc(mesa_adversario, sizeof(Cartas*) * (quant_jogo + 1));
        //-> FUNÇÃO DE ADICIONAR AS CARTAS DO ADVERSÁRIO NA MESA
        fuc_mesa_adv(mesa_adversario, quant_jogo, quant, separa, line, cartas_mesa_adv);
        //-> ATUALIZAÇÃO PARA COMEÇAR O PRÓXIMO JOGO (VETOR DE CARTAS)
        quant_jogo++;
      }
    } while (strcmp(line, myId));

    /*================================== NOSSA VEZ ==================================*/
    
    fprintf(log, "TOTAL LIXO:\n");
    print_mao(lixo, log, tam_lixo);
    fprintf(log, "\n");

    /*>>>>>>>>>>>>>> ESTRATEGIA DE PUXAR O LIXO <<<<<<<<<<<<<*/
    int entrou =0;
    for(int j = 0; j < tam_mao; j++){
      //-> CASO OS NAIPES SEJAM IGUAIS
      if((j+1 < tam_mao) && (strcmp(lixo[tam_lixo-1].suit, mao[j].suit)==0) && (strcmp(lixo[tam_lixo-1].suit, mao[j+1].suit)==0)){
        //-> CASO A CARTA SEJA A PRIMEIRA CARTA DA SEQUÊNCIA
        if((j+1 < tam_mao) && (muda_int(lixo[tam_lixo-1].value[0]) == muda_int(mao[j].value[0])-1) && (muda_int(lixo[tam_lixo-1].value[0]) == muda_int(mao[j+1].value[0])-2)){
          //-> ADICIONA O LIXO NA NOSSA MAO
          mao = add_lixo_mao(mao, lixo, log, tam_lixo, tam_mao, j);
          //-> ATUALIZA O TAMANHO DA MÃO
          tam_mao += tam_lixo;
          //-> ABAIXA O JOGO DA MESA
          nossa_mesa = abaixa_mesa(nossa_mesa, mao, log, quant_jogo_nosso, quant_cartas_nosso, cartas_mesa_nossa, tam_mao-1, j, j+1);
          quant_jogo_nosso++;
          quant_cartas_nosso=0;
          //-> TIRA AS CARTAS ABAIXADAS DA MAO
          mao = tira_carta_mao(mao,tam_mao, 3);
          tam_mao -= 3;
          //-> ORDENA AS CARTAS NA MÃO
          ordenar(mao, tam_mao);
          //-> REALOCA O VETOR LIXO QUANDO ESTÁ VAZIO
          lixo = realloc(lixo, sizeof(Cartas) * 1);
          //-> ZERA O VETOR LIXO
          tam_lixo = 0;
          entrou = 1;
          break;
        }
        //-> CASO A CARTA SEJA A CARTA DO MEIO DA SEQUÊNCIA
        else if((j+1 < tam_mao) && ((muda_int(lixo[tam_lixo-1].value[0]) == muda_int(mao[j].value[0])+1) && (muda_int(lixo[tam_lixo-1].value[0]) == muda_int(mao[j+1].value[0])-1))){
          mao = add_lixo_mao(mao, lixo, log, tam_lixo, tam_mao, j);
          tam_mao += tam_lixo;
          nossa_mesa = abaixa_mesa(nossa_mesa, mao, log, quant_jogo_nosso, quant_cartas_nosso, cartas_mesa_nossa, j, tam_mao-1, j+1);
          quant_jogo_nosso++;
          quant_cartas_nosso=0;
          mao = tira_carta_mao(mao, tam_mao, 3);
          tam_mao -= 3;
          ordenar(mao, tam_mao);
          lixo = realloc(lixo, sizeof(Cartas) * 1);
          tam_lixo = 0;
          entrou =1;
          break;
        }
        //-> CASO A CARTA SEJA A ÚLTIMA CARTA DA SEQUÊNCIA
        else if((j+1 < tam_mao) && (muda_int(lixo[tam_lixo-1].value[0]) == muda_int(mao[j].value[0])+2) && (muda_int(lixo[tam_lixo-1].value[0]) == muda_int(mao[j+1].value[0])+1)){
          mao = add_lixo_mao(mao, lixo, log, tam_lixo, tam_mao, j);
          tam_mao += tam_lixo;
          nossa_mesa = abaixa_mesa(nossa_mesa, mao, log, quant_jogo_nosso, quant_cartas_nosso, cartas_mesa_nossa, j, j+1, tam_mao-1);
          quant_jogo_nosso++;
          quant_cartas_nosso=0;
          mao = tira_carta_mao(mao, tam_mao, 3);
          tam_mao -= 3;
          ordenar(mao, tam_mao);
          lixo = realloc(lixo, sizeof(Cartas) * 1);
          tam_lixo = 0;
          entrou =1;
          break;
        }
        else if(muda_int(lixo[tam_lixo-1].value[0]) == as){
          lixo[tam_lixo-1].value[0] = 'Z';
          if((j+1 < tam_mao) && (muda_int(lixo[tam_lixo-1].value[0]) == muda_int(mao[j].value[0])-1) && (muda_int(lixo[tam_lixo-1].value[0]) == muda_int(mao[j+1].value[0])-2)){
            lixo[tam_lixo-1].value[0] = 'A';
            //-> ADICIONA O LIXO NA NOSSA MAO
            mao = add_lixo_mao(mao, lixo, log, tam_lixo, tam_mao, j);
            //-> ATUALIZA O TAMANHO DA MÃO
            tam_mao += tam_lixo;
            //-> ABAIXA O JOGO DA MESA
            nossa_mesa = abaixa_mesa(nossa_mesa, mao, log, quant_jogo_nosso, quant_cartas_nosso, cartas_mesa_nossa, tam_mao-1, j, j+1);
            quant_jogo_nosso++;
            quant_cartas_nosso=0;
            //-> TIRA AS CARTAS ABAIXADAS DA MAO
            mao = tira_carta_mao(mao,tam_mao, 3);
            tam_mao -= 3;
            //-> ORDENA AS CARTAS NA MÃO
            ordenar(mao, tam_mao);
            //-> REALOCA O VETOR LIXO QUANDO ESTÁ VAZIO
            lixo = realloc(lixo, sizeof(Cartas) * 1);
            //-> ZERA O VETOR LIXO
            tam_lixo = 0;
            entrou = 1;
            break;
          }
          if(lixo[tam_lixo-1].value[0] == 'Z'){
            lixo[tam_lixo-1].value[0] = 'A';
          }
        }
      }
    }
    
    print_total_mesa(nossa_mesa, log, quant_jogo_nosso, quant_cartas_nosso, cartas_mesa_nossa);
    fprintf(log, "MÃO: ");
    print_mao(mao, log, tam_mao);


    if(entrou != 1){
      Cartas puxada[1];
      print_GET_STOCK(puxada, log, line);
      cont_GET_STOCK --;
      int controle =0;
      
      /*>>>>>>>>>>>>>>>>>>>>>>>>>>>> MELD NEW <<<<<<<<<<<<<<<<<<<<<<<<<<*/
      if(cont_GET_STOCK <= 10 && cont_GET_STOCK > 0){
        nossa_mesa = condicoes_MELD_NEW(mao, nossa_mesa, cartas_mesa_nossa, log, tam_mao, quant_cartas_nosso);

        cont_aux_mao = contagem_x(mao, tam_mao, cont_aux_mao);
        
        if(cont_aux_mao > 0){
          mao = tira_carta_mao(mao, tam_mao, cont_aux_mao);

          tam_mao -= cont_aux_mao;
    
          ordenar(mao, tam_mao);
          cont_aux_mao=0;
        }
      }
      /*>>>>>>>>>>>>>>>>>>>>>>>>>>>> MELD JOIN <<<<<<<<<<<<<<<<<<<<<<<<<<*/
      if(quant_jogo_nosso>=1){
        condicoes_MELD_JOIN(mao, log, nossa_mesa, cartas_mesa_nossa, tam_mao, quant_jogo_nosso);
        
        cont_aux_mao = contagem_x(mao, tam_mao, cont_aux_mao);

        if(cont_aux_mao > 0){
          mao = tira_carta_mao(mao, tam_mao, cont_aux_mao);

          tam_mao -= cont_aux_mao;

          ordenar(mao, tam_mao);
                
          cont_aux_mao=0;
        }
      }
      /*>>>>>>>>>>>>>>>>>>>>>>>>>>>> CONDIÇÕES DISCARD NORMAL <<<<<<<<<<<<<<<<<<<<<<<<<<*/
      for(int k = 0; k < tam_mao; k++){
        if(strcmp(puxada[0].suit, mao[k].suit) == 0){
          //-> CASO A CARTA SEJA A PRIMEIRA CARTA DA SEQUÊNCIA (puxada k k+1)
          if((k+1 < tam_mao) && (muda_int(puxada[0].value[0]) == muda_int(mao[k].value[0])-1) && (muda_int(puxada[0].value[0]) == muda_int(mao[k+1].value[0])-2)){
            controle =1;
            lixo = realloc(lixo, sizeof(Cartas) * (tam_lixo + 1));
            condicoes_DISCARD(mao, puxada, log, tam_mao, tam_lixo, lixo);
            tam_lixo++;
            ordenar(mao, tam_mao);
            break;
          }
          //-> CASO A CARTA SEJA A CARTA DO MEIO DA SEQUÊNCIA (k puxada k+1)
          else if((k+1 < tam_mao) && ((muda_int(puxada[0].value[0]) == muda_int(mao[k].value[0])+1) && (muda_int(puxada[0].value[0]) == muda_int(mao[k+1].value[0])-1))){
            controle =1;
            lixo = realloc(lixo, sizeof(Cartas) * (tam_lixo + 1));
            condicoes_DISCARD(mao, puxada, log, tam_mao, tam_lixo, lixo);
            tam_lixo++;
            ordenar(mao, tam_mao);
            break;
          }
          //-> CASO A CARTA SEJA A ÚLTIMA CARTA DA SEQUÊNCIA (k k+1 puxada)
          else if((k+1 < tam_mao) && (muda_int(puxada[0].value[0]) == muda_int(mao[k].value[0])+2) && (muda_int(puxada[0].value[0]) == muda_int(mao[k+1].value[0])+1)){
            controle =1;
            lixo = realloc(lixo, sizeof(Cartas) * (tam_lixo + 1));
            condicoes_DISCARD(mao, puxada, log, tam_mao, tam_lixo, lixo);
            tam_lixo++;
            ordenar(mao, tam_mao);
            break;
          }
        }
      }
      if(controle !=1){
        printf("DISCARD %s\n", line);
        lixo = realloc(lixo, sizeof(Cartas) * (tam_lixo + 1));
        lixo[tam_lixo] = puxada[0];
        tam_lixo++;
        fprintf(log, "DISCARD %s\n", line);
        controle = 0;
      }
      entrou =0;
    }
    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>> CONDIÇÕES DPS DE PEGAR LIXO <<<<<<<<<<<<<<<<<<<<<<<<<<*/
    else if(entrou ==1){
      /*>>>>>>>>>>>>>>>>>>>>>>>>>>>> MELD NEW DPS DE PEGAR LIXO <<<<<<<<<<<<<<<<<<<<<<<<<<*/
      if(rodada >=1){
        nossa_mesa= condicoes_MELD_NEW(mao, nossa_mesa, cartas_mesa_nossa, log, tam_mao, quant_cartas_nosso);
        cont_aux_mao = contagem_x(mao, tam_mao, cont_aux_mao);

        if(cont_aux_mao > 0){
          mao = tira_carta_mao(mao, tam_mao, cont_aux_mao);

          tam_mao -= cont_aux_mao;
    
          ordenar(mao, tam_mao);
                          
          cont_aux_mao=0;
        }
      }
      /*>>>>>>>>>>>>>>>>>>>>>>>>>>>> MELD JOIN DPS DE PEGAR LIXO <<<<<<<<<<<<<<<<<<<<<<<<<<*/
      if(quant_jogo_nosso>=1){
        condicoes_MELD_JOIN(mao, log, nossa_mesa, cartas_mesa_nossa, tam_mao, quant_jogo_nosso);
        
        cont_aux_mao = contagem_x(mao, tam_mao, cont_aux_mao);

        if(cont_aux_mao > 0){
          mao = tira_carta_mao(mao, tam_mao, cont_aux_mao);

          tam_mao -= cont_aux_mao;
          
          ordenar(mao, tam_mao);
                
          cont_aux_mao=0;
        }
      }
      /*>>>>>>>>>>>>>>>>>>>>>>>>>>>> CONDIÇÕES DISCARD LIXO <<<<<<<<<<<<<<<<<<<<<<<<<<*/
      Cartas auxiliar[1];
      int conta = 0, indice =0;
      for(int r = 0; r < tam_mao; r++){
        //-> SE SÓ EXISTE UMA CARTA DE UM NAIPE
        if(((r-1 >= 0) && (r+1 < tam_mao)) && (strcmp(mao[r].suit, mao[r-1].suit) != 0) && (strcmp(mao[r].suit, mao[r+1].suit) != 0)){
          //-> REALOCA O VETOR LIXO QUANDO ESTÁ VAZIO
          lixo = malloc(sizeof(Cartas) * 1);
          //-> ZERA O VETOR LIXO
          tam_lixo = 0;

          lixo[tam_lixo] = mao[r];
          print_DISCARD(mao, log, r);
          mao[r].value[0] = 'x';
          cont_aux_mao++;

          tam_lixo++;

          mao = tira_carta_mao(mao, tam_mao, cont_aux_mao);
          tam_mao -= cont_aux_mao;

          ordenar(mao, tam_mao);
          cont_aux_mao=0;

          conta = 1;
          break;
        }
      }
      if(conta ==0){
        int vet[100];
        int x=0;
        for(int s = 0; s < tam_mao; s++){
          if((s+1 < tam_mao) && ((muda_int(mao[s].value[0]) != muda_int(mao[s+1].value[0])-1) || (muda_int(mao[s].value[0]) != muda_int(mao[s+1].value[0])-2)) && (strcmp(mao[s].suit, mao[s+1].suit) == 0)){
            vet[x] = s+1;
            x++;
            auxiliar[0].value[0] = mao[s+1].value[0];
            
          if(s+2 == tam_mao){
              for(int m = 0; m < x; m++){
                if((muda_int(mao[vet[m]].value[0]) >= muda_int(auxiliar[0].value[0]))){
                  auxiliar[0] = mao[vet[m]];
                  indice = vet[m];
                }
              }
              //-> REALOCA O VETOR LIXO QUANDO ESTÁ VAZIO
              lixo = malloc(sizeof(Cartas) * 1);
              //-> ZERA O VETOR LIXO
              tam_lixo = 0;

              lixo[tam_lixo] = auxiliar[0];
              print_DISCARD(auxiliar, log, 0);
              mao[indice].value[0] = 'x';
              cont_aux_mao++;

              tam_lixo++;
              
              mao = tira_carta_mao(mao, tam_mao, cont_aux_mao);

              tam_mao -= cont_aux_mao;

              ordenar(mao, tam_mao);
              
              cont_aux_mao=0;
              conta = 1;
              x=0;
              break;
            }
          }
        }
      }
      if(conta == 0){
        int z=0;
        auxiliar[0].value[0] = mao[0].value[0];
        //-> ÚLTIMO CASO SENDO A MAIOR DA MÃO
        for(int t = 0; t < tam_mao; t++){
          if((muda_int(mao[t].value[0]) > muda_int(auxiliar[0].value[0]))){
            auxiliar[0] = mao[t];
            z = t;
          }
        }
        //-> REALOCA O VETOR LIXO QUANDO ESTÁ VAZIO
        lixo = realloc(lixo, sizeof(Cartas) * 1);
        //-> ZERA O VETOR LIXO
        tam_lixo = 0;

        lixo[tam_lixo] = auxiliar[0];
        print_DISCARD(auxiliar, log, 0);
        mao[z].value[0] = 'x';
        cont_aux_mao++;

        tam_lixo++;

        mao = tira_carta_mao(mao, tam_mao, cont_aux_mao);
        
        tam_mao -= cont_aux_mao;
        ordenar(mao, tam_mao);

        cont_aux_mao=0;
      }
      entrou = 0;
      conta=0;
    }
    fprintf(log, "MÃO: ");
    print_mao(mao, log, tam_mao);
    rodada++;
  }
  free_vetor(mao);
  free_vetor(lixo);
  free_matriz(nossa_mesa);
  free_matriz(mesa_adversario);
  fclose(log);
  return 0;
}
