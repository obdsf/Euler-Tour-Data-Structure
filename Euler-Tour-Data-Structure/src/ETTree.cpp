/* This file contains the implementation
 * of the Euler Tour Data Structure.
 */

//  Standard Template Library
#include <string>
#include <list>
// LEDA
#include "LEDA/graph/graph.h"
#include "LEDA/graph/graph_alg.h"
#include "LEDA/core/list.h"
// Custom
#include "ETTree.h"
// Forward Declarations
extern bool consoleLog;
extern void displayMsg(std::string msg, std::string stateStr="status");
// #############################################################################################################
// TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN
// #############################################################################################################
// #############################################################################################################
// TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN
// #############################################################################################################
void ETTree::m_eulerTour(const leda::node &x) {
  m_visit(x);
  leda::node v;
  forall_adj_nodes(v, x) {
    if(!m_isVisited(v)) {
      m_eulerTour(v);
      m_visit(x);
    }
  }
}

void ETTree::m_visit(const leda::node &x) {
  m_visits[x]++;
  if(consoleLog) {
    m_G.print_node(x);
    std::cout << '\n';
  }
}

const bool ETTree::m_isVisited(const leda::node &x) {
  if(m_visits[x]==0) return false;
  else return true;
}

ETTree::ETTree(const leda::graph &G,const leda::node &root)
  : m_G{G}, m_visits{m_G, 0} {
  displayMsg("In constructor");
  // Make G directed ()
  if(m_G.is_undirected()) {
    displayMsg("graph must be directed", "error");
    exit;
  }
  if(m_G.empty()) {
    displayMsg("graph cannot be empty", "error");
    exit;
  }
  leda::node v;
  int i{0};
  forall_nodes(v, G) {
    if(v==root) {
      break;
    }
    i++;
  }
  forall_nodes(v, m_G) {
    if(i==0) {
      m_root=v;
      break;
    }
    i--;
  }
  m_eulerTour(m_root);
}

const void ETTree::getVisits() {
  leda::node v;
  forall_nodes(v, m_G) {
    std::cout << "m_visits";
    m_G.print_node(v);
    std::cout << ": " << m_visits[v] << '\n';
  }
}
