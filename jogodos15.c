#include <stdio.h>
#include <stdlib.h> //ler doc
#include <time.h>
#include <string.h>

#define INF 1000000

int nos_visitados;
clock_t tempoinicial, tempofinal;

typedef struct node{
    int **tabuleiro;
    int profundidade;
    int h;
    char ultimoMov;
    struct node* pai;
}node;

node* newNode(int** tabuleiro, node* pai, int p, int h, char um){
    node *n = malloc(sizeof(node));

    n->tabuleiro = (int**)malloc(4*sizeof(int*));
    for(int i = 0; i < 4; i++){
      n->tabuleiro[i] = (int*)malloc(4*sizeof(int));
    }

    for(int i = 0; i < 4; i++){ //fazer uma copia do tabuleiro a alterar garante que podemos reutilizar a matriz
        for(int j = 0; j < 4; j++){
            n->tabuleiro[i][j] = tabuleiro[i][j];
        }
    }

    n->pai = pai;
    n->profundidade = p;
    n->h=h;
    n->ultimoMov = um;

    return n;
}
/*TESTE*/
typedef struct pqueue_node{
    node* data;
    int priority;
    struct pqueue_node* next;
}pqueue_node;

typedef struct pqueue{
    pqueue_node* head;
}pqueue;

pqueue_node* new_pqueue_node(node* data, int priority){
    pqueue_node* new_node = (pqueue_node*)malloc(sizeof(pqueue_node));
    new_node->data = data;
    new_node->priority = priority;
    new_node->next = NULL;
    return new_node;
}

pqueue* new_pqueue(){

    pqueue* new_queue = (pqueue*)malloc(sizeof(pqueue));
    new_queue->head = NULL;
    return new_queue;
}
node* find_node(pqueue* queue, node* target_node){
    pqueue_node* current = queue->head;
    while(current != NULL){
        if(current->data == target_node){
            return target_node;
        }
        current = current->next;
    }
    return NULL;
}
void enqueueq(pqueue* queue, node* data){
    //printf("Entrou\n");
    int priority = data->h;
    pqueue_node* new_node = new_pqueue_node(data, priority);

    if(queue->head == NULL){
        queue->head = new_node;
    }
    node* found_node = find_node(queue, data);
    if(found_node == NULL){
      if(priority < queue->head->priority){
        new_node->next = queue->head;
        queue->head = new_node;
      }
      else{
          pqueue_node* current = queue->head;
          while(current->next != NULL && current->priority <= priority){
              current = current->next;
          }
          new_node->next = current->next;
          current->next = new_node;
      }
    }
}

node* dequeueq(pqueue* queue){
    if(queue->head == NULL){
        return NULL;
    }
    else{
        pqueue_node* temp = queue->head;
        node* data = temp->data;
        queue->head = queue->head->next;
        free(temp);
        return data;
    }
}
//implementacao simples de uma queue de pointers para nodes
typedef struct _queue{
  node** elements;
  int capacity;
  int size;
  int front;
  int h;
  int rear;
}queue;

queue* newQueue(int maxcapacity){
  queue* q;
  q = (queue*)malloc(sizeof(queue));
  q->elements = (node**)malloc(maxcapacity*sizeof(node*));
  q->capacity = maxcapacity;
  q->size = 0;
  q->front = 0;
  q->h = 0;
  q->rear = -1;
  return q;
}

node* dequeue(queue* q){
  node* n = q->elements[q->front];
  q->size--;
  q->front++;
  return n;
}

void enqueue(queue* q, node* n){
  q->size++;
  q->rear++;
  q->elements[q->rear] = n;
  return;
}

int isEmptyQ(queue* q){
  return (q->size == 0);
}

typedef struct _stack
{
    node** data;
    int top;
    int size;
}stack;

stack* newStack(int size){
  stack *s = malloc(sizeof(stack));
  s->data = malloc(size*sizeof(node*));
  s->top = 0;
  s->size = size;
  return s;
}

void push(stack *s, node* n){
  (s->top)++;
  s->data[s->top] = n;
}

node* pop(stack *s){
  node* n = (s->data[s->top]);
  (s->top)--;
  return n;
}

int isEmpty(stack *s){
  return ((s->top)==0);
}


// Implementacao da heap

typedef struct _minheap{
    node** arr;
    int size;
    int capacity;
}minHeap;

