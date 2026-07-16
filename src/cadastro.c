#include "cadastro.h"
#include <stdio.h>
#include <string.h>

static Evento eventos[MAX_EVENTOS];
static int qtd_eventos = 0;
static int proximo_id = 1;

void cadastro_init(void)
{
    qtd_eventos = 0;
    proximo_id = 1;
}

int cadastro_adicionar(const char *titulo, int dia, int mes, int ano,
                       int hora, int minuto, int prioridade)
{
    if (qtd_eventos >= MAX_EVENTOS) {
        return 0;
    }
    if (titulo == NULL || titulo[0] == '\0') {
        return 0;
    }
    if (prioridade < 1 || prioridade > 3) {
        return 0;
    }

    Evento *e = &eventos[qtd_eventos];
    e->id = proximo_id++;
    snprintf(e->titulo, MAX_TITULO, "%s", titulo);
    e->dia = dia;
    e->mes = mes;
    e->ano = ano;
    e->hora = hora;
    e->minuto = minuto;
    e->prioridade = prioridade;
    qtd_eventos++;

    return 1;
}

int cadastro_editar(int id, const char *titulo, int dia, int mes, int ano,
                    int hora, int minuto, int prioridade)
{
    if (titulo == NULL || titulo[0] == '\0') {
        return 0;
    }
    if (prioridade < 1 || prioridade > 3) {
        return 0;
    }

    for (int i = 0; i < qtd_eventos; i++) {
        if (eventos[i].id == id) {
            snprintf(eventos[i].titulo, MAX_TITULO, "%s", titulo);
            eventos[i].dia = dia;
            eventos[i].mes = mes;
            eventos[i].ano = ano;
            eventos[i].hora = hora;
            eventos[i].minuto = minuto;
            eventos[i].prioridade = prioridade;
            return 1;
        }
    }
    return 0;
}

int cadastro_remover(int id)
{
    for (int i = 0; i < qtd_eventos; i++) {
        if (eventos[i].id == id) {
            eventos[i] = eventos[qtd_eventos - 1];
            qtd_eventos--;
            return 1;
        }
    }
    return 0;
}

int cadastro_listar(Evento *saida, int max)
{
    int n = qtd_eventos < max ? qtd_eventos : max;
    for (int i = 0; i < n; i++) {
        saida[i] = eventos[i];
    }
    return n;
}

int cadastro_qtd(void)
{
    return qtd_eventos;
}
