#ifndef proj_h
#define proj_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_NOME_VACINA 50
#define MAX_NOME_LOTE 20
#define MAX_NOME_UTENTE 200
#define MAX_NUM_LOTES 1000
#define MAX_DATA_DE_VALIDADE 11

#define MENSAGEM_ENGLISH_TOO_MANY_VACCINES "too many vaccines"
#define MENSAGEM_ENGLISH_DUPLICATE_BATCH "duplicate batch number"
#define MENSAGEM_ENGLISH_INVALID_BATCH "invalid batch"
#define MENSAGEM_ENGLISH_INVALID_NAME "invalid name"
#define MENSAGEM_ENGLISH_INVALID_DATE "invalid date"
#define MENSAGEM_ENGLISH_INVALID_QUANTITY "invalid quantity"
#define MENSAGEM_ENGLISH_NO_SUCH_VACCINE "no such vaccine"
#define MENSAGEM_ENGLISH_NO_STOCK "no stock"
#define MENSAGEM_ENGLISH_ALREADY_VACCINATED "already vaccinated"
#define MENSAGEM_ENGLISH_NO_SUCH_BATCH "no such batch"
#define MENSAGEM_ENGLISH_NO_SUCH_USER "no such user"
#define MENSAGEM_ENGLISH_NO_MEMORY "No memory"


#define MENSAGEM_PORTUGUESE_TOO_MANY_VACCINES "demasiadas vacinas"
#define MENSAGEM_PORTUGUESE_DUPLICATE_BATCH "número de lote duplicado"
#define MENSAGEM_PORTUGUESE_INVALID_BATCH "lote inválido"
#define MENSAGEM_PORTUGUESE_INVALID_NAME "nome inválido"
#define MENSAGEM_PORTUGUESE_INVALID_DATE "data inválida"
#define MENSAGEM_PORTUGUESE_INVALID_QUANTITY "quantidade inválida"
#define MENSAGEM_PORTUGUESE_NO_SUCH_VACCINE "vacina inexistente"
#define MENSAGEM_PORTUGUESE_NO_STOCK "esgotado"
#define MENSAGEM_PORTUGUESE_ALREADY_VACCINATED "já vacinado"
#define MENSAGEM_PORTUGUESE_NO_SUCH_BATCH "lote inexistente"
#define MENSAGEM_PORTUGUESE_NO_SUCH_USER "utente inexistente"
#define MENSAGEM_PORTUGUESE_NO_MEMORY "sem memória"

typedef struct Vacina {
    char nome[MAX_NOME_VACINA];
    char lote[MAX_NOME_LOTE];
    char data_de_validade[MAX_DATA_DE_VALIDADE];
    int numDosesDisponiveis;
} Vacina;

typedef struct Data {
    int dia;
    int mes;
    int ano;
} Data;

typedef struct Inoculacao {
    char* nomeUtente;
    char loteVacina[MAX_NOME_LOTE];
    char nomeVacina[MAX_NOME_VACINA];
    Data dataAplicacaoVacina;
} Inoculacao;

// Funções
const char* selecionaMensagem(int idioma, const char* mensagemEnglish, const char* mensagemPortuguese);
void libertaMemoriaReservada(Inoculacao* inoculacoes, int numInoculacoes, int idioma);
int verificaNome(char nome[]);
int verificaValidadeData(Data data);
int comparaDatas(Data data1, Data data2);
Data converteStringParaData(char* dataString);
int verificaData(char *dataString, Data dataAtual);
int verificaNumVacina(Vacina[], int numVacinas);
int verificaLote(char lote[]);
int verificaLoteRepetido(Vacina vacinas[], int numVacinas, char lote[]);
void introduzNovoLote(char *string, Vacina vacinas[], int *numVacinas, Data *dataAtual, int idioma);
int avancaTempoEstipulado(char *args, Data *dataAtual, int idioma);
int comparaVacinas(Vacina* vacinaX, Vacina* vacinaY);
void ordenaVacinas(Vacina vacinas[], int numVacinas);
void vacinasOrdemCronologica(Vacina vacinas[], int numVacinas, char* nome,
                              Inoculacao* inoculacoes, int numInoculacoes, int idioma);
char* extraiStringEntreAspas(char* string);
char* extraiNomeUtente(char* args, char* stringRestante);
int procuraIndiceLoteValido(Vacina vacinas[], int numVacinas, char* nomeVacina,
                             Data dataAtual, char* loteDeterminado);
int verificaVacinacaoPreviaNoMesmoDia(Inoculacao* inoculacoes, int numInoculacoes,
                                       char* nomeUtente, char* nomeVacina, Data dataAtual);
void aplicaDoseVacina(char* args, Vacina vacinas[], int numVacinas, Inoculacao** inoculacoes,
                      int* numInoculacoes, Data dataAtual, int idioma);
int comparaInoculacoesPorData(Inoculacao inoculacao1, Inoculacao inoculacao2);
void ordenaInoculacoesPorData(Inoculacao* inoculacoes, int numInoculacoes);
void listaAplicacoes(char* args, Inoculacao* inoculacoes, int numInoculacoes, int idioma);
void retiraDisponibilidadeVacina(char* lote, Vacina vacinas[], int* numVacinas,
                                 Inoculacao* inoculacoes, int numInoculacoes, int idioma);
Data extraiData(char* args, char* stringRestante);
int verificaSeUtenteExiste(Inoculacao* inoculacoes, int numInoculacoes, char* nomeUtente);
int verificaSeLoteExiste(Inoculacao* inoculacoes, int numInoculacoes, char* lote);
int apagaAplicacoesDadoUtente(Inoculacao** inoculacoes, int* numInoculacoes, char* nomeUtente);
int apagaAplicacoesDadaData(Inoculacao** inoculacoes, int* numInoculacoes, char* nomeUtente, Data dataAplicacao);
int apagaAplicacoesDadoLote(Inoculacao** inoculacoes, int* numInoculacoes,
                            char* nomeUtente, Data dataAplicacao, char* lote);
void apagaRegistoAplicacaoVacina(char* args, Inoculacao** inoculacoes, int* numInoculacoes, int idioma);

#endif