int parent(int i) {
    return (i - 1) / 2;
}

int left_child(int i) {
    return (2*i + 1);
}

int right_child(int i) {
    return (2*i + 2);
}

int isEmptyh(minHeap *n) {
  return ((n->size) == 0);
}

minHeap* heapify(minHeap* heap, int index) {
      if (heap->size <= 1)
          return heap;

      int left = left_child(index);
      int right = right_child(index);

      int smallest = index;

      if (left < heap->size && heap->arr[left]->h < heap->arr[index]->h)
          smallest = left;

      if (right < heap->size && heap->arr[right]->h < heap->arr[smallest]->h)
          smallest = right;
      if (smallest != index)
      {
          node* temp = heap->arr[index];
          heap->arr[index] = heap->arr[smallest];
          heap->arr[smallest] = temp;
          heap = heapify(heap, smallest);
      }

      return heap;
}

minHeap* newHeap(int capacity) {
    minHeap* n = (minHeap*) malloc (sizeof(minHeap));
    n->arr = (node**) malloc (capacity * sizeof(node*));
    n->capacity = capacity;
    n->size = 0;
    return n;
}



node* PopMin(minHeap *h){
    node* pop;
    if(h->size==0){
        printf("\n__Heap is Empty__\n");
        return NULL;
    }
    // replace first node by last and delete last
    pop = h->arr[0];
    h->arr[0] = h->arr[h->size-1];
    h->size--;
    heapify(h, 0);
    return pop;
}

void insertminHeap(minHeap* heap, node* element) {
    if (heap->size == heap->capacity) {
        fprintf(stderr, "Cannot insert %d. Heap is already full!\n", element->profundidade);
        return ;
    }
    heap->size++;
      heap->arr[heap->size - 1] = element;

      int curr = heap->size - 1;

      while (curr > 0 && heap->arr[parent(curr)]->h > heap->arr[curr]->h) {
          node* temp = heap->arr[parent(curr)];
          heap->arr[parent(curr)] = heap->arr[curr];
          heap->arr[curr] = temp;
          curr = parent(curr);
      }
      return ;
  }


