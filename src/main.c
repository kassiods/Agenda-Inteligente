#include <stdio.h>
#include <string.h>
#include "cadastro.h"

static void imprimir_evento(const Evento *e)
{
    const char *prior[] = {"", "Alta", "Media", "Baixa"};
    printf("  [%d] %02d/%02d/%04d %02d:%02d | %s | %s\n",
           e->id, e->dia, e->mes, e->ano, e->hora, e->minuto,
           prior[e->prioridade], e->titulo);
}

static void cmd_adicionar(void)
{
    char titulo[MAX_TITULO];
    int dia, mes, ano, hora, minuto, pri;

    printf("Titulo: ");
    scanf(" %[^\n]", titulo);
    printf("Data (dd mm aaaa): ");
    scanf("%d %d %d", &dia, &mes, &ano);
    printf("Hora (hh mm): ");
    scanf("%d %d", &hora, &minuto);
    printf("Prioridade (1=Alta 2=Media 3=Baixa): ");
    scanf("%d", &pri);

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
    scanf("%d", &id);
    printf("Novo titulo: ");
    scanf(" %[^\n]", titulo);
    printf("Nova data (dd mm aaaa): ");
    scanf("%d %d %d", &dia, &mes, &ano);
    printf("Nova hora (hh mm): ");
    scanf("%d %d", &hora, &minuto);
    printf("Nova prioridade (1=Alta 2=Media 3=Baixa): ");
    scanf("%d", &pri);

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
    scanf("%d", &id);

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

int main(void)
{
    cadastro_init();
    int opcao;

    do {
        printf("\n=== Agenda Inteligente ===\n");
        printf("1 - Adicionar evento\n");
        printf("2 - Editar evento\n");
        printf("3 - Remover evento\n");
        printf("4 - Listar eventos\n");
        printf("0 - Sair\n");
        printf("Opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
        case 1: cmd_adicionar(); break;
        case 2: cmd_editar(); break;
        case 3: cmd_remover(); break;
        case 4: cmd_listar(); break;
        case 0: printf("Saindo...\n"); break;
        default: printf("Opcao invalida.\n"); break;
        }
    } while (opcao != 0);

    return 0;
}
