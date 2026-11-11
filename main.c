/* Vaccine management system - IAED project, Instituto Superior Técnico */
#include "proj.h"

// Função que seleciona o idioma da mensagem  
const char* selecionaMensagem(int idioma, const char* mensagemEnglish, const char* mensagemPortuguese) {
    if (idioma == 0) {
        return mensagemEnglish;
    } else {
        return mensagemPortuguese;
    }
}

// Função que liberta a memória reservada, caso a memória se esgote
void libertaMemoriaReservada(Inoculacao* inoculacoes, int numInoculacoes, int idioma) {
    int i;
    for (i = 0; i < numInoculacoes; i++) {
        if (inoculacoes[i].nomeUtente != NULL && inoculacoes != NULL) {
            free(inoculacoes[i].nomeUtente);
        }
    }
    if (inoculacoes != NULL) {
        free(inoculacoes);
    }
    printf("%s\n", selecionaMensagem(idioma, MENSAGEM_ENGLISH_NO_MEMORY, MENSAGEM_PORTUGUESE_NO_MEMORY));
    exit(1);
}

int verificaNome(char nome[]) {
    int i;
    int len = strlen(nome);
    if (len > MAX_NOME_VACINA) {
        return 1;
    }
    for (i = 0; i < len; i++) {
        if (nome[i] == ' ' || nome[i] == '\t' || nome[i] == '\n') {
            return 1;
        }
    }
    return 0;
}

// Verifica se uma data é válida, números de dias num mês e anos bissextos 
int verificaValidadeData(Data data) {
    int numMaxDias;

    if (data.dia < 1 || data.dia > 31 || data.mes < 1 || data.mes > 12) {
        return 1;
    }

    if (data.mes == 2) {
        if (data.ano % 400 == 0 || (data.ano % 4 == 0 && data.ano % 100 != 0)) {
            numMaxDias = 29;
            if (data.dia > numMaxDias) {
                return 1;
            }
        } else {
            numMaxDias = 28;
            if (data.dia > numMaxDias) {
                return 1;
            }
        }
    } else if (data.mes == 4 || data.mes == 6 || data.mes == 9 || data.mes == 11) {
        numMaxDias = 30;
        if (data.dia > numMaxDias) {
            return 1;
        }
    } else if (data.mes == 1 || data.mes == 3 || data.mes == 5 || data.mes == 7 || data.mes == 8 || data.mes == 10 || data.mes == 12) {
        numMaxDias = 31;
    }

    if (data.dia > numMaxDias) {
        return 1;
    }
    return 0;
}

// Compara duas datas: retorna -1 se data1 < data2, 0 se iguais, 1 se data1 > data2 
int comparaDatas(Data data1, Data data2) {
    if (data1.ano < data2.ano)
        return -1;
    if (data1.ano > data2.ano)
        return 1;
    if (data1.mes < data2.mes)
        return -1;
    if (data1.mes > data2.mes)
        return 1;
    if (data1.dia < data2.dia)
        return -1;
    if (data1.dia > data2.dia)
        return 1;
    return 0;
} 

Data converteStringParaData(char* dataString) {
    Data data;
    sscanf(dataString, "%d-%d-%d", &data.dia, &data.mes, &data.ano);
    return data;
}

int verificaData(char *dataString, Data dataAtual) {
    Data data = converteStringParaData(dataString);
    if (verificaValidadeData(data) == 1) {
        return 1;
    }
    if (comparaDatas(data, dataAtual) < 0) {
        return 1;
    }
    return 0;
}

int verificaNumVacina(Vacina[], int numVacinas) {
    if (numVacinas >= MAX_NUM_LOTES) {
        return 1;
    }
    return 0;
}

int verificaLote(char lote[]) {
    int i;
    int len = strlen(lote);
    
    if (len > MAX_NOME_LOTE) {
        return 1;
    }
    for (i = 0; i < len; i++) {
        if (!isdigit(lote[i]) && (lote[i] < 'A' || lote[i] > 'F')) {
            return 1;
        }
    }
    return 0;
}

int verificaLoteRepetido(Vacina vacinas[], int numVacinas, char lote[]) {
    int i;
    for (i = 0; i < numVacinas; i++) {
        if (strcmp(vacinas[i].lote, lote) == 0) {
            return 1;
        }
    }
    return 0;
}