//=====================================================
void printTab(int** tab){
    for(int i = 0 ; i < 4 ; i++){
        for(int j = 0 ; j < 4 ; j++){
            printf("%d\t", tab[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int inversoes(int** matriz) {
	int num_inversoes = 0;
	int array[16];
	int a=0;

	for (int i=0; i<4; i++) {
		for (int j=0; j<4; j++) {
			array[a] = matriz[i][j];
			a++;
		}
	}

	for (int i=0; i<15; i++) {
		for (int j=i+1; j<16; j++) {
			if (array[j] < array[i] && array[j] != 0) {
				num_inversoes++;
			}
		}
	}
	return num_inversoes;
}

int paridade_zero(int** matriz) {
	for (int i=0; i<4; i++) {
		for (int j=0; j<4; j++) {
			if (matriz[i][j] == 0) {
				return !((i+1)%2);
			}
		}
	}
  return -1;
}

int paridade_matriz(int** matriz) {
	if (paridade_zero(matriz) == 0) {
		if (inversoes(matriz)%2 == 0)
			return 0; //paridade par e num inversoes par
		else return 1; //ao contrario
	}
	else {
		if (inversoes(matriz)%2 == 0)
			return 1;
		else return 0;
	}
}

int solubilidade(int** inicial, int** final) {

	return paridade_matriz(inicial) == paridade_matriz(final);

}

int** mover_direita(int** tabuleiro){
    int **novo_tabuleiro = (int**)malloc(4*sizeof(int*));
    for(int i = 0; i < 4; i++){
      novo_tabuleiro[i] = (int*)malloc(4*sizeof(int));
    }
    int zeroi, zeroj;

    for(int i = 0; i < 4; i++){ //fazer uma copia do tabuleiro a alterar garante que podemos reutilizar a matriz
        for(int j = 0; j < 4; j++){
            novo_tabuleiro[i][j] = tabuleiro[i][j];
            if(tabuleiro[i][j] == 0){   //guarda os indices do espaco em branco
                zeroi = i;
                zeroj = j;
            }
        }
    }

    novo_tabuleiro[zeroi][zeroj] = tabuleiro[zeroi][zeroj+1];
    novo_tabuleiro[zeroi][zeroj+1] = 0;

    return novo_tabuleiro;
}

int** mover_esquerda(int** tabuleiro){
    int **novo_tabuleiro = (int**)malloc(4*sizeof(int*));
    for(int i = 0; i < 4; i++){
      novo_tabuleiro[i] = (int*)malloc(4*sizeof(int));
    }
    int zeroi, zeroj;

    for(int i = 0; i < 4; i++){ //fazer uma copia do tabuleiro a alterar garante que podemos reutilizar a matriz
        for(int j = 0; j < 4; j++){
            novo_tabuleiro[i][j] = tabuleiro[i][j];
            if(tabuleiro[i][j] == 0){   //guarda os indices do espaco em branco
                zeroi = i;
                zeroj = j;
            }
        }
    }

    novo_tabuleiro[zeroi][zeroj] = tabuleiro[zeroi][zeroj-1];
    novo_tabuleiro[zeroi][zeroj-1] = 0;

    return novo_tabuleiro;
}

int** mover_cima(int** tabuleiro){
    int **novo_tabuleiro = (int**)malloc(4*sizeof(int*));
    for(int i = 0; i < 4; i++){
      novo_tabuleiro[i] = (int*)malloc(4*sizeof(int));
    }
    int zeroi, zeroj;

    for(int i = 0; i < 4; i++){ //fazer uma copia do tabuleiro a alterar garante que podemos reutilizar a matriz
        for(int j = 0; j < 4; j++){
            novo_tabuleiro[i][j] = tabuleiro[i][j];
            if(tabuleiro[i][j] == 0){   //guarda os indices do espaco em branco
                zeroi = i;
                zeroj = j;
            }
        }
    }

    novo_tabuleiro[zeroi][zeroj] = tabuleiro[zeroi-1][zeroj];
    novo_tabuleiro[zeroi-1][zeroj] = 0;

    return novo_tabuleiro;
}

int** mover_baixo(int** tabuleiro){
    int **novo_tabuleiro = (int**)malloc(4*sizeof(int*));
    for(int i = 0; i < 4; i++){
      novo_tabuleiro[i] = (int*)malloc(4*sizeof(int));
    }
    int zeroi, zeroj;

    for(int i = 0; i < 4; i++){ //fazer uma copia do tabuleiro a alterar garante que podemos reutilizar a matriz
        for(int j = 0; j < 4; j++){
            novo_tabuleiro[i][j] = tabuleiro[i][j];
            if(tabuleiro[i][j] == 0){   //guarda os indices do espaco em branco
                zeroi = i;
                zeroj = j;
            }
        }
    }

    novo_tabuleiro[zeroi][zeroj] = tabuleiro[zeroi+1][zeroj];
    novo_tabuleiro[zeroi+1][zeroj] = 0;

    return novo_tabuleiro;
}

int TabIgual(int** tab1, int** tab2){

    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            if(tab1[i][j] != tab2[i][j]){
                return 0;
            }
        }
    }

    return 1;

}

int misplaced(int** tab1, int**tab2){
  int count = 0;
  for(int i = 0; i < 4; i++){
    for(int j = 0; j < 4; j++){
      if(tab1[i][j] != tab2[i][j] && tab1[i][j] != 0){
          count++;
      }
    }
  }
  return count;
}

int manhattan(int** tab1, int**tab2, int x){
  int itab1,jtab1,itab2,jtab2,final;
  for(int i = 0; i < 4; i++){
    for(int j = 0; j < 4; j++){
      if(tab1[i][j] == x){
        itab1 = i;
        jtab1 = j;
      }
      if(tab2[i][j] == x){
        itab2 = i;
        jtab2 = j;
      }
    }
  }
  final = abs(itab1-itab2) +  abs(jtab1-jtab2);
  return final;
}

int manhattan_distance(int** tab1, int**tab2){
  int x;
  for(int i = 1; i < 16; i++){
    x = x + manhattan(tab1,tab2,i);
  }
  return x;
}

void gerarSucessoresBFS(node* v, queue *fila){
  int zeroi,zeroj;
  node* p = v->pai;
  int** temptab = (int**)malloc(4*sizeof(int*));
  for(int i = 0; i < 4; i++){
    temptab[i] = (int*)malloc(4*sizeof(int));
  }

  for(int i = 0; i < 4; i++){
      for(int j = 0; j < 4; j++){
          if(v->tabuleiro[i][j] == 0){
              zeroi = i;
              zeroj = j;
          }
      }
  }

  //Cima
  if(zeroi > 0){
      temptab = mover_cima(v->tabuleiro);
      if(p== NULL || !TabIgual(temptab,p->tabuleiro)){
          node* n1 = newNode(temptab, v, (v->profundidade)+1, (v->h)+1, 'c');
          enqueue(fila,n1);
      }
  }
  //Baixo
  if(zeroi < 3){
    temptab = mover_baixo(v->tabuleiro);
    if(p==NULL || !TabIgual(temptab,p->tabuleiro)){
        node* n2 = newNode(temptab, v, (v->profundidade)+1, (v->h)+1, 'b');
        enqueue(fila,n2);
    }
  }
  //Direita
  if(zeroj < 3){
    temptab = mover_direita(v->tabuleiro);
    if(p==NULL || !TabIgual(temptab,p->tabuleiro)){
        node* n3 = newNode(temptab, v, (v->profundidade)+1, (v->h)+1, 'd');
        enqueue(fila,n3);
    }
  }
  //Esquerda
  if(zeroj > 0){
    temptab = mover_esquerda(v->tabuleiro);
    if(p==NULL || !TabIgual(temptab,p->tabuleiro)){
        node* n4 = newNode(temptab, v, (v->profundidade)+1, (v->h)+1, 'e');
        enqueue(fila,n4);
    }
  }
  return;
}

void gerarSucessoresDFS(node* v, stack *pilha){
    int zeroi,zeroj;
    node* p = v->pai;
    int** temptab = (int**)malloc(4*sizeof(int*));
    for(int i = 0; i < 4; i++){
      temptab[i] = (int*)malloc(4*sizeof(int));
    }

    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            if(v->tabuleiro[i][j] == 0){
                zeroi = i;
                zeroj = j;
            }
        }
    }

    //Cima
    if(zeroi > 0){
        temptab = mover_cima(v->tabuleiro);
        if(p== NULL || !TabIgual(temptab,p->tabuleiro)){
            node* n1 = newNode(temptab, v, (v->profundidade)+1, (v->h)+1, 'c');
            push(pilha,n1);
        }
    }
    //Baixo
    if(zeroi < 3){
      temptab = mover_baixo(v->tabuleiro);
      if(p==NULL || !TabIgual(temptab,p->tabuleiro)){
          node* n2 = newNode(temptab, v, (v->profundidade)+1, (v->h)+1, 'b');
          push(pilha,n2);
      }
    }
    //Direita
    if(zeroj < 3){
      temptab = mover_direita(v->tabuleiro);
      if(p==NULL || !TabIgual(temptab,p->tabuleiro)){
          node* n3 = newNode(temptab, v, (v->profundidade)+1, (v->h)+1, 'd');
          push(pilha,n3);
      }
    }
    //Esquerda
    if(zeroj > 0){
      temptab = mover_esquerda(v->tabuleiro);
      if(p==NULL || !TabIgual(temptab,p->tabuleiro)){
          node* n4 = newNode(temptab, v, (v->profundidade)+1, (v->h)+1, 'e');
          push(pilha,n4);
      }
    }
    return;
}

void gerarSucessores_misplaced(node* v, pqueue *fila, int** tfinal){
    int zeroi,zeroj;
    node* p = v->pai;
    int** temptab = (int**)malloc(4*sizeof(int*));
    for(int i = 0; i < 4; i++){
      temptab[i] = (int*)malloc(4*sizeof(int));
    }

    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            if(v->tabuleiro[i][j] == 0){
                zeroi = i;
                zeroj = j;
            }
        }
    }
    //Cima
    if(zeroi > 0){
      temptab = mover_cima(v->tabuleiro);
      if(p== NULL || !TabIgual(temptab,p->tabuleiro)){
          node* n1 = newNode(temptab, v, (v->profundidade)+1, misplaced(temptab,tfinal), 'c');
          enqueueq(fila,n1);
      }
    }
      //Baixo
  if(zeroi < 3){
    temptab = mover_baixo(v->tabuleiro);
    if(p==NULL || !TabIgual(temptab,p->tabuleiro)){
        node* n2 = newNode(temptab, v, (v->profundidade)+1, misplaced(temptab,tfinal), 'b');
        enqueueq(fila,n2);
    }
  }
  //Direita
  if(zeroj < 3){
    temptab = mover_direita(v->tabuleiro);
    if(p==NULL || !TabIgual(temptab,p->tabuleiro)){
        node* n3 = newNode(temptab, v, (v->profundidade)+1, misplaced(temptab,tfinal), 'd');
        enqueueq(fila,n3);
    }
  }
  //Esquerda
  if(zeroj > 0){
    temptab = mover_esquerda(v->tabuleiro);
    if(p==NULL || !TabIgual(temptab,p->tabuleiro)){
        node* n4 = newNode(temptab, v, (v->profundidade)+1, misplaced(temptab,tfinal), 'e');
        enqueueq(fila,n4);
    }
  }
  return;
}

