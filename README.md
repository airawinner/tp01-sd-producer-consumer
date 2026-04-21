# Sistemas Distribuídos - TP01 (CEFET-MG 2026/1)

**Professora:** Michelle Hanne  
**Aluno:** Aira Winner

---

# Visão Geral

Este repositório contém a implementação do Trabalho Prático 1 de Sistemas Distribuídos.

## Conteúdo:

- Pipes (processos com fork + pipe)
- Threads com semáforos
- Experimentos de desempenho
- Geração de gráficos em Python

---

# Estrutura

/pipes → implementação com processos  
 Makefile → automação
/semaphore → implementação com threads  
 /dados → logs e medições  
 /graficos → gráficos de tempo  
 /graficos_buffer → gráficos de ocupação  
 script.py → execução dos experimentos  
 teste.py → geração dos gráficos  
 Makefile → automação

---

# Requisitos

- g++ (C++20)
- Python 3
- pip install matplotlib pandas numpy

---

# Execução

## Pipes

./programa 1000
ou
make run

## Semaphores

make run

---

# Conclusão

O trabalho compara desempenho entre processos e threads,
avaliando impacto de sincronização e concorrência.
