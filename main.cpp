#include "graph.h"
#include <iostream>

int main() {
  GraphContainer g = {{{1, 10}, {2, 5}}, {{2, 2}}, {{3, 7}}, {}};

  Graph graph(g);

  std::cout << "Test 1: Initial graph structure" << std::endl;

  GraphContainer cyclicGraph = {{{1, 1}}, {{2, 1}}, {{3, 1}}, {{0, 1}}};
  Graph graphWithCycle(cyclicGraph);

  std::cout << "Test 2: Cycle detection - "
            << (graphWithCycle.are_ciclu() ? "Passed" : "Failed") << std::endl;

  GraphContainer sccGraph = {{{1, 1}}, {{2, 1}}, {{0, 1}},
                             {{4, 1}}, {{5, 1}}, {{3, 1}}};
  Graph graphWithSCC(sccGraph);

  std::cout << "Test 3: Strongly Connected Components Count - "
            << (graphWithSCC.comp_conexe() == 2 ? "Passed" : "Failed")
            << std::endl;

  GraphContainer testGraph = {{{1, 1}, {2, 1}}, {}, {{3, 1}, {4, 1}},
                              {{5, 1}},         {}, {}};
  Graph graphWithTestGraph(testGraph);

  std::vector<int> topoSortedTest = graphWithTestGraph.sortare_topologica();
  std::cout << "Test 4: Topological Sort - "
            << (topoSortedTest == std::vector<int>({0, 2, 4, 3, 5, 1})
                    ? "Passed"
                    : "Failed");
  std::cout << std::endl;

  GraphContainer bipartiteGraph = {
      {{1, 1}, {2, 1}}, {{3, 1}}, {{4, 1}}, {}, {}};
  Graph graphWithBipartiteCheck(bipartiteGraph);

  std::cout << "Test 5: Bipartite Graph Check - "
            << (graphWithBipartiteCheck.este_bipartit() ? "Passed" : "Failed")
            << std::endl;

  GraphContainer nonBipartiteGraph = {{{1, 1}, {2, 1}}, {{2, 1}}, {{0, 1}}};
  Graph graphWithNonBipartiteCheck(nonBipartiteGraph);

  std::cout << "Test 6: Non-Bipartite Graph Check - "
            << (graphWithNonBipartiteCheck.este_bipartit() ? "Failed"
                                                           : "Passed")
            << std::endl;

  GraphContainer weightedGraph = {
      {{1, 4}, {2, 3}}, {{2, 1}, {3, 2}}, {{3, 5}}, {}};
  Graph graphForKruskal(weightedGraph);

  return 0;
}
