# Sistemas Distribuídos - TP01 (CEFET-MG 2026/1)

**Professora:** Michelle Hanne  
**Aluno:** Aira Winner e Arthur Secundino

---

## Visão Geral

Este repositório contém a implementação do Trabalho Prático 1 da disciplina de Sistemas Distribuídos. O projeto foca na comparação de mecanismos de comunicação e sincronização.

### Tópicos Abordados:

- **Pipes:** Comunicação entre processos utilizando `fork()` e `pipe()`.
- **Threads:** Sincronização de threads utilizando semáforos e mutexes.
- **Análise:** Experimentos de desempenho e geração de gráficos comparativos.

---

## Estrutura do Projeto

Abaixo está a organização dos diretórios e arquivos:

```text
.
├── pipes/
│   ├── pros_cons_pipe.cpp
│   └── Makefile
├── semaphore/
│   ├── dados/            # Logs e medições brutas
│   ├── graficos/         # Gráficos de tempo de execução
│   ├── graficos_buffer/  # Gráficos de ocupação do buffer
│   ├── pros_cons_sem.cpp
│   ├── script.py         # Script de automação dos experimentos
│   ├── teste.py          # Script de geração de gráficos
│   └── Makefile
└── README.md
```

---

## Requisitos

- **Compilador:** `g++` (suporte a C++20)
- **Linguagem:** Python 3.x
- **Bibliotecas Python:**
  ```bash
  pip install matplotlib pandas numpy
  ```

---

## Como Executar

### 1. Implementação com Pipes

Navegue até a pasta e execute o binário ou o comando via Makefile:

```bash
cd pipes
make run
```

_Ou manualmente:_

```bash
g++ -std=c++20 pros_cons_pipe.cpp -o programa
./programa 10
```

obs: esse valor 10 pode ser qualquer um dentro desse requisito aqui.
O programa produtor deve gerar números inteiros aleatórios e crescentes,
da seguinte forma:
Ni = Ni−1 + ∆, N0 = 1, ∆ ∈ [1, 100].

### 2. Implementação com Semáforos (Produtor-Consumidor)

Navegue até a pasta para rodar os experimentos automatizados:

```bash
cd semaphore
make run
```

Esse make run compile e roda o programa já salvando em txt cada caso, calcula média e os gráficos pedidos usando um script em python

_Para compilar manualmente:_

```bash
g++ -std=c++20 pro_con_sem.cpp -o programa
```

_Em que o uso é assim :_
./programa N Np Nc

```bash
 ./programa 1 1 1
```

_então basta digitar de acordo com o que foi pedido :_

Para o estudo de caso, considere que o programa termina sua execução
após o consumidor processar M = 105 números. Considere ainda os valores
N = 1, 10, 100, 1000, com os seguintes combinações de número de threads
produtor/consumidor:

(Np, Nc) ∈ {(1, 1), (1, 2), (1, 4), (1, 8), (2, 1), (4, 1), (8, 1)}.

_ou use script em python usam o "programa" para executar_

```bash
python3 txt_graph1.py graph2.py
```

Basicamente o script txt_grahp1.py ele executa cada estudo de caso 10x e calcula as médias e gera o gráfico tempo médio de execução em função do número de threads produtor/consumidor para cada valor de N. Já o graph2.py gera cada cenário, trace um gráfico representativo com a ocupação do
buffer compartilhado ao longo do tempo.

---

## Conclusão e Análise

O trabalho compara o desempenho entre processos e threads, avaliando o impacto da sincronização e concorrência no tempo total de processamento. A análise detalhada dos resultados pode ser encontrada nos gráficos gerados nas pastas correspondentes.

---
