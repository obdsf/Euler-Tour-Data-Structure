/* This file contains the driver fuction main
 * and all it's dependancies.
 */

//  Standard Template Library
#include <iostream>
#include <array>
#include <chrono>
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
bool exampleEnabled{false};
bool testEnabled{false};
void parseInput(int &argc, char** &argv);
int writeGraphDOT(const leda::graph &G,
                  const std::string& filename); // Stores a given leda::graph object, DOT format
void displayMsg(std::string msg, std::string stateStr="status"); // Prints status or error message
void euler_tour_tree_example();
void euler_tour_tree_test();
leda::vertex test_leda_dynamic_trees(leda::dynamic_trees& D, leda::vertex &a, leda::vertex &b, leda::vertex &c, leda::vertex &d);
leda::node test_leda_graph1(leda::graph& G, leda::node &a, leda::node &b, leda::node &c, leda::node &d);

// Driver
int main(int argc, char** argv)
{
  parseInput(argc, argv); // Determines behavior according to user input

  if(exampleEnabled) euler_tour_tree_example();
  if(testEnabled) euler_tour_tree_test();

  return 0;
}

void parseInput(int &argc, char** &argv) {
  for(int i = 1; i < argc; i++)
  {
    std::string str = argv[i];
    if(str == "-e") exampleEnabled=true;
    else if(str == "-t") testEnabled=true;
  }
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

void euler_tour_tree_example() {
  leda::graph G1;
  leda::node a, b, c, d;
  leda::node s1{test_leda_graph1(G1, a, b, c, d)}; // ET starting node.
  G1.make_bidirected(); // Graph G needs to be of type graph and bidirected.
  writeGraphDOT(G1, "dot-graph-files/example-graph.dot"); // Writes graph to file
  ETTree T{G1, s1}; // Creates Euler Tour Tree of graph G
  ETTree T1{G1, s1}, T2{G1, s1}, T3{G1, s1};
  std::cout << "\nT: Euler Tour Visits:\n";
  T.print_visits(); // Prints the table of the total visits of each node
  std::cout << "\nT: Euler Tour:\n";
  T.print_euler_tour(); // Prints the ET sequence of nodes (order in which they were visited)
  std::cout << "\nT: Change root 0 to 1\n";
  T.change_root(s1, c); // Change root 0 to 1
  std::cout << "\nT: Euler Tour:\n";
  T.print_euler_tour(); // Prints the ET sequence of nodes (order in which they were visited)
  std::cout << "\nT: Change root 1 to 0\n";
  T.change_root(c, s1); // Change root 1 to 0
  std::cout << "\nT: Euler Tour:\n";
  T.print_euler_tour(); // Prints the ET sequence of nodes (order in which they were visited)
  std::cout << "\nT: Delete 6-10\n";
  T.delete_edge(a, b); // Edge 6-10 or 10-6
  std::cout << "\nT: Euler Tour:\n";
  T.print_euler_tour();
  std::cout << "\nT: Delete 0-1\n";
  leda::list<leda::node*> L{T.delete_edge(c, s1)}; // Edge 0-1 or 1-0
  std::cout << "\nT: Euler Tour:\n";
  T.print_euler_tour();
  std::cout << "\nTesting fail-safes:\n";
  leda::node kappa; // Testing fail-safes
  if(T.delete_edge(s1, kappa).empty()) displayMsg("cannot delete given edge, edge does not exist on tree", "error");
  if(T.change_root(s1, kappa)) displayMsg("cannot change root, given node/s does/do not exist on tree", "error");
  if(T.associate(T3)) displayMsg("Trees are already associated", "error");
  if(T.associate(T1)) displayMsg("Trees are already associated", "error");
  if(T.associate(T1)) displayMsg("Trees are already associated", "error");
  if(T.disassociate(T2)) displayMsg("Trees are not associated", "error");
  if(T.associate(T2)) displayMsg("Trees are already associated", "error");
  std::cout << "\nOther:\n";
  std::cout << "Tree T has id: " << T.tree_id() << '\n'
            << "Tree T1 has id: " << T1.tree_id() << '\n'
            << "Tree T2 has id: " << T2.tree_id() << '\n'
            << "Tree T3 has id: " << T3.tree_id() << '\n';
  std::cout << '\n';
  int keepo;
  int i{1};
  while((keepo=T.assoc_ETTrees())!=-1) {
    std::cout << i++ << ") T is associated with: " << keepo << '\n';
  }
  while((keepo=T2.assoc_ETTrees())!=-1) {
    std::cout << i++ << ") T2 is associated with: " << keepo << '\n';
  }
  if(T.disassociate(T2)) displayMsg("Trees are not associated", "error");
  std::cout << '\n';
  i=1;
  while((keepo=T.assoc_ETTrees())!=-1) {
    std::cout << i++ << ") T is associated with: " << keepo << '\n';
  }
  while((keepo=T2.assoc_ETTrees())!=-1) {
    std::cout << i++ << ") T2 is associated with: " << keepo << '\n';
  }
  std::cout << '\n';
  if(T.is_associated_to(T2)) displayMsg("T is associated to T2\n");
  else displayMsg("T is not associated to T2\n");
  T2.disable();
  if(T2.print_visits()) displayMsg("T2 is disabled and cannot print visits\n", "error");
  leda::list<leda::node*> dtETT1{T.euler_tour()};
  leda::list_item it;
  i=0;
  std::cout << "\nT: Euler Tour (iteration on external list created with euler_tour()):\n";
  forall_items(it, dtETT1) {
    i++;
    G1.print_node(*dtETT1.contents(it));
  }
  std::cout << '\n';
  std::cout << "\nT: Join T with L creating edge 0-4 (L needs to be re-rooted to 4)\n";
  T.join_tree(s1, d, L);
  if(L.empty()) displayMsg("List L is now empty");
  std::cout << "T: Euler Tour:\n";
  T.print_euler_tour();
}

void euler_tour_tree_test () {
  // ETTree Creation
  leda::graph G1;
  leda::node a1, b1, c1, d1;
  leda::node s1{test_leda_graph1(G1, a1, b1, c1, d1)};
  G1.make_bidirected(); // Graph G needs to be of type graph and bidirected.
  writeGraphDOT(G1, "dot-graph-files/example-graph.dot"); // Writes graph to file
  ETTree T1{G1, s1}; // Creates Euler Tour Tree of graph G
  // LEDA Dynamic Tree Creation
  leda::dynamic_trees D2;
  leda::vertex a2, b2, c2, d2;
  leda::vertex s2{test_leda_dynamic_trees(D2, a2, b2, c2, d2)};

  // Changing Roots <----------------------------------------------------------- TEST 1
  std::cout << "\nTEST 1: Changing Tree Root\n";
  // ETTree
  auto start = std::chrono::high_resolution_clock::now(); // Starts counting
  T1.change_root(s1, c1); // Change root 0 to 1
  auto stop = std::chrono::high_resolution_clock::now(); // Stops counting
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop-start);
  std::cout << "Time - ETTree change_root:\t" << duration.count() << " microseconds" << '\n';
  // LEDA Dynamic Tree
  start = std::chrono::high_resolution_clock::now(); // Starts counting
  D2.evert(c2);
  stop = std::chrono::high_resolution_clock::now(); // Stops counting
  duration = std::chrono::duration_cast<std::chrono::microseconds>(stop-start);
  std::cout << "Time - DTree evert:\t\t" << duration.count() << " microseconds" << '\n';
  // Deleting Edge <------------------------------------------------------------ TEST 2
  std::cout << "\nTEST 2: Deleting Tree Edge\n";
  // ETTree
  start = std::chrono::high_resolution_clock::now(); // Starts counting
  leda::list<leda::node*> L1{T1.delete_edge(a1, b1)};
  stop = std::chrono::high_resolution_clock::now(); // Stops counting
  duration = std::chrono::duration_cast<std::chrono::microseconds>(stop-start);
  std::cout << "Time - ETTree delete_edge:\t" << duration.count() << " microseconds" << '\n';
  // LEDA Dynamic Tree
  start = std::chrono::high_resolution_clock::now(); // Starts counting
  D2.cut(b2);
  stop = std::chrono::high_resolution_clock::now(); // Stops counting
  duration = std::chrono::duration_cast<std::chrono::microseconds>(stop-start);
  std::cout << "Time - DTree cut:\t\t" << duration.count() << " microseconds" << '\n';
  // Join Edge <------------------------------------------------------------ TEST 3
  std::cout << "\nTEST 3: Joining Tree Edge\n";
  // ETTree
  start = std::chrono::high_resolution_clock::now(); // Starts counting
  T1.join_tree(d1, b1, L1);
  stop = std::chrono::high_resolution_clock::now(); // Stops counting
  duration = std::chrono::duration_cast<std::chrono::microseconds>(stop-start);
  std::cout << "Time - ETTree join_tree:\t" << duration.count() << " microseconds" << '\n';
  // LEDA Dynamic Tree
  start = std::chrono::high_resolution_clock::now(); // Starts counting
  D2.link(b2, d2, 0);
  stop = std::chrono::high_resolution_clock::now(); // Stops counting
  duration = std::chrono::duration_cast<std::chrono::microseconds>(stop-start);
  std::cout << "Time - DTree link:\t\t" << duration.count() << " microseconds" << '\n';
  std::cout << '\n';
}

