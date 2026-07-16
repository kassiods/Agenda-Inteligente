# Agenda Inteligente

## Visão geral

Projeto acadêmico em **C**: uma agenda de linha de comando que cadastra eventos/tarefas, guarda tudo em arquivo e usa algumas regras simples para ajudar o usuário (ordenar por prioridade, avisar conflito de horário, lembrar do que está por vir). Prazo: **2 semanas** (16/07/2026 a 30/07/2026), equipe de **5 pessoas**.

Trello do projeto: https://trello.com/b/21WYrsiF/agenda-inteligente

## Escopo do MVP

- Cadastro de eventos (título, data, hora, prioridade): adicionar, editar, remover, listar.
- Persistência em **arquivo texto/CSV** (sem SQLite nem libs externas).
- Interface: **menu de texto simples** (loop com `scanf`/`printf`, opções numeradas).
- "Inteligência" do MVP:
  - ordenar eventos por prioridade e data;
  - detectar conflito quando dois eventos têm data/hora sobrepostas;
  - avisar sobre eventos do dia e eventos atrasados ao abrir o programa.

Qualquer coisa além disso (parsing de linguagem natural, notificações em tempo real, ncurses) fica de fora do MVP — só entra se sobrar tempo depois da Semana 2.

## Equipe e responsabilidades

| Pessoa | Papel | Módulo |
|---|---|---|
| Kassio | Scrum Master + Dev | Setup do projeto, integração final (`main.c`), condução do Trello/reuniões |
| Elton | Dev | Cadastro de eventos (CRUD) |
| Evillyn | Dev | Persistência em arquivo |
| Nailan | Dev | Módulo inteligente (prioridade, conflito de horário, lembretes) |
| Lincoln | Dev | Busca/filtro de eventos + validação de entrada |

Cada pessoa é dona do próprio módulo (arquivo `.c`/`.h`) e escreve nele sem depender dos outros módulos estarem prontos — combinem a `struct Evento` (campos e tipos) logo na Semana 1 para todo mundo compilar contra a mesma interface.

## Estrutura de pastas sugerida

```
Agenda-Inteligente/
├── src/          # .c de cada módulo
├── include/      # .h com as interfaces (struct Evento, protótipos)
├── data/         # arquivo de dados (agenda.csv), ignorado no git se tiver dado de teste
├── Makefile
└── CLAUDE.md
```

## Fluxo de trabalho: Git + Trello

- **Uma branch por integrante**, criada a partir da `main`: `kassio`, `elton`, `evillyn`, `nailan`, `lincoln` (ou `nome/modulo`, ex. `elton/cadastro`, se preferirem mais granularidade).
- `main` só recebe código via **Pull Request** revisado por outro integrante (nunca revisar o próprio PR).
- Para o board do Trello mostrar branches/commits/PRs nos cards:
  1. Ativar o **Power-Up "GitHub"** no board (menu do board → Power-Ups → GitHub → conectar o repositório).
  2. Ao abrir um PR ou fazer commit relacionado a um card, anexar o link do card no PR/commit, ou usar o botão de anexo do próprio card no Trello para linkar a branch/PR.
- Cada tarefa do board = 1 card. Mover o card conforme o trabalho avança:
  `Product Backlog → Sprint Atual → Em Andamento → Em Revisão → Concluído`
  (usar `Impedimentos` só quando algo travar o time).
- Sugestão de WIP limit: no máximo 1 card por pessoa em "Em Andamento" por vez.

## Cronograma (2 semanas)

**Semana 1 (16/07–23/07) — módulos em paralelo**
Setup do repositório e branches, definição da `struct Evento`, e cada integrante implementa seu módulo isolado (cadastro, persistência, módulo inteligente, busca/validação). Cards já estão em "Sprint Atual" no Trello.

**Semana 2 (24/07–30/07) — integração e fechamento**
Integração dos módulos no `main.c`, testes de casos extremos de cada módulo, revisão de código cruzada via PR, documentação final e ensaio da apresentação. Cards estão no "Product Backlog", movidos para "Sprint Atual" no início da semana 2.

## Build

```sh
gcc -Wall -Wextra -Iinclude -o agenda src/*.c
./agenda
```

## Definition of Done (por card)

- Compila sem warnings.
- Testado manualmente pelo autor do módulo.
- Revisado por outro integrante via Pull Request.
- Card movido para "Concluído" no Trello só depois do merge na `main`.
