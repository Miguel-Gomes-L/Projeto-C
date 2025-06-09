#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

#define MAX_JOGOS 100
#define DATABASE "jogos.db"

struct Jogo {
    int id;
    char titulo[50];
    char desenvolvedora[50];
    int ano;
    char genero[50];
    char plataforma[50];
    char status[30];
};

void criarTabela() {
    sqlite3 *db;
    char *err_msg = 0;
    int rc;
    
    rc = sqlite3_open(DATABASE, &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Não foi possível abrir o banco de dados: %s\n", sqlite3_errmsg(db));
        return;
    }
    
    char *sql = "CREATE TABLE IF NOT EXISTS Jogos ("
                "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                "titulo TEXT NOT NULL,"
                "desenvolvedora TEXT NOT NULL,"
                "ano INTEGER NOT NULL,"
                "genero TEXT NOT NULL,"
                "plataforma TEXT NOT NULL,"
                "status TEXT NOT NULL);";
    
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erro SQL: %s\n", err_msg);
        sqlite3_free(err_msg);
    }
    
    sqlite3_close(db);
}

void inserirDadosIniciais() {
    sqlite3 *db;
    char *err_msg = 0;
    int rc;
    
    rc = sqlite3_open(DATABASE, &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Não foi possível abrir o banco de dados: %s\n", sqlite3_errmsg(db));
        return;
    }
    
    int count = 0;
    char *sql_count = "SELECT COUNT(*) FROM Jogos;";
    rc = sqlite3_exec(db, sql_count, [](void *data, int argc, char **argv, char **colName) {
        int *count = (int*)data;
        *count = atoi(argv[0]);
        return 0;
    }, &count, &err_msg);
    
    if (count == 0) {
        printf("Inserindo dados iniciais...\n");
        
        char *sql_insert = "INSERT INTO Jogos (titulo, desenvolvedora, ano, genero, plataforma, status) VALUES "
                           "('Uncharted: The Nathan Drake Collection', 'Naughty Dog', 2015, 'Ação e aventura', 'PS4, PS5, PC', 'Disponivel'),"
                           "('Shadow of the Colossus', 'Team ICO', 2005, 'Ação e aventura', 'PS2, PS3, PS4', 'Disponivel'),"
                           "('The Legend of Zelda: Breath of the Wild', 'Nintendo EPD', 2017, 'Ação e aventura', 'Nintendo Switch', 'Indisponível (alugado)');";
        
        rc = sqlite3_exec(db, sql_insert, 0, 0, &err_msg);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Erro SQL: %s\n", err_msg);
            sqlite3_free(err_msg);
        }
    }
    
    sqlite3_close(db);
}

void carregarJogos(struct Jogo jogos[], int *totalJogos) {
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;
    
    rc = sqlite3_open(DATABASE, &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Não foi possível abrir o banco de dados: %s\n", sqlite3_errmsg(db));
        *totalJogos = 0;
        return;
    }
    
    char *sql = "SELECT id, titulo, desenvolvedora, ano, genero, plataforma, status FROM Jogos;";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);
    
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erro ao preparar consulta: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        *totalJogos = 0;
        return;
    }
    
    *totalJogos = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW && *totalJogos < MAX_JOGOS) {
        jogos[*totalJogos].id = sqlite3_column_int(stmt, 0);
        strcpy(jogos[*totalJogos].titulo, (const char*)sqlite3_column_text(stmt, 1));
        strcpy(jogos[*totalJogos].desenvolvedora, (const char*)sqlite3_column_text(stmt, 2));
        jogos[*totalJogos].ano = sqlite3_column_int(stmt, 3);
        strcpy(jogos[*totalJogos].genero, (const char*)sqlite3_column_text(stmt, 4));
        strcpy(jogos[*totalJogos].plataforma, (const char*)sqlite3_column_text(stmt, 5));
        strcpy(jogos[*totalJogos].status, (const char*)sqlite3_column_text(stmt, 6));
        
        (*totalJogos)++;
    }
    
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

void adicionarJogo(struct Jogo jogos[], int *totalJogos) {
    if (*totalJogos >= MAX_JOGOS) {
        printf("Limite máximo de jogos atingido!\n");
        return;
    }

    struct Jogo novo;
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

    sqlite3 *db;
    char *err_msg = 0;
    int rc;
    
    rc = sqlite3_open(DATABASE, &db);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Não foi possível abrir o banco de dados: %s\n", sqlite3_errmsg(db));
        return;
    }
    
    char sql[500];
    snprintf(sql, sizeof(sql), 
             "INSERT INTO Jogos (titulo, desenvolvedora, ano, genero, plataforma, status) "
             "VALUES ('%s', '%s', %d, '%s', '%s', '%s');",
             novo.titulo, novo.desenvolvedora, novo.ano, novo.genero, novo.plataforma, novo.status);
    
    rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erro SQL: %s\n", err_msg);
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return;
    }
    
    novo.id = (int)sqlite3_last_insert_rowid(db);
    sqlite3_close(db);
    
    jogos[*totalJogos] = novo;
    (*totalJogos)++;
    printf("Jogo adicionado com sucesso!\n");
}

int main() {
    // Inicializa o banco de dados
    criarTabela();
    inserirDadosIniciais();
    
    struct Jogo jogos[MAX_JOGOS];
    int totalJogos = 0;
    
    carregarJogos(jogos, &totalJogos);

    /* (O resto do main permanece igual) */
    
    return 0;
}