// Comando c, função que introduz um novo lote no sistema
void introduzNovoLote(char *string, Vacina vacinas[], int *numVacinas, Data *dataAtual, int idioma) {
    char nome[BUFSIZ], lote[BUFSIZ], data_de_validade[MAX_DATA_DE_VALIDADE];
    int numDosesDisponiveis;
    
    sscanf(string, "%s %s %d %s", lote, data_de_validade, &numDosesDisponiveis, nome);
    
    if (verificaNome(nome) == 1) {
        printf("%s\n", selecionaMensagem(idioma, MENSAGEM_ENGLISH_INVALID_NAME, MENSAGEM_PORTUGUESE_INVALID_NAME));
        return;
    }
    if (verificaLote(lote) == 1) {
        printf("%s\n", selecionaMensagem(idioma, MENSAGEM_ENGLISH_INVALID_BATCH, MENSAGEM_PORTUGUESE_INVALID_BATCH));
        return;
    }
    if (verificaData(data_de_validade, *dataAtual) == 1) {
        printf("%s\n", selecionaMensagem(idioma, MENSAGEM_ENGLISH_INVALID_DATE, MENSAGEM_PORTUGUESE_INVALID_DATE));
        return;
    }
    if (numDosesDisponiveis <= 0) {
        printf("%s\n", selecionaMensagem(idioma, MENSAGEM_ENGLISH_INVALID_QUANTITY, MENSAGEM_PORTUGUESE_INVALID_QUANTITY));
        return;
    }
    if (verificaNumVacina(vacinas, *numVacinas) == 1) {
        printf("%s\n", selecionaMensagem(idioma, MENSAGEM_ENGLISH_TOO_MANY_VACCINES, MENSAGEM_PORTUGUESE_TOO_MANY_VACCINES));
        return;
    }
    if (verificaLoteRepetido(vacinas, *numVacinas, lote) == 1) {
        printf("%s\n", selecionaMensagem(idioma, MENSAGEM_ENGLISH_DUPLICATE_BATCH, MENSAGEM_PORTUGUESE_DUPLICATE_BATCH));
        return;
    }
    
    // Insere a nova vacina no array
    strcpy(vacinas[*numVacinas].nome, nome);
    strcpy(vacinas[*numVacinas].lote, lote);
    strcpy(vacinas[*numVacinas].data_de_validade, data_de_validade);
    vacinas[*numVacinas].numDosesDisponiveis = numDosesDisponiveis;
    
    (*numVacinas)++;
    
    printf("%s\n", lote);
}

// Comando t, função que avança o tempo estipulado
int avancaTempoEstipulado(char *args, Data *dataAtual, int idioma) {
    // Nova data a ser definida
    Data dataNova;
    
    // Se não há argumentos, apenas mostra a data atual
    if (args == NULL) {
        printf("%02d-%02d-%d\n", dataAtual->dia, dataAtual->mes, dataAtual->ano);
        return 0;
    }
    
    if (sscanf(args, "%d-%d-%d", &dataNova.dia, &dataNova.mes, &dataNova.ano) != 3) {
        printf("%s\n", selecionaMensagem(idioma, MENSAGEM_ENGLISH_INVALID_DATE, MENSAGEM_PORTUGUESE_INVALID_DATE));
        return 1;
    }
        
    if (verificaValidadeData(dataNova)) {
        printf("%s\n", selecionaMensagem(idioma, MENSAGEM_ENGLISH_INVALID_DATE, MENSAGEM_PORTUGUESE_INVALID_DATE));
        return 1;
    }

    if (comparaDatas(dataNova, *dataAtual) < 0) {
        printf("%s\n", selecionaMensagem(idioma, MENSAGEM_ENGLISH_INVALID_DATE, MENSAGEM_PORTUGUESE_INVALID_DATE));
        return 1;
    }
    
    *dataAtual = dataNova;

    printf("%02d-%02d-%d\n", dataAtual->dia, dataAtual->mes, dataAtual->ano);
    return 0;
}
                
// Compara duas vacinas
int comparaVacinas(Vacina* vacinaX, Vacina* vacinaY) {
    Data dataX = converteStringParaData(vacinaX->data_de_validade);
    Data dataY = converteStringParaData(vacinaY->data_de_validade);

    int resultadoData = comparaDatas(dataX, dataY);

    if (resultadoData == 0) {
        return strcmp(vacinaX->lote, vacinaY->lote);
    }
    return resultadoData;
}

