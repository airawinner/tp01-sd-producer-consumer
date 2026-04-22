#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <random>
#include <cmath>

using namespace std;

const int MSG_SIZE = 20; // Tamanho fixo das mensagens enviadas no pipe

// Função que verifica se um número é primo
bool ehPrimo(long long n) {
    if (n <= 1) return false;

    // Testa divisores de 2 até a raiz de n
    for (long long i = 2; i <= sqrt(n); i++) {
        if (n % i == 0)
            return false;
    }
    return true;
}

// Função do processo produtor
void produtor(int fd_write, int quantidade) {
    close(STDIN_FILENO); // opcional, não é necessário para o funcionamento

    long long numero = 1; // valor inicial
    int incremento;

    // Gerador de números aleatórios
    random_device seed;
    mt19937 gen(seed());
    uniform_int_distribution<> dist(1, 100);

    char buffer[MSG_SIZE];

    // Gera "quantidade" números crescentes
    for (int i = 0; i < quantidade; i++) {
        incremento = dist(gen);
        numero += incremento;

        // Converte número para string e envia pelo pipe
        snprintf(buffer, MSG_SIZE, "%lld", numero);
        write(fd_write, buffer, MSG_SIZE);
    }

    // Envia 0 como sentinela para indicar fim dos dados
    snprintf(buffer, MSG_SIZE, "0");
    write(fd_write, buffer, MSG_SIZE);

    close(fd_write); // fecha escrita do pipe
}

// Função do processo consumidor
void consumidor(int fd_read) {
    char buffer[MSG_SIZE];
    long long numero;

    while (true) {
        // Lê dados do pipe
        ssize_t bytes = read(fd_read, buffer, MSG_SIZE);

        if (bytes <= 0)
            break;

        // Converte string para número
        numero = atoll(buffer);

        // Verifica sentinela de encerramento
        if (numero == 0)
            break;

        // Testa se é primo e imprime resultado
        if (ehPrimo(numero))
            cout << "O número " << numero << " é primo\n";
        else
            cout << "O número " << numero << " NÃO é primo\n";
    }

    close(fd_read); // fecha leitura do pipe
}

// Função principal
int main(int argc, char* argv[]) {
    // Verifica se o argumento foi passado
    if (argc < 2) {
        cerr << "Informe a quantidade de números\n";
        return 1;
    }

    int quantidade;

    // Converte argumento para inteiro
    try {
        quantidade = stoi(argv[1]);
    } catch (...) {
        cerr << "Argumento inválido\n";
        return 1;
    }

    int fd[2];

    // Cria o pipe (fd[0] leitura, fd[1] escrita)
    if (pipe(fd) == -1) {
        cerr << "Erro ao criar pipe\n";
        return 1;
    }

    // Cria novo processo
    pid_t pid = fork();

    if (pid < 0) {
        cerr << "Erro no fork\n";
        return 1;
    }

    if (pid > 0) {
        // Processo pai atua como produtor
        close(fd[0]); // fecha leitura
        produtor(fd[1], quantidade);
    } else {
        // Processo filho atua como consumidor
        close(fd[1]); // fecha escrita
        consumidor(fd[0]);
    }

    return 0;
}