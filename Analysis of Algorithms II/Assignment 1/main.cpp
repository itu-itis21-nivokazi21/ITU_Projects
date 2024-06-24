/**
 * BLG 336E - Analysis of Algorithms II
 * Assignment 1
 * Erblina Nivokazi
 * 150200917
 */

/* * * * * * * * * * * * * * * * * *
 * DO NOT MODIFY UNLESS REQUESTED  *
 * * * * * * * * * * * * * * * * * */

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <stack>
#include <queue>
#include <chrono> // For time measurement
#include <algorithm>
#include <vector>

/* INCLUDE ANY OTHER NECESSARY LIBRARIES HERE */
/* START YOUR CODE HERE */

/* END YOUR CODE HERE */

using namespace std;
using namespace std::chrono;

/* USE HERE IF YOU NEED TO DEFINE EXTRA FUNCTIONS */
/* START YOUR CODE HERE */

/* END YOUR CODE HERE */

/**
 * Perform Depth-First Search (DFS) traversal on a map.
 *
 * @param map The map represented as a 2D vector of integers.
 * @param row The starting row index for the DFS traversal.
 * @param col The starting column index for the DFS traversal.
 * @param resource The resource value to search for during the traversal.
 * @return The size of the colony found during the DFS traversal.
 */
int dfs(vector<vector<int>> &map, int row, int col, int resource)
{
    int count = 0;
    stack<pair<int, int>> dfstack;     // using stack to keep track of the elements
    dfstack.push({row, col}); 

    const int rows = map.size();    // get the number of rows
    const int cols = map[0].size(); // number of columns

    // Exploring the mapp
    while (!dfstack.empty())
    {
        pair<int, int> top = dfstack.top(); // accessing the top pair from the stack

        int r = top.first;  // row index
        int c = top.second; //column index

        dfstack.pop(); // remove the top element from the stack 

        if (map[r][c] != resource)  //skip iteration if the element does not have same resource
            continue;
        count++;

        map[r][c] = -1;

        // get the neighbors of the element and check if they are the resource
        vector<pair<int, int>> neighbors = {{r + 1, c}, {r - 1, c}, {r, c + 1}, {r, c - 1}};

        // traverse the neighbors of the element, check if they have the same resource
        for (const auto &neighbor : neighbors)
        {
            int nr = neighbor.first; // get the first element of the neighbor pair
            int nc = neighbor.second;

            // Adjusting the row index if out of bound
            if (nr < 0)              // if the neighbor's row is above the top edge of the map
            {
                nr = rows - 1;       // set the row to the last row (wrap to the bottom row)
            }
            else if (nr >= rows)     // If the neighbor's row is below the bottom edge of the map
            {
                nr = 0;             // set the row to the first row (wrap to the top row)
            }

            // Adjusting the column index if out of bound
            if (nc < 0)             // If the neighbor's column is to the left of the left edge of the map
            {
                nc = cols - 1;      // set the column to the last column (wrap to the rightmost column)
            }
            else if (nc >= cols)    // If the neighbor's column is to the right of the right edge of the map
            {
                nc = 0;             // set the column to the first column (wrap to the leftmost column)
            }

            
            if (map[nr][nc] == resource)
            {
                dfstack.push({nr, nc});     // If the neighbor has the same resource value then push it to the stack
            }
        }
    }
    return count;
}

/**
 * Perform Breadth-First Search (BFS) traversal on a map.
 *
 * @param map The map represented as a 2D vector of integers.
 * @param row The starting row index for the BFS traversal.
 * @param col The starting column index for the BFS traversal.
 * @param resource The resource value to search for during the traversal.
 * @return The size of the colony found during the BFS traversal.
 */
int bfs(vector<vector<int>> &map, int row, int col, int resource)
{
    int count = 0;           // Initialize count to 0
    queue<pair<int, int>> bfqueue; // Using queue to keep track of the elements

    bfqueue.push(make_pair(row, col));    // Using make_pair instead of {}
    const int rows = map.size();    // Get the number of rows
    const int cols = map[0].size(); // Get the number of columns

    // traversing the map
    while (!bfqueue.empty())
    {
        pair<int, int> front = bfqueue.front();
        int r = front.first;              // row index
        int c = front.second;             // column index
        bfqueue.pop();                          // Remove the front element from the queue
        if (map[r][c] != resource)
            continue; // If the element is not the resource, continue
        count++;

        map[r][c] = -1; // Mark the element as visited

        vector<pair<int, int>> neighbors = {{r + 1, c}, {r - 1, c}, {r, c + 1}, {r, c - 1}}; // get the neighbors of the element
        
        // Traverse the neighbors of the element checking if they have the same resource
        for (const auto &neighbor : neighbors)
        { // traverse the neighbors of the element tp check if they are the resource
            int nr = neighbor.first;    
            int nc = neighbor.second;

            // Adjusting the row index for wrapping
            if (nr < 0)              // if the neighbors row is above the top edge
            {
                nr = rows - 1;       // wrap to the bottom row
            }
            else if (nr >= rows)     //if the neighbor row is below the bottom edge
            {
                nr = 0;              // wrap to the top row
            }

            // Adjusting the column index for wrapping
            if (nc < 0)              // if the neighbor's column is left of the left edg
            {
                nc = cols - 1;       // wrap to the rightmost column
            }
            else if (nc >= cols)     // if the neighbor's column is right of the right edge
            {
                nc = 0;              //wrap to the leftmost column
            }

            if (map[nr][nc] == resource)
            { // If the neighbor has the same value as the resource then push it to the queue
                bfqueue.push({nr, nc});
            }
        }
    }
    return count;
}

