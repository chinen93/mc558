# Mc558 - Projeto e Análise de Algoritmos II 2s2017

- Professor : (Flávio Keidi Miyazawa)[https://www.ic.unicamp.br/~fkm/]

# Laboratórios

02. Encontrar Usuários em Rede Social Ligados por email.

``` 
   A solução foi pensada para utilizar a ideia de lista de adjacência
   para percorrer os vertices de um grafo.
   Primeiramente foi criado a lista de adjacência dos emails a partir
   da lista de adjacência dos usuarios passada por parametro para a
   funcao principal.
   Com as duas listas de adjacência prontas é feito um
   depth-first-search em todos os componentes do grafo. Para cada
   componente seus vertices referentes aos usernames são separados
   para serem mostrados ao final do programa.
   
   A criação da lista é feita em O(N), sendo N o número de usuários
   presentes no grafo.
   E o depth-first-search é feito em O(N+M), sendo M o número de
   emails presentes no grafo.
   
   A maior complexidade é do depth-first-search, que é linear com o
   tamanho da entrada.
```
03. Segmentar Imagens.
```
  Colocar em uma fila de prioridade os nós adjacentes aos pontos inicias.
  Enquanto a fila não estiver vazia, pegar o pixel com custo mais baixo.
  Se pixel não tiver sido vizitado pelo algoritmo.
    Somar o seu valor na solução final para ser retornada.
    Pegar os pixels adjacentes a ele e coloca na fila de prioridade.  
```
04. Encontrar Distrancia entre Escola-Casa.
```
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
```
05. Encontrar Número Máximo de Caminho entre as Apresentações.
```
  Temos um grafo com uma fonte e um ralo, a apresentação do parque e a
  loja de conveniências respectivamente. 
  
  A quantidade de repetições é o custo de cada aresta. E gostariamos
  de saber qual o fluxo máximo entre a fonte o ralo.
  
  Utilizamos o algoritmo de Edmonds-Karp para encontrar esses
  fluxos. Utilizando busca em largura para encontrar os caminhos entre
  a fonte e o ralo em cada iteração.
```
06. Encontrar K+1 rotas distintas entre dois pontos do grafo.
```
 Minimizar: Soma(Xe*Le) [e -> E]

 Restrições:
   Soma(Xe) [e -> S+(s)]       - Soma(Xe) [e -> S-(s)] = k 
   Soma(Xe) [e -> S+(t)]       - Soma(Xe) [e -> S-(t)] = -k 
   Soma(Xe) [e -> S+(V-{s,t})] - Soma(Xe) [e -> S-(V-{s,t})] = 0
   Soma(Xe) [e -> S-(V-{s,t})] <= 1
   Xe >= 0 [e -> E]
   Xe <= 1 [e -> E]
   
 Para cada nó do vertice, que não seja s nem t, verificar a
 quantidade de fluxo que sai deste vertice. A soma desses fluxos
 tem que ser menor ou igual a 1, a fim de fazer com que todos os
 vertices sejam usados apenas uma vez por cada caminho de s a t.
```
