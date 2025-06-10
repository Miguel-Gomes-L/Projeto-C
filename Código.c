//permite visualizar os jogos disponiveis, alterar entre Disponível e indisponivel, adicionar e remover jogo
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_JOGOS 100
#define ARQUIVO_DADOS "jogos.txt"

// Estrutura que define um Jogo
struct Jogo {
    int id;
    char titulo[50];
    char desenvolvedora[50];
    int ano;
    char genero[50];
    char plataforma[50];
    char status[30];
};

// Função que exibe o catálogo completo de jogos
void mostrarCatalogo(struct Jogo jogos[], int totalJogos) {
    printf("\n=== Catálogo de Jogos ===\n");
    for (int i = 0; i < totalJogos; i++) {
        printf("%d. %s (%d) - %s\n", jogos[i].id, jogos[i].titulo, jogos[i].ano, jogos[i].status);
    }
}

// Função que exibe os detalhes de um jogo específico
void mostrarDetalhes(struct Jogo jogo) {
    printf("\n=== Detalhes do Jogo ===\n");
    printf("ID: %d\n", jogo.id);
    printf("Título: %s\n", jogo.titulo);
    printf("Desenvolvedora: %s\n", jogo.desenvolvedora);
    printf("Ano: %d\n", jogo.ano);
    printf("Gênero: %s\n", jogo.genero);
    printf("Plataforma: %s\n", jogo.plataforma);
    printf("Status: %s\n", jogo.status);
}

// Função que adiciona um novo jogo ao catálogo
void adicionarJogo(struct Jogo jogos[], int *totalJogos) {
    if (*totalJogos >= MAX_JOGOS) {
        printf("Limite máximo de jogos atingido!\n");
        return;
    }

    struct Jogo novo;
    novo.id = *totalJogos + 1;

    printf("\n=== Adicionar Novo Jogo ===\n");
    printf("Título: ");
    scanf(" %[^\n]", novo.titulo);
    printf("Desenvolvedora: ");
    scanf(" %[^\n]", novo.desenvolvedora);
    printf("Ano: ");
    scanf("%d", &novo.ano);
    printf("Gênero: ");
    scanf(" %[^\n]", novo.genero);
    printf("Plataforma: ");
    scanf(" %[^\n]", novo.plataforma);
    strcpy(novo.status, "Disponível");

    jogos[*totalJogos] = novo;
    (*totalJogos)++;
    printf("Jogo adicionado com sucesso!\n");
}

// Função que remove um jogo do catálogo pelo ID
void removerJogo(struct Jogo jogos[], int *totalJogos) {
    if (*totalJogos == 0) {
        printf("Não há jogos cadastrados para remover!\n");
        return;
    }

    mostrarCatalogo(jogos, *totalJogos);
    printf("\nDigite o ID do jogo que deseja remover: ");
    int idRemover;
    scanf("%d", &idRemover);

    int encontrado = 0;
    for (int i = 0; i < *totalJogos; i++) {
        if (jogos[i].id == idRemover) {
            encontrado = 1;
            for (int j = i; j < *totalJogos - 1; j++) {
                jogos[j] = jogos[j + 1];
            }
            (*totalJogos)--;
            printf("Jogo removido com sucesso!\n");
            break;
        }
    }

    if (!encontrado) {
        printf("Jogo não encontrado!\n");
    }
}

// Função para alterar o status de um jogo
void alterarStatus(struct Jogo jogos[], int totalJogos) {
    if (totalJogos == 0) {
        printf("Não há jogos cadastrados para alterar status!\n");
        return;
    }

    mostrarCatalogo(jogos, totalJogos);
    printf("\nDigite o ID do jogo que deseja alterar o status: ");
    int id;
    scanf("%d", &id);

    int encontrado = 0;
    for (int i = 0; i < totalJogos; i++) {
        if (jogos[i].id == id) {
            encontrado = 1;
            if (strcmp(jogos[i].status, "Disponível") == 0) {
                strcpy(jogos[i].status, "Indisponível");
            } else {
                strcpy(jogos[i].status, "Disponível");
            }
            printf("Status alterado com sucesso! Novo status: %s\n", jogos[i].status);
            break;
        }
    }

    if (!encontrado) {
        printf("Jogo com ID %d não encontrado.\n", id);
    }
}

