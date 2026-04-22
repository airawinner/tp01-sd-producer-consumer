import subprocess
import pandas as pd
import matplotlib.pyplot as plt
import os
import glob
import shutil

# ==============================
# CONFIG
# ==============================
EXECUTABLE = "./programa"

Ns = [1, 10, 100, 1000]
configs = [(1,1), (1,2), (1,4), (1,8), (2,1), (4,1), (8,1)]
labels = ["1P/1C", "1P/2C", "1P/4C", "1P/8C", "2P/1C", "4P/1C", "8P/1C"]

# Garante que as pastas existam
os.makedirs("graficos", exist_ok=True)
os.makedirs("dados", exist_ok=True)

results = []

print(" Iniciando experimentos...")
print("Nota: O programa C++ já realiza as 10 rodadas internamente.\n")

# ==============================
# EXECUÇÃO ÚNICA (O C++ já faz o loop de 10)
# ==============================
for i, (Np, Nc) in enumerate(configs):
    for N in Ns:
        print(f"Testando Config {labels[i]}: N={N}, Np={Np}, Nc={Nc}...", end=" ", flush=True)

        # Chama o programa UMA vez para cada N/Config
        process = subprocess.run(
            [EXECUTABLE, str(N), str(Np), str(Nc)],
            capture_output=True,
            text=True
        )

        try:
            # O stdout do seu C++ é a média das 10 execuções
            media = float(process.stdout.strip())
            print(f"Média: {media:.6f}s")
            
            results.append({
                "N": N,
                "config_label": labels[i],
                "tempo_medio": media
            })
        except:
            print("ERRO na leitura do tempo")

# ==============================
# ORGANIZAÇÃO DOS ARQUIVOS .TXT
# ==============================
# Move os arquivos de ocupação gerados pelo C++ para a pasta /dados
txt_files = glob.glob("ocupacao_N*.txt")
for file in txt_files:
    shutil.move(file, os.path.join("dados", file))

# ==============================
# SALVAR DATAFRAME
# ==============================
df = pd.DataFrame(results)
df.to_csv("dados/medias.csv", index=False)

# ==============================
# GRÁFICO FINAL
# ==============================
plt.figure(figsize=(12, 7))

for N in Ns:
    subset = df[df["N"] == N]
    # O gráfico segue a ordem das labels definidas em configs
    plt.plot(subset["config_label"], subset["tempo_medio"], marker='o', label=f"N={N}")

plt.xlabel("Configurações (Produtores/Consumidores)")
plt.ylabel("Tempo Médio de 10 Execuções (s)")
plt.title("Desempenho: Tempo Médio vs Configuração de Threads")
plt.legend()
plt.grid(True, linestyle='--', alpha=0.7)

plt.savefig("graficos/tempo_execucao.png")
print("\n✔ Gráfico salvo em: graficos/tempo_execucao.png")
print("✔ Dados salvos na pasta: /dados")