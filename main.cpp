#include "graph.h"
#include <iostream>

int main() {
  GraphContainer g = {{{1, 10}, {2, 5}}, {{2, 2}}, {{3, 7}}, {}};

  Graph graph(g);

  std::cout << "Test 1: Initial graph structure" << std::endl;

  GraphContainer cyclicGraph = {{{1, 1}}, {{2, 1}}, {{3, 1}}, {{0, 1}}};
  Graph graphWithCycle(cyclicGraph);
  graphWithCycle.print_graph();

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

  Graph mst = graphForKruskal.kruskal();
  int expectedWeight = 6;

  std::cout << "Test 7: Kruskal's MST Weight Check - "
            << (mst.greutate() == expectedWeight && !mst.are_ciclu() ? "Passed"
                                                                     : "Failed")
            << std::endl;

  Graph test8_1(GraphContainer({{{1, 1}, {2, 1}, {3, 1}},
                                {{0, 1}, {2, 1}},
                                {{0, 1}, {1, 1}},
                                {{0, 1}, {4, 1}},
                                {{3, 1}}}));
  Graph test8_2(
      GraphContainer({{{1, 1}}, {{0, 1}, {2, 1}}, {{1, 1}, {3, 1}}, {{2, 1}}}));

  Graph test8_3(GraphContainer({{{1, 1}, {2, 1}},
                                {{0, 1}, {2, 1}, {3, 1}, {4, 1}, {6, 1}},

                                {{0, 1}, {1, 1}},
                                {{1, 1}, {5, 1}},
                                {{1, 1}, {5, 1}},
                                {{3, 1}, {4, 1}},
                                {{1, 1}}}));

  std::cout << "Test 8: Trajan's critical points - "
            << (test8_1.puncte_critice() == std::vector<int>({0, 3}) &&
                        test8_2.puncte_critice() == std::vector<int>({1, 2}) &&
                        test8_3.puncte_critice() == std::vector<int>({1})
                    ? "Passed"
                    : "Failed")
            << std::endl;
  Graph test9_1(GraphContainer({{{1, 4}, {7, 8}},
                                {{0, 4}, {2, 8}, {7, 11}},
                                {{1, 8}, {3, 7}, {8, 2}, {5, 4}},
                                {{2, 7}, {4, 9}, {5, 14}},
                                {{3, 9}, {5, 10}},
                                {{2, 4}, {3, 14}, {4, 10}, {6, 2}},
                                {{5, 2}, {7, 1}, {8, 6}},
                                {{0, 8}, {1, 11}, {6, 1}, {8, 7}},
                                {{2, 2}, {6, 6}, {7, 7}}}));
  std::cout << "Test 9: Dijkstra - "
            << (test9_1.dijkstra(0) ==
                        std::vector<int>({0, 4, 12, 19, 21, 11, 9, 8, 14})
                    ? "Passesd"
                    : "Failed")
            << std::endl;

  Graph test10_1(
      GraphContainer({{{Edge{1, 0, 0, 10}, Edge{2, 0, 0, 10}}},
                      {{Edge{3, 0, 0, 4}, Edge{2, 0, 0, 2}, Edge{4, 0, 0, 8}}},
                      {{Edge{4, 0, 0, 9}}},
                      {{Edge{5, 0, 0, 10}}},
                      {{Edge{5, 0, 0, 10}}},
                      {}}));

  Graph test10_2(GraphContainer({{{Edge{1, 0, 0, 20}, Edge{2, 0, 0, 20}}},
                                 {{Edge{3, 0, 0, 5}}},
                                 {{Edge{3, 0, 0, 15}}},
                                 {{Edge{4, 0, 0, 10}}},
                                 {{Edge{5, 0, 0, 20}}},
                                 {}}));

  Graph test10_3(GraphContainer({{{Edge{1, 0, 0, 16}, Edge{2, 0, 0, 13}}},
                                 {{Edge{2, 0, 0, 10}, Edge{3, 0, 0, 12}}},
                                 {{Edge{4, 0, 0, 14}}},
                                 {{Edge{2, 0, 0, 9}, Edge{5, 0, 0, 20}}},
                                 {{Edge{3, 0, 0, 7}, Edge{5, 0, 0, 4}}},
                                 {}}));

  std::cout << "Test 10: Edmonds-Karp - "
            << (test10_1.edmonds_karp(0, 5) == 14 &&
                        test10_2.edmonds_karp(0, 5) == 10 &&
                        test10_3.edmonds_karp(0, 5) == 23
                    ? "Passed"
                    : "Failed")
            << std::endl;
}