void ordenaVacinas(Vacina vacinas[], int numVacinas) {
    int i, j;
    Vacina temporaria;

    for(i = 1; i < numVacinas; i++) {
        temporaria = vacinas[i];
        j = i - 1;

        while(j >= 0 && comparaVacinas(&vacinas[j], &temporaria) > 0) {
            vacinas[j + 1] = vacinas[j];
            j = j - 1;
        }
        vacinas[j + 1] = temporaria;
    }
}

// Comando l, função que lista os lotes das vacinas criadas
void vacinasOrdemCronologica(Vacina vacinas[], int numVacinas, char* nome,
                            Inoculacao* inoculacoes, int numInoculacoes, int idioma) {
    int i, j;

    if(numVacinas == 0) {
        return;
    }

    // Array temporário para ordenar as vacinas
    Vacina* vacinasTemporarias = malloc(numVacinas * sizeof(Vacina));
    if (vacinasTemporarias == NULL) {
        libertaMemoriaReservada(inoculacoes, numInoculacoes, idioma);
        return;
    }

    int contadorTemporario = 0;

    // Se não houver nome, seleciona todas as vacinas
    if (strlen(nome) == 0) {
        for (i = 0; i < numVacinas; i++) {
            vacinasTemporarias[contadorTemporario] = vacinas[i];
            contadorTemporario++;
        }
    } else {
        // Caso haja nome, seleciona todas as vacinas com esse nome
        int vacinaEncontrada = 0;
        for (i = 0; i < numVacinas; i++) {
            if (strcmp(vacinas[i].nome, nome) == 0) {
                vacinasTemporarias[contadorTemporario] = vacinas[i];
                contadorTemporario++;
                vacinaEncontrada = 1;
            }
        }
        if (vacinaEncontrada == 0) {
            printf("%s: %s\n", nome, selecionaMensagem(idioma, MENSAGEM_ENGLISH_NO_SUCH_VACCINE, MENSAGEM_PORTUGUESE_NO_SUCH_VACCINE));
            free(vacinasTemporarias);
            return;
        }
    }

    ordenaVacinas(vacinasTemporarias, contadorTemporario);

    for (i = 0; i < contadorTemporario; i++) {
        int numAplicacoes = 0;
        for (j = 0; j < numInoculacoes; j++) {
            if (strcmp(inoculacoes[j].loteVacina, vacinasTemporarias[i].lote) == 0) {
                numAplicacoes++;
            }
        }
        Data dataDeValidade = converteStringParaData(vacinasTemporarias[i].data_de_validade);
        printf("%s %s %02d-%02d-%d %d %d\n", vacinasTemporarias[i].nome, 
            vacinasTemporarias[i].lote,
            dataDeValidade.dia, dataDeValidade.mes, dataDeValidade.ano, 
            vacinasTemporarias[i].numDosesDisponiveis,
            numAplicacoes);
    } 
    free(vacinasTemporarias);   
}


// Função que extrai string entre aspas
char* extraiStringEntreAspas(char* string) {
    char* inicio = string + 1; // Salta a primeira aspa
    char* fim = strchr(inicio, '"');
    int tamanho = fim - inicio;
    char* novaString;

    if (fim == NULL) {
        return NULL;
    }

    novaString = (char*)malloc(tamanho + 1);
    if (novaString == NULL) {
        return NULL;
    }

    strncpy(novaString, inicio, tamanho);
    novaString[tamanho] = '\0';

    return novaString;
}

