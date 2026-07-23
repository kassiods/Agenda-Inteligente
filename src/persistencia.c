#include "persistencia.h"
#include <stdio.h>
#include <string.h>

#define MAX_LINHA 256

int persistencia_salvar(const Evento *eventos, int qtd)
{
    FILE *arquivo = fopen(ARQUIVO_AGENDA, "w");
    if (arquivo == NULL) {
        return 0;
    }

    fprintf(arquivo, "id,titulo,dia,mes,ano,hora,minuto,prioridade\n");
    for (int i = 0; i < qtd; i++) {
        fprintf(arquivo, "%d,%s,%d,%d,%d,%d,%d,%d\n",
                eventos[i].id, eventos[i].titulo,
                eventos[i].dia, eventos[i].mes, eventos[i].ano,
                eventos[i].hora, eventos[i].minuto, eventos[i].prioridade);
    }

    fclose(arquivo);
    return 1;
}

int persistencia_carregar(Evento *saida, int max)
{
    FILE *arquivo = fopen(ARQUIVO_AGENDA, "r");
    if (arquivo == NULL) {
        return 0;
    }

    char linha[MAX_LINHA];
    int count = 0;

    /* Pular cabeçalho */
    if (fgets(linha, MAX_LINHA, arquivo) == NULL) {
        fclose(arquivo);
        return 0;
    }

    while (count < max && fgets(linha, MAX_LINHA, arquivo) != NULL) {
        /* Remover newline */
        linha[strcspn(linha, "\r\n")] = '\0';

        if (sscanf(linha, "%d,%99[^,],%d,%d,%d,%d,%d,%d",
                   &saida[count].id, saida[count].titulo,
                   &saida[count].dia, &saida[count].mes, &saida[count].ano,
                   &saida[count].hora, &saida[count].minuto,
                   &saida[count].prioridade) == 8) {
            count++;
        }
    }

    fclose(arquivo);
    return count;
}

int persistencia_salvar_proximo_id(int id)
{
    FILE *arquivo = fopen("data/proximo_id.txt", "w");
    if (arquivo == NULL) {
        return 0;
    }

    fprintf(arquivo, "%d\n", id);
    fclose(arquivo);
    return 1;
}

int persistencia_carregar_proximo_id(void)
{
    FILE *arquivo = fopen("data/proximo_id.txt", "r");
    if (arquivo == NULL) {
        return 1;
    }

    int id = 1;
    fscanf(arquivo, "%d", &id);
    fclose(arquivo);
    return id > 0 ? id : 1;
}
