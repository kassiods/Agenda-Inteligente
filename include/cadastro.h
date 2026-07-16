#ifndef CADASTRO_H
#define CADASTRO_H

#include "evento.h"

/* Inicializa o módulo de cadastro. */
void cadastro_init(void);

/* Adiciona um evento novo. Retorna 1 em caso de sucesso, 0 em caso de erro. */
int cadastro_adicionar(const char *titulo, int dia, int mes, int ano,
                       int hora, int minuto, int prioridade);

/* Edita um evento existente pelo id. Retorna 1 em caso de sucesso, 0 se não encontrou. */
int cadastro_editar(int id, const char *titulo, int dia, int mes, int ano,
                    int hora, int minuto, int prioridade);

/* Remove um evento pelo id. Retorna 1 em caso de sucesso, 0 se não encontrou. */
int cadastro_remover(int id);

/* Lista todos os eventos cadastrados. Retorna a quantidade de eventos. */
int cadastro_listar(Evento *saida, int max);

/* Retorna a quantidade total de eventos cadastrados. */
int cadastro_qtd(void);

#endif /* CADASTRO_H */
