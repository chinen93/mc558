// =========================================================================
// Informações do Laboratório
// =========================================================================

/* 
   Pedro Hideaki Uiechi Chinen - RA 175828
   
   RELATORIO:
   
   Para cada aluno faça
     Arrume a distancia de todos os vertices para infinito
     Coloque como 0 como a distancia desse aluno
     Insira numa fila de prioridade de minimos essa distancia
     Enquanto nao achou uma escola para este aluno faça
       Retire dessa fila um valor
       Verifique se eh o valor de uma escola
       Percorra os adjacentes a este valor retirado
         Faça relax neste valor
         Se o relax for feito entao
           Coloque o valor na fila de prioridade
     Insira o aluno numa lista de alunos por escola
   Retorne a lista de alunos por escola

*/

// =========================================================================
// Importação de bibliotecas
// =========================================================================
#include "solve.hpp"

// =========================================================================
// Declaração de Estruturas
// =========================================================================
struct Distancia{
  int vertice;
  int distancia;
};

struct DistanciaEscola{
  int indice;
  int vertice;
  int distancia;
};

// =========================================================================
// Função auxiliares para impressão das estruturas
// =========================================================================
void imprimeEntrada(vector< vector< pair<int, int> > > &g, 
                            vector< vector <int> > &e, 
                            vector <int> &s){
    
  cout << "G:\n";
  for(unsigned int i = 0; i < g.size(); i++){
    for(unsigned int j = 0; j < g[i].size(); j++){
      cout << i << "->" << g[i][j].first<< ": " 
           << g[i][j].second << " distancia";
      cout << "\n";
    }
  }
  cout << "\n";

  cout << "E: " << "N escolas: " << e.size()
       <<"\n";
  for(unsigned int i = 0; i < e.size(); i++){
    cout << "Escola " << i << "\n";
    cout << "Ponto onibus: ";
    for(unsigned int j = 0; j < e[i].size(); j++){
      cout << "v" << e[i][j] << " ";
    }
    cout << "\n";    
  }
  cout << "\n";

  cout << "S: N alunos " << s.size()  << "\n";
  for(unsigned int i = 0; i < s.size(); i++){
    cout << "Estudante " << i << " ";
    cout << "Casa em v" << s[i] << "\n";
  }
  cout << "\n";
}

void imprimeHeap(vector <Distancia *> heap){
  cout << "Heap \n";
  for(unsigned int i = 0; i < heap.size(); i++){
    cout << "(";
    cout << heap[i]->vertice << " " << heap[i]->distancia ;
    cout << ")";
  }
  cout << "\n";
}

// =========================================================================
// Função para retorno do indice da escola
// =========================================================================
int indiceEscola(vector< vector <int> > &e, int vertice){
  int escola = INT_MAX;

  // Percorre o vetor de escolas
  for(unsigned int i = 0; i < e.size() && escola == INT_MAX; i++){

    // Percorre o vetor de pontos de onibus da escola
    for(unsigned int j = 0; j < e[i].size() && escola == INT_MAX; j++){

      // Verifica se o vertice faz parte de algum ponto de onibus
      if(vertice == e[i][j]){

        // Se fazer parte, retorna o indice da escola;
        escola = i;
      }
    }
  }

  return escola;
}

// =========================================================================
// Função para atribuição da escola
// =========================================================================
void atribuiEscola(int iEscola,
                   Distancia* vertice,
                   DistanciaEscola* escola){
  // Se a distancia da escola antiga eh maior que a distancia para a nova escola

  if(escola->distancia >= vertice->distancia){ 
    if(escola->indice > iEscola){
      escola->indice = iEscola;
      escola->vertice = vertice->vertice; 
      escola->distancia = vertice->distancia;
    }
  }

  return;
}

// =========================================================================
// Função de comparação para uso no heap
// =========================================================================
int comparator(Distancia * a, Distancia * b){
  if(a->distancia != b->distancia){
    return a->distancia > b->distancia;
  }
  else{
    return a->vertice > b->vertice;
  }
}


