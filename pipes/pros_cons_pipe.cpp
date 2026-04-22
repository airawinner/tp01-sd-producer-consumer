#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <random>
#include <cmath>

using namespace std;

const int MSG_SIZE = 20;

// ---------------------------
// Função: verifica se é primo
// ---------------------------
bool ehPrimo(long long n) {
    if (n <= 1) return false;

    for (long long i = 2; i <= sqrt(n); i++) {
        if (n % i == 0)
            return false;
    }
    return true;
}

// ---------------------------
// Função: PRODUTOR
// ---------------------------
void produtor(int fd_write, int quantidade) {
    close(STDIN_FILENO); // opcional

    long long numero = 1;
    int incremento;

    random_device seed;
    mt19937 gen(seed());
    uniform_int_distribution<> dist(1, 100);

    char buffer[MSG_SIZE];

    for (int i = 0; i < quantidade; i++) {
        incremento = dist(gen);
        numero += incremento;

        snprintf(buffer, MSG_SIZE, "%lld", numero);
        write(fd_write, buffer, MSG_SIZE);
    }

    // Envia 0 para encerrar
    snprintf(buffer, MSG_SIZE, "0");
    write(fd_write, buffer, MSG_SIZE);

    close(fd_write);
}

// ---------------------------
// Função: CONSUMIDOR
// ---------------------------
void consumidor(int fd_read) {
    char buffer[MSG_SIZE];
    long long numero;

    while (true) {
        ssize_t bytes = read(fd_read, buffer, MSG_SIZE);

        if (bytes <= 0)
            break;

        numero = atoll(buffer);

        if (numero == 0)
            break;

        if (ehPrimo(numero))
            cout << "O número " << numero << " é primo\n";
        else
            cout << "O número " << numero << " NÃO é primo\n";
    }

    close(fd_read);
}

// ---------------------------
// MAIN
// ---------------------------
int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Informe a quantidade de números\n";
        return 1;
    }

    int quantidade;

    try {
        quantidade = stoi(argv[1]);
    } catch (...) {
        cerr << "Argumento inválido\n";
        return 1;
    }

    int fd[2];

    if (pipe(fd) == -1) {
        cerr << "Erro ao criar pipe\n";
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0) {
        cerr << "Erro no fork\n";
        return 1;
    }

    if (pid > 0) {
        // PRODUTOR (pai)
        close(fd[0]); // fecha leitura
        produtor(fd[1], quantidade);
    } else {
        // CONSUMIDOR (filho)
        close(fd[1]); // fecha escrita
        consumidor(fd[0]);
    }

    return 0;
}