#include <iostream>
#include <vector>
#include <thread>
#include <semaphore>
#include <random>
#include <atomic>
#include <chrono>
#include <fstream>
#include <mutex>
#include <algorithm>

using namespace std;

const int M = 100000; // Quantidade total de consumos por teste

// Classe que representa o buffer compartilhado (circular)
class Buffer {
private:
    vector<int> data;     // Armazena os elementos
    int size;             // Capacidade do buffer
    int in = 0, out = 0;  // Índices de inserção e remoção (circular)
    int currentSize = 0;  // Quantidade atual de elementos no buffer
    mutex mtx;            // Protege acesso concorrente

public:
    counting_semaphore<> sem_empty; // Controla espaços vazios
    counting_semaphore<> sem_full;  // Controla espaços ocupados

    // Construtor inicializa buffer e semáforos
    Buffer(int n)
        : data(n), size(n), sem_empty(n), sem_full(0) {}

    // Insere um valor no buffer (produtor)
    void insert(int value) {
        sem_empty.acquire();            // Aguarda espaço disponível
        lock_guard<mutex> lock(mtx);    // Região crítica

        data[in] = value;               // Insere valor
        in = (in + 1) % size;           // Atualiza índice circular
        currentSize++;                  // Atualiza ocupação

        sem_full.release();             // Sinaliza item disponível
    }

    // Remove um valor do buffer (consumidor)
    int remove() {
        sem_full.acquire();             // Aguarda item disponível
        lock_guard<mutex> lock(mtx);    // Região crítica

        int value = data[out];          // Remove valor
        out = (out + 1) % size;         // Atualiza índice circular
        currentSize--;                  // Atualiza ocupação

        sem_empty.release();            // Libera espaço no buffer
        return value;
    }

    // Retorna o tamanho atual do buffer de forma segura
    int getSize() {
        lock_guard<mutex> lock(mtx);
        return currentSize;
    }
};

// Verifica se um número é primo
bool isPrime(int n) {
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;

    // Testa apenas números ímpares até a raiz de n
    for (int i = 3; i * i <= n; i += 2)
        if (n % i == 0) return false;

    return true;
}

// Variáveis globais atômicas para controle de execução
atomic<int> consumedCount(0); // Total de itens consumidos
atomic<bool> running(true);   // Indica se produtores continuam rodando

// Função produtora
void producer(Buffer& buffer) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(1, 10000000);

    // Produz enquanto estiver ativo
    while (running) {
        if (consumedCount >= M) break; // Para se atingir limite
        buffer.insert(dist(gen));      // Insere número aleatório
    }
}

// Função consumidora
void consumer(Buffer& buffer, vector<int>& localOccupancy) {
    // Reserva espaço para reduzir realocações
    localOccupancy.reserve(M / 2); 

    while (true) {
        int num = buffer.remove(); // Remove item do buffer

        // Verifica sentinela de encerramento
        if (num == -1) break;

        // Processa número (verificação de primalidade)
        isPrime(num);
        
        int current = ++consumedCount; // Incrementa contador global
        
        // Armazena ocupação atual do buffer
        localOccupancy.push_back(buffer.getSize());

        // Se atingiu o limite, sinaliza parada
        if (current >= M) {
            running = false; 
        }
    }
}

// Função principal
int main(int argc, char* argv[]) {
    // Verifica parâmetros de entrada
    if (argc < 4) {
        cerr << "Uso: ./programa <N> <Np> <Nc>" << endl;
        return 1;
    }

    int N = stoi(argv[1]);   // Tamanho do buffer
    int Np = stoi(argv[2]);  // Número de produtores
    int Nc = stoi(argv[3]);  // Número de consumidores

    double totalTime = 0;           // Tempo total das execuções
    vector<int> final_occupancy;   // Armazena ocupação final

    // Executa o experimento 10 vezes
    for (int run = 0; run < 10; run++) {
        Buffer buffer(N);
        consumedCount = 0;
        running = true;

        vector<vector<int>> allOccupancies(Nc); // Dados por consumidor
        vector<thread> producers, consumers;

        auto start = chrono::high_resolution_clock::now();

        // Cria threads consumidoras
        for (int i = 0; i < Nc; i++)
            consumers.emplace_back(consumer, ref(buffer), ref(allOccupancies[i]));

        // Cria threads produtoras
        for (int i = 0; i < Np; i++)
            producers.emplace_back(producer, ref(buffer));

        // Aguarda produtores terminarem
        for (auto& t : producers) t.join();

        // Envia sentinela para encerrar consumidores
        for (int i = 0; i < Nc; i++) buffer.insert(-1);

        // Aguarda consumidores terminarem
        for (auto& t : consumers) t.join();

        auto end = chrono::high_resolution_clock::now();

        // Acumula tempo da execução
        totalTime += chrono::duration<double>(end - start).count();

        // Guarda dados da última execução
        if (run == 9) {
            for (auto& v : allOccupancies)
                final_occupancy.insert(final_occupancy.end(), v.begin(), v.end());
        }
    }

    // Calcula e imprime tempo médio
    double avg = totalTime / 10.0;
    cout << avg << endl;

    // Salva ocupação do buffer em arquivo
    string filename = "ocupacao_N" + to_string(N) + "_Np" + to_string(Np) + "_Nc" + to_string(Nc) + ".txt";
    ofstream file(filename);

    for (int v : final_occupancy)
        file << v << "\n";

    return 0;
}