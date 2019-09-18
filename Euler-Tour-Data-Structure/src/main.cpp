/* This file contains the driver fuction main
 * and all it's dependancies.
 */

//  Standard Template Library
#include <iostream>
#include <array>
// LEDA
#include "LEDA/graph/graph.h"
#include "LEDA/graph/ugraph.h"
#include "LEDA/graph/node_array.h"
#include "LEDA/graph/graph_alg.h"
#include "LEDA/core/dynamic_trees.h"
#include "LEDA/graph/graph_misc.h"
// Custom
#include "ETTree.h"
//  Forward Declarations
bool consoleLog{true};
bool tc{false};
bool tcLog{false};
void displayMsg(std::string msg, std::string stateStr="status");
int writeGraphDOT(const leda::graph &G,
  const std::string& filename); // Stores a given leda::graph object, DOT format

// #############################################################################################################
// TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN
// #############################################################################################################
void test_leda_dynamic_trees();
leda::node test_leda_graph(leda::graph& G);
// #############################################################################################################
// TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN
// #############################################################################################################

// Driver
int main()
{
// #############################################################################################################
// TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN
// #############################################################################################################
  if(tc) {
    leda::graph G;
    leda::node s{test_leda_graph(G)};
    leda::node_array<int> dist{G, -1};

    writeGraphDOT(G, "dot-graph-files/random-graph.dot");
    leda::BFS(G, s, dist);
    leda::node v;
    if(tcLog) {
      if(G.is_directed()) std::cout << "Graph G is directed\n";
      else std::cout << "Graph G is undirected\n";
      if(Is_Connected(G)) std::cout << "Graph G is also connected\n";
      if(Is_Biconnected(G)) std::cout << "Graph G is also biconnected\n";
      forall_nodes(v, G) {
        std::cout << "Node: ";
        G.print_node(v);
        std::cout << ", dist: " << dist[v] << '\n';
      }
    }

    G.make_undirected();

    writeGraphDOT(G, "dot-graph-files/random-ugraph.dot");
    leda::node_array<int> dist2{G, -1};
    leda::BFS(G, s, dist2);
    if(tcLog) {
      if(G.is_directed()) std::cout << "Graph G is directed\n";
      else std::cout << "Graph G is undirected\n";
      // if(Is_Connected(G)) std::cout << "Graph G is also connected\n";
      // if(Is_Biconnected(G)) std::cout << "Graph G is also biconnected\n";
      forall_nodes(v, G) {
        std::cout << "Node: ";
        G.print_node(v);
        std::cout << ", dist: " << dist2[v] << '\n';
      }
    }
  }

  leda::graph Gb;
  leda::node s{test_leda_graph(Gb)};
  Gb.make_bidirected();

  writeGraphDOT(Gb, "dot-graph-files/random-bigraph.dot");
  ETTree T{Gb, s};
  T.getVisits();

  if(tcLog) {
    leda::node_array<int> dist3{Gb, -1};
    leda::BFS(Gb, s, dist3);
    if(Gb.is_directed()) std::cout << "Graph G is directed\n";
    else std::cout << "Graph G is undirected\n";
    if(Is_Connected(Gb)) std::cout << "Graph G is also connected\n";
    if(Is_Biconnected(Gb)) std::cout << "Graph G is also biconnected\n";
    leda::node v;
    forall_nodes(v, Gb) {
      std::cout << "Node: ";
      Gb.print_node(v);
      std::cout << ", dist: " << dist3[v] << '\n';
    }
  }

// #############################################################################################################
// TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN
// #############################################################################################################

  return 0;
}

