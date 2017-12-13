// =========================================================================
// Informações do Laboratório
// =========================================================================

/*
  Pedro Hideaki Uiechi Chinen - 175828

  Laboratorio 5 - Parques


  Relatorio:

  Função: elemento O(V)
  Função: criarGrafo O(V+E)
  Função: BFS O(V+E)
  Função: temCaminho O(1)
  Função: fluxo O(E)
  Função: novoGrafo O(E)
  Função: fluxoMaximo O(VE^2)
  Função: Principal O(VE^2)

  Temos um grafo com uma fonte e um ralo, a apresentação do parque e a
  loja de conveniências respectivamente. 

  A quantidade de repetições é o custo de cada aresta. E gostariamos
  de saber qual o fluxo máximo entre a fonte o ralo.

  Utilizamos o algoritmo de Edmonds-Karp para encontrar esses
  fluxos. Utilizando busca em largura para encontrar os caminhos entre
  a fonte e o ralo em cada iteração.
*/

// =========================================================================
// Includes
// =========================================================================

#include "solve.hpp"


// =========================================================================
// Structs
// =========================================================================
struct No{
  int repeticoes;
  vector<int> adjacentes;
};

struct Grafo{
  vector<No> nos;
};


// =========================================================================
// Funções Auxiliares
// =========================================================================

// =========================================================================
// Função: elemento
// 
// Verifica se um elemento esta em um vector
// =========================================================================
int elemento(vector<int> vetor, int valor){
  int elemento = 1;

  // Percorre vector
  for(unsigned int i = 0; i < vetor.size() && elemento == 1; i++){

    // Se elemento esta no vector para
    if(vetor[i] == valor){
      elemento = 0;
    }
  }

  // 1 = não é elemento
  // 0 = é elemento
  return elemento;
}


// =========================================================================
// Função: BFS
// 
// Retorna um vector de sucessores entre a fonte e o ralo
// Utilizando a Busca em Largura de um grafo
// =========================================================================
vector<int> BFS(Grafo *grafo, int fonte, int ralo){
  int sucesso = 1;

  // Inicializa fila e sucessores
  std::queue<int> fila;
  vector<int> sucessores;
  sucessores.resize(grafo->nos.size(), INT_MAX);

  vector<int> visitados; 
  visitados.resize(grafo->nos.size()+1, 0);
  visitados[fonte] = 1;
  // visitados[ralo] = 1;

  // Coloca a fonte na fila
  fila.push(fonte);

  // Enquanto fila não esta vazia e não encontrou o ralo
  while(!fila.empty() && sucesso == 1){

    // Retira o primeiro da fila
    int indice = fila.front();
    fila.pop();

    // Se não é o ralo coloca adjacentes na fila
    for(unsigned int i = 0; 
        i < grafo->nos[indice].adjacentes.size() && sucesso == 1; 
        i++){
        
      // Pega o valor do filho adjacente
      int indiceFilho = grafo->nos[indice].adjacentes[i];

      // se o vertice do indiceFilho ainda não foi visitado
      // e a quantidade de repeticoes atual é valida
      if((visitados[indiceFilho] == 0) &&
         (grafo->nos[indice].repeticoes >= -1)){

        // Marcar ele como visitado e seu sucessor
        sucessores[indiceFilho] = indice;
        fila.push(indiceFilho);
        visitados[indiceFilho] = 1;


        // Se indice que foi colocado na fila é o ralo
        if(indiceFilho == ralo){

          // Entao nao precisa mais procurar o ralo
          sucesso = 0;
        }
        // Se indice que foi colocado na fila é o ralo
        // if(sucessores[ralo] != INT_MAX){

        //   // Entao nao precisa mais procurar o ralo
        //   sucesso = 0;
        // }
      }
    }
  }

  return sucessores; 
}

// =========================================================================
// Função: temCaminho
// 
// Verifica se existe caminho entre a fonte e o ralo
// =========================================================================
bool temCaminho(vector<int> sucessores){
  return sucessores.back() != INT_MAX;
}


// =========================================================================
// Função: fluxo
// 
// Pegar o menor valor de fluxo que passa pro este caminho
// =========================================================================
int fluxo(Grafo *grafo, vector<int> sucessores){
  int minimo = INT_MAX;
  int repeticoes;
  
  // Pega o indice do ralo
  int i = sucessores.size()-1;

  // Enquanto não chegou no indice do fonte faça
  while(i>0){

    // Pega repeticoes para este indice
    repeticoes = grafo->nos[i].repeticoes;

    // Se for menor que o menor, trocar o menor
    if(minimo > repeticoes && repeticoes != -1){
      minimo = repeticoes;
    }
    
    // Pegar o indice do pai do atual
    i = sucessores[i];
  }
  
  // Retornar o menor valor de fluxo neste caminho
  return minimo;
}


