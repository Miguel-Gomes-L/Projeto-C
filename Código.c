#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAX_JOGOS 100
#define ARQUIVO_DADOS "jogos.txt"

// Estrutura que define um Jogo
struct Jogo {
    int id;                // ID único do jogo
    char titulo[50];       // Título do jogo
    char desenvolvedora[50]; // Nome da desenvolvedora
    int ano;               // Ano de lançamento
    char genero[50];       // Gênero do jogo (ex: Ação, Aventura)
    char plataforma[50];   // Plataforma(s) do jogo (ex: PS4, PC)
    char status[30];       // Status do jogo (ex: Disponível, Alugado, Indisponível)
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
    if (*totalJogos >= MAX_JOGOS) {  // Verifica se atingiu o limite de jogos
        printf("Limite máximo de jogos atingido!\n");
        return;
    }

    struct Jogo novo;  // Cria um novo jogo
    novo.id = *totalJogos + 1;  // Define o ID do novo jogo

    printf("\n=== Adicionar Novo Jogo ===\n");
    // Coleta as informações do novo jogo
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
    strcpy(novo.status, "Disponível");  // Define o status como "Disponível"

    jogos[*totalJogos] = novo;  // Adiciona o novo jogo ao catálogo
    (*totalJogos)++;  // Incrementa o total de jogos
    printf("Jogo adicionado com sucesso!\n");
}

// Função que remove um jogo do catálogo pelo ID
void removerJogo(struct Jogo jogos[], int *totalJogos) {
    if (*totalJogos == 0) {  // Verifica se há jogos para remover
        printf("Não há jogos cadastrados para remover!\n");
        return;
    }

    mostrarCatalogo(jogos, *totalJogos);  // Mostra o catálogo para o usuário
    printf("\nDigite o ID do jogo que deseja remover: ");
    int idRemover;
    scanf("%d", &idRemover);

    int encontrado = 0;
    // Procura pelo jogo com o ID informado
    for (int i = 0; i < *totalJogos; i++) {
        if (jogos[i].id == idRemover) {
            encontrado = 1;

            // Move os jogos subsequentes para "tapar o buraco" do jogo removido
            for (int j = i; j < *totalJogos - 1; j++) {
                jogos[j] = jogos[j + 1];
            }
            (*totalJogos)--;  // Decrementa o total de jogos
            printf("Jogo removido com sucesso!\n");
            break;
        }
    }

    if (!encontrado) {  // Caso o jogo com o ID informado não tenha sido encontrado
        printf("Jogo não encontrado!\n");
    }
}

// Função que salva todos os jogos no arquivo "jogos.txt"
void salvarJogos(struct Jogo jogos[], int totalJogos) {
    FILE *arquivo = fopen(ARQUIVO_DADOS, "w");  // Abre o arquivo para escrita
    if (arquivo == NULL) {  // Se o arquivo não abrir, exibe erro
        printf("Erro ao salvar dados!\n");
        return;
    }

    // Salva os dados de todos os jogos no arquivo
    for (int i = 0; i < totalJogos; i++) {
        fprintf(arquivo, "%d\n", jogos[i].id);
        fprintf(arquivo, "%s\n", jogos[i].titulo);
        fprintf(arquivo, "%s\n", jogos[i].desenvolvedora);
        fprintf(arquivo, "%d\n", jogos[i].ano);
        fprintf(arquivo, "%s\n", jogos[i].genero);
        fprintf(arquivo, "%s\n", jogos[i].plataforma);
        fprintf(arquivo, "%s\n", jogos[i].status);
    }

    fclose(arquivo);  // Fecha o arquivo após salvar
}

// Função que carrega os jogos salvos no arquivo "jogos.txt"
void carregarJogos(struct Jogo jogos[], int *totalJogos) {
    FILE *arquivo = fopen(ARQUIVO_DADOS, "r");  // Abre o arquivo para leitura
    if (arquivo == NULL) {  // Se o arquivo não existir, inicializa com 0 jogos
        *totalJogos = 0;
        return;
    }

    *totalJogos = 0;
    while (!feof(arquivo) && *totalJogos < MAX_JOGOS) {  // Lê até o final do arquivo ou até o limite de jogos
        struct Jogo jogo;
        if (fscanf(arquivo, "%d\n", &jogo.id) != 1) break;  // Lê o ID do jogo
        
        fgets(jogo.titulo, 50, arquivo);  // Lê o título do jogo
        jogo.titulo[strcspn(jogo.titulo, "\n")] = 0;  // Remove o caractere de nova linha
        
        fgets(jogo.desenvolvedora, 50, arquivo);  // Lê a desenvolvedora
        jogo.desenvolvedora[strcspn(jogo.desenvolvedora, "\n")] = 0;
        
        fscanf(arquivo, "%d\n", &jogo.ano);  // Lê o ano de lançamento
        
        fgets(jogo.genero, 50, arquivo);  // Lê o gênero
        jogo.genero[strcspn(jogo.genero, "\n")] = 0;
        
        fgets(jogo.plataforma, 50, arquivo);  // Lê as plataformas
        jogo.plataforma[strcspn(jogo.plataforma, "\n")] = 0;
        
        fgets(jogo.status, 30, arquivo);  // Lê o status do jogo
        jogo.status[strcspn(jogo.status, "\n")] = 0;

        jogos[*totalJogos] = jogo;  // Adiciona o jogo ao array
        (*totalJogos)++;  // Incrementa o número de jogos
    }

    fclose(arquivo);  // Fecha o arquivo após leitura
}

// Função principal que controla o fluxo do programa
int main() {
    struct Jogo jogos[MAX_JOGOS];  // Array para armazenar os jogos
    int totalJogos = 0;

    carregarJogos(jogos, &totalJogos);  // Carrega os jogos do arquivo, se houver

    // Se não houver jogos salvos, inicializa com alguns exemplos
    if (totalJogos == 0) {
        printf("Nenhum jogo encontrado. Inicializando com exemplos...\n");
        struct Jogo exemplos[] = {
            {1, "Uncharted: The Nathan Drake Collection", "Naughty Dog", 2015, "Ação e aventura", "PS4, PS5, PC", "Disponível"},
            {2, "Shadow of the Colossus", "Team ICO", 2005, "Ação e aventura", "PS2, PS3, PS4", "Disponível"},
            {3, "The Legend of Zelda: Breath of the Wild", "Nintendo EPD", 2017, "Ação e aventura", "Nintendo Switch", "Indisponível (alugado)"}
        };

        for (int i = 0; i < 3; i++) {
            jogos[totalJogos++] = exemplos[i];  // Adiciona jogos de exemplo
        }
        salvarJogos(jogos, totalJogos);  // Salva os jogos iniciais
    }

    int opcao,