// Função para extrair o nome do utente (preparada para lidar com aspas)
char* extraiNomeUtente(char* args, char* stringRestante) {
    char* nomeUtente;
    char* temp = args;
    int tamanho;

    // Salta espaços no início da string
    while (*temp && isspace(*temp)) {
        temp++;
    }

    if (*temp == '"') {
        nomeUtente = extraiStringEntreAspas(temp);
        if (nomeUtente == NULL) {
            return NULL;
        }
        // Após as aspas finais
        temp = strchr(temp + 1, '"');
        if (temp == NULL) {
            free(nomeUtente);
            return NULL;
        }
        temp++; 
        //Salta espaços após o nome
        while (*temp && isspace(*temp)) {
            temp++;
        }
        if (stringRestante != NULL) {
            strcpy(stringRestante, temp);
        }
    } else {
        // Caso o nome não comece com aspas
        char* espaco = strchr(temp, ' ');
        if (espaco == NULL) {
            // Não havendo espaço, então o nome é toda a string
            tamanho = strlen(temp);
            nomeUtente = (char*)malloc(tamanho + 1);
            if (nomeUtente == NULL) {
                return NULL;
            }
            strcpy(nomeUtente, temp);
            if (stringRestante != NULL) {
                stringRestante[0] = '\0';
            }
        } else {
            // Caso haja espaço, então o nome é a string até ao espaço
            tamanho = espaco - temp;
            nomeUtente = (char*)malloc(tamanho + 1);
            if (nomeUtente == NULL) {
                return NULL;
            }
            strncpy(nomeUtente, temp, tamanho);
            nomeUtente[tamanho] = '\0';

            temp = espaco + 1;

            // Salta espaços após o nome
            while (*temp && isspace(*temp)) {
                temp++;
            }
            if (stringRestante != NULL) {
                strcpy(stringRestante, espaco + 1);
            }
        }
    }
    return nomeUtente;
}
    
    
// Procura o índice do lote válido dentro do array vacinas[]
int procuraIndiceLoteValido(Vacina vacinas[], int numVacinas, char* nomeVacina,
                         Data dataAtual, char* loteDeterminado) {
    int i;
    int indiceLoteValido = -1;
    int loteValido = 0;
    Data dataLoteValido = {0};
    Data dataTemp;

    for (i = 0; i < numVacinas; i++) {
        if (strcmp(vacinas[i].nome, nomeVacina) == 0 && vacinas[i].numDosesDisponiveis > 0) {
            dataTemp = converteStringParaData(vacinas[i].data_de_validade);
            if (comparaDatas(dataTemp, dataAtual) >= 0) {
                if (loteValido == 0 || comparaDatas(dataTemp, dataLoteValido) < 0) {
                    indiceLoteValido = i;
                    dataLoteValido = dataTemp;
                    loteValido = 1;
                }
            }
        }
    }
    if (loteValido) {
        strcpy(loteDeterminado, vacinas[indiceLoteValido].lote);
        return indiceLoteValido;
    }
    return -1; // Não há lote válido
}

// Verifica se o utente foi vacinado no mesmo dia
int verificaVacinacaoPreviaNoMesmoDia(Inoculacao* inoculacoes, int numInoculacoes, 
                                    char* nomeUtente, char* nomeVacina, Data dataAtual) {
    int i;
    
    for (i = 0; i < numInoculacoes; i++) {
        if (strcmp(inoculacoes[i].nomeUtente, nomeUtente) == 0 &&
        strcmp(inoculacoes[i].nomeVacina, nomeVacina) == 0 &&
        inoculacoes[i].dataAplicacaoVacina.dia == dataAtual.dia &&
        inoculacoes[i].dataAplicacaoVacina.mes == dataAtual.mes &&
        inoculacoes[i].dataAplicacaoVacina.ano == dataAtual.ano) {
        return 1;
        }
    }
    return 0;
}

