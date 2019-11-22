/* This file contains the definition
 * of the Euler Tour Data Structure.
 */
#pragma once
//  Standard Template Library
#include <string>
#include <list>
// LEDA
#include "LEDA/graph/graph.h"
// Custom

class ETTree {
private:
  // Variable class members
  static leda::list<int> m_ETTreeIDTable;
  int m_treeID;
  leda::graph* m_G;
  int m_nodeNum;
  leda::array<leda::node> m_nodes;
  leda::node_array<int> m_nodeID;
  leda::node* m_root;
  leda::node_array<int> m_visits;
  leda::list<leda::node*> m_ETTree;
  leda::list<int> m_assocETTrees;
  int m_currAssoc;
  bool m_disabled;
  // Function class members
  int m_requestUniqueTreeID();
  const void m_eulerTour(leda::node &x);
  const void m_visit(leda::node &x);
  const bool m_isVisited(const leda::node &x);
  const bool m_isPart(const leda::node &a, const leda::node &b);
  const void m_printList(const leda::list<leda::node*> &L);
  void m_changeRoot(const leda::node &a, leda::list<leda::node*> &L);
public:
  // Constructors
  ETTree();
  ETTree(leda::graph &G, leda::node &root);
  // Access Operations
  const leda::graph* linked_graph();
  const leda::node* root();
  const int tree_id();
  const int assoc_ETTrees();
  const leda::list<leda::node*> euler_tour();
  // Update Operations
  const int associate(ETTree &T);
  const int disassociate(ETTree &T);
  const bool is_associated_to(ETTree &T);
  void disable();

  leda::list<leda::node*> delete_edge(const leda::node &a, const leda::node &b);
  int change_root(const leda::node &r, const leda::node &s);
  int join_tree(const leda::node &a, leda::node &b, leda::list<leda::node*> &L);
  // I/O Operations
  const int print_visits();
  const int print_euler_tour();
};