void gerarSucessores_manhattan_distance(node* v, pqueue* fila,int** tfinal){
  int zeroi,zeroj;
    node* p = v->pai;
    int** temptab = (int**)malloc(4*sizeof(int*));
    for(int i = 0; i < 4; i++){
      temptab[i] = (int*)malloc(4*sizeof(int));
    }

    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            if(v->tabuleiro[i][j] == 0){
                zeroi = i;
                zeroj = j;
            }
        }
    }
    //Cima
    if(zeroi > 0){
      temptab = mover_cima(v->tabuleiro);
      if(p== NULL || !TabIgual(temptab,p->tabuleiro)){
          node* n1 = newNode(temptab, v, (v->profundidade)+1, manhattan_distance(temptab,tfinal), 'c');
          enqueueq(fila,n1);
      }
    }
  //Baixo
  if(zeroi < 3){
    temptab = mover_baixo(v->tabuleiro);
    if(p==NULL || !TabIgual(temptab,p->tabuleiro)){
        node* n2 = newNode(temptab, v, (v->profundidade)+1, manhattan_distance(temptab,tfinal), 'b');
        enqueueq(fila,n2);
    }
  }
  //Direita
  if(zeroj < 3){
    temptab = mover_direita(v->tabuleiro);
    if(p==NULL || !TabIgual(temptab,p->tabuleiro)){
        node* n3 = newNode(temptab, v, (v->profundidade)+1, manhattan_distance(temptab,tfinal), 'd');
        enqueueq(fila,n3);
    }
  }
  //Esquerda
  if(zeroj > 0){
    temptab = mover_esquerda(v->tabuleiro);
    if(p==NULL || !TabIgual(temptab,p->tabuleiro)){
        node* n4 = newNode(temptab, v, (v->profundidade)+1, manhattan_distance(temptab,tfinal), 'e');
        enqueueq(fila,n4);
    }
  }
  return;
}

