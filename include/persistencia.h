#ifndef PERSISTENCIA_H
#define PERSISTENCIA_H

#include "evento.h"

#define ARQUIVO_AGENDA "data/agenda.csv"

/* Salva todos os eventos no arquivo CSV. Retorna 1 em caso de sucesso, 0 em caso de erro. */
int persistencia_salvar(const Evento *eventos, int qtd);

/* Carrega eventos do arquivo CSV. Retorna a quantidade de eventos carregados. */
int persistencia_carregar(Evento *saida, int max);

/* Salva o próximo ID para manter consistência. */
int persistencia_salvar_proximo_id(int id);

/* Carrega o próximo ID salvo. Retorna 0 se não encontrar. */
int persistencia_carregar_proximo_id(void);

#endif /* PERSISTENCIA_H */
