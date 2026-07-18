#include <ctype.h>
#include <string.h>
#include "busca.h"
#include "validacao.h"

/* Helper estático para busca case-insensitive de substring */
static int contem_palavra_chave(const char *texto, const char *palavra)
{
    if (!texto || !palavra) {
        return 0;
    }
    if (palavra[0] == '\0') {
        return 1;
    }

    size_t len_t = strlen(texto);
    size_t len_p = strlen(palavra);

    if (len_p > len_t) {
        return 0;
    }

    for (size_t i = 0; i <= len_t - len_p; i++) {
        size_t j = 0;
        while (j < len_p && tolower((unsigned char)texto[i + j]) == tolower((unsigned char)palavra[j])) {
            j++;
        }
        if (j == len_p) {
            return 1;
        }
    }

    return 0;
}

/* Converte data para formato YYYYMMDD para comparação rápida */
static long data_para_long(int dia, int mes, int ano)
{
    return (long)ano * 10000L + (long)mes * 100L + (long)dia;
}

int busca_por_data(const Evento *eventos, int qtd, int dia, int mes, int ano,
                   Evento *resultado, int max_resultado)
{
    if (!eventos || !resultado || qtd <= 0 || max_resultado <= 0) {
        return 0;
    }

    int encontrados = 0;
    for (int i = 0; i < qtd; i++) {
        if (eventos[i].dia == dia && eventos[i].mes == mes && eventos[i].ano == ano) {
            if (encontrados < max_resultado) {
                resultado[encontrados] = eventos[i];
                encontrados++;
            }
        }
    }

    return encontrados;
}

int busca_por_prioridade(const Evento *eventos, int qtd, int prioridade,
                         Evento *resultado, int max_resultado)
{
    if (!eventos || !resultado || qtd <= 0 || max_resultado <= 0) {
        return 0;
    }

    int encontrados = 0;
    for (int i = 0; i < qtd; i++) {
        if (eventos[i].prioridade == prioridade) {
            if (encontrados < max_resultado) {
                resultado[encontrados] = eventos[i];
                encontrados++;
            }
        }
    }

    return encontrados;
}

int busca_por_palavra_chave(const Evento *eventos, int qtd, const char *palavra_chave,
                            Evento *resultado, int max_resultado)
{
    if (!eventos || !resultado || !palavra_chave || qtd <= 0 || max_resultado <= 0) {
        return 0;
    }

    int encontrados = 0;
    for (int i = 0; i < qtd; i++) {
        if (contem_palavra_chave(eventos[i].titulo, palavra_chave)) {
            if (encontrados < max_resultado) {
                resultado[encontrados] = eventos[i];
                encontrados++;
            }
        }
    }

    return encontrados;
}

int busca_por_intervalo_datas(const Evento *eventos, int qtd,
                              int dia_ini, int mes_ini, int ano_ini,
                              int dia_fim, int mes_fim, int ano_fim,
                              Evento *resultado, int max_resultado)
{
    if (!eventos || !resultado || qtd <= 0 || max_resultado <= 0) {
        return 0;
    }

    long d_ini = data_para_long(dia_ini, mes_ini, ano_ini);
    long d_fim = data_para_long(dia_fim, mes_fim, ano_fim);

    int encontrados = 0;
    for (int i = 0; i < qtd; i++) {
        long d_ev = data_para_long(eventos[i].dia, eventos[i].mes, eventos[i].ano);
        if (d_ev >= d_ini && d_ev <= d_fim) {
            if (encontrados < max_resultado) {
                resultado[encontrados] = eventos[i];
                encontrados++;
            }
        }
    }

    return encontrados;
}