void gerarSucessores_misplaced2(node* v, int** tfinal, minHeap *Heap){
  int zeroi,zeroj;
  node* p = v->pai;
  int** temptab = (int**)malloc(4*sizeof(int*));
  for(int i = 0; i < 4; i++){
    temptab[i] = (int*)malloc(4*sizeof(int));
  }

  for(int i = 0; i < 4; i++){
      for(int j = 0; j < 4; j++){
          if(v->tabuleiro[i][j] == 0){
              zeroi = i;
              zeroj = j;
          }
      }
  }
  //Cima
  if(zeroi > 0){
      temptab = mover_cima(v->tabuleiro);
      if(p== NULL || !TabIgual(temptab,p->tabuleiro)){
          node* n1 = newNode(temptab, v, (v->profundidade)+1, misplaced(temptab,tfinal)+(v->profundidade) + 1, 'c');
          insertminHeap(Heap,n1);
      }
  }
  //Baixo
  if(zeroi < 3){
    temptab = mover_baixo(v->tabuleiro);
    if(p==NULL || !TabIgual(temptab,p->tabuleiro)){
        node* n2 = newNode(temptab, v, (v->profundidade)+1, misplaced(temptab,tfinal)+(v->profundidade) + 1, 'b');
        insertminHeap(Heap,n2);
    }
  }
  //Direita
  if(zeroj < 3){
    temptab = mover_direita(v->tabuleiro);
    if(p==NULL || !TabIgual(temptab,p->tabuleiro)){
        node* n3 = newNode(temptab, v, (v->profundidade)+1, misplaced(temptab,tfinal)+(v->profundidade) + 1, 'd');
        insertminHeap(Heap,n3);
    }
  }
  //Esquerda
  if(zeroj > 0){
    temptab = mover_esquerda(v->tabuleiro);
    if(p==NULL || !TabIgual(temptab,p->tabuleiro)){
        node* n4 = newNode(temptab, v, (v->profundidade)+1, misplaced(temptab,tfinal)+(v->profundidade) + 1, 'e');
        insertminHeap(Heap,n4);
    }
  }
  return;
}

