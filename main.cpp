#include "graph.h"
#include <iostream>

int main() {
  // Create a graph
  GraphContainer g = {
      {1, {{2, 10}, {3, 5}}}, {2, {{3, 2}}}, {3, {{4, 7}}}, {4, {}}};

  Graph graph(g);

  // Test: Initial graph structure
  std::cout << "Test 1: Initial graph structure" << std::endl;
  graph.print_graph();
  std::cout << std::endl;

  // Test: delete_node
  graph.delete_node(3);

  std::cout << "Test 2: Delete node 3 - "
            << (graph.are_ciclu() == false ? "Passed" : "Failed") << std::endl;

  // Test: delete_edge
  graph.delete_edge(1, 2);
  std::cout << "Test 3: Delete edge 1 -> 2 - "
            << (graph.are_ciclu() == false ? "Passed" : "Failed") << std::endl;

  // Test cycle detection on another example
  GraphContainer cyclicGraph = {
      {1, {{2, 1}}}, {2, {{3, 1}}}, {3, {{4, 1}}}, {4, {{1, 1}}}};
  Graph graphWithCycle(cyclicGraph);

  std::cout << "Test 4: Cycle detection - "
            << (graphWithCycle.are_ciclu() ? "Passed" : "Failed") << std::endl;

  // Test: Strongly Connected Components
  GraphContainer sccGraph = {{1, {{2, 1}}}, {2, {{3, 1}}}, {3, {{1, 1}}},
                             {4, {{5, 1}}}, {5, {{6, 1}}}, {6, {{4, 1}}}};
  Graph graphWithSCC(sccGraph);

  std::cout << "Test 5: Strongly Connected Components Count - "
            << (graphWithSCC.comp_conexe() == 2 ? "Passed" : "Failed")
            << std::endl;

  // Test: Topological Sort
  GraphContainer dagGraph = {
      {1, {{2, 1}, {3, 1}}}, {2, {{4, 1}}}, {3, {{4, 1}}}, {4, {}}};
  Graph graphWithDAG(dagGraph);

  std::vector<int> topoSorted = graphWithDAG.sortare_topologica();
  std::cout << "Test 6: Topological Sort - "
            << ((topoSorted == std::vector<int>{1, 2, 3, 4} ||
                 topoSorted == std::vector<int>{1, 3, 2, 4})
                    ? "Passed"
                    : "Failed")
            << std::endl;

  return 0;
}