// =========================================================================
// Função principal
// =========================================================================
vector< vector<int> > solve(vector< vector< pair<int, int> > > &g, 
                            vector< vector <int> > &e, 
                            vector <int> &s){
 
  // Pega os numeros de elementos de cada vetor
  unsigned int numAlunos = s.size();
  unsigned int numEscolas = e.size();
  unsigned int numVertices = g.size();
  
  // Cria matriz de retorno com tamanho de escolas
  vector< vector <int> > retorno;
  retorno.resize(numEscolas);

  // Criar vetor de distancia
  vector < int > dist; 
  dist.resize(numVertices);

  // Cria o heap para visitar os vertices
  vector < Distancia *> heap;

  // Para cada aluno
  for(unsigned int indiceAluno = 0; indiceAluno < numAlunos; indiceAluno++){

    // Zera o heap
    heap.resize(0);

    // Pega o vertice da casa do aluno
    int verticeAluno = s[indiceAluno];

    // Cria a distancia para da escola ate a casa do aluno como INT_MAX
    DistanciaEscola *escola = new DistanciaEscola;
    escola->indice = INT_MAX;
    escola->vertice = INT_MAX;
    escola->distancia = INT_MAX;

    // Arrruma as distancias de todos os vertices como INT_MAX
    for(unsigned int j = 0; j < numVertices; j ++){
      dist[j] = INT_MAX;
    }

    // Coloca apenas a casa do aluno na lista de prioridade
    Distancia *d = new Distancia;
    d->vertice = verticeAluno;
    d->distancia = 0;
    dist[verticeAluno] = 0;

    // Coloca a casa do aluno no heap
    heap.push_back(d);
    std::push_heap(heap.begin(), heap.end(), comparator);
    
    // Cria variaveis para mostrar que encontrou a escola de menor
    // distancia ate o aluno
    bool continuarProcurando = true;
    int passosAteEscolaMaisPerto = INT_MAX;

    // Enquanto não achou uma escola para este aluno
    while(continuarProcurando == true && !heap.empty()){
      
      // pegar o vertice com a menor distancia do heap
      Distancia *vert = heap.front();
      std::pop_heap(heap.begin(), heap.end(), comparator);
      heap.pop_back();

      // Se vertice é uma escola: atribui escola ao aluno
      int iEscola = indiceEscola(e, vert->vertice);
      if (iEscola != INT_MAX){
        atribuiEscola(iEscola, vert, escola);

        // Se passos ate a escola mais perto for maior que distancia
        // ate a escola mais perto: atualiza essa contagem de passos
        if(passosAteEscolaMaisPerto > escola->distancia){
          passosAteEscolaMaisPerto = escola->distancia;
        }
      }

      // Se a distancia ate o vertice atual eh maior que a distancia
      // ate a escola mais perto, nao precisa mais procurar por que ja
      // encontrou a escola mais perto
      if(vert->distancia > passosAteEscolaMaisPerto){
        continuarProcurando = false;
      }
      
      int k = vert->vertice;
      int distVert = vert->distancia;

      // Para todos os vertices adjacentes ao vertice que foi retirado do heap
      for(unsigned int j = 0; j < g[k].size(); j++){

        int verticeEncontrado = g[k][j].first;
        int distEncontrado = g[k][j].second;

        // Se distancia ate o vertice eh maior do que a distancia
        // encontrada
        if(dist[verticeEncontrado] > distVert+distEncontrado){

          // Atualiza a distancia ate o vertice
          dist[verticeEncontrado] = distVert+distEncontrado;
          
          // Cria uma nova distancia
          Distancia *novaDistancia = new Distancia();
          novaDistancia->vertice = verticeEncontrado;
          novaDistancia->distancia = dist[verticeEncontrado];

          // Coloca distancia no heap
          heap.push_back(novaDistancia);
          std::push_heap(heap.begin(), heap.end(), comparator);
        }
      }
    }
   
    // Insere o vertice da casa do aluno no vetor de aluno-por-escola
    retorno[escola->indice].push_back(verticeAluno);
  }


  // Ordenar as casas dos alunos
  for(unsigned int i = 0; i < numEscolas; i++){
    std::sort(retorno[i].begin(), retorno[i].end());
  }

  return retorno;
}


