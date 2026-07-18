#include <stdio.h>
#include <string.h>
#include "cadastro.h"
#include "validacao.h"
#include "busca.h"

static void imprimir_evento(const Evento *e)
{
    const char *prior[] = {"", "Alta", "Media", "Baixa"};
    int p = (e->prioridade >= 1 && e->prioridade <= 3) ? e->prioridade : 0;
    printf("  [%d] %02d/%02d/%04d %02d:%02d | %-5s | %s\n",
           e->id, e->dia, e->mes, e->ano, e->hora, e->minuto,
           prior[p], e->titulo);
}

static void cmd_adicionar(void)
{
    char titulo[MAX_TITULO];
    int dia, mes, ano, hora, minuto, pri;

    printf("Titulo: ");
    scanf(" %[^\n]", titulo);

    printf("Data (dd mm aaaa): ");
    if (scanf("%d %d %d", &dia, &mes, &ano) != 3 || !validacao_data(dia, mes, ano)) {
        printf("Erro: Data invalida!\n");
        return;
    }

    printf("Hora (hh mm): ");
    if (scanf("%d %d", &hora, &minuto) != 2 || !validacao_horario(hora, minuto)) {
        printf("Erro: Horario invalido!\n");
        return;
    }

    printf("Prioridade (1=Alta 2=Media 3=Baixa): ");
    if (scanf("%d", &pri) != 1 || !validacao_prioridade(pri)) {
        printf("Erro: Prioridade invalida! Digite 1, 2 ou 3.\n");
        return;
    }

    if (cadastro_adicionar(titulo, dia, mes, ano, hora, minuto, pri)) {
        printf("Evento adicionado com sucesso!\n");
    } else {
        printf("Erro ao adicionar evento.\n");
    }
}

static void cmd_editar(void)
{
    int id;
    char titulo[MAX_TITULO];
    int dia, mes, ano, hora, minuto, pri;

    printf("ID do evento: ");
    if (scanf("%d", &id) != 1) {
        printf("Erro: ID invalido!\n");
        return;
    }

    printf("Novo titulo: ");
    scanf(" %[^\n]", titulo);

    printf("Nova data (dd mm aaaa): ");
    if (scanf("%d %d %d", &dia, &mes, &ano) != 3 || !validacao_data(dia, mes, ano)) {
        printf("Erro: Data invalida!\n");
        return;
    }

    printf("Nova hora (hh mm): ");
    if (scanf("%d %d", &hora, &minuto) != 2 || !validacao_horario(hora, minuto)) {
        printf("Erro: Horario invalido!\n");
        return;
    }

    printf("Nova prioridade (1=Alta 2=Media 3=Baixa): ");
    if (scanf("%d", &pri) != 1 || !validacao_prioridade(pri)) {
        printf("Erro: Prioridade invalida! Digite 1, 2 ou 3.\n");
        return;
    }

    if (cadastro_editar(id, titulo, dia, mes, ano, hora, minuto, pri)) {
        printf("Evento editado com sucesso!\n");
    } else {
        printf("Evento nao encontrado.\n");
    }
}

static void cmd_remover(void)
{
    int id;
    printf("ID do evento: ");
    if (scanf("%d", &id) != 1) {
        printf("Erro: ID invalido!\n");
        return;
    }

    if (cadastro_remover(id)) {
        printf("Evento removido com sucesso!\n");
    } else {
        printf("Evento nao encontrado.\n");
    }
}

static void cmd_listar(void)
{
    Evento lista[MAX_EVENTOS];
    int n = cadastro_listar(lista, MAX_EVENTOS);

    if (n == 0) {
        printf("Nenhum evento cadastrado.\n");
        return;
    }
    printf("Eventos (%d):\n", n);
    for (int i = 0; i < n; i++) {
        imprimir_evento(&lista[i]);
    }
}

static void cmd_buscar_data(void)
{
    Evento todos[MAX_EVENTOS];
    Evento resultado[MAX_EVENTOS];
    int dia, mes, ano;

    printf("Data para busca (dd mm aaaa): ");
    if (scanf("%d %d %d", &dia, &mes, &ano) != 3 || !validacao_data(dia, mes, ano)) {
        printf("Erro: Data invalida!\n");
        return;
    }

    int total = cadastro_listar(todos, MAX_EVENTOS);
    int encontrados = busca_por_data(todos, total, dia, mes, ano, resultado, MAX_EVENTOS);

    if (encontrados == 0) {
        printf("Nenhum evento encontrado para a data %02d/%02d/%04d.\n", dia, mes, ano);
        return;
    }

    printf("Eventos encontrados (%d):\n", encontrados);
    for (int i = 0; i < encontrados; i++) {
        imprimir_evento(&resultado[i]);
    }
}

