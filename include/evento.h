#ifndef EVENTO_H
#define EVENTO_H

#define MAX_EVENTOS 100
#define MAX_TITULO 100

typedef struct {
    int id;
    char titulo[MAX_TITULO];
    int dia, mes, ano;
    int hora, minuto;
    int prioridade; /* 1 = alta, 2 = media, 3 = baixa */
} Evento;

#endif