// Comando a, função que aplica uma dose de vacina ao utente
void aplicaDoseVacina(char* args, Vacina vacinas[], int numVacinas, Inoculacao** inoculacoes, 
                        int* numInoculacoes, Data dataAtual, int idioma) {
    char nomeVacina[MAX_NOME_VACINA];
    char loteDeterminado[MAX_NOME_LOTE];
    char* nomeUtente = extraiNomeUtente(args, nomeVacina);

    if (nomeUtente == NULL) {
        libertaMemoriaReservada(*inoculacoes, *numInoculacoes, idioma);
        return;
    }

    if (verificaVacinacaoPreviaNoMesmoDia(*inoculacoes, *numInoculacoes, nomeUtente, nomeVacina, dataAtual)) {
        printf("%s\n", selecionaMensagem(idioma, MENSAGEM_ENGLISH_ALREADY_VACCINATED, MENSAGEM_PORTUGUESE_ALREADY_VACCINATED));
        free(nomeUtente);
        return;
    }

    int indiceLoteValido = procuraIndiceLoteValido(vacinas, numVacinas, nomeVacina, dataAtual, loteDeterminado);
    if (indiceLoteValido == -1) {
        printf("%s\n", selecionaMensagem(idioma, MENSAGEM_ENGLISH_NO_STOCK, MENSAGEM_PORTUGUESE_NO_STOCK));
        free(nomeUtente);
        return;
    }

    // Verifica se precisa realocar o array de inoculacoes
    Inoculacao* novasInoculacoes;
    if (*inoculacoes == NULL) {
        novasInoculacoes = (Inoculacao*)malloc(sizeof(Inoculacao));
    } else {
        novasInoculacoes = (Inoculacao*)realloc(*inoculacoes, (*numInoculacoes + 1) * sizeof(Inoculacao));
    }
    if (novasInoculacoes == NULL) {
        free(nomeUtente);
        libertaMemoriaReservada(*inoculacoes, *numInoculacoes, idioma);
        return; 
    }
    
    *inoculacoes = novasInoculacoes;
    
    // Regista a inoculação
    (*inoculacoes)[*numInoculacoes].nomeUtente = nomeUtente;
    strcpy((*inoculacoes)[*numInoculacoes].loteVacina, loteDeterminado);
    strcpy((*inoculacoes)[*numInoculacoes].nomeVacina, nomeVacina);
    (*inoculacoes)[*numInoculacoes].dataAplicacaoVacina = dataAtual;

    (*numInoculacoes)++;
    vacinas[indiceLoteValido].numDosesDisponiveis--;

    printf("%s\n", loteDeterminado);
}
    

// Inicio comando u

int comparaInoculacoesPorData(Inoculacao inoculacao1, Inoculacao inoculacao2) {
    return comparaDatas(inoculacao1.dataAplicacaoVacina, inoculacao2.dataAplicacaoVacina);

}

void ordenaInoculacoesPorData(Inoculacao* inoculacoes, int numInoculacoes) {
    int i, j;
    Inoculacao temporaria;

    for(i = 1; i < numInoculacoes; i++) {
        temporaria = inoculacoes[i];
        j = i - 1;

        while(j >= 0 && comparaInoculacoesPorData(inoculacoes[j], temporaria) > 0) {
            inoculacoes[j + 1] = inoculacoes[j];
            j = j - 1;
        }
        inoculacoes[j + 1] = temporaria;
    }
}

// Comando u, função que lista as aplicações de vacinas, ou as aplicações de um utente
void listaAplicacoes(char* args, Inoculacao* inoculacoes, int numInoculacoes, int idioma) {
    int i;
    int j = 0;
    char* temp = args;
    int encontrouUtente = 0;
    int numInoculacoesUtente = 0;

    // Caso em que args está vazio após espaços e lista todas as inoculações
    while (*temp && isspace(*temp)) {
        temp++;
    }

    if (*temp == '\0') {
        Inoculacao* inoculacoesTemporarias = malloc(numInoculacoes * sizeof(Inoculacao));
        if (inoculacoesTemporarias == NULL) {
            libertaMemoriaReservada(inoculacoes, numInoculacoes, idioma);
            return;
        }
        memcpy(inoculacoesTemporarias, inoculacoes, numInoculacoes * sizeof(Inoculacao));
        ordenaInoculacoesPorData(inoculacoesTemporarias, numInoculacoes);

        for (i = 0; i < numInoculacoes; i++) {
            printf("%s %s %02d-%02d-%d\n", inoculacoesTemporarias[i].nomeUtente,
                inoculacoesTemporarias[i].loteVacina,
                inoculacoesTemporarias[i].dataAplicacaoVacina.dia,
                inoculacoesTemporarias[i].dataAplicacaoVacina.mes,
                inoculacoesTemporarias[i].dataAplicacaoVacina.ano);
        }
        free(inoculacoesTemporarias);
    } else {
        char* nomeUtente = extraiNomeUtente(args, NULL);
        if (nomeUtente == NULL) {
            libertaMemoriaReservada(inoculacoes, numInoculacoes, idioma);
            return;
        }
        // Conta o número de inoculações do utente
        for (i = 0; i < numInoculacoes; i++) {
            if (strcmp(inoculacoes[i].nomeUtente, nomeUtente) == 0) {
                encontrouUtente = 1;
                numInoculacoesUtente++;
            }
        }
        if (encontrouUtente == 0) {
            printf("%s: %s\n", nomeUtente, selecionaMensagem(idioma, MENSAGEM_ENGLISH_NO_SUCH_USER, MENSAGEM_PORTUGUESE_NO_SUCH_USER));
            free(nomeUtente);
            return;
        }

       Inoculacao* inoculacoesUtente = malloc(numInoculacoesUtente * sizeof(Inoculacao));
       if (inoculacoesUtente == NULL) {
            free(nomeUtente);
            libertaMemoriaReservada(inoculacoes, numInoculacoes, idioma);
            return;
        }
        
        // Copia as inoculações do utente para um novo array
        for (i = 0; i < numInoculacoes; i++) {
            if (strcmp(inoculacoes[i].nomeUtente, nomeUtente) == 0) {
                inoculacoesUtente[j++] = inoculacoes[i];            }
        }
        ordenaInoculacoesPorData(inoculacoesUtente, numInoculacoesUtente);

        for (i = 0; i < numInoculacoesUtente; i++) {
            printf("%s %s %02d-%02d-%d\n", inoculacoesUtente[i].nomeUtente,
                inoculacoesUtente[i].loteVacina,
                inoculacoesUtente[i].dataAplicacaoVacina.dia,
                inoculacoesUtente[i].dataAplicacaoVacina.mes,
                inoculacoesUtente[i].dataAplicacaoVacina.ano);
        }
        free(inoculacoesUtente);
        free(nomeUtente);
    }
}

