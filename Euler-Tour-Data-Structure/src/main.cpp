/* This file contains the
 * driver fuction main
 * and all it's dependancies.
 */

 //  Standard Libraries
#include <iostream>
// #include <string>
// #include <fstream>
// #include <random>
// #include <ctime>
// #include <cstdlib>
// #include <chrono>
// #include <list>
// #include <iterator>
// #include <limits>
// #include <cmath>
// Other Libraries
// LEDA
#include "LEDA/graph/graph.h"
#include "LEDA/graph/ugraph.h"
#include "LEDA/graph/node_array.h"
#include "LEDA/graph/graph_alg.h"
#include "LEDA/core/list.h"
#include "LEDA/core/dynamic_trees.h"
// Custom

//  Type Definitions

//  Forward Declarations
int writeGraphDOT(const leda::graph &G, const std::string& filename); // Stores a given leda::graph object, DOT format

// Driver
int main()
{
  leda::graph G;
  random_graph(G, 19, 20);
  leda::edge e;
  forall_edges(e, G) {
    G.print_edge(e);
    std::cout << '\n';
  }
  writeGraphDOT(G, "dot-graph-files/random-graph.dot");

// test start ##################################################################
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
// test end ####################################################################
  return 0;
}

int writeGraphDOT(const leda::graph &G, const std::string& filename)
{
	std::string graphType{"digraph G {\nnode[shape = circle];"};
	std::string graphEdgeType{" -> "};
	std::ofstream ofs;
	ofs.open(filename.c_str());
  ofs << graphType << '\n';

  int i = 0;
  leda::edge e;
  leda::node v;
  leda::node_array<int> A(G);
  forall_nodes(v, G) {
    A[v] = i++;

    std::string colorFill{", fillcolor = green, style = filled "};
    if(A[v]==1)
      colorFill = ", fillcolor = brown, style = filled ";
    else if(A[v]%2==0)
      colorFill = ", fillcolor = red, style = filled ";
    else if(A[v]%3==0)
      colorFill = ", fillcolor = blue, style = filled ";
    else if(A[v]==4)
      colorFill = ", fillcolor = pink, style = filled ";

    ofs << A[v] << " [ xlabel = \""
    << "node label" << "\""
    << colorFill << "];" << '\n';
  }
  forall_edges(e, G) {
    ofs << A[G.source(e)] << graphEdgeType << A[G.target(e)]
    << " [ label = \"" << "edge label" << "\" ];" << '\n';
  }
	ofs << "}" << '\n';
	ofs.close();
}
