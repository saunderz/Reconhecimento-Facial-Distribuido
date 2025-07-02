/*
 * backend_reconhecimento_facial.c
 * 
 * Servidor TCP em C para Recepção de Imagens Faciais, Processamento via chamada externa,
 * Autorização e Persistência em SQLite.
 *
 * Compilar:
 *   gcc backend_reconhecimento_facial.c -o backend -lsqlite3
 *
 * Uso:
 *   ./backend 8000 database.db
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sqlite3.h>
#include <time.h>

#define BUFFER_SIZE 4096

// Inicializa servidor TCP e retorna descritor de socket
int init_server(int port) {
    int server_fd;
    struct sockaddr_in address;
    int opt = 1;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 5) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    return server_fd;
}

// Grava dados binários em arquivo
int save_image(const char *filename, char *data, int len) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        perror("fopen");
        return -1;
    }
    fwrite(data, 1, len, fp);
    fclose(fp);
    return 0;
}

// Executa script Python para reconhecimento facial e retorna resultado
int recognize_face(const char *img_path, char *result, size_t res_len) {
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "python3 recognize.py %s", img_path);
    FILE *pipe = popen(cmd, "r");
    if (!pipe) {
        perror("popen");
        return -1;
    }
    if (fgets(result, res_len, pipe) == NULL) {
        pclose(pipe);
        return -1;
    }
    pclose(pipe);
    // remover newline
    result[strcspn(result, "\n")] = '\0';
    return 0;
}

// Insere log de acesso na base SQLite
int log_access(sqlite3 *db, const char *client_ip, const char *user, const char *status) {
    char *err_msg = NULL;
    char sql[512];
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    char ts[64];
    strftime(ts, sizeof(ts), "%Y-%m-%d %H:%M:%S", tm_info);

    snprintf(sql, sizeof(sql),
        "INSERT INTO logs (timestamp, client_ip, user, status) "
        "VALUES ('%s', '%s', '%s', '%s');",
        ts, client_ip, user, status);

    if (sqlite3_exec(db, sql, 0, 0, &err_msg) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        return -1;
    }
    return 0;
}

// Inicializa base de dados e cria tabela se não existir
int init_database(sqlite3 **db, const char *db_path) {
    if (sqlite3_open(db_path, db) != SQLITE_OK) {
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(*db));
        return -1;
    }
    const char *sql =
        "CREATE TABLE IF NOT EXISTS logs ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "timestamp TEXT,"
        "client_ip TEXT,"
        "user TEXT,"
        "status TEXT);";
    char *err_msg = NULL;
    if (sqlite3_exec(*db, sql, 0, 0, &err_msg) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", err_msg);
        sqlite3_free(err_msg);
        return -1;
    }
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <port> <database>\n", argv[0]);
        return EXIT_FAILURE;
    }
    int port = atoi(argv[1]);
    const char *db_path = argv[2];

    int server_fd = init_server(port);
    sqlite3 *db;
    if (init_database(&db, db_path) != 0) return EXIT_FAILURE;

    printf("Servidor ouvindo na porta %d\n", port);
    while (1) {
        struct sockaddr_in client_addr;
        socklen_t addr_len = sizeof(client_addr);
        int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);
        if (client_fd < 0) {
            perror("accept");
            continue;
        }
        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
        printf("Conexão de %s\n", client_ip);

        // Receber tamanho da imagem
        int img_size;
        if (read(client_fd, &img_size, sizeof(img_size)) <= 0) {
            close(client_fd);
            continue;
        }
        img_size = ntohl(img_size);

        // Alocar buffer e ler imagem
        char *img_buf = malloc(img_size);
        int received = 0;
        while (received < img_size) {
            int r = read(client_fd, img_buf + received, img_size - received);
            if (r <= 0) break;
            received += r;
        }

        // Salvar imagem recebida
        const char *img_path = "received.jpg";
        save_image(img_path, img_buf, img_size);
        free(img_buf);

        // Reconhecimento facial
        char user[128] = {0}, status[16] = {0};
        if (recognize_face(img_path, user, sizeof(user)) == 0) {
            strcpy(status, "PERMITIDO");
        } else {
            strcpy(user, "desconhecido");
            strcpy(status, "NEGADO");
        }

        // Enviar resposta
        char response[256];
        snprintf(response, sizeof(response), "%s;%s", status, user);
        write(client_fd, response, strlen(response));

        // Log no banco
        log_access(db, client_ip, user, status);

        close(client_fd);
    }
    sqlite3_close(db);
    close(server_fd);
    return 0;
}
