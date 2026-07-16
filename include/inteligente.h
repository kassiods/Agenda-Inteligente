#ifndef INTELIGENTE_H
#define INTELIGENTE_H

#include "evento.h"

/* Ordena eventos por prioridade (1>2>3) e depois por data/hora crescente.
   Retorna a quantidade de eventos ordenados. */
int inteligente_ordenar(Evento *eventos, int qtd);

/* Verifica se há conflito entre dois eventos (mesmo dia e horários sobrepostos).
   Retorna 1 se há conflito, 0 caso contrário. */
int inteligente_conflito(const Evento *a, const Evento *b);

/* Mostra todos os conflitos existentes entre os eventos.
   Retorna a quantidade de conflitos encontrados. */
int inteligente_listar_conflitos(const Evento *eventos, int qtd);

/* Mostra eventos do dia atual (dia, mes, ano passados como parâmetro).
   Retorna a quantidade de eventos do dia. */
int inteligente_eventos_do_dia(const Evento *eventos, int qtd,
                               int dia, int mes, int ano);

/* Mostra eventos atrasados (anteriores à data atual).
   Retorna a quantidade de eventos atrasados. */
int inteligente_eventos_atrasados(const Evento *eventos, int qtd,
                                  int dia, int mes, int ano);

/* Mostra resumo completo: eventos do dia, atrasados e conflitos. */
void inteligente_resumo(const Evento *eventos, int qtd,
                        int dia, int mes, int ano);

#endif /* INTELIGENTE_H */
