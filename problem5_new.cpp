#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <numeric>
#include <chrono>

using namespace std;

// Disjoint Set implementation
class DisjointSet {
private:
    vector<int> parent;
    vector<int> rank;

public:
    DisjointSet(int n) {
        // TODO: Initialize parent and rank arrays
        parent.resize(n);
        rank.resize(n, 0);
        for (int i = 0; i < n; i++) {
            parent[i] = i;
        }
    }

    int find(int x) {
        // TODO: Implement find with path compression
        if (parent[x] != x) {
            parent[x] = find(parent[x]); 
        }
        return parent[x];
    }

    void unite(int x, int y) {
        // TODO: Implement union by rank
        if (connected(x, y)) return; 
        
        int rootX = find(x);
        int rootY = find(y);

        if (rank[rootX] < rank[rootY]) {
            parent[rootX] = rootY;
        } 
        else if (rank[rootX] > rank[rootY]) {
            parent[rootY] = rootX;
        } 
        else {
            parent[rootY] = rootX;
            rank[rootX]++; 
        }
    }

    bool connected(int x, int y) {
        return find(x) == find(y);
    }
};

struct Wall {
    int cell1;
    int cell2;
    int direction; // 0: right, 1: down
    bool removed = false;
};

class MazeGenerator {
private:
    int width;
    int height;
    DisjointSet ds;
    vector<Wall> walls;
    
    // Grid representation for visualization
    // cells[r][c] stores bitmask of open directions: 1=Right, 2=Down, 4=Left, 8=Up
    vector<vector<int>> grid; 

public:
    MazeGenerator(int w, int h) : width(w), height(h), ds(w * h), grid(h, vector<int>(w, 0)) {
        // Initialize all possible walls
        // Cell index = row * width + col
        for (int r = 0; r < height; r++) {
            for (int c = 0; c < width; c++) {
                int current = r * width + c;
                
                // Add vertical wall to the right (if not last column)
                if (c < width - 1) {
                    int right = r * width + (c + 1);
                    walls.push_back({current, right, 0}); // 0 for right
                }
                
                // Add horizontal wall below (if not last row)
                if (r < height - 1) {
                    int down = (r + 1) * width + c;
                    walls.push_back({current, down, 1}); // 1 for down
                }
            }
        }
    }

    void generate() {
        // TODO: Implement Randomized Kruskal's Algorithm
        random_device rd;
        mt19937 gen(rd());
        
        shuffle(walls.begin(), walls.end(), gen);
        
        for (int i = 0; i < walls.size(); i++) {
            int cell1 = walls[i].cell1;
            int cell2 = walls[i].cell2;

            if (!ds.connected(cell1, cell2)) {
                ds.unite(cell1, cell2);
                walls[i].removed = true;
                
                // Update grid
                int r = cell1 / width;
                int c = cell1 % width;
                if (walls[i].direction == 0) { // right
                    grid[r][c] |= 1; // open right
                    grid[r][c+1] |= 4; // open left for right cell
                } else { // down
                    grid[r][c] |= 2; // open down
                    grid[r+1][c] |= 8; // open up for down cell
                }
            }
        }
    }

    void printMaze() {
        // Top border
        for (int c = 0; c < width; c++) cout << "+---";
        cout << "+" << endl;

        for (int r = 0; r < height; r++) {
            // Left border of the row
            cout << "|";
            for (int c = 0; c < width; c++) {
                // Print cell space
                cout << "   ";
                
                // Check right wall using grid
                if (c < width - 1 && (grid[r][c] & 1)) {
                    cout << " "; // No wall
                } else {
                    cout << "|"; // Wall
                }
            }
            cout << endl;

            // Bottom border of the row
            cout << "+";
            for (int c = 0; c < width; c++) {
                // Check down wall using grid
                if (r < height - 1 && (grid[r][c] & 2)) {
                    cout << "   +"; // No wall
                } else {
                    cout << "---+"; // Wall
                }
            }
            cout << endl;
        }
    }
};

int main() {
    int width = 10;
    int height = 10;

    cout << "Generating " << width << "x" << height << " maze..." << endl;
    
    MazeGenerator maze(width, height);
    maze.generate();
    maze.printMaze();

    return 0;
}