// =========================================================================
// Função: novoGrafo
// 
// Arruma o grafo a partir do caminho e do fluxo que passa por ele
// =========================================================================
void novoGrafo(Grafo *grafo, 
               vector<int> sucessores, 
               int valorFluxo){

  // Pega o indice do ralo
  int indiceAtual = sucessores.size()-1;

  // Enquanto não chegou no indice do fonte faça
  while(indiceAtual>0){

    // Se a quantidade de repeticoes da atracao atual != -1
    // entao ele é uma atracao real e não a apresentacao ou a loja
    if(grafo->nos[indiceAtual].repeticoes != -1){

      // Diminui a quantiade de repeticoes disponíveis para se ir
      // nessa apresentacao
      grafo->nos[indiceAtual].repeticoes -= valorFluxo;

      // Se a quantidade de repeticoes do vertice atual ficar igual a
      // zero faz ficar com um valor simbolico, de modo que ele não
      // será mais utilizado para formar caminhos
      if(grafo->nos[indiceAtual].repeticoes == 0){
        grafo->nos[indiceAtual].repeticoes = -100;
      }
    }

    // Pegar o indice do pai do atual
    indiceAtual = sucessores[indiceAtual];    
  }

  return; 
}



// =========================================================================
// Função: fluxoMaximo
// 
// Pega o maior fluxo possivel entre a primeira atracao e a ultima atracao
// =========================================================================
int fluxoMaximo(Grafo *grafo, int numAtracoes){
  
  vector<int> sucessores;
  int valorFluxo;
  int maxFluxo = 0;
  int fonte = 0, ralo = numAtracoes-1;
  
  // Pega o primeiro caminho
  sucessores = BFS(grafo, fonte, ralo);

  // Enquanto houver caminho entre fonte e ralo e o valor fo Fluxo
  // não for negativo
  do{

    // Achar a menor capacidade das arestas que compoe o caminho, vai
    // ser o valor fo fluxo dessa iteração
    valorFluxo = fluxo(grafo, sucessores);
    
    // Atualiza o grafo com relação a esse novo fluxo
    novoGrafo(grafo, sucessores, valorFluxo);
  
    // Aumenta a quantidade de fluxo passado
    maxFluxo += valorFluxo;

    // Escolhe outro caminho para percorrer no novo grafo
    sucessores = BFS(grafo, fonte, ralo);
  
  }while((temCaminho(sucessores) == true));

  return maxFluxo;
}

// =========================================================================
// Função: criarGrafo
// 
// =========================================================================
Grafo* criarGrafo(vector<atracao> atracoes, int numAtracoes){
  Grafo* grafo;

  // Cria um novo grafo
  grafo = new Grafo();

  // Faz com que ele tenha o tamanho do numero das atracoes
  grafo->nos.resize(numAtracoes);

  // Para cada atracao
  for(int i=0; i < numAtracoes; i++){

    // Colocar a quantidade de repeticoes para essa atracao
    grafo->nos[i].repeticoes = atracoes[i].repeticoes;

    // Cria um vetor de atracoes adjacentes
    grafo->nos[i].adjacentes.resize(0);

    // Para cada atracado possivel
    for(int j=0; j < numAtracoes; j++){

      // Verifica se ela nao esta na lista de restricoes
      if(elemento(atracoes[j].indexNaoSeq, i) == 1){

        // Se nao estiver, coloca ela no vetor de adjacentes a esta atracao
        grafo->nos[i].adjacentes.push_back(j);
      }
    }
  }

  return grafo;
}

// =========================================================================
// Função Principal
// =========================================================================
int solve(int numAtracoes, vector<atracao> atracoes){
  int numeroVisitas = 0;  
  Grafo *grafo;

  // Cria um grafo a partir das restricoes das atracoes
  grafo = criarGrafo(atracoes, numAtracoes);
  
  // Fazer o algoritmo de fluxo maximo no grafo partindo da
  // apresentacao a te a loja
  numeroVisitas = fluxoMaximo(grafo, numAtracoes);
  
  return numeroVisitas;
}
