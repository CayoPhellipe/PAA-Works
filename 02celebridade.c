#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define Nnodes 5000
#define MaxLen 70

#define DEBUG 0

#if DEBUG
void printnomes(char Nomes[][MaxLen], int N)
{
  printf("Nomes:\n");
  for (int i = 0; i < N; i++)
    printf("%d: %s!\n", i, Nomes[i]);
  printf("\n");
}
void printvetor(int *V, int N)
{
  printf("N=%d: ", N);
  for (int i = 0; i < N; i++)
    printf("%d ", V[i]);
  printf("\n");
}
void printmatrizquadrada(int **M, int N)
{
  for (int i = 0; i < N; i++)
  {
    printf("%d: ", i);
    for (int j = 0; j < N; j++)
      printf("%d ", M[i][j]);
    printf("\n");
  }
}
#endif

void celebridade(char *str)
{
  if (!strcmp(str, ""))
    printf("Nao ha celebridade.\n");
  else
    printf("%s eh celebridade.\n", str);
}

int achaCelebridade(int **G, int N, int *Nodes)
{
  if (N == 0 || (N == 1 && G[Nodes[1]][Nodes[1]])) // Caso seja um grafo vazio ou um unico elemento que conhece ele mesmo
    return 0;
  if (N == 1 && !G[Nodes[1]][Nodes[1]]) // Caso haja apenas um elemento e ele não conheça ele mesmo então é uma celebridade
    return Nodes[1];

  int i = Nodes[1], j = Nodes[2];
  if (G[i][j] == 1) // i não é celebridade, portanto Nodes[1] não é celebridade
  {
    Nodes[1] = Nodes[N];
    Nodes[N] = j = i;
  }
  else // i possivelmente é uma celebridade, portanto removemos Nodes[2]
  {
    Nodes[2] = Nodes[N];
    Nodes[N] = j;
  }

#if DEBUG
  printvetor(Nodes, N + 1);
#endif

  int k = achaCelebridade(G, N - 1, Nodes);

  // se algum momento for visto que k conhece j, ou j não conhece k, então não tem celebridade
  if (k > 0 && (!G[j][k] || G[k][j]))
    k = 0;

  return k;
}

int main()
{
  // Inicialização
  char **Nomes = (char **)malloc(Nnodes * sizeof(char *)); // Vetor com Nnodes posições para nomes
  int **G = (int **)malloc(Nnodes * sizeof(int *));        // Ignore the idx 0, G[i][j] = 1 significa que pessoa [i] conhece(1) pessoa [j]
  for (int i = 0; i < Nnodes; i++)
  {
    Nomes[i] = (char *)malloc(MaxLen * sizeof(char));
    Nomes[i][0] = '\0';

    G[i] = (int *)malloc(Nnodes * sizeof(int));
    for (int j = 0; j < Nnodes; j++)
      G[i][j] = 0;
  }

  // Entrada
  int N = 0, i = 0, j = 0, k = 0;
  char str[2 * MaxLen], nome1[MaxLen], nome2[MaxLen], Aux[8] = "conhece";
  while (fgets(str, 60, stdin) != NULL) // Enquanto houver entradas válidas
  {
    int Nstr = strlen(str);
    if (str[Nstr - 1] == '\n')
      str[Nstr - 1] = '\0';
    for (i = 0; i < Nstr; i++)
      if (str[i] == ' ')
      {
        strncpy(Aux, str + i + 1, 7);
        if (!strcmp(Aux, "conhece"))
        {
#if DEBUG
          printf("str:%s!\ni:%d!\n", str, i);
#endif
          strncpy(nome1, str, i); //  O primeiro nome está até a posição i
          nome1[i] = '\0';
          // O segundo nome começa depois da string " conhece " que tem 9 caracteres,
          // e vai até o numero da entrada menos o '\n'
          int Nstrnome2 = Nstr - (i + 9) + 1;
          strncpy(nome2, str + (i + 9), Nstrnome2);
          nome2[Nstrnome2] = '\0';
#if DEBUG
          printf("Nome1: %s!\nNome2: %s!\n", nome1, nome2);
#endif
          break;
        }
      }

    i = j = 0;
    for (k = N; k > 0 && !i; k--)
    { // Procura se já inseriu o nome1 pelo final porque parece haver mais chances de ser sequencial
      if (!strcmp(Nomes[k], nome1))
        i = k;
    }
    if (!k && !i) // Não achou i
    {
      N++;
      if (N == Nnodes) // Caso dê problema com número de memória alocada
      {
        printf("Estourou");
        break;
      }
      i = N;
      strcpy(Nomes[N], nome1);
    }

    for (k = N; k > 0 && !j; k--)
    { // Procura se já inseriu o nome2 pelo final porque parece haver mais chances de ser sequencial
      if (!strcmp(Nomes[k], nome2))
        j = k;
    }
    if (!k && !j)
    { // Não achou j
      N++;
      if (N == Nnodes) // Caso dê problema com número de memória alocada
      {
        printf("Estourou");
        break;
      }
      j = N;
      strcpy(Nomes[N], nome2);
    }
    G[i][j] = 1;
  }

  // Vetor que marca os elementos ainda no conjunto
  int *Nodes = malloc((N + 1) * sizeof(int)); // N possui um número a mais já que ignoramos o zero
  for (i = 0; i < N + 1; i++)
    Nodes[i] = i;

#if DEBUG
  printnomes(Nomes, N + 1);
  printmatrizquadrada(G, N + 1);
  printf("Nodes->");
  printvetor(Nodes, N + 1);
  printf("\n");
#endif

  // Se achar celebridade passa o nome, se não passa string vazia
  int achou = achaCelebridade(G, N, Nodes); // Dentro da função ele vai de 1 até N
  celebridade(achou ? Nomes[achou] : "");

  // Libera a memória ocupada
  free(Nodes);
  for (i = 0; i < Nnodes; i++)
  {
    free(G[i]);
    free(Nomes[i]);
  }
  free(G);
  free(Nomes);

  return 0;
}