// Inicio do comando r

void retiraDisponibilidadeVacina(char* lote, Vacina vacinas[], int* numVacinas, 
                                Inoculacao* inoculacoes, int numInoculacoes, int idioma) {
    int i;
    int indiceLote = -1;
    int numDosesAplicadas = 0;
    
    // Encontra o índice do lote pretendido no array de vacinas
    for (i = 0; i < *numVacinas; i++) {
        if (strcmp(vacinas[i].lote, lote) == 0) {
            indiceLote = i;
            break;
        }
    }

    if (indiceLote == -1) {
        printf("%s: %s\n", lote, selecionaMensagem(idioma, MENSAGEM_ENGLISH_NO_SUCH_BATCH, MENSAGEM_PORTUGUESE_NO_SUCH_BATCH));
        return;
    }

    // Tendo em conta o lote, conta o número de doses aplicadas
    for (i = 0; i < numInoculacoes; i++) {
        if (strcmp(inoculacoes[i].loteVacina, lote) == 0) {
            numDosesAplicadas++;
        }
    }

    // Caso não haja doses aplicadas, remover o lote
    if (numDosesAplicadas == 0) {
        // Preencher o lote removido através do deslocamento dos lotes seguintes
        for (i = indiceLote; i < *numVacinas - 1; i++) {
            vacinas[i] = vacinas[i + 1];
        }
        (*numVacinas)--;
    } else {
        vacinas[indiceLote].numDosesDisponiveis = 0;
    }

    printf("%d\n", numDosesAplicadas);
}


// Inicio comando d

// rever esta função
Data extraiData(char* args, char* stringRestante) {
    Data dataExtraida = {0};
    int dia, mes, ano;
    char* temp = args;

    if (sscanf(args, "%d-%d-%d", &dia, &mes, &ano) == 3) {
        dataExtraida.dia = dia;
        dataExtraida.mes = mes;
        dataExtraida.ano = ano;

        while (*temp && *temp != ' ' && *temp != '\t') {
            temp++;
        }
        while (*temp && (*temp == ' ' || *temp == '\t')) {
            temp++;
        }
        if (stringRestante != NULL) {
            strcpy(stringRestante, temp);
        }
    }
    return dataExtraida;
}

int verificaSeUtenteExiste(Inoculacao* inoculacoes, int numInoculacoes, char* nomeUtente) {
    int i;
    for (i = 0; i < numInoculacoes; i++) {
        if (strcmp(inoculacoes[i].nomeUtente, nomeUtente) == 0) {
            return 1;
        }
    }
    return 0;
}

int verificaSeLoteExiste(Inoculacao* inoculacoes, int numInoculacoes, char* lote) {
    int i;
    for (i = 0; i < numInoculacoes; i++) {
        if (strcmp(inoculacoes[i].loteVacina, lote) == 0) {
            return 1;
        }
    }
    return 0;
}

