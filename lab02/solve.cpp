/* 
   Nome: Pedro Hideaki Uiechi Chinen 
   RA: 175828 

   Laboratório 2:
   Encontrar o conjunto de usernames de uma determinada pessoa
   espalhadas em diversos sites. Esses usernames serão ligados pelos
   emails. Caso um username seja comparilhado por um email, é provável
   que seja a mesma pessoa
*/


/*
 * Include
 */
#include "solve.hpp"
#include <cstdio>


/*
 * Structs & Constants
 */
struct No{
  int id;
  int tipo;
};


/* 
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
*/

/*
 * Code
 */
void getUsernames(int n,
		  vector< vector<int> > &userEmail,
		  vector< vector<int> > &emailUser,
		  vector< int > &visitados,
		  No * nodeRoot,
		  vector<int> &usernames){
  /* 
     Variaveis:
     n         = Numero de usuarios
     userEmail = Lista de Listas, relacionando usuário os emails.
     emailUser = Lista de Listas, relacionando emails aos usuários.
     visitados = Lista verificando se vertice ja foi visitado.
     nodeRoot  = id de onde começar a procurar e seu tipo
     usernames = Lista dos usernames relacionados ao id.

     
     Objetivo: 
     Dado os relacionamentos entre usuario e emails, e um id inicial
     retornar todos os usuarios que são conectados ao id inicial.
  */
  unsigned int i;
  int id, tipo, idVisitado;

  id = nodeRoot->id;
  tipo = nodeRoot->tipo; 

  /* Node Root é um usuario, então vai colocar emails */
  if (tipo == 0){

    /* Usuario ja foi visitado retorna sem fazer nada*/
    if (visitados[id]){
      return;
    }
      
    /* Mostra que usuario foi visitado e que faz parte deste
       componente */
    visitados[id] = true;
    usernames[id] = true;
 
    /* Para todos os emails adjacêntes a este usuario, fazer um
       depth-first-search */
    for(i = 0; i < userEmail[id].size(); i++){

      /* No é um email*/
      No * node = new No(); 
      node->id = userEmail[id][i];
      node->tipo = 1;

      /* Desloca o ID do email para ficar na posição correta */
      idVisitado = node->id + n;

      /* Verifica se o email não foi visitado: fazer o
	 depth-first-search nele */
      if(!visitados[idVisitado]){
	getUsernames(n, userEmail, emailUser, 
		     visitados, node, usernames);
      }
    }
  }

  /* Node Root é um email, então vai colocar usuarios */
  if (tipo == 1){

    /* Desloca o ID do email para ficar na posição correta */
    idVisitado = id + n;

    /* Usuario ja foi visitado: não faz nada */
    if (visitados[idVisitado]){
      return;
    }

    /* Marca que email foi visitado*/
    visitados[idVisitado] = true;

    /* Para cada username adjacente ao email, fazer
       depth-first-search */
    for(i = 0; i < emailUser[id].size(); i++){

      /* No é um usuario*/
      No * node = new No(); 
      node->id = emailUser[id][i];
      node->tipo = 0;

      /* Verifica se o username não foi visitado: fazer o
	 depth-first-search */
      if(!visitados[node->id]){
	getUsernames(n, userEmail, emailUser, 
		     visitados, node, usernames);
      }
    }
  }
}


void solve(int n, int m, vector< vector<int> > &userEmail,
	   vector< vector<int> > &userSolution )
{
  /* 
     Variaveis: n = Número de Usuários.  m = Número de Emails.
     userEmail = Lista de Listas, relacionando usuário os emails.
     userSolution = Lista de Listas, relacionando pessoa aos usuarios.
     
     Objetivo: 
     Dado a lista de listas USEREMAIL retorne um pair
     correspondente aos usuarios de cada componente.
  */

  vector< vector <int> > emailUser;
  unsigned int i, j;
  int idUsuario, idEmail;
  vector< int > visitados;


  /* Cria lista de vertices visitados*/
  visitados.resize(n+m, false);

  /* Criar lista de Adjacencia para os emails */
  emailUser.resize(m);

  /* Cria a lista de adjacência dos emails */
  for(i = 0; i < (unsigned int) n; i++){
    for(j = 0; j < (unsigned int) userEmail[i].size(); j++){
      idEmail = userEmail[i][j];
      idUsuario = i;
      emailUser[idEmail].push_back(idUsuario);
    }
  }

  /* Fazer uma busca em profundidade em todos os componentes
     do grafo para achar os usuarios de cada pessoa*/
  
  for(i = 0; i < (unsigned int) n; i++){

    /* Se usuario não foi visitado: Criar um componente novo */
    if(!visitados[i]){

      /* Lista com os usernames que vão ser usados no componente */
      vector< int > usernamesFromPerson;

      /* Lista vai conter todos os usuarios e apenas dizer quais estão
	 no componente e quais não estão */
      usernamesFromPerson.resize(n, false);
      
      /*Lista com APENAS os usuarios que estão no componte atual
	ordenados*/
      vector< int > sortedUsernames;
      
      No * node = new No();
      node->id = i;
      node->tipo = 0;

      /* Pegar os usuarios conectados a este usuario  */
      getUsernames(n, userEmail, emailUser, 
		   visitados, node, usernamesFromPerson);
      
      /* Percorrer a lista com os usernames que vão ser usados no componente */
      for(unsigned int i = 0; i < usernamesFromPerson.size(); i++){
	
	/* Se o username faz parte do componete  */
	if(usernamesFromPerson[i] == true){
	  
	  /* Adicionar ele na lista com os componentes  */
	  sortedUsernames.push_back(i);
	}
      }
      
      /* Inserir componente no resultado final */
      userSolution.push_back(sortedUsernames);
    }
  }
}

