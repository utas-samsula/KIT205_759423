# KIT205 Dev Log - Lab Work 2
Student: Samsul Alam (759423)

## Week 8 - Graph Data Structure

### Commit 1 - initial graph struct setup
- Created graph.h with Edge, EdgeNode, EdgeList, Graph structs
- Set up create_graph and add_edge functions
- Basic structure matching the tutorial definitions

### Commit 2 - file loading and adjacency list
- Added load_graph function reading from graph.txt
- Used fscanf with comma-separated format
- Tested with the 7-vertex example from tutorial

### Commit 3 - in-degree verification
- Added print_in_degrees to verify graph is built correctly
- Counted edges pointing to each vertex
- Compared against expected values from tutorial diagram

## Week 9 - PageRank

### Commit 1 - pagerank function skeleton
- Added pagerank function to graph.c
- Set up out-degrees array, PR array initialised to 1
- Confirmed structure matches tutorial pseudocode

### Commit 2 - pagerank iteration logic
- Implemented the three separate loops as tutorial specifies
- Loop 1: reset sums
- Loop 2: accumulate PR(from)/out(from) for each edge
- Loop 3: apply damping factor d=0.85

### Commit 3 - testing and verification
- Tested with 20-vertex input from tutorial
- Compared high-rank vertices against expected topology
- Vertex 3 has highest rank as expected (most incoming edges)
- Added unweighted graph loader for PageRank test input