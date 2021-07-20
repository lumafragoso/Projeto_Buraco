
#ifndef DEF
#define DEF
//-> TAMANHO MÁX DE UMA LINHA NA ENTRADA PADRÃO
#define MAX_LINE 200

//-> TAMANHO MAX DO NOME DO BOT
#define ID_SIZE 50

int quant_jogo_nosso;

//-> STRUCT COM NAIPE E VALOR
typedef struct{
  char value[2];
  char suit[5];
}Cartas;

//-> ENUMERAÇÃO DAS CARTAS
typedef enum{
  um=1, dois= 2, tres, quatro, cinco, seis, sete, oito, nove, dez, valete, rainha, rei, as
} Valor;

#endif