static void cmd_buscar_prioridade(void)
{
    Evento todos[MAX_EVENTOS];
    Evento resultado[MAX_EVENTOS];
    int pri;

    printf("Prioridade (1=Alta 2=Media 3=Baixa): ");
    if (scanf("%d", &pri) != 1 || !validacao_prioridade(pri)) {
        printf("Erro: Prioridade invalida!\n");
        return;
    }

    int total = cadastro_listar(todos, MAX_EVENTOS);
    int encontrados = busca_por_prioridade(todos, total, pri, resultado, MAX_EVENTOS);

    if (encontrados == 0) {
        printf("Nenhum evento encontrado com prioridade %d.\n", pri);
        return;
    }

    printf("Eventos encontrados (%d):\n", encontrados);
    for (int i = 0; i < encontrados; i++) {
        imprimir_evento(&resultado[i]);
    }
}

static void cmd_buscar_palavra_chave(void)
{
    Evento todos[MAX_EVENTOS];
    Evento resultado[MAX_EVENTOS];
    char palavra[MAX_TITULO];

    printf("Palavra-chave: ");
    scanf(" %[^\n]", palavra);

    int total = cadastro_listar(todos, MAX_EVENTOS);
    int encontrados = busca_por_palavra_chave(todos, total, palavra, resultado, MAX_EVENTOS);

    if (encontrados == 0) {
        printf("Nenhum evento encontrado contendo \"%s\".\n", palavra);
        return;
    }

    printf("Eventos encontrados (%d):\n", encontrados);
    for (int i = 0; i < encontrados; i++) {
        imprimir_evento(&resultado[i]);
    }
}

static void cmd_buscar_intervalo(void)
{
    Evento todos[MAX_EVENTOS];
    Evento resultado[MAX_EVENTOS];
    int d1, m1, a1, d2, m2, a2;

    printf("Data inicial (dd mm aaaa): ");
    if (scanf("%d %d %d", &d1, &m1, &a1) != 3 || !validacao_data(d1, m1, a1)) {
        printf("Erro: Data inicial invalida!\n");
        return;
    }

    printf("Data final (dd mm aaaa): ");
    if (scanf("%d %d %d", &d2, &m2, &a2) != 3 || !validacao_data(d2, m2, a2)) {
        printf("Erro: Data final invalida!\n");
        return;
    }

    if (!validacao_intervalo_datas(d1, m1, a1, d2, m2, a2)) {
        printf("Erro: Data inicial deve ser anterior ou igual a data final!\n");
        return;
    }

    int total = cadastro_listar(todos, MAX_EVENTOS);
    int encontrados = busca_por_intervalo_datas(todos, total, d1, m1, a1, d2, m2, a2, resultado, MAX_EVENTOS);

    if (encontrados == 0) {
        printf("Nenhum evento encontrado no intervalo de %02d/%02d/%04d ate %02d/%02d/%04d.\n",
               d1, m1, a1, d2, m2, a2);
        return;
    }

    printf("Eventos encontrados no intervalo (%d):\n", encontrados);
    for (int i = 0; i < encontrados; i++) {
        imprimir_evento(&resultado[i]);
    }
}

int main(void)
{
    cadastro_init();
    int opcao;

    do {
        printf("\n=== Agenda Inteligente ===\n");
        printf("1 - Adicionar evento\n");
        printf("2 - Editar evento\n");
        printf("3 - Remover evento\n");
        printf("4 - Listar todos os eventos\n");
        printf("5 - Buscar por data\n");
        printf("6 - Buscar por prioridade\n");
        printf("7 - Buscar por palavra-chave\n");
        printf("8 - Buscar por intervalo de datas\n");
        printf("0 - Sair\n");
        printf("Opcao: ");
        if (scanf("%d", &opcao) != 1) {
            printf("Opcao invalida.\n");
            break;
        }

        switch (opcao) {
        case 1: cmd_adicionar(); break;
        case 2: cmd_editar(); break;
        case 3: cmd_remover(); break;
        case 4: cmd_listar(); break;
        case 5: cmd_buscar_data(); break;
        case 6: cmd_buscar_prioridade(); break;
        case 7: cmd_buscar_palavra_chave(); break;
        case 8: cmd_buscar_intervalo(); break;
        case 0: printf("Saindo...\n"); break;
        default: printf("Opcao invalida.\n"); break;
        }
    } while (opcao != 0);

    return 0;
}
