# Sistemas Distribuídos - TP01 (CEFET-MG 2026/1)

**Professora:** Michelle Hanne  
**Aluno:** Aira Winner e Arthur Secundino

---

## 📑 Visão Geral

Este repositório contém a implementação do Trabalho Prático 1 da disciplina de Sistemas Distribuídos. O projeto foca na comparação de mecanismos de comunicação e sincronização.

### Tópicos Abordados:

- **Pipes:** Comunicação entre processos utilizando `fork()` e `pipe()`.
- **Threads:** Sincronização de threads utilizando semáforos e mutexes.
- **Análise:** Experimentos de desempenho e geração de gráficos comparativos.

---

## 📂 Estrutura do Projeto

Abaixo está a organização dos diretórios e arquivos:

```text
.
├── pipes/
│   ├── main.cpp
│   └── Makefile
├── semaphore/
│   ├── dados/            # Logs e medições brutas
│   ├── graficos/         # Gráficos de tempo de execução
│   ├── graficos_buffer/  # Gráficos de ocupação do buffer
│   ├── main.cpp
│   ├── script.py         # Script de automação dos experimentos
│   ├── teste.py          # Script de geração de gráficos
│   └── Makefile
└── README.md
```

---

## 🛠 Requisitos

- **Compilador:** `g++` (suporte a C++20)
- **Linguagem:** Python 3.x
- **Bibliotecas Python:**
  ```bash
  pip install matplotlib pandas numpy
  ```

---

## 🚀 Como Executar

### 1. Implementação com Pipes

Navegue até a pasta e execute o binário ou o comando via Makefile:

```bash
cd pipes
make run
```

_Ou manualmente:_

```bash
g++ -std=c++20 main.cpp -o programa
./programa 1000
```

### 2. Implementação com Semáforos (Produtor-Consumidor)

Navegue até a pasta para rodar os experimentos automatizados:

```bash
cd semaphore
make run
```

_Para gerar apenas os gráficos após os dados coletados:_

```bash
python3 teste.py
```

---

## 📈 Conclusão e Análise

O trabalho compara o desempenho entre processos e threads, avaliando o impacto da sincronização e concorrência no tempo total de processamento. A análise detalhada dos resultados pode ser encontrada nos gráficos gerados nas pastas correspondentes.

---
