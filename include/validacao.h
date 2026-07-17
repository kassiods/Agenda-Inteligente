#ifndef VALIDACAO_H
#define VALIDACAO_H

/* Retorna 1 se o ano for bissexto, 0 caso contrário. */
int validacao_eh_bissexto(int ano);

/* Retorna a quantidade de dias do mês (1..12) no ano especificado.
   Retorna 0 se o mês for inválido. */
int validacao_dias_no_mes(int mes, int ano);

/* Valida se a data (dia, mês, ano) é válida no calendário gregoriano.
   Retorna 1 se válida, 0 caso contrário. */
int validacao_data(int dia, int mes, int ano);

/* Valida se o horário (hora, minuto) é válido (0..23h, 0..59min).
   Retorna 1 se válido, 0 caso contrário. */
int validacao_horario(int hora, int minuto);

/* Valida a prioridade do evento (1 = Alta, 2 = Média, 3 = Baixa).
   Retorna 1 se válida, 0 caso contrário. */
int validacao_prioridade(int prioridade);

/* Valida se a primeira data (dia1/mes1/ano1) é anterior ou igual à segunda data (dia2/mes2/ano2).
   Retorna 1 se o intervalo for válido, 0 caso contrário. */
int validacao_intervalo_datas(int dia1, int mes1, int ano1,
                              int dia2, int mes2, int ano2);

#endif /* VALIDACAO_H */
