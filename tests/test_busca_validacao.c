#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "validacao.h"
#include "busca.h"

static void test_validacao_bissexto(void)
{
    assert(validacao_eh_bissexto(2024) == 1);
    assert(validacao_eh_bissexto(2026) == 0);
    assert(validacao_eh_bissexto(2000) == 1);
    assert(validacao_eh_bissexto(1900) == 0);
    assert(validacao_eh_bissexto(-4) == 0);
    printf("[PASS] test_validacao_bissexto\n");
}

static void test_validacao_dias_no_mes(void)
{
    assert(validacao_dias_no_mes(1, 2026) == 31);
    assert(validacao_dias_no_mes(2, 2024) == 29);
    assert(validacao_dias_no_mes(2, 2026) == 28);
    assert(validacao_dias_no_mes(4, 2026) == 30);
    assert(validacao_dias_no_mes(13, 2026) == 0);
    assert(validacao_dias_no_mes(0, 2026) == 0);
    printf("[PASS] test_validacao_dias_no_mes\n");
}

static void test_validacao_data(void)
{
    assert(validacao_data(16, 7, 2026) == 1);
    assert(validacao_data(29, 2, 2024) == 1);
    assert(validacao_data(29, 2, 2026) == 0);
    assert(validacao_data(31, 4, 2026) == 0);
    assert(validacao_data(0, 5, 2026) == 0);
    assert(validacao_data(10, 15, 2026) == 0);
    assert(validacao_data(10, 10, 1850) == 0);
    printf("[PASS] test_validacao_data\n");
}

static void test_validacao_horario(void)
{
    assert(validacao_horario(0, 0) == 1);
    assert(validacao_horario(14, 30) == 1);
    assert(validacao_horario(23, 59) == 1);
    assert(validacao_horario(24, 0) == 0);
    assert(validacao_horario(12, 60) == 0);
    assert(validacao_horario(-1, 30) == 0);
    printf("[PASS] test_validacao_horario\n");
}

static void test_validacao_prioridade(void)
{
    assert(validacao_prioridade(1) == 1);
    assert(validacao_prioridade(2) == 1);
    assert(validacao_prioridade(3) == 1);
    assert(validacao_prioridade(0) == 0);
    assert(validacao_prioridade(4) == 0);
    printf("[PASS] test_validacao_prioridade\n");
}

static void test_validacao_intervalo(void)
{
    assert(validacao_intervalo_datas(10, 7, 2026, 15, 7, 2026) == 1);
    assert(validacao_intervalo_datas(10, 7, 2026, 10, 7, 2026) == 1);
    assert(validacao_intervalo_datas(15, 7, 2026, 10, 7, 2026) == 0);
    assert(validacao_intervalo_datas(31, 12, 2025, 1, 1, 2026) == 1);
    printf("[PASS] test_validacao_intervalo\n");
}

static void test_busca(void)
{
    Evento lista[3] = {
        {1, "Reuniao do Trabalho", 16, 7, 2026, 14, 30, 1},
        {2, "Prova de Calculo", 18, 7, 2026, 8, 0, 1},
        {3, "Ir ao mercado", 16, 7, 2026, 18, 0, 3}
    };

    Evento res[10];

    /* Teste busca por data */
    int n = busca_por_data(lista, 3, 16, 7, 2026, res, 10);
    assert(n == 2);
    assert(res[0].id == 1);
    assert(res[1].id == 3);

    /* Teste busca por prioridade */
    n = busca_por_prioridade(lista, 3, 1, res, 10);
    assert(n == 2);
    assert(res[0].id == 1);
    assert(res[1].id == 2);

    /* Teste busca por palavra-chave (case-insensitive) */
    n = busca_por_palavra_chave(lista, 3, "reuniao", res, 10);
    assert(n == 1);
    assert(res[0].id == 1);

    n = busca_por_palavra_chave(lista, 3, "TRABALHO", res, 10);
    assert(n == 1);
    assert(res[0].id == 1);

    /* Teste busca por intervalo de datas */
    n = busca_por_intervalo_datas(lista, 3, 15, 7, 2026, 17, 7, 2026, res, 10);
    assert(n == 2);
    assert(res[0].id == 1);
    assert(res[1].id == 3);

    printf("[PASS] test_busca\n");
}

int main(void)
{
    printf("--- Testes de Validação e Busca ---\n");
    test_validacao_bissexto();
    test_validacao_dias_no_mes();
    test_validacao_data();
    test_validacao_horario();
    test_validacao_prioridade();
    test_validacao_intervalo();
    test_busca();
    printf("Todos os testes passaram com sucesso!\n");
    return 0;
}