int writeGraphDOT(const leda::graph &G, const std::string& filename)
{
  // Open file
  std::ofstream ofs;
  ofs.open(filename.c_str());
  // Set graph type
  std::string graphType{"graph G {\nnode[shape = circle];"};
  std::string graphEdgeType{" -- "};
  if(G.is_directed()) {
    graphType="digraph G {\nnode[shape = circle];";
    graphEdgeType=" -> ";
  }

  ofs << graphType << '\n';
  // Insert graph information
  int i = 0;
  leda::edge e;
  leda::node v;
  leda::node_array<int> A{G};
  forall_nodes(v, G) {
    A[v] = i++;
    // Node Coloring
    std::string colorFill{", fillcolor = red, style = filled "};
    // if(A[v]==1)
    //   colorFill = ", fillcolor = brown, style = filled ";
    // else if(A[v]%2==0)
    //   colorFill = ", fillcolor = red, style = filled ";
    // else if(A[v]%3==0)
    //   colorFill = ", fillcolor = blue, style = filled ";
    // else if(A[v]==4)
    //   colorFill = ", fillcolor = pink, style = filled ";
    // Nodes & Node Labels
    ofs << A[v] << " [ xlabel = \""
    << "node label" << "\""
    << colorFill << "];" << '\n';
  }
  // Edges & Edge Labels
  forall_edges(e, G) {
    ofs << A[G.source(e)] << graphEdgeType << A[G.target(e)]
    << " [ label = \"" << "edge label" << "\" ];" << '\n';
  }
	ofs << "}" << '\n';
	ofs.close();
}

void displayMsg(std::string msg, std::string stateStr) {
  if(consoleLog) {
    if(stateStr=="status") stateStr="status: ";
    else stateStr="error: ";
    std::cout << stateStr << msg << '\n';
  }
}

// #############################################################################################################
// TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN
// #############################################################################################################
void test_leda_dynamic_trees() {
  leda::dynamic_trees D;

  //generate 6 trees containing a single vertex
  leda::vertex v1=D.make();leda::vertex v2=D.make();leda::vertex v3=D.make();
  leda::vertex v4=D.make();leda::vertex v5=D.make();leda::vertex v6=D.make();

  D.link(v1,v2,1); //combine the trees v1 and v2:
                   //       v2
                   //     1/
                   //    v1

  D.link(v3,v4,2); //combine the trees v3 and v4:
                   //       v4
                   //     2/
                   //    v3

  D.link(v4,v2,3); //combine the trees of v4 and v2:
                   //     v2
                   //   1/ \3
                   //  v1   v4
                   //        \2
                   //         v3

  D.link(v6,v2,4); //combine the trees of v6 and v2:
                   //      v2
                   //   1/ |3\4
                   //  v1  v4 v6
                   //      |2
                   //      v3

  D.cut(v4);       //cut subtree with root v4:
                   //     v2
                   //   1/  \4
                   //  v1    v6       v4
                   //                 |2
                   //                 v3

  D.link(v4,v6,1); //combine the trees of v4 and v6:
                   //       v2
                   //     1/  \4
                   //    v1     v6
                   //           |1
                   //           v4
                   //           |2
                   //           v3

  //compute functions on D
  std::cout << "Cost of edge (v3,v4) is " << D.cost(v3) << std::endl;
  std::cout << "mincost(v3)=" << D.cost(D.mincost(v3)) << std::endl;
}

leda::node test_leda_graph(leda::graph& G) {
  const int nodeNum{10};
  std::array<leda::node, nodeNum> V;
  for(int i=0; i<nodeNum; i++) V[i]=G.new_node();
  G.new_edge(V[1], V[0]);
  G.new_edge(V[2], V[1]);
  G.new_edge(V[3], V[0]);
  G.new_edge(V[3], V[2]);
  G.new_edge(V[3], V[4]);
  G.new_edge(V[3], V[5]);
  G.new_edge(V[3], V[6]);
  G.new_edge(V[4], V[7]);
  G.new_edge(V[4], V[8]);
  G.new_edge(V[6], V[9]);
  G.new_edge(V[9], V[9]);
  return V[3];
}
// #############################################################################################################
// TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN
// #############################################################################################################