leda::vertex test_leda_dynamic_trees(leda::dynamic_trees& D, leda::vertex &a, leda::vertex &b, leda::vertex &c, leda::vertex &d) {
  const int nodeNum{17};
  std::array<leda::vertex, nodeNum> V;
  for(int i=0; i<nodeNum; i++) V[i]=D.make();
  a=V[6];
  b=V[10];
  c=V[1];
  d=V[4];
  D.link(V[0], V[1], 0);
  D.link(V[0], V[2], 0);
  D.link(V[0], V[3], 0);
  D.link(V[1], V[4], 0);
  D.link(V[1], V[5], 0);
  D.link(V[2], V[6], 0);
  D.link(V[3], V[7], 0);
  D.link(V[3], V[8], 0);
  D.link(V[6], V[9], 0);
  D.link(V[6], V[10], 0);
  D.link(V[9], V[11], 0);
  D.link(V[10], V[12], 0);
  D.link(V[10], V[13], 0);
  D.link(V[12], V[14], 0);
  D.link(V[12], V[15], 0);
  D.link(V[14], V[16], 0);
  return D.root(V[0]);
}

leda::node test_leda_graph1(leda::graph& G, leda::node &a, leda::node &b, leda::node &c, leda::node &d) {
  const int nodeNum{17};
  std::array<leda::node, nodeNum> V;
  for(int i=0; i<nodeNum; i++) V[i]=G.new_node();
  a=V[6];
  b=V[10];
  c=V[1];
  d=V[4];
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