int apagaAplicacoesDadoUtente(Inoculacao** inoculacoes, int* numInoculacoes, char* nomeUtente) {
    int i = 0;
    int j;
    int numAplicacoesApagadas = 0;

    while (i < *numInoculacoes) {
        if (strcmp((*inoculacoes)[i].nomeUtente, nomeUtente) == 0) {
            free((*inoculacoes)[i].nomeUtente);
            // Move os elementos seguintes uma posição para a esquerda
            for (j = i; j < *numInoculacoes - 1; j++) {
                (*inoculacoes)[j] = (*inoculacoes)[j + 1];
            }
            (*numInoculacoes)--;
            numAplicacoesApagadas++;
        } else {
            i++;
        }
    }
    return numAplicacoesApagadas;
}

// rever se esta função está correta
int apagaAplicacoesDadaData(Inoculacao** inoculacoes, int* numInoculacoes,
                            char* nomeUtente, Data dataAplicacao) {
    int i = 0;
    int j;
    int numAplicacoesApagadas = 0;

    while (i < *numInoculacoes) {
        if (strcmp((*inoculacoes)[i].nomeUtente, nomeUtente) == 0 &&
            comparaDatas((*inoculacoes)[i].dataAplicacaoVacina, dataAplicacao) == 0) {
            free((*inoculacoes)[i].nomeUtente);
            // Move os elementos seguintes uma posição para a esquerda
            for (j = i; j < *numInoculacoes - 1; j++) {
                (*inoculacoes)[j] = (*inoculacoes)[j + 1];
            }
            (*numInoculacoes)--;
            numAplicacoesApagadas++;
        } else {
            i++;
        }
    }
    return numAplicacoesApagadas;
}

// Apaga aplicações de utente numa data dado um lote	
int apagaAplicacoesDadoLote(Inoculacao** inoculacoes, int* numInoculacoes, 
                            char* nomeUtente, Data dataAplicacao, char* lote) {
    int i = 0;
    int j;
    int numAplicacoesApagadas = 0;
    
    while (i < *numInoculacoes) {
        if (strcmp((*inoculacoes)[i].nomeUtente, nomeUtente) == 0 &&
            comparaDatas((*inoculacoes)[i].dataAplicacaoVacina, dataAplicacao) == 0 &&
            strcmp((*inoculacoes)[i].loteVacina, lote) == 0) {
            free((*inoculacoes)[i].nomeUtente);
            // Move os elementos seguintes uma posição para a esquerda
            for (j = i; j < *numInoculacoes - 1; j++) {
                (*inoculacoes)[j] = (*inoculacoes)[j + 1];
            }
            (*numInoculacoes)--;
            numAplicacoesApagadas++;
        } else {
            i++;
        }
    }
    return numAplicacoesApagadas;
}

void apagaRegistoAplicacaoVacina(char* args, Inoculacao** inoculacoes, 
                                int* numInoculacoes, int idioma) { // apagaste , Data dataAplicacao
    
    char argumentosRestantes[BUFSIZ] = "";
    char* nomeUtente = extraiNomeUtente(args, argumentosRestantes);
    int numAplicacoesApagadas = 0;
    char argumentoLote[BUFSIZ] = "";

    if (nomeUtente == NULL) {
        libertaMemoriaReservada(*inoculacoes, *numInoculacoes, idioma);
        return;
    }

    if (verificaSeUtenteExiste(*inoculacoes, *numInoculacoes, nomeUtente) == 0) {
        printf("%s: %s\n", nomeUtente, selecionaMensagem(idioma, MENSAGEM_ENGLISH_NO_SUCH_USER, MENSAGEM_PORTUGUESE_NO_SUCH_USER));
        free(nomeUtente);
        return;
    }

    // Verifica se há argumentos após nome Utente
    if (strlen(argumentosRestantes) > 0) {
        Data dataAplicacao = extraiData(argumentosRestantes, argumentoLote);
        // Verifica se a data é válida
        if (verificaValidadeData(dataAplicacao) == 1) {
            printf("%s\n", selecionaMensagem(idioma, MENSAGEM_ENGLISH_INVALID_DATE, MENSAGEM_PORTUGUESE_INVALID_DATE));
            free(nomeUtente);
            return;
        }
        // Verifica o lote específico
        if (strlen(argumentoLote) > 0) {
            if (verificaSeLoteExiste(*inoculacoes, *numInoculacoes, argumentoLote) == 0) {
                printf("%s: %s\n", argumentoLote, selecionaMensagem(idioma, MENSAGEM_ENGLISH_NO_SUCH_BATCH, MENSAGEM_PORTUGUESE_NO_SUCH_BATCH));
                free(nomeUtente);
                return;
            }
            // Apaga por utente, data e lote
            numAplicacoesApagadas = apagaAplicacoesDadoLote(inoculacoes, numInoculacoes, nomeUtente, dataAplicacao, argumentoLote);
        } else {
            // Apaga por utente e data
            numAplicacoesApagadas = apagaAplicacoesDadaData(inoculacoes, numInoculacoes, nomeUtente, dataAplicacao);
        }
    } else {
        // Apaga todas as aplicações do utente
        numAplicacoesApagadas = apagaAplicacoesDadoUtente(inoculacoes, numInoculacoes, nomeUtente);
    }
    // Redimensiona o array de inoculações
    if (*numInoculacoes > 0) {
        Inoculacao* inoculacoesTemporarias = (Inoculacao*)realloc(*inoculacoes, *numInoculacoes * sizeof(Inoculacao));
        if (inoculacoesTemporarias != NULL) {
            *inoculacoes = inoculacoesTemporarias;
        } else {
            free(nomeUtente);
            libertaMemoriaReservada(*inoculacoes, *numInoculacoes, idioma);
            return;
        }
    } else if (*numInoculacoes == 0) {
        free(*inoculacoes);
        *inoculacoes = NULL;
    }
    free(nomeUtente);
    printf("%d\n", numAplicacoesApagadas);
}

