#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "persistencia.h"

#define BACKUP_AGENDA "data/agenda_backup_test.csv"
#define BACKUP_PROXIMO_ID "data/proximo_id_backup_test.txt"
#define ARQUIVO_PROXIMO_ID "data/proximo_id.txt"

/* Move os arquivos reais de dados para fora do caminho antes dos testes,
   para nao sobrescrever dados reais do usuario, e traz de volta ao final. */
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

static void escrever_arquivo_bruto(const char *conteudo)
{
    FILE *arquivo = fopen(ARQUIVO_AGENDA, "w");
    assert(arquivo != NULL);
    fputs(conteudo, arquivo);
    fclose(arquivo);
}

static void test_arquivo_inexistente(void)
{
    remove(ARQUIVO_AGENDA);
    remove(ARQUIVO_PROXIMO_ID);

    Evento saida[10];
    int n = persistencia_carregar(saida, 10);
    assert(n == 0);

    int id = persistencia_carregar_proximo_id();
    assert(id == 1);

    printf("[PASS] test_arquivo_inexistente\n");
}

static void test_arquivo_vazio(void)
{
    escrever_arquivo_bruto("");

    Evento saida[10];
    int n = persistencia_carregar(saida, 10);
    assert(n == 0);

    printf("[PASS] test_arquivo_vazio\n");
}

static void test_arquivo_so_cabecalho(void)
{
    escrever_arquivo_bruto("id,titulo,dia,mes,ano,hora,minuto,prioridade\n");

    Evento saida[10];
    int n = persistencia_carregar(saida, 10);
    assert(n == 0);

    printf("[PASS] test_arquivo_so_cabecalho\n");
}

static void test_arquivo_corrompido(void)
{
    /* Linhas com id nao numerico, campos faltando e uma linha valida no meio. */
    escrever_arquivo_bruto(
        "id,titulo,dia,mes,ano,hora,minuto,prioridade\n"
        "abc,Titulo Invalido,1,1,2026,10,0,1\n"
        "3,Sem Campos Suficientes\n"
        "2,Consulta Medica,15,7,2026,10,30,2\n"
        ",,,,,,,\n");

    Evento saida[10];
    int n = persistencia_carregar(saida, 10);

    assert(n == 1);
    assert(saida[0].id == 2);
    assert(strcmp(saida[0].titulo, "Consulta Medica") == 0);
    assert(saida[0].dia == 15 && saida[0].prioridade == 2);

    printf("[PASS] test_arquivo_corrompido\n");
}

static void test_titulo_muito_longo_e_ignorado_sem_travar(void)
{
    /* Titulo com mais de 99 caracteres nao casa com o campo seguinte
       (a virgula fica fora da janela de leitura), entao a linha inteira
       eh descartada pelo parser -- o importante e que isso nao trava o
       programa, e a linha valida seguinte continua sendo carregada. */
    char linha[400] = "id,titulo,dia,mes,ano,hora,minuto,prioridade\n1,";
    size_t inicio = strlen(linha);
    for (size_t i = inicio; i < inicio + 150; i++) {
        linha[i] = 'A';
    }
    linha[inicio + 150] = '\0';
    strcat(linha, ",1,1,2026,10,0,1\n");
    strcat(linha, "2,Evento Normal,20,7,2026,9,0,1\n");

    escrever_arquivo_bruto(linha);

    Evento saida[10];
    int n = persistencia_carregar(saida, 10);
    assert(n == 1);
    assert(saida[0].id == 2);
    assert(strcmp(saida[0].titulo, "Evento Normal") == 0);

    printf("[PASS] test_titulo_muito_longo_e_ignorado_sem_travar\n");
}

static void test_round_trip(void)
{
    Evento eventos[3] = {
        {1, "Reuniao", 20, 7, 2026, 9, 0, 1},
        {2, "Estudo", 21, 7, 2026, 14, 30, 2},
        {3, "Academia", 22, 7, 2026, 18, 0, 3}
    };

    assert(persistencia_salvar(eventos, 3) == 1);

    Evento saida[10];
    int n = persistencia_carregar(saida, 10);
    assert(n == 3);
    for (int i = 0; i < 3; i++) {
        assert(saida[i].id == eventos[i].id);
        assert(strcmp(saida[i].titulo, eventos[i].titulo) == 0);
        assert(saida[i].dia == eventos[i].dia);
        assert(saida[i].prioridade == eventos[i].prioridade);
    }

    printf("[PASS] test_round_trip\n");
}

static void test_salvar_qtd_zero(void)
{
    Evento eventos[1] = {0};
    assert(persistencia_salvar(eventos, 0) == 1);

    Evento saida[10];
    int n = persistencia_carregar(saida, 10);
    assert(n == 0);

    printf("[PASS] test_salvar_qtd_zero\n");
}

static void test_proximo_id_round_trip(void)
{
    assert(persistencia_salvar_proximo_id(42) == 1);
    assert(persistencia_carregar_proximo_id() == 42);

    printf("[PASS] test_proximo_id_round_trip\n");
}

int main(void)
{
    printf("--- Testes de Persistencia (arquivo vazio/corrompido) ---\n");

    backup_arquivos();

    test_arquivo_inexistente();
    test_arquivo_vazio();
    test_arquivo_so_cabecalho();
    test_arquivo_corrompido();
    test_titulo_muito_longo_e_ignorado_sem_travar();
    test_round_trip();
    test_salvar_qtd_zero();
    test_proximo_id_round_trip();

    restaurar_arquivos();

    printf("Todos os testes de persistencia passaram com sucesso!\n");
    return 0;
}
