#ifndef BUSCA_H
#define BUSCA_H

#include "evento.h"

/* Busca e filtra eventos por data exata (dia, mês, ano).
   Armazena os eventos encontrados no array `resultado` (respeitando `max_resultado`).
   Retorna a quantidade de eventos encontrados. */
int busca_por_data(const Evento *eventos, int qtd, int dia, int mes, int ano,
                   Evento *resultado, int max_resultado);

/* Busca e filtra eventos por prioridade (1 = Alta, 2 = Média, 3 = Baixa).
   Armazena os eventos encontrados no array `resultado` (respeitando `max_resultado`).
   Retorna a quantidade de eventos encontrados. */
int busca_por_prioridade(const Evento *eventos, int qtd, int prioridade,
                         Evento *resultado, int max_resultado);

/* Busca e filtra eventos que contenham a palavra-chave no título (busca insensível a maiúsculas/minúsculas).
   Armazena os eventos encontrados no array `resultado` (respeitando `max_resultado`).
   Retorna a quantidade de eventos encontrados. */
int busca_por_palavra_chave(const Evento *eventos, int qtd, const char *palavra_chave,
                            Evento *resultado, int max_resultado);

/* Busca e filtra eventos dentro de um intervalo de datas (inclusive).
   Armazena os eventos encontrados no array `resultado` (respeitando `max_resultado`).
   Retorna a quantidade de eventos encontrados. */
int busca_por_intervalo_datas(const Evento *eventos, int qtd,
                              int dia_ini, int mes_ini, int ano_ini,
                              int dia_fim, int mes_fim, int ano_fim,
                              Evento *resultado, int max_resultado);

#endif /* BUSCA_H */
