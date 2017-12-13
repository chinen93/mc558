/*
  Pedro Hideaki Uiechi Chinen, RA 175828
  
  
  Complexidade do Algoritmo: 

  pegarVizinhos: Para 4 direções ele verifica se pode inserir essa
  direção na fila de prioridade. portanto ele faz em
  O(4*N*log(N)). Sendo N o tamanho da fila de prioridade

  cost: É feito em tempo linear O(N).

  mst_image: Percorre todos os elementos da matriz e verifica seus
  vizinhos, Executando em O(N*M) sendo N o número de linhas e M o
  número de colunas da matriz.

  Explicação do Algotirmo:

  Colocar em uma fila de prioridade os nós adjacentes aos pontos inicias.
  Enquanto a fila não estiver vazia, pegar o pixel com custo mais baixo.
  Se pixel não tiver sido vizitado pelo algoritmo.
    Somar o seu valor na solução final para ser retornada.
    Pegar os pixels adjacentes a ele e coloca na fila de prioridade.  
*/

#include "solve.hpp"
#include <algorithm>

// (x,y) posição do pai deste pixel
// (i,j) posição do pixel
// custo mínimo para chegar a este pixel partindo de um ponto de origem.
struct Pixel{
  int x ,y;
  int i, j;
  int custo;
};

// Imprime informações do pixel
void imprimePixel(Pixel pixel){
  cout << "(" << pixel.x << " " << pixel.y << ")";
  cout << "(" << pixel.i << " " << pixel.j << ")";
  cout << "(" << pixel.custo << ")\n";
}

// Imprime custos que estão no heap
void imprimeHeap(vector< Pixel*> heap){
  cout << "(";
  for(unsigned int i =0; i <heap.size(); i++){
    cout << heap[i]->custo << " |";
  }

  cout << ")\n";
}

// Imprime endereços dos pixels vizitados
void imprimePixels(vector< vector< Pixel*> > pixels){
  cout << "{\n";
    for(unsigned i = 0; i < pixels.size(); i++){
      cout << "\t(";
      for(unsigned j = 0; j < pixels[i].size(); j++){
	if(pixels[i][j] != NULL){
	  cout << pixels[i][j] << " |";
	}else{
	  cout << pixels[i][j] << "        |";
	}
      }
      cout << ")\n";
    }

  cout << "}\n";
}

// Comparação entre pixels, para uso no heap
bool compare(Pixel* p1, Pixel* p2){
  return p1->custo >= p2->custo;
}

// Percorre os vizinhos do pixelAtual e os coloca no heap
void pegarVizinhos(vector< vector<int> > &image,
		   pair<int, int> pixelAtual,
		   vector<Pixel *> &heap){
  pair <int, int> pixelCima, pixelEsquerda, pixelBaixo, pixelDireita;
  int coluna, linha;
  int custo;
  Pixel *aux;

  coluna = pixelAtual.first;
  linha = pixelAtual.second;

  // Cima
  if((linha - 1 >= 0) && (image[coluna][linha - 1] != -1)){
    pixelCima     = std::make_pair(coluna    , linha - 1);
    custo = cost(image, pixelAtual, pixelCima);

    aux = new Pixel();
    aux->x = coluna;
    aux->y = linha;    
    aux->i = pixelCima.first;
    aux->j = pixelCima.second;
    aux->custo = custo;

    heap.push_back(aux);
    std::push_heap(heap.begin(), heap.end(), compare);
  }

  // Esquerda
  if((coluna - 1 >= 0) && (image[coluna - 1][linha] != -1)){
    pixelEsquerda = std::make_pair(coluna - 1, linha);
    custo = cost(image, pixelAtual, pixelEsquerda);

    aux = new Pixel();
    aux->x = coluna;
    aux->y = linha;    
    aux->i = pixelEsquerda.first;
    aux->j = pixelEsquerda.second;
    aux->custo = custo;

    heap.push_back(aux);
    std::push_heap(heap.begin(), heap.end(), compare);
  }

  // Baixo
  if((linha + 1 < (int) image[0].size()) && (image[coluna][linha + 1] != -1)){

    pixelBaixo    = std::make_pair(coluna    , linha + 1);
    custo = cost(image, pixelAtual, pixelBaixo);

    aux = new Pixel();
    aux->x = coluna;
    aux->y = linha;    
    aux->i = pixelBaixo.first;
    aux->j = pixelBaixo.second;
    aux->custo = custo;

    heap.push_back(aux);
    std::push_heap(heap.begin(), heap.end(), compare);

  }

  // Direita
  if((coluna + 1 < (int) image.size()) && (image[coluna + 1][linha] != -1)){
    pixelDireita  = std::make_pair(coluna + 1, linha);
    custo = cost(image, pixelAtual, pixelDireita);

   aux = new Pixel();
   aux->x = coluna;
   aux->y = linha;    
   aux->i = pixelDireita.first;
   aux->j = pixelDireita.second;
   aux->custo = custo;
   
   heap.push_back(aux);
   std::push_heap(heap.begin(), heap.end(), compare);
  }

  // Marcar que ja passou pelo elemento atual
  image[coluna][linha] = -1;  
}


// Função que calcula o custo entre dois pontos da imagem.
int cost(vector< vector<int> > &image, 
	 pair<int, int> a, 
	 pair<int, int> b){
    
    //Se os pontos não forem vizinhos, retorne INT_MAX
    if( abs(a.first - b.first) + abs(a.second - b.second) > 1)
        return INT_MAX;
    //Se forem pontos vizinhos, retorne o módulo da subtração dos pontos
    return abs(image[a.first][a.second] - image[b.first][b.second]);
    
}

//Retorne um long long int correspondente ao custo total dos componentes.
long long int mst_image(vector< vector<int> > &image, 
			vector< pair<int, int> > &points){
    long long int solution = 0;
    
    vector<Pixel *> heap;
    Pixel * proximoPixel;
    pair<int, int> posicao;

    // Coloca os vizinhos dos pixels iniciais no heap
    for(unsigned int i = 0; i < points.size(); i++){
      pegarVizinhos(image, points[i], heap);
    }


    // Enquanto o heap não estiver vazio: procurar vizinhos 
    while(heap.size() > 0){
      std::pop_heap(heap.begin(), heap.end(), compare);
      proximoPixel = heap.back();
      heap.pop_back();
      
      posicao = std::make_pair(proximoPixel->i, proximoPixel->j);

      if(image[proximoPixel->i][proximoPixel->j] != -1){
    	solution += proximoPixel->custo;

	pegarVizinhos(image, posicao, heap);
      }
    }

    return solution;
}
