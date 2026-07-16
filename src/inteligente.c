#include "inteligente.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Converte data/hora para minutos desde o início do ano para comparação */
static long para_minutos(int dia, int mes, int ano, int hora, int minuto) {
    long minutos = 0;
    minutos += (long)ano * 365 * 24 * 60;
    minutos += (long)mes * 30 * 24 * 60;
    minutos += (long)dia * 24 * 60;
    minutos += (long)hora * 60;
    minutos += (long)minuto;
    return minutos;
}

/* Função de comparação para qsort: prioridade primeiro, depois data/hora */
static int comparar_eventos(const void *a, const void *b) {
    const Evento *ea = (const Evento *)a;
    const Evento *eb = (const Evento *)b;

    /* Primeiro compara por prioridade (menor número = maior prioridade) */
    if (ea->prioridade != eb->prioridade) {
        return ea->prioridade - eb->prioridade;
    }

    /* Depois por data/hora crescente */
    long min_a = para_minutos(ea->dia, ea->mes, ea->ano, ea->hora, ea->minuto);
    long min_b = para_minutos(eb->dia, eb->mes, eb->ano, eb->hora, eb->minuto);

    if (min_a < min_b) return -1;
    if (min_a > min_b) return 1;
    return 0;
}

int inteligente_ordenar(Evento *eventos, int qtd) {
    if (eventos == NULL || qtd <= 0) {
        return 0;
    }
    qsort(eventos, (size_t)qtd, sizeof(Evento), comparar_eventos);
    return qtd;
}

int inteligente_conflito(const Evento *a, const Evento *b) {
    if (a == NULL || b == NULL) {
        return 0;
    }

    /* Verifica se são do mesmo dia */
    if (a->dia != b->dia || a->mes != b->mes || a->ano != b->ano) {
        return 0;
    }

    /* Converte para minutos para facilitar comparação */
    long ini_a = para_minutos(a->dia, a->mes, a->ano, a->hora, a->minuto);
    long ini_b = para_minutos(b->dia, b->mes, b->ano, b->hora, b->minuto);

    /* Considera duração de 1 hora para cada evento */
    long fim_a = ini_a + 60;
    long fim_b = ini_b + 60;

    /* Há sobreposição se um começa antes do outro terminar */
    if (ini_a < fim_b && ini_b < fim_a) {
        return 1;
    }

    return 0;
}

int inteligente_listar_conflitos(const Evento *eventos, int qtd) {
    int conflitos = 0;

    if (eventos == NULL || qtd <= 0) {
        return 0;
    }

    printf("\n=== Conflitos de Horario ===\n");

    for (int i = 0; i < qtd; i++) {
        for (int j = i + 1; j < qtd; j++) {
            if (inteligente_conflito(&eventos[i], &eventos[j])) {
                printf("CONFLITO: \"%s\" (%02d/%02d/%04d %02d:%02d) com "
                       "\"%s\" (%02d/%02d/%04d %02d:%02d)\n",
                       eventos[i].titulo, eventos[i].dia, eventos[i].mes,
                       eventos[i].ano, eventos[i].hora, eventos[i].minuto,
                       eventos[j].titulo, eventos[j].dia, eventos[j].mes,
                       eventos[j].ano, eventos[j].hora, eventos[j].minuto);
                conflitos++;
            }
        }
    }

    if (conflitos == 0) {
        printf("Nenhum conflito encontrado.\n");
    }

    return conflitos;
}

int inteligente_eventos_do_dia(const Evento *eventos, int qtd,
                               int dia, int mes, int ano) {
    int encontrados = 0;

    if (eventos == NULL || qtd <= 0) {
        return 0;
    }

    printf("\n=== Eventos do Dia %02d/%02d/%04d ===\n", dia, mes, ano);

    for (int i = 0; i < qtd; i++) {
        if (eventos[i].dia == dia && eventos[i].mes == mes &&
            eventos[i].ano == ano) {
            printf("[%d] %02d:%02d - %s (Prioridade: %d)\n",
                   eventos[i].id, eventos[i].hora, eventos[i].minuto,
                   eventos[i].titulo, eventos[i].prioridade);
            encontrados++;
        }
    }

    if (encontrados == 0) {
        printf("Nenhum evento para hoje.\n");
    }

    return encontrados;
}

int inteligente_eventos_atrasados(const Evento *eventos, int qtd,
                                  int dia, int mes, int ano) {
    int atrasados = 0;
    long hoje = para_minutos(dia, mes, ano, 0, 0);

    if (eventos == NULL || qtd <= 0) {
        return 0;
    }

    printf("\n=== Eventos Atrasados ===\n");

    for (int i = 0; i < qtd; i++) {
        long evento = para_minutos(eventos[i].dia, eventos[i].mes,
                                   eventos[i].ano, eventos[i].hora,
                                   eventos[i].minuto);
        if (evento < hoje) {
            printf("[%d] %02d/%02d/%04d %02d:%02d - %s\n",
                   eventos[i].id, eventos[i].dia, eventos[i].mes,
                   eventos[i].ano, eventos[i].hora, eventos[i].minuto,
                   eventos[i].titulo);
            atrasados++;
        }
    }

    if (atrasados == 0) {
        printf("Nenhum evento atrasado.\n");
    }

    return atrasados;
}

void inteligente_resumo(const Evento *eventos, int qtd,
                        int dia, int mes, int ano) {
    printf("\n========================================\n");
    printf("       RESUMO DA AGENDA INTELIGENTE\n");
    printf("========================================\n");

    inteligente_eventos_do_dia(eventos, qtd, dia, mes, ano);
    inteligente_eventos_atrasados(eventos, qtd, dia, mes, ano);
    inteligente_listar_conflitos(eventos, qtd);

    printf("========================================\n\n");
}
