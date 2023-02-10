#include <stdio.h>
#include <stdlib.h>

#define N_MAX 600
#define L_MAX 60

#define DEBUG 0


int main(){

  int** Matriz = (int**) malloc((L_MAX+1) * sizeof(int*));
  for(int i=0; i<=L_MAX; i++){
    Matriz[i] = (int*) malloc((N_MAX+1) * sizeof(int));
  }

  int N;
  scanf("%d", &N);
  while(N){ //Entrada, tem outro scanf no final pra checar se é zero o N
    
    //Guardo os valores em um vetor de contagem (uma ideia similar ao counting sort) e salvo o nível maximo
    int count[L_MAX+1], max=0;
    for(int i=0; i<N; i++) 
      count[i]=0;
    for(int i=0, x; i<N; i++){ 
      scanf("%d", &x); 
      if(max<x) max=x;
      count[x]++;
    }
    //Mapeio a posição inicial pra cada nível usando a quantidade de elementos anteriores (uma soma acumulada)
    int inicio[L_MAX+1];
    for(int i=0, in=0; i<=L_MAX; i++){
      inicio[i] = in;
      in+=count[i];
    }

#if DEBUG
    printf("[C] [I]\n");
    for(int i=0; i<=max; i++){
      printf("[%d] [%d]\n", count[i], inicio[i]);
    }
#endif

    //A matriz irá representar a árvore, onde cada linha é um nível e cada posição de coluna é um elemento da árvore, 
    //  a arvore n fica sempre alinhada na esquerda, e sim na posição inicial de seu nível, e sempre é colocado as folhas da entrada
    //  e depois os nós intermediários relativos as folhas mais abaixo
    // int Matriz[L_MAX+1][N_MAX+1];
    for(int i=0; i<=L_MAX; i++){
      for(int j=0; j<=N_MAX; j++)  
        Matriz[i][j] = 0;
    }

    //Essa variável salva o valor máximo do nível abaixo pois será a frequencia das folhas do nível atual
    int max_value=1;
    //Percorremos então a matriz a partir do ultimo nível até o nível 1
    for(int i=max; i>0; i--){
      //j indica começa na posição inicial do nível, irá ŕimeiro marcar as folhas com max_value, e depois irá atualizar max value com os nós intermediários
      //o for para se perceber que já percorreu todos as folhas e nós intermediários, salvos em count[i]
      for(int j=inicio[i], flag=1; j<=N_MAX && j!=inicio[i]+count[i]; j++){
        //Folha, damos a ela o valor minimo possível que é o maior dos níveis inferiores
        if(Matriz[i][j] == 0 )Matriz[i][j] = max_value;
        //Caso onde salvamos o maior valor do nível
        else if(max_value < Matriz[i][j]) max_value = Matriz[i][j];

        //fim das folhas no nível acima, onde iremos adicionar os nós intermediários
        int fim = inicio[i-1]+count[i-1];
        //flag fica alternando e salvando para somar dois valores consecutivos na posição de fim, e adicionar 
        // ao count[i-1] o novo valor que irá alterar a posição fim, fazendo com que só ande no nível superior a cada 2 posições do nível atual 
        if(flag){ 
           Matriz[i-1][fim] = Matriz[i][j];
        }
        if(!flag) {
          Matriz[i-1][fim] += Matriz[i][j]; 
          count[i-1]++;
        }
        flag=!flag;
      }
    }

    //O nível 0 só tem elemento em 0, é onde está nossa resposta já q ja foi somado no nível anterior
    printf("%d\n", Matriz[0][0]);

#if DEBUG
    for(int i=0; i<=max; i++){
      printf("L%d: ", i);
      for(int j=0; j<=N_MAX; j++)  
        printf("[%d]", Matriz[i][j]);
      printf("\n");
    }
#endif

    scanf("%d", &N);
  }

  for(int i=0; i<=L_MAX; i++){
    free(Matriz[i]);
  }
  free(Matriz);
}