/**
 * Finds the top-k largest colonies in a given map.
 *
 * @param map The map represented as a 2D vector of integers.
 * @param useDFS A boolean flag indicating whether to use Depth-First Search (DFS) or Breadth-First Search (BFS) algorithm.
 * @param k The number of top colonies to find.
 * @return A vector of pairs representing the size and resource type of the top-k largest colonies.
 */
vector<pair<int, int>> top_k_largest_colonies(vector<vector<int>> &map, bool useDFS, unsigned int k)
{

    auto start = high_resolution_clock::now(); // Start measuring time

    vector<pair<int, int>> colonies;

    const int rows = map.size();

    if (rows == 0) // If the map is empty then return colonies
        return colonies;

    const int cols = map[0].size(); // Get the number of columns

    //Traverse the entire map to find all colonies
    // if the element is not visited then call the DFS or BFS function
    // and push the size and resource to the colonies vector
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            
            // Check if the current cell has not been visited and contains a resource
            if (map[i][j] != -1)
            {
                int resource = map[i][j];
                int colonySize;

                // Use DFS or BFS to find the size of the colony starting from the current cell
                if (useDFS)
                {
                    colonySize = dfs(map, i, j, resource);
                }
                else
                {
                    colonySize = bfs(map, i, j, resource);
                }
                colonies.push_back({colonySize, resource});
            }
        }
    }

    //cout << "coloniesss" << colonies[0].first << endl;
    // Sort the colonies by size (descending) and resource type (ascending)

    std::sort(colonies.begin(), colonies.end(), [](const pair<int, int> &a, const pair<int, int> &b)
              {
        if (a.first != b.first) return a.first > b.first;
        return a.second < b.second; });

    auto stop = high_resolution_clock::now();                 // Stop measuring time
    auto duration = duration_cast<nanoseconds>(stop - start); // Calculate the duration
    cout << "Time taken: " << duration.count() << " nanoseconds" << endl;

    
    //copy the top-k elements from colonies to topColonies the number of elements copied is the minimum of k and the total number of colonies found
    vector<pair<int, int>> topColonies;
    for (unsigned int i = 0; i < min(k, (unsigned int)colonies.size()); ++i)
    {
        topColonies.push_back(colonies[i]);
    }

    return topColonies;
}

// Main function
int main(int argc, char *argv[])
{
    
    // Check the command line arguments
    if (argc != 4)
    {

        // It should be: <1 or 0> <k> <filename.txt>
        cerr << "Usage: " << argv[0] << " <1 or 0> <k> <filename.txt>" << endl;
        return 1;
    }

    // Get the arguments
    bool useDFS = (atoi(argv[1]) == 1);
    int k = atoi(argv[2]);
    
    // Read the map from the text file
    string filename = argv[3];
    ifstream file(filename);
    if (!file.is_open())
    {
        cerr << "Error: Unable to open file." << endl;
        return 1;
    }

    vector<vector<int>> sector_map;
    string line;
    while (getline(file, line))
    {
        istringstream iss(line);
        vector<int> row;
        int num;
        while (iss >> num)
        {
            row.push_back(num);
        }
        sector_map.push_back(row);
    }
    file.close();

    // Find the largest colony
    vector<pair<int, int>> result = top_k_largest_colonies(sector_map, useDFS, k);

    // Print the result
    cout << "Algorithm: " << (useDFS ? "DFS" : "BFS") << endl;
    cout << "Map: " << filename << endl;

    // If no colonies are found
    if (result.empty())
    {
        cout << "No colonies found." << endl;
        return 0;
    }

    // Print the top-k colonies
    for (size_t i = 0; i < result.size(); ++i)
    {
        cout << "Colony " << i + 1 << ": Size = " << result[i].first << ", Resource Type = " << result[i].second << endl;
    }

    return 0;
}