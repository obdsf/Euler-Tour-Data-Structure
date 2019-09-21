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
// #include "LEDA/graph/graph_alg.h"
// #include "LEDA/graph/graph_misc.h"
#include "LEDA/core/array.h"
#include "LEDA/core/dynamic_trees.h"
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
leda::node test_leda_graph(leda::graph& G, leda::node &a, leda::node &b);
// #############################################################################################################
// TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN
// #############################################################################################################

// Driver
int main()
{
// #############################################################################################################
// TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN
// #############################################################################################################

  leda::graph G;
  leda::node a, b;
  leda::node s{test_leda_graph(G, a, b)}; // ET starting node.
  G.make_bidirected(); // Graph G needs to be of type graph and bidirected.

  writeGraphDOT(G, "dot-graph-files/example-bigraph.dot"); // Writes graph to file
  ETTree T{G, s}; // Creates Euler Tour Tree of graph G
  // G.clear();
  T.print_euler_tour(); // Prints the ET sequence of nodes (order in which they were visited)
  T.print_visits(); // Prints the table of the total visits of each node
  T.delete_edge(a, b);

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
  std::string graphType{"digraph G {\nnode[shape = circle];"};
  std::string graphEdgeType{" -> "};
  if(G.is_undirected()) {
    graphType="graph G {\nnode[shape = circle];";
    graphEdgeType=" -- ";
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
    std::string colorFill{", fillcolor = white, style = filled "};
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
    << "          " << "\""
    << colorFill << "];" << '\n';
  }
  // Edges & Edge Labels
  forall_edges(e, G) {
    ofs << A[G.source(e)] << graphEdgeType << A[G.target(e)]
    << " [ label = \"" << "          " << "\" ];" << '\n';
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

leda::node test_leda_graph(leda::graph& G, leda::node &a, leda::node &b) {
  const int nodeNum{17};
  std::array<leda::node, nodeNum> V;
  for(int i=0; i<nodeNum; i++) V[i]=G.new_node();
  a=V[6];
  b=V[10];
  G.new_edge(V[0], V[1]);
  G.new_edge(V[0], V[2]);
  G.new_edge(V[0], V[3]);
  G.new_edge(V[1], V[4]);
  G.new_edge(V[1], V[5]);
  G.new_edge(V[2], V[6]);
  G.new_edge(V[3], V[7]);
  G.new_edge(V[3], V[8]);
  G.new_edge(V[6], V[9]);
  G.new_edge(V[6], V[10]);
  G.new_edge(V[9], V[11]);
  G.new_edge(V[10], V[12]);
  G.new_edge(V[10], V[13]);
  G.new_edge(V[12], V[14]);
  G.new_edge(V[12], V[15]);
  G.new_edge(V[14], V[16]);
  return V[0];
}
// #############################################################################################################
// TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN
// #############################################################################################################
