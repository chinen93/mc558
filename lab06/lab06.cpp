
#include "gurobi_c++.h"
#include <vector>
#include <iostream>
#include <unordered_map>

using namespace std;

/*
 * Pedro Hideaki Uiechi Chinen 175828
 * 
 * Minimizar: Soma(Xe*Le) [e -> E]
 *
 * Restrições:
 *   Soma(Xe) [e -> S+(s)]       - Soma(Xe) [e -> S-(s)] = k 
 *   Soma(Xe) [e -> S+(t)]       - Soma(Xe) [e -> S-(t)] = -k 
 *   Soma(Xe) [e -> S+(V-{s,t})] - Soma(Xe) [e -> S-(V-{s,t})] = 0
 *   Soma(Xe) [e -> S-(V-{s,t})] <= 1
 *   Xe >= 0 [e -> E]
 *   Xe <= 1 [e -> E]
 *
 * Para cada nó do vertice, que não seja s nem t, verificar a
 * quantidade de fluxo que sai deste vertice. A soma desses fluxos
 * tem que ser menor ou igual a 1, a fim de fazer com que todos os
 * vertices sejam usados apenas uma vez por cada caminho de s a t.
*/

int main(int   argc, char *argv[]) {

  try {

    GRBEnv env = GRBEnv();
    GRBModel model = GRBModel(env); // Criando Modelo

    int nodes,arcs;
    int source,target,numMaxLossNodes;

    cin >> nodes >> arcs; // Numero de nos e de arcos
    cin >> source >> target >> numMaxLossNodes; // indice da origem e do destino

    numMaxLossNodes++; // aumenta o numero maximo de nos que podem ser perdidos em 1.

    vector< unordered_map < int, GRBVar > > graph(nodes); // estrutura para representar o grafo, (i -> j, graph[i][j] )
    
    // Lendo Grafo        
    char varname[10];
    for(int i = 0 ; i < arcs; i ++){
	    int arcOrig, arcDest;
	    int arcCost;
	    cin >> arcOrig >> arcDest >> arcCost;

            sprintf(varname, "X_[%d,%d]",arcOrig ,arcDest); // Nome da variavel do arco.
	    
	    /*
	     *  Adicionando arco no grafo, 
	     *  valor minimo = 0.0,
	     *  maximo = 1.0,
	     *  valor para a funcao objetivo = arcCost,
	     *  GRB_CONTINUOUS, indica que é uma variavel real (uma alternativa, por exemplo, seria definir como inteira)
	     *  varname é o nome que vamos dar a essa variavel (formato "X_[%d,%d]").
	     *  
	     */
            graph[arcOrig][arcDest] = model.addVar(0.0, 1.0, arcCost ,GRB_CONTINUOUS, varname);
     }
    

   /* Funcao Objetivo
    * Somatorio das variaveis de cada arco, multiplicado pelo seu custo respectivo.
    *
    * \Sum_i^V \Sum _j^V (X_ij * w_ij)
    *
    * Como todas as variaveis criadas no modelo fazem parte da minimização, o Gurobi tem uma forma simplificada de escrita da
    * função objetiva, vamos definir somente que as variaveis do modelo compoem nossa função objetivo e que é um problema de 
    * minimização = GRB_MINIMIZE.
    *
    */
      model.set(GRB_IntAttr_ModelSense, GRB_MINIMIZE);


    /* Restricao origem.
     * O somatorio das variaveis que representam os arcos que saem da origem deve ser igual a 1.
     *
     * \Sum_sj \in A (X_sj) >= 1
     */
          
     GRBLinExpr r1 = GRBLinExpr(); // Expressao linear
     for(auto j:graph[source]){ // todas as variavel na "linha" do vertice de origem.
         r1 += j.second; // somando as variaveis
     }
     model.addConstr(r1 >= numMaxLossNodes, "R1"); //A soma das variaveis deve ser maior ou igual a 1.

 
    /* Restricao Destino    
     * Somatorio das variaveis que representam os arcos que chegam no destinos devem ser igual a 1.
     *
     * \Sum_it \in A (X_it) >= 1
     *
     */

     GRBLinExpr r2 = GRBLinExpr();
     for(int v = 0 ; v < graph.size(); v++){ // Para cada vertice do grafo
	   if(graph[v].find(target) != graph[v].end()){ // verifica se ele tem conexao com o destino
         	r2 += graph[v][target]; // a soma dessas variaveis
	   }
     }
    model.addConstr( r2 >= numMaxLossNodes , "R2"); // a soma dessas variaveis deve ser maior ou igual a 1.

    /* Restricao de manutencao de fluxo
     * Para cada vertice que nao seja a origem ou o destino, a soma dos arcos que entram no vertice devem ser igual aos que saem.
     *
     * \Sum_iv (x_iv) - \Sum_vj (x_vj) = 0 , \forall v \in V
     *
     *
     */ 

    for(int v = 0 ; v < graph.size(); v ++){ // para cada vertice

	if(v == source || v == target) // que nao seja a origem ou o destino
	   continue;

        GRBLinExpr in_v = GRBLinExpr(); // expressao linear dos vertices que entram em v
        GRBLinExpr out_v = GRBLinExpr(); // expressao linear dos vertices que saem de v

	for(auto j: graph[v]){ // os que saem de v
		out_v += j.second;
	}

	for(auto i: graph){ // para cada vertice
		if(i.find(v) != i.end()) // se ele entra em v, some
			in_v += i[v];
	}

      model.addConstr( out_v - in_v == 0 , "c2"); // soma dos que saem, menos os que entram igual a 0
     } 


    /* ==================================================================================================
     * Restricao da quantidade de fluxo passando por um vertice
     *   Soma(Xe) [e -> S-(V-{s,t})] <= 1
     */
    for(int v = 0 ; v < graph.size(); v ++){ // para cada vertice

	if(v == source || v == target) // que nao seja a origem ou o destino
	   continue;

        GRBLinExpr out_v = GRBLinExpr(); // expressao linear dos vertices que saem de v

	for(auto j: graph[v]){ // os que saem de v
		out_v += j.second;
	}

	model.addConstr( out_v <= 1 , "c2"); // soma dos que saem, menos os que entram igual a 0
     } 

    /* ================================================================================================== 
     */

    model.update(); // atualiza o modelo

    // resolve o modelo.
    model.optimize();

    //A presenta o valor das variaveis
    for(auto v:graph){
	for(auto i:v){
		cout << (i.second).get(GRB_StringAttr_VarName) << " " << (i.second).get(GRB_DoubleAttr_X) << endl;
	}
    }
    
    //Valor final
    cout << "Latencia Total: " << model.get(GRB_DoubleAttr_ObjVal) << endl;

  } catch(GRBException e) {
    cout << "Error code = " << e.getErrorCode() << endl;
    cout << e.getMessage() << endl;
  } catch(...) {
    cout << "Exception during optimization" << endl;
  }

  return 0;
}