void gerarSucessores_manhattan_distance2(node* v, int** tfinal, minHeap *Heap){
  int zeroi,zeroj;
  node* p = v->pai;
  int** temptab = (int**)malloc(4*sizeof(int*));
  for(int i = 0; i < 4; i++){
    temptab[i] = (int*)malloc(4*sizeof(int));
  }

  for(int i = 0; i < 4; i++){
      for(int j = 0; j < 4; j++){
          if(v->tabuleiro[i][j] == 0){
              zeroi = i;
              zeroj = j;
          }
      }
  }

  //Cima
  if(zeroi > 0){
      temptab = mover_cima(v->tabuleiro);
      if(p== NULL || !TabIgual(temptab,p->tabuleiro)){
          node* n1 = newNode(temptab, v, (v->profundidade)+1, manhattan_distance(temptab,tfinal)+(v->profundidade) + 1, 'c');
          insertminHeap(Heap,n1);
      }
  }
  //Baixo
  if(zeroi < 3){
    temptab = mover_baixo(v->tabuleiro);
    if(p==NULL || !TabIgual(temptab,p->tabuleiro)){
        node* n2 = newNode(temptab, v, (v->profundidade)+1, manhattan_distance(temptab,tfinal)+(v->profundidade) + 1, 'b');
        insertminHeap(Heap,n2);
    }
  }
  //Direita
  if(zeroj < 3){
    temptab = mover_direita(v->tabuleiro);
    if(p==NULL || !TabIgual(temptab,p->tabuleiro)){
        node* n3 = newNode(temptab, v, (v->profundidade)+1, manhattan_distance(temptab,tfinal)+(v->profundidade) + 1, 'd');
        insertminHeap(Heap,n3);
    }
  }
  //Esquerda
  if(zeroj > 0){
    temptab = mover_esquerda(v->tabuleiro);
    if(p==NULL || !TabIgual(temptab,p->tabuleiro)){
        node* n4 = newNode(temptab, v, (v->profundidade)+1, manhattan_distance(temptab,tfinal)+(v->profundidade) + 1, 'e');
        insertminHeap(Heap,n4);
    }
  }
  return;
}

void pathToSolution(node* n){
    char* mov = (char*)malloc(100*sizeof(char));
    int c = 0;
    node* v = n;

    while(v->pai != NULL){
        mov[c] = v->ultimoMov;
        c++;
        v = v->pai;
    }

    printf("\n%d jogadas: ", c);
    c--;
    while(c >= 0){
        if(mov[c] == 'd'){
            printf("direita; " );
        }
        else{
          if(mov[c] == 'e'){
            printf("esquerda; " );
          }
          else{
            if(mov[c] == 'c'){
              printf("cima; " );
            }
            else{
              if(mov[c] == 'b'){
                printf("baixo; " );
              }
            }
          }
        }

        c--;
    }
    printf("\n");
    return;
}
int is_pqueue_empty(pqueue* queue){
    return (queue->head == NULL);
}
node* pop_minq(pqueue* queue){
    if(queue->head == NULL){ // Se a lista está vazia
        return NULL; // Retorna NULL indicando que não há elementos na lista
    }
    else{
        node* min_node = queue->head->data; // Salva o nó com menor prioridade
        pqueue_node* tmp = queue->head; // Salva o ponteiro para o nó com menor prioridade
        queue->head = queue->head->next; // Atualiza a cabeça da lista
        free(tmp); // Libera o nó com menor prioridade
        return min_node; // Retorna o nó com menor prioridade
    }
}

