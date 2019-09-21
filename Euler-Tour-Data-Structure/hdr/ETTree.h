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

// #############################################################################################################
// TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN
// #############################################################################################################
// #############################################################################################################
// TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN
// #############################################################################################################

class ETTree {
private:
  // Variable class members
  leda::graph* m_G;
  int m_nodeNum;
  leda::array<leda::node> m_nodes;
  leda::node_array<int> m_nodeID;
  leda::node* m_root;
  leda::node_array<int> m_visits;
  leda::list<leda::node*> m_ETTree;
  // Function class members
  const void m_eulerTour(leda::node &x);
  const void m_visit(leda::node &x);
  const bool m_isVisited(const leda::node &x);
public:
  // Constructors
  ETTree(leda::graph &G, leda::node &root);
  // Access Operations
  const void euler_tour();
  // Update Operations
  int delete_edge(const leda::node &a, const leda::node &b);
  // I/O Operations
  const void print_visits();
  const void print_euler_tour();
};
