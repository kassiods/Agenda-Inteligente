#include "validacao.h"

int validacao_eh_bissexto(int ano)
{
    if (ano <= 0) {
        return 0;
    }
    return ((ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0)) ? 1 : 0;
}

int validacao_dias_no_mes(int mes, int ano)
{
    if (mes < 1 || mes > 12) {
        return 0;
    }

    switch (mes) {
    case 2:
        return validacao_eh_bissexto(ano) ? 29 : 28;
    case 4:
    case 6:
    case 9:
    case 11:
        return 30;
    default:
        return 31;
    }
}

int validacao_data(int dia, int mes, int ano)
{
    if (ano < 1900 || ano > 2100) {
        return 0;
    }

    if (mes < 1 || mes > 12) {
        return 0;
    }

    int max_dias = validacao_dias_no_mes(mes, ano);
    if (dia < 1 || dia > max_dias) {
        return 0;
    }

    return 1;
}

int validacao_horario(int hora, int minuto)
{
    if (hora < 0 || hora > 23) {
        return 0;
    }
    if (minuto < 0 || minuto > 59) {
        return 0;
    }
    return 1;
}

int validacao_prioridade(int prioridade)
{
    if (prioridade < 1 || prioridade > 3) {
        return 0;
    }
    return 1;
}

int validacao_intervalo_datas(int dia1, int mes1, int ano1,
                              int dia2, int mes2, int ano2)
{
    if (!validacao_data(dia1, mes1, ano1) || !validacao_data(dia2, mes2, ano2)) {
        return 0;
    }

    if (ano1 < ano2) {
        return 1;
    }
    if (ano1 > ano2) {
        return 0;
    }

    /* Anos iguais */
    if (mes1 < mes2) {
        return 1;
    }
    if (mes1 > mes2) {
        return 0;
    }

    /* Meses e anos iguais */
    return (dia1 <= dia2) ? 1 : 0;
}