int main(int argc, char* argv[]) {
    char comando, str[BUFSIZ*8];
    Vacina vacinas[MAX_NUM_LOTES]; // Array de estruturas de dados de vacinas
    int numVacinas = 0; // Número de vacinas atualmente na lista
    int i;
    int idioma = 0; // 0 predefinido para inglês, 1 para português

    Inoculacao* inoculacoes = NULL;
    int numInoculacoes = 0;

    Data dataAtual = {1, 1, 2025};

    // Verifica se o idioma é português, através de argumento "pt"
    if (argc > 1 && strcmp(argv[1], "pt") == 0) {
        idioma = 1;
    }

    while (1){
        fgets(str, BUFSIZ*8, stdin);
        str[strcspn(str, "\n")] = 0; 
        comando = str[0];
        char *args = str + 1; // Ponteiro para os argumentos após o comando
        while (*args && isspace(*args)) {  // Salta os espaços
            args++;
        }

        switch (comando) {
            case 'q':
                // Dá free na memória antes de sair
                for (i = 0; i < numInoculacoes; i++) {
                    free(inoculacoes[i].nomeUtente);
                }
                free(inoculacoes);
                exit(0);
                break;
            case 'c':
            if (strlen(args) == 0) {
                printf("invalid command\n");
                break;
            } else {
                introduzNovoLote(args, vacinas, &numVacinas, &dataAtual, idioma);
                break; 
            }
            case 'l':
                if (strlen(args) == 0) {
                    vacinasOrdemCronologica(vacinas, numVacinas, "", inoculacoes, numInoculacoes, idioma);
                } else {
                    // Dividir os args por espaços para processar cada nome de vacina
                    char* nomesDeVacinas = strdup(args);
                    if (nomesDeVacinas == NULL) {
                        libertaMemoriaReservada(inoculacoes, numInoculacoes, idioma);
                        break;
                    }
                    char* nomeVacina = strtok(nomesDeVacinas, " \t");
                    while (nomeVacina != NULL) {
                        vacinasOrdemCronologica(vacinas, numVacinas, nomeVacina, inoculacoes, numInoculacoes, idioma);
                        nomeVacina = strtok(NULL, " \t");
                    }
                    free(nomesDeVacinas);
                }
                break;
            case 'a':
                aplicaDoseVacina(args, vacinas, numVacinas, &inoculacoes, &numInoculacoes, dataAtual, idioma);
                break;
            case 'r':
                retiraDisponibilidadeVacina(args, vacinas, &numVacinas, inoculacoes, numInoculacoes, idioma);
                break;
            case 'd':
                apagaRegistoAplicacaoVacina(args, &inoculacoes, &numInoculacoes, idioma); // apagaste dataAtual
                break;
            case 'u':
                listaAplicacoes(args, inoculacoes, numInoculacoes, idioma);
                break;
            case 't':
                avancaTempoEstipulado(args, &dataAtual, idioma);
                break;
            default:
                break;
        }
    }
}
