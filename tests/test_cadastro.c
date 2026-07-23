#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "cadastro.h"
#include "persistencia.h"

#define BACKUP_AGENDA "data/agenda_backup_test.csv"
#define BACKUP_PROXIMO_ID "data/proximo_id_backup_test.txt"
#define ARQUIVO_PROXIMO_ID "data/proximo_id.txt"

/* Move os arquivos reais de dados para fora do caminho antes dos testes,
   para que o cadastro comece de uma agenda vazia e os dados do usuario
   nao sejam sobrescritos pelos eventos de teste. */
static void backup_arquivos(void)
{
    remove(BACKUP_AGENDA);
    remove(BACKUP_PROXIMO_ID);
    rename(ARQUIVO_AGENDA, BACKUP_AGENDA);
    rename(ARQUIVO_PROXIMO_ID, BACKUP_PROXIMO_ID);
}

static void restaurar_arquivos(void)
{
    remove(ARQUIVO_AGENDA);
    remove(ARQUIVO_PROXIMO_ID);
    rename(BACKUP_AGENDA, ARQUIVO_AGENDA);
    rename(BACKUP_PROXIMO_ID, ARQUIVO_PROXIMO_ID);
}

static void test_lista_vazia(void)
{
    cadastro_init();
    assert(cadastro_qtd() == 0);

    Evento buf[MAX_EVENTOS];
    int n = cadastro_listar(buf, MAX_EVENTOS);
    assert(n == 0);

    printf("[PASS] test_lista_vazia\n");
}

static void test_campos_invalidos(void)
{
    int qtd_antes = cadastro_qtd();

    assert(cadastro_adicionar("", 1, 1, 2026, 10, 0, 1) == 0);
    assert(cadastro_adicionar(NULL, 1, 1, 2026, 10, 0, 1) == 0);
    assert(cadastro_adicionar("Valido", 1, 1, 2026, 10, 0, 0) == 0);
    assert(cadastro_adicionar("Valido", 1, 1, 2026, 10, 0, 4) == 0);

    assert(cadastro_qtd() == qtd_antes);
    printf("[PASS] test_campos_invalidos\n");
}

static void test_editar_remover_inexistente(void)
{
    assert(cadastro_editar(-1, "Qualquer", 1, 1, 2026, 0, 0, 1) == 0);
    assert(cadastro_remover(-1) == 0);
    printf("[PASS] test_editar_remover_inexistente\n");
}

static void test_adicionar_editar_remover(void)
{
    int qtd_antes = cadastro_qtd();

    assert(cadastro_adicionar("Reuniao", 20, 7, 2026, 9, 0, 1) == 1);
    assert(cadastro_qtd() == qtd_antes + 1);

    Evento buf[MAX_EVENTOS];
    int n = cadastro_listar(buf, MAX_EVENTOS);
    int id = buf[n - 1].id;

    assert(cadastro_editar(id, "Reuniao Editada", 21, 7, 2026, 10, 0, 2) == 1);

    n = cadastro_listar(buf, MAX_EVENTOS);
    int encontrado = 0;
    for (int i = 0; i < n; i++) {
        if (buf[i].id == id) {
            assert(strcmp(buf[i].titulo, "Reuniao Editada") == 0);
            assert(buf[i].dia == 21 && buf[i].prioridade == 2);
            encontrado = 1;
        }
    }
    assert(encontrado);

    assert(cadastro_remover(id) == 1);
    assert(cadastro_qtd() == qtd_antes);
    assert(cadastro_remover(id) == 0);

    printf("[PASS] test_adicionar_editar_remover\n");
}

static void test_lista_cheia(void)
{
    assert(cadastro_qtd() == 0);

    char titulo[32];
    for (int i = 0; i < MAX_EVENTOS; i++) {
        snprintf(titulo, sizeof(titulo), "Evento %d", i);
        assert(cadastro_adicionar(titulo, 1, 1, 2026, 0, 0, 1) == 1);
    }
    assert(cadastro_qtd() == MAX_EVENTOS);

    assert(cadastro_adicionar("Evento Extra", 1, 1, 2026, 0, 0, 1) == 0);
    assert(cadastro_qtd() == MAX_EVENTOS);

    Evento buf[MAX_EVENTOS];
    int n = cadastro_listar(buf, MAX_EVENTOS - 1);
    assert(n == MAX_EVENTOS - 1);

    printf("[PASS] test_lista_cheia\n");
}

int main(void)
{
    printf("--- Testes de Cadastro (casos extremos) ---\n");

    backup_arquivos();

    test_lista_vazia();
    test_campos_invalidos();
    test_editar_remover_inexistente();
    test_adicionar_editar_remover();
    test_lista_cheia();

    restaurar_arquivos();

    printf("Todos os testes de cadastro passaram com sucesso!\n");
    return 0;
}
