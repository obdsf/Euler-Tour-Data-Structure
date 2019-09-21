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
extern void displayMsg(std::string msg, std::string stateStr="status");

// #############################################################################################################
// TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN
// #############################################################################################################
// #############################################################################################################
// TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN TRASH CAN
// #############################################################################################################

// Function class members
const void ETTree::m_eulerTour(leda::node &x) {
  m_visit(x);
  leda::node v;
  forall_adj_nodes(v, x) {
    if(!m_isVisited(v)) {
      m_eulerTour(v);
      m_visit(x);
    }
  }
}

const void ETTree::m_visit(leda::node &x) {
  m_visits[x]++;
  m_ETTree.push_back(&m_nodes[m_nodeID[x]]);
}

const bool ETTree::m_isVisited(const leda::node &x) {
  if(m_visits[x]==0) return false;
  else return true;
}

// Constructors
ETTree::ETTree(leda::graph &G,leda::node &root)
  : m_G{&G}, m_nodeNum{G.number_of_nodes()}, m_root{&root}, m_visits{G, 0} {
  // Check if G is directed
  if(G.is_undirected()) {
    displayMsg("graph must be directed", "error");
    exit;
  }
  // Check if G is empty
  if(G.empty()) {
    displayMsg("graph cannot be empty", "error");
    exit;
  }

  m_nodeID.init(G);
  m_nodes.resize(m_nodeNum);
  leda::node v;
  int i{0};
  forall_nodes(v, G) {
    m_nodes[i]=v;
    m_nodeID[v]=i++;
  }

  m_eulerTour(*m_root);
}

// Access Operations
const void ETTree::euler_tour() {

}

// Update Operations
int ETTree::delete_edge(const leda::node &a, const leda::node &b) {
  // std::cout << m_ETTree.rank(a) << '\n';
}

// I/O Operations
const void ETTree::print_visits() {
  for(int i=0; i<m_nodeNum; i++) {
    leda::node v{m_nodes[i]};
    std::cout << "m_visits[" << m_nodeID[v] << "]:\t" << m_visits[v] << '\n';
  }
}

const void ETTree::print_euler_tour() {
  leda::list_item it;
  forall_items(it, m_ETTree) {
    std::cout << "[" << m_nodeID[*m_ETTree[it]] << "]";
  }
  std::cout << '\n';
}
