# Boas Práticas para Projetos em C

Guia de referência do projeto. Baseado em convenções amplamente adotadas pela comunidade (estruturas de projetos open source, guias de estilo clássicos de C e o guia de hardening de compiladores da OpenSSF).

---

## 1. Estrutura de Pastas

```
meu-projeto/
├── bin/                  # Executáveis finais (gerados pelo build, não versionar)
├── build/                # Arquivos intermediários (*.o) (gerados, não versionar)
├── docs/                 # Documentação do projeto
├── include/              # Headers públicos (.h)
│   └── meu_projeto/      # Subpasta com o nome do projeto evita conflito de includes
├── lib/                  # Bibliotecas de terceiros (vendored)
├── scripts/              # Scripts de setup, build auxiliar, etc.
├── src/                  # Código-fonte (.c) e headers privados
│   └── main.c            # Ponto de entrada
├── tests/                # Testes
├── .clang-format         # Configuração do formatador
├── .clang-tidy           # Configuração do linter
├── .gitignore
├── Makefile              # (ou CMakeLists.txt)
├── LICENSE
└── README.md
```

### Regras
- **`src/` só contém código do projeto.** Headers usados apenas internamente ficam junto do `.c` correspondente em `src/`; headers que formam a API pública ficam em `include/`.
- **Nunca versionar artefatos de build** (`bin/`, `build/`, `*.o`, `*.out`). Adicione ao `.gitignore`.
- **Não criar níveis de pastas demais.** Comece simples; adicione subpastas em `src/` apenas quando um módulo crescer o suficiente para justificar (ex.: `src/net/`, `src/parser/`).
- **Um módulo = um par `nome.c` + `nome.h`.** O `.h` declara apenas o que outros módulos precisam; o resto fica `static` dentro do `.c`.
- Projetos que são **biblioteca** (não executável) trocam `bin/` por `lib/` de saída e caprichem no `include/`, pois é o que o usuário final consome.

---

## 2. Headers (.h)

- **Sempre use include guard** (ou `#pragma once`, mas o guard é mais portável):
  ```c
  #ifndef MEU_PROJETO_MODULO_H
  #define MEU_PROJETO_MODULO_H
  /* conteúdo */
  #endif /* MEU_PROJETO_MODULO_H */
  ```
- **Headers devem ser autocontidos:** cada `.h` inclui tudo que ele mesmo precisa para compilar sozinho.
- **Exponha o mínimo.** O header é um contrato: só protótipos, tipos e constantes que outros módulos realmente usam.
- **Nunca use caminhos absolutos** em `#include`. Use `<...>` para sistema e `"..."` para o projeto, com `-I include/` no compilador.
- Não defina variáveis em headers; apenas declare com `extern` se for inevitável (prefira evitar globais).

## 3. Nomenclatura

| Elemento | Convenção | Exemplo |
|---|---|---|
| Variáveis, funções | `snake_case` minúsculo | `parse_line()` |
| Tipos (`struct`, `enum`, `typedef`) | `snake_case` + sufixo `_t` opcional | `buffer_t` |
| Macros e constantes | `MAIUSCULAS_COM_UNDERSCORE` | `MAX_BUFFER_SIZE` |
| Funções públicas de um módulo | prefixo do módulo | `net_connect()`, `parser_init()` |
| Arquivos | minúsculos, `.c` / `.h` | `string_utils.c` |

- **Prefixo de módulo em tudo que é global** (funções e símbolos com linkage externo) evita colisão de nomes e mostra a arquitetura.
- **Nunca comece nomes com underscore** (`_x`, `__x`): são reservados para o sistema/compilador.
- Nomes devem dizer o que a coisa é. `retry_count` em vez de `rc`; inclua unidade quando fizer sentido (`timeout_ms`).

## 4. Estilo de Código

- **Linhas até ~80 colunas**; linhas muito longas por indentação profunda são sinal de código mal organizado.
- **Arquivos até ~1000 linhas**; acima disso, considere dividir em módulos.
- **Funções curtas e com uma responsabilidade.** Evite aninhamento profundo (mais de 3 níveis de `if`/`for` pede refatoração).
- Declare ponteiros com o `*` junto ao nome: `int *p, *q;` (evita a pegadinha de `int* p, q;`).
- Sempre use chaves `{}` mesmo em blocos de uma linha.
- Ao editar código existente, **siga o estilo do arquivo**, não o seu gosto pessoal.
- Automatize: use `clang-format` para formatação e `clang-tidy` para lint — não discuta estilo na mão.

## 5. Macros e Pré-processador

