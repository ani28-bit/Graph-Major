# 🚍 Graph Major: Dhaka Smart Route Navigation System

A graph-based route navigation system for Dhaka city that finds optimal travel paths using different transportation modes. The project models Dhaka's road network as a weighted graph and applies shortest path algorithms to provide efficient routing solutions.

---

## 📌 Features

- 🗺️ Graph representation of Dhaka's road network
- 🚶 Multiple transportation modes:
  - Walking
  - Rickshaw
  - Bus
  - Metro Rail
- ⏱️ Time-aware routing and scheduling
- 📍 Source and destination selection
- 🛣️ Shortest path calculation using Dijkstra's Algorithm
- 💰 Cost estimation for different transport modes
- 📊 Route statistics:
  - Total distance
  - Estimated travel time
  - Transportation cost
- 📄 Route output generation

---

## 🏗️ Project Structure

```text
Graph-Major/
│
├── src/
│   ├── Algorithms/
│   │   └── dijkstra.c
│   │
│   ├── Models/
│   │   ├── location.c
│   │   └── transport.c
│   │
│   ├── Routing/
│   │   ├── route_finder.c
│   │   ├── cost_calculator.c
│   │   ├── time_scheduler.c
│   │   └── route_converter.c
│   │
│   ├── Utils/
│   │   ├── priority_queue.c
│   │   ├── haversine.c
│   │   └── csv_parser.c
│   │
│   └── main.c
│
├── data/
│   ├── roads.csv
│   ├── metro_stations.csv
│   └── transport_data.csv
│
├── output/
│
├── obj/
├── bin/
└── Makefile
```

---

## 🧠 Algorithms Used

### Dijkstra's Algorithm
Used to find the shortest path between source and destination nodes.

**Time Complexity:** `O((V + E) log V)`

**Data Structures Used:**
- Graph (Adjacency List)
- Priority Queue (Min Heap)
- Structures
- Dynamic Memory Allocation

---

## 🚦 Routing Process

1. Load map data from CSV files.
2. Construct graph nodes and edges.
3. Select source and destination.
4. Choose transportation mode.
5. Apply Dijkstra's Algorithm.
6. Calculate:
   - Distance
   - Estimated time
   - Travel cost
7. Display the optimal route.

---

## 🛠️ Technologies Used

- Language: C (C11 Standard)
- Compiler: GCC
- Build Tool: Make
- Data Storage: CSV Files
- Operating System: Linux

---

## ⚙️ Compilation

Clone the repository:

```bash
git clone https://github.com/your-username/Graph-Major.git
cd Graph-Major
```

Compile:

```bash
make
```

Clean build files:

```bash
make clean
```

Run:

```bash
./bin/dhaka_router
```

---

## 📸 Sample Output

```text
===== DHAKA SMART ROUTER =====

Source: Farmgate
Destination: Motijheel
Transport: Metro + Walking

Shortest Distance : 8.5 km
Estimated Time    : 23 minutes
Estimated Cost    : 40 BDT

Optimal Route:
Farmgate
   ↓
Karwan Bazar
   ↓
Shahbag
   ↓
Motijheel
```

---

## 🎯 Learning Objectives

This project demonstrates practical implementation of:

- Graph Theory
- Shortest Path Algorithms
- Priority Queues
- Data Structures
- File Handling
- Modular Programming in C
- Real-world Navigation System Design

---

## 🚀 Future Improvements

- GUI-based visualization
- Real-time traffic updates
- A* Search Algorithm
- GPS integration
- Interactive map interface
- Multiple route suggestions
- Route caching and optimization

---

## Author

Anita Pervin

    Department of Software Engineering, University of Dhaka

---

## 📄 License

This project is developed for academic and educational purposes.
