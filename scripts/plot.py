import sys
import os
import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

if len(sys.argv) > 1 and sys.argv[1] == "--boxplot":
    summary_file = sys.argv[2]
    title = sys.argv[3] if len(sys.argv) > 3 else "Comparacao"

    df = pd.read_csv(summary_file)
    problems = df["problem"].unique()

    data, labels = [], []
    for p in problems:
        values = df[df["problem"] == p]["best_fitness"].values
        data.append(values)
        labels.append(p)

    plt.figure(figsize=(12, 7))
    bp = plt.boxplot(data, tick_labels=labels, patch_artist=True, widths=0.6)

    colors = ["#4ECDC4", "#FF6B6B", "#FFD93D", "#6A67CE"]
    for patch, color in zip(bp["boxes"], colors):
        patch.set_facecolor(color)
        patch.set_alpha(0.7)

    for i, values in enumerate(data, start=1):
        x = np.random.normal(i, 0.04, size=len(values))
        plt.scatter(x, values, alpha=0.8, s=40, zorder=3)

    plt.yscale("log")
    plt.ylabel("Fitness (escala log)")
    plt.title(title, fontsize=16, fontweight="bold")
    plt.grid(True, alpha=0.3, axis="y")
    plt.xticks(rotation=8)
    plt.tight_layout()

    out_dir = os.path.dirname(summary_file)
    safe = title.replace(" ", "_").replace("-", "_")
    plt.savefig(f"{out_dir}/{safe}.png", dpi=300, bbox_inches="tight")
    plt.close()

else:
    csv_file = sys.argv[1]
    title = sys.argv[2] if len(sys.argv) > 2 else "Problem"

    df = pd.read_csv(csv_file)

    plt.figure(figsize=(10, 6))
    plt.plot(df["generation"], df["best_value"], label="Melhor Valor", linewidth=3)
    plt.plot(df["generation"], df["average_value"], label="Media Valor", linewidth=0.5)
    plt.xlabel("Geracao", fontsize=12)
    plt.ylabel("Valor", fontsize=12)
    plt.title(f"Grafico de Convergencia - {title}", fontsize=14, fontweight="bold")
    plt.legend(fontsize=11)
    plt.grid(True, alpha=0.5)

    out_dir = os.path.dirname(csv_file)
    os.makedirs(out_dir, exist_ok=True)
    safe = title.replace(" ", "_").replace("-", "_")
    plt.savefig(f"{out_dir}/{safe}.png", dpi=300, bbox_inches="tight")
    plt.close()