- Prefira `static inline` a macro-função; prefira `enum` ou `const` a `#define` para constantes tipadas.
- Se usar macro-função, envolva parâmetros e o corpo em parênteses: `#define SQ(x) ((x) * (x))`.
- Evite compilação condicional espalhada (`#ifdef` por todo lado) — dificulta teste. Isole diferenças de plataforma em arquivos próprios.

## 6. Memória e Segurança

- **Todo `malloc` tem dono.** Documente em comentário quem é responsável por liberar. Padrão comum: quem aloca, libera; ou funções `x_create()` / `x_destroy()` em pares.
- **Cheque todo retorno de alocação** (`malloc`, `calloc`, `realloc`) e de I/O.
- Após `free(p)`, faça `p = NULL;` para transformar use-after-free em crash óbvio.
- Prefira funções com limite de tamanho: `snprintf` em vez de `sprintf`, `strncpy`/`strlcpy` em vez de `strcpy`, `fgets` em vez de `gets` (nunca use `gets`).
- Inicialize variáveis na declaração; nunca leia memória não inicializada.
- Valide entradas externas (arquivos, rede, argv) antes de usar.

## 7. Compilação

### Flags de desenvolvimento (sempre)
```
-std=c11 -Wall -Wextra -Wpedantic -Werror -g
```
Warnings são bugs em potencial — **compile com zero warnings**.

### Flags recomendadas adicionais
```
-Wconversion -Wshadow -Wformat=2 -Wcast-qual
```

### Sanitizers em builds de debug/teste
```
-fsanitize=address,undefined -fno-omit-frame-pointer
```
- **ASan** detecta buffer overflow, use-after-free e vazamentos com custo de ~2-3x de lentidão — use em todo teste local e no CI.
- **UBSan** pega comportamento indefinido (overflow de int com sinal, etc.).
- **Nunca envie builds com sanitizer para produção** — são ferramentas de desenvolvimento.

### Hardening para release (recomendação OpenSSF)
```
-O2 -D_FORTIFY_SOURCE=2 -fstack-protector-strong -fPIE -pie
```

## 8. Build System

- Use **Makefile** para projetos pequenos ou **CMake** para projetos maiores/multiplataforma.
- O build deve: compilar `.o` em `build/`, gerar o executável em `bin/`, ter alvos `all`, `clean`, `test`.
- Gere `compile_commands.json` (CMake: `-DCMAKE_EXPORT_COMPILE_COMMANDS=ON`; Make: use `bear`) para que clang-tidy e o editor funcionem bem.

### Makefile mínimo de referência
```makefile
CC      := gcc
CFLAGS  := -std=c11 -Wall -Wextra -Wpedantic -Werror -g -Iinclude
SRC     := $(wildcard src/*.c)
OBJ     := $(SRC:src/%.c=build/%.o)
BIN     := bin/app

all: $(BIN)

$(BIN): $(OBJ) | bin
	$(CC) $(CFLAGS) $^ -o $@

build/%.o: src/%.c | build
	$(CC) $(CFLAGS) -c $< -o $@

bin build:
	mkdir -p $@

clean:
	rm -rf bin build

.PHONY: all clean
```

## 9. Testes

- Todo módulo em `src/` deve ter teste correspondente em `tests/` (`tests/test_parser.c` testa `src/parser.c`).
- Rode os testes com sanitizers ativados.
- Frameworks leves para C: **Unity**, **CMocka**, ou asserts simples com um runner próprio.
- Teste pela interface pública do módulo (o que está no `.h`).

## 10. Documentação e Comentários

- Comentários explicam **por quê**, não **o quê** (o código já diz o quê).
- Documente no header cada função pública: o que faz, parâmetros, retorno, quem é dono da memória, comportamento em erro.
- `README.md` deve ter: o que o projeto faz, como compilar, como rodar os testes.
- Evite "decoração" em comentários (fileiras de asteriscos etc.) — ruído sem informação.

## 11. Git / Higiene do Repositório

`.gitignore` mínimo:
```
bin/
build/
*.o
*.out
*.exe
compile_commands.json
```

- Commits pequenos e com mensagem descritiva.
- Nunca commite código que não compila ou com warnings.
- Configure CI (GitHub Actions) para compilar com `-Werror` + rodar testes com sanitizers em todo push.

---

## Checklist Rápido (antes de todo commit)

- [ ] Compila com `-Wall -Wextra -Wpedantic -Werror` sem nenhum warning
- [ ] Testes passam com ASan/UBSan ligados
- [ ] Nenhum artefato de build no commit
- [ ] Headers novos têm include guard e prefixo de módulo
- [ ] Todo `malloc` tem `free` correspondente e dono documentado
- [ ] Código formatado com `clang-format`