// Função que salva todos os jogos no arquivo "jogos.txt"
void salvarJogos(struct Jogo jogos[], int totalJogos) {
    FILE *arquivo = fopen(ARQUIVO_DADOS, "w");
    if (arquivo == NULL) {
        printf("Erro ao salvar dados!\n");
        return;
    }

    for (int i = 0; i < totalJogos; i++) {
        fprintf(arquivo, "%d\n", jogos[i].id);
        fprintf(arquivo, "%s\n", jogos[i].titulo);
        fprintf(arquivo, "%s\n", jogos[i].desenvolvedora);
        fprintf(arquivo, "%d\n", jogos[i].ano);
        fprintf(arquivo, "%s\n", jogos[i].genero);
        fprintf(arquivo, "%s\n", jogos[i].plataforma);
        fprintf(arquivo, "%s\n", jogos[i].status);
    }

    fclose(arquivo);
}

// Função que carrega os jogos salvos no arquivo "jogos.txt"
void carregarJogos(struct Jogo jogos[], int *totalJogos) {
    FILE *arquivo = fopen(ARQUIVO_DADOS, "r");
    if (arquivo == NULL) {
        *totalJogos = 0;
        return;
    }

    *totalJogos = 0;
    while (!feof(arquivo) && *totalJogos < MAX_JOGOS) {
        struct Jogo jogo;
        if (fscanf(arquivo, "%d\n", &jogo.id) != 1) break;
        fgets(jogo.titulo, 50, arquivo);
        jogo.titulo[strcspn(jogo.titulo, "\n")] = 0;
        fgets(jogo.desenvolvedora, 50, arquivo);
        jogo.desenvolvedora[strcspn(jogo.desenvolvedora, "\n")] = 0;
        fscanf(arquivo, "%d\n", &jogo.ano);
        fgets(jogo.genero, 50, arquivo);
        jogo.genero[strcspn(jogo.genero, "\n")] = 0;
        fgets(jogo.plataforma, 50, arquivo);
        jogo.plataforma[strcspn(jogo.plataforma, "\n")] = 0;
        fgets(jogo.status, 30, arquivo);
        jogo.status[strcspn(jogo.status, "\n")] = 0;

        jogos[*totalJogos] = jogo;
        (*totalJogos)++;
    }

    fclose(arquivo);
}

// Função principal que controla o fluxo do programa
int main() {
    struct Jogo jogos[MAX_JOGOS];
    int totalJogos = 0;

    carregarJogos(jogos, &totalJogos);

    if (totalJogos == 0) {
        printf("Nenhum jogo encontrado. Inicializando com exemplos...\n");
        struct Jogo exemplos[] = {
            {1, "Uncharted: The Nathan Drake Collection", "Naughty Dog", 2015, "Ação e aventura", "PS4, PS5, PC", "Disponível"},
            {2, "Shadow of the Colossus", "Team ICO", 2005, "Ação e aventura", "PS2, PS3, PS4", "Disponível"},
            {3, "The Legend of Zelda: Breath of the Wild", "Nintendo EPD", 2017, "Ação e aventura", "Nintendo Switch", "Indisponível"}
        };
        for (int i = 0; i < 3; i++) {
            jogos[totalJogos++] = exemplos[i];
        }
        salvarJogos(jogos, totalJogos);
    }

    int opcao;
    do {
        printf("\n=== Menu Principal ===\n");
        printf("1. Mostrar catálogo de jogos\n");
        printf("2. Ver detalhes de um jogo\n");
        printf("3. Adicionar novo jogo\n");
        printf("4. Remover jogo\n");
        printf("5. Salvar e sair\n");
        printf("6. Alterar status de um jogo\n"); // Nova opção
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                mostrarCatalogo(jogos, totalJogos);
                break;

            case 2: {
                mostrarCatalogo(jogos, totalJogos);
                printf("\nDigite o ID do jogo para ver detalhes: ");
                int idDetalhes;
                scanf("%d", &idDetalhes);
                int encontrado = 0;
                for (int i = 0; i < totalJogos; i++) {
                    if (jogos[i].id == idDetalhes) {
                        mostrarDetalhes(jogos[i]);
                        encontrado = 1;
                        break;
                    }
                }
                if (!encontrado) {
                    printf("Jogo não encontrado!\n");
                }
                break;
            }

            case 3:
                adicionarJogo(jogos, &totalJogos);
                salvarJogos(jogos, totalJogos);
                break;

            case 4:
                removerJogo(jogos, &totalJogos);
                salvarJogos(jogos, totalJogos);
                break;

            case 5:
                salvarJogos(jogos, totalJogos);
                printf("Dados salvos. Até logo!\n");
                break;

            case 6:
                alterarStatus(jogos, totalJogos);
                salvarJogos(jogos, totalJogos);
                break;

            default:
                printf("Opção inválida! Tente novamente.\n");
        }

    } while (opcao != 5);

    return 0;
}
