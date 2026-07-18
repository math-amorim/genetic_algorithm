# Projeto de Computação Evolucionária - Framework de Algoritmo Genético
Implementação de um Algoritmo Genético em C++ com múltiplos problemas de otimização e visualização de resultados via Python.

## Problemas

| Problema | Descrição | Representação | Seleção | Crossover | Mutação |
|---|---|---|---|---|---|
| **Função Cosseno** | Otimização da função f(x) = cos(20x) − (\|x\|/2 + x³/4) para x ∈ [-2, 2]. | Binário (16 bits) | Roleta | One-Point | Bit-Flip |
| **Rádio** | Problema de alocação de funcionários entre dois modelos de rádio, buscando maximizar a produção respeitando as restrições do problema. | Binário (10 bits) | Roleta | One-Point | Bit-Flip |
| **3-SAT** | Problema clássico de satisfatibilidade booleana. O objetivo é encontrar uma atribuição para as variáveis que satisfaça o maior número possível de cláusulas. | Binário (100 bits) | Roleta | One-Point | Bit-Flip |
| **N-Rainhas Normal** | Problema clássico das N-Rainhas, cujo objetivo é posicionar \(N\) rainhas em um tabuleiro \(N\times N\) sem que duas rainhas se ataquem. | Permutação | Torneio | OX | Swap |
| **N-Rainhas Valorado** | Variante do problema das N-Rainhas em que cada posição do tabuleiro possui um valor associado. O objetivo é encontrar uma configuração válida que maximize a soma dos valores das posições ocupadas pelas rainhas. | Permutação | Torneio | OX | Swap |

## Representação e Operadores

O projeto suporta dois tipos de representação de indivíduos:

### Representação Binária

Utilizada nos problemas da **Função Cosseno**, **Rádio** e **3-SAT**. Cada indivíduo é representado por um vetor de bits.

- **Seleção:** Roleta
- **Crossover:** One-Point
- **Mutação:** Bit-Flip

### Representação por Permutação

Utilizada nas duas versões do problema das **N-Rainhas**. Cada cromossomo é uma permutação dos índices das colunas do tabuleiro, garantindo automaticamente uma rainha por linha e por coluna.

- **Seleção:** Torneio
- **Crossover:** Order Crossover (OX)
- **Mutação:** Swap


## Estrutura

```
├── main.cpp                  # Configuração dos problemas e menu
├── src/
│   ├── GA.h                  # Classe principal do AG
│   ├── Runner.h              # Execução e estatísticas
│   ├── Operators.h           # Operadores de crossover e mutação
│   ├── Selection.h           # Operadores de seleção
│   ├── Individual.h          # Estrutura do indivíduo
│   ├── Utils.h               # Funções auxiliares
│   └── Logger.h              # Log de convergência
├── problems/
│   ├── Problem.h             # Classe base abstrata
│   ├── cos/Cosine.h          # Problema função cosseno
│   ├── radio/Radio.h         # Problema rádio
│   ├── 3sat/3SAT.h           # Problema 3-SAT
│   └── nqueens/
│       ├── NQueens.h         # N-Rainhas (normal)
│       └── NQueensValued.h   # N-Rainhas (valorado)
├── scripts/
│   └── plot.py               # Gráficos de convergência e boxplot
└── results/                  # Saídas geradas (CSVs e gráficos)
```

## Parâmetros Configuráveis

Os principais parâmetros do algoritmo estão definidos no topo do `main.cpp` e podem ser alterados conforme o experimento.

| Parâmetro | Valor Padrão | Descrição |
|---|---:|---|
| `BITS_COSINE` | 16 | Número de bits utilizados para representar a solução da função cosseno. |
| `BITS_RADIO` | 10 | Número de bits utilizados para representar a solução do problema do rádio. |
| `BITS_3SAT` | 100 | Número de bits utilizados para representar uma atribuição de variáveis do problema 3-SAT. |
| `POP_SIZE` | 10 | Tamanho da população para problemas com representação binária. |
| `POP_SIZE_NQUEENS` | 30 | Tamanho da população para os problemas das N-Rainhas. |
| `GENERATIONS` | 10000 | Número máximo de gerações executadas pelo algoritmo. |
| `CROSSOVER_RATE` | 0.9 | Probabilidade de aplicação do operador de crossover. |
| `MUTATION_RATE` | 0.01 | Probabilidade de mutação aplicada a cada indivíduo. |
| `TOURNAMENT_SIZE` | 2 | Número de indivíduos participantes na seleção por torneio. |
| `NUM_RUNS` | 10 | Número de execuções independentes realizadas para cada problema. |
| `BASE_SEED` | 42 | Semente base utilizada para inicializar o gerador de números aleatórios. |
| `ELITE_COUNT` | 1 | Quantidade de indivíduos preservados pelo elitismo a cada geração. |

## Saídas

Cada problema gera em `results/<problema>/`:
- `convergence.csv` — dados de convergência por geração
- Gráfico de convergência (PNG)
- `summary.csv` — melhor fitness de cada execução
- Boxplot comparativo (PNG)
