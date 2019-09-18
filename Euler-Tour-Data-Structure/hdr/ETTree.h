/* This file contains the definition
 * of the Euler Tour Data Structure.
 */
#pragma once
//  Standard Template Library
#include <string>
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
  leda::graph m_G;
  leda::node m_root;
  leda::node_array<int> m_visits;
  void m_eulerTour(const leda::node &x);
  void m_visit(const leda::node &x);
  const bool m_isVisited(const leda::node &x);
public:
  ETTree(const leda::graph &G, const leda::node &root);
  const void getVisits();
};