void DFS(int** tinicial, int** tfinal, int profMax){
  stack * pilhaAberta = newStack(INF);
  stack * pilhaFechada = newStack(INF);
  node* inicial = newNode(tinicial, NULL, 0, 0, ' ');
  node* v = inicial;
  push(pilhaAberta,inicial);
  while(!isEmpty(pilhaAberta)){
    v = pop(pilhaAberta);
    nos_visitados++;
    push(pilhaFechada,v);
    if(TabIgual(v->tabuleiro,tfinal)){
      pathToSolution(v);
      return;
    }
    else{
      if(v->profundidade < profMax){
        gerarSucessoresDFS(v,pilhaAberta);
      }
    }
  }
    free(pilhaAberta->data);
    free(pilhaFechada->data);
  printf("Nao foi possivel encontrar solucao com profundidade ate %d jogadas.\n", profMax);
  return;
}

void IDFS(int** tinicial, int** tfinal){
      stack * pilhaAberta;
      stack * pilhaFechada;
      for(int depth = 0; depth < INF; depth++){ //limit of 3 for testing
          pilhaAberta = newStack(INF);
          pilhaFechada= newStack(INF);
          node* inicial = newNode(tinicial, NULL, 0, 0, ' ');
          node* v = inicial;
          push(pilhaAberta,inicial);
          while(!isEmpty(pilhaAberta)){
              v = pop(pilhaAberta);
              nos_visitados++;
              push(pilhaFechada,v);
              if(TabIgual(v->tabuleiro, tfinal)){
                  pathToSolution(v);
                  return;
              }
              else{
                if(v->profundidade < depth){
                  gerarSucessoresDFS(v, pilhaAberta);
                }
              }
          }
      free(pilhaAberta->data);
      free(pilhaFechada->data);
    }
}
void BFS(int** tinicial, int** tfinal){
  queue * filaAberta = newQueue(INF);
  stack * pilhaFechada = newStack(INF);
  node* inicial = newNode(tinicial, NULL, 0, 0, ' ');
  node* v = inicial;
  enqueue(filaAberta,inicial);
  while(!isEmptyQ(filaAberta)){
    v = dequeue(filaAberta);
    nos_visitados++;
    push(pilhaFechada,v);
    if(TabIgual(v->tabuleiro,tfinal)){
      pathToSolution(v);
      return;
    }
    else{
      gerarSucessoresBFS(v,filaAberta);
    }
  }
}

//Gulosa com heuristica
void Gulosa_misplaced(int** tinicial, int** tfinal){
    pqueue* my_pqueue = new_pqueue();
    stack * pilhaFechada = newStack(INF);
    node* inicial = newNode(tinicial,NULL,0, misplaced(tinicial,tfinal),' ');
    node* v = inicial;
    enqueueq(my_pqueue,v);
    while(!is_pqueue_empty(my_pqueue)){
        v = dequeueq(my_pqueue);
        nos_visitados++;
        push(pilhaFechada,v);
        if(TabIgual(v->tabuleiro, tfinal)){
            pathToSolution(v);
            return;
        }
        else{
          gerarSucessores_misplaced(v, my_pqueue,tfinal);
        }
    }
}
void Gulosa_manhattan_distance(int** tinicial, int** tfinal){
  minHeap* Heap = newHeap(INF);
  stack * pilhaFechada = newStack(INF);
  node* v = newNode(tinicial,NULL,0, manhattan_distance(tinicial,tfinal),' ');
  insertminHeap(Heap,v);

  while(!isEmptyh(Heap)){
    v = PopMin(Heap);
    nos_visitados++;
    push(pilhaFechada, v);
    if(TabIgual(v->tabuleiro, tfinal)){
      pathToSolution(v);
      return;
    }
    else{
      gerarSucessores_manhattan_distance2(v, tfinal, Heap);
    }
  }
}

void AStar_misplaced(int** tinicial, int** tfinal){
  minHeap* Heap = newHeap(INF);
  stack * pilhaFechada = newStack(INF);
  node* v = newNode(tinicial,NULL,0, misplaced(tinicial,tfinal),' ');
  insertminHeap(Heap,v);

  while(!isEmptyh(Heap)){
    v = PopMin(Heap);
    nos_visitados++;
    push(pilhaFechada, v);
    if(TabIgual(v->tabuleiro, tfinal)){
      pathToSolution(v);
      return;
    }
    else{
      gerarSucessores_misplaced2(v, tfinal, Heap);
    }
  }
}
void AStar_manhattan_distance(int** tinicial, int** tfinal){
  minHeap* Heap = newHeap(INF);
  stack * pilhaFechada = newStack(INF);
  node* v = newNode(tinicial,NULL,0, manhattan_distance(tinicial,tfinal),' ');
  insertminHeap(Heap,v);

  while(!isEmptyh(Heap)){
    v = PopMin(Heap);
    nos_visitados++;
    push(pilhaFechada, v);
    if(TabIgual(v->tabuleiro, tfinal)){
      pathToSolution(v);
      return;
    }
    else{
      gerarSucessores_manhattan_distance2(v, tfinal, Heap);
    }
  }
}

