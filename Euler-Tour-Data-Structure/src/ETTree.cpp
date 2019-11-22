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
leda::list<int> ETTree::m_ETTreeIDTable;

// Function class members
int ETTree::m_requestUniqueTreeID() {
  leda::list_item it;
  int uniqueID{0};
  forall_items(it, m_ETTreeIDTable) {
    if(uniqueID==m_ETTreeIDTable.contents(it)) uniqueID++;
  }
  m_ETTreeIDTable.push_back(uniqueID);
  return uniqueID;
}

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

const bool ETTree::m_isPart(const leda::node &a, const leda::node &b) {
  bool isPartA{false};
  bool isPartB{false};
  leda::node* addrA;
  leda::node* addrB;
  for(int i=0; i<m_nodeNum; i++) {
    if(a==m_nodes[i])  addrA=&m_nodes[i];
    if(b==m_nodes[i])  addrB=&m_nodes[i];
  }
  if(m_ETTree.rank(addrA)!=0) isPartA=true;
  if(m_ETTree.rank(addrB)!=0) isPartB=true;
  if(isPartA && isPartB) return true;
  else return false;
}

const void ETTree::m_printList(const leda::list<leda::node*> &L) {
  leda::list_item it;
  forall_items(it, L) {
    std::cout << "[" << m_nodeID[*L[it]] << "]";
  }
  std::cout << '\n';
}

void ETTree::m_changeRoot(const leda::node &a, leda::list<leda::node*> &L) {
  leda::list_item it;
  leda::list_item liB;
  forall_items(it, L) {
    if(*L[it]==a) {
      liB=it;
      break;
    }
  }
  leda::list<leda::node*> L1, L2;
  L.split(liB, L1, L2);
  L1.pop_front();
  if(!L.empty()) L.clear();
  L.conc(L2);
  L.conc(L1);
  L.push_back(&m_nodes[m_nodeID[a]]);
}

// Constructors
ETTree::ETTree()
  : m_treeID{m_requestUniqueTreeID()}, m_currAssoc{0}, m_disabled{true} {
  m_assocETTrees.push_back(m_treeID);
}

ETTree::ETTree(leda::graph &G,leda::node &root)
  : m_treeID{m_requestUniqueTreeID()}, m_G{&G}, m_nodeNum{G.number_of_nodes()},
    m_root{&root}, m_visits{G, 0}, m_currAssoc{0}, m_disabled{false} {
  // Check if G is undirected
  if(G.is_undirected()) {
    std::cout << "ETTree: error: cannot link tree with object of type ugraph\n";
    exit;
  }
  m_assocETTrees.push_back(m_treeID);
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
const leda::graph* ETTree::linked_graph() {
  return m_G;
}

const leda::node* ETTree::root() {
  return m_root;
}

const int ETTree::tree_id() {
  return m_treeID;
}

const int ETTree::assoc_ETTrees() {
  if(m_currAssoc==m_assocETTrees.length()) {
    m_currAssoc=0;
    return -1;
  }
  return m_assocETTrees.contents(m_assocETTrees.get_item(m_currAssoc++));
}

const leda::list<leda::node*> ETTree::euler_tour() {
  return m_ETTree;
}

// Update Operations
const int ETTree::associate(ETTree &T) {
  leda::list_item it;
  int newTreeID{T.tree_id()};
  forall_items(it, m_assocETTrees) {
    if(m_assocETTrees.contents(it)==newTreeID) {
      return 1;
    }
  }
  m_assocETTrees.push_back(newTreeID);
  T.m_assocETTrees.push_back(m_treeID);
  return 0;
}

const int ETTree::disassociate(ETTree &T) {
  leda::list_item it;
  int newTreeID{T.tree_id()};
  forall_items(it, m_assocETTrees) {
    if(m_assocETTrees.contents(it)==newTreeID) {
      m_assocETTrees.remove(newTreeID);
      T.m_assocETTrees.remove(m_treeID);
      return 0;
    }
  }
  return 1;
}

const bool ETTree::is_associated_to(ETTree &T) {
  leda::list_item it;
  int newTreeID{T.tree_id()};
  forall_items(it, m_assocETTrees) {
    if(m_assocETTrees.contents(it)==newTreeID) {
      return true;
    }
  }
  return false;
}

void ETTree::disable() {
  m_disabled=true;
  m_ETTree.clear();
  leda::graph G;
  m_visits.init(G);
}

leda::list<leda::node*> ETTree::delete_edge(const leda::node &a, const leda::node &b) {
  leda::list<leda::node*> L;
  if(m_disabled) return L;
  if(!m_isPart(a, b)) {
    return L; // Return empty list
  }
  leda::list_item it;
  leda::list_item liA;
  leda::list_item liB;
  bool ABSearchArmed{true};
  bool lookForB{false};
  bool lookForA{false};
  int usefullABsFound{0};
  bool orderAB{false};
  forall_items(it, m_ETTree) {
    if(ABSearchArmed) {
      if(*m_ETTree[it]==a) {
        ABSearchArmed=false;
        lookForB=true;
      } else if (*m_ETTree[it]==b) {
        ABSearchArmed=false;
        lookForA=true;
        orderAB=true;
      }
    }
    if(lookForB) {
      if(*m_ETTree[it]==b) {
        liB=it;
        lookForB=false;
        lookForA=true;
        usefullABsFound++;
        if(usefullABsFound==2) break;
      }
    } else if(lookForA) {
      if(*m_ETTree[it]==a) {
        liA=it;
        lookForA=false;
        lookForB=true;
        usefullABsFound++;
        if(usefullABsFound==2) break;
      }
    }
  }
  leda::list<leda::node*> L1;
  if(orderAB) m_ETTree.extract(liA, liB, L1, true);
  else m_ETTree.extract(liB, liA, L1, true);
  L1.pop_back();

  return L1;
}

int ETTree::change_root(const leda::node &r, const leda::node &s) {
  if(m_disabled) return 1;
  if(!m_isPart(r, s)) {
    return 1;
  }
  if(s==*m_root) return 1;

  m_changeRoot(s, m_ETTree);

  m_root=&m_nodes[m_nodeID[s]];

  return 0;
}

int ETTree::join_tree(const leda::node &a, leda::node &b, leda::list<leda::node*> &L) {
  if(m_disabled) return 1;
  leda::node* s{L.contents(L.first())};
  if(!(*s==b)) { // ETTree in list L needs to be rooted to b
    m_changeRoot(b, L);
    L.push_back(&m_nodes[m_nodeID[a]]);
  }
  leda::list_item liA{m_ETTree.get_item(m_ETTree.rank(&m_nodes[m_nodeID[a]]))};
  leda::list<leda::node*> L1, L2;
  m_ETTree.split(liA, L1, L2);
  m_ETTree.conc(L1);
  m_ETTree.conc(L);
  m_ETTree.conc(L2);
}

// I/O Operations
const int ETTree::print_visits() {
  if(m_disabled) return 1;
  for(int i=0; i<m_nodeNum; i++) {
    leda::node v{m_nodes[i]};
    std::cout << "m_visits[" << m_nodeID[v] << "]:\t" << m_visits[v] << '\n';
  }
  return 0;
}

const int ETTree::print_euler_tour() {
  if(m_disabled) return 1;
  leda::list_item it;
  forall_items(it, m_ETTree) {
    std::cout << "[" << m_nodeID[*m_ETTree[it]] << "]";
  }
  std::cout << '\n';
  return 0;
}