int main(int argc, char** argv) {
  int opt = 0;
  int end = 0;
  int novaconfig = 0;
  double tempoexec;
  int prof;
  int** inicial = (int**)malloc(4*sizeof(int*));
  for(int i = 0; i < 4; i++){
    inicial[i] = (int*)malloc(4*sizeof(int));
  }
  int** final = (int**)malloc(4*sizeof(int*));
    for(int i = 0; i < 4; i++){
      final[i] = (int*)malloc(4*sizeof(int));
  }
    //printf("Insira a configuracao inicial:\n");
    for(int i = 0; i < 4; i++){
      for(int j = 0; j < 4; j++){
        scanf("%d", &inicial[i][j]);
      }
    }

    //printf("Insira a configuracao objetivo:\n");
    for(int i = 0; i < 4; i++){
      for(int j = 0; j < 4; j++){
        scanf("%d", &final[i][j]);
      }
    }
     if(solubilidade(inicial,final)){
            nos_visitados = 0;
            tempoinicial = clock();

            if (strcmp(argv[1], "BFS") == 0){
                BFS(inicial,final);
                tempofinal = clock();
                tempoexec = (double) (tempofinal - tempoinicial)/CLOCKS_PER_SEC;
                printf("Nos visitados: %d\nTempo de execucao: %f segundos\n\n",nos_visitados, tempoexec);
            }
            else if (strcmp(argv[1], "DFS") == 0){
                prof = atoi(argv[2]);
                //printf("%d",prof);
                DFS(inicial,final, prof);
                tempofinal = clock();
                tempoexec = (double) (tempofinal - tempoinicial)/CLOCKS_PER_SEC;
                printf("Nos visitados: %d\nTempo de execucao: %f segundos\n\n",nos_visitados, tempoexec);
            }
            else if (strcmp(argv[1], "IDFS") == 0){
                IDFS(inicial,final);
                tempofinal = clock();
                tempoexec = (double) (tempofinal - tempoinicial)/CLOCKS_PER_SEC;
                printf("Nos visitados: %d\nTempo de execucao: %f segundos\n\n",nos_visitados, tempoexec);
            }
            else if (strcmp(argv[1], "Greedy-misplaced") == 0){
                Gulosa_misplaced(inicial,final);
                tempofinal = clock();
                tempoexec = (double) (tempofinal - tempoinicial)/CLOCKS_PER_SEC;
                printf("Nos visitados: %d\nTempo de execucao: %f segundos\n\n",nos_visitados, tempoexec);
            }
            else if (strcmp(argv[1], "Greedy-Manhattan") == 0){
                Gulosa_manhattan_distance(inicial,final);
                tempofinal = clock();
                tempoexec = (double) (tempofinal - tempoinicial)/CLOCKS_PER_SEC;
                printf("Nos visitados: %d\nTempo de execucao: %f segundos\n\n",nos_visitados, tempoexec);
            }
            else if (strcmp(argv[1], "A*-misplaced") == 0){
                AStar_misplaced(inicial,final);
                tempofinal = clock();
                tempoexec = (double) (tempofinal - tempoinicial)/CLOCKS_PER_SEC;
                printf("Nos visitados: %d\nTempo de execucao: %f segundos\n\n",nos_visitados, tempoexec);
            }
            else if (strcmp(argv[1], "A*-Manhattan") == 0){
                AStar_manhattan_distance(inicial,final);
                tempofinal = clock();
                tempoexec = (double) (tempofinal - tempoinicial)/CLOCKS_PER_SEC;
                printf("Nos visitados: %d\nTempo de execucao: %f segundos\n\n",nos_visitados, tempoexec);
            }
            else{
              printf("Erro no nome da busca!");
            }
     }
     else{
      printf("O caso de teste nao e soluvel!");
     }
	return 0;
}
