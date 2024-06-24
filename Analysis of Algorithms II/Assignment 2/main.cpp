/**
 * BLG 336E - Analysis of Algorithms II
 * Assignment 2
 * Erblina Nivokazi
 * 150200917
 */

#include <iostream>  // For input/output
#include <cmath>     // For distance calculations
#include <vector>    // For vector operations
#include <algorithm> // For sorting operations
#include <limits>    // For min(), max() functions
#include <fstream>   // To read/write files
#include <chrono>    // For time measurement

//(These libraries are sufficient for this assignment.)

using namespace std;

// Structure to represent a point in 2D space
struct Point
{
    double x, y;
};

// Helper function to calculate distance between two points
double distance(Point p1, Point p2)
{
    return sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
}

// Helper function to compare points based on x-coordinate can use this for sorting.
bool compareX(Point p1, Point p2)
{
    return p1.x < p2.x;
}

// Helper function to compare points based on y-coordinate, can use this for sorting.
bool compareY(Point p1, Point p2)
{
    return (p1.y != p2.y) ? (p1.y < p2.y) : (p1.x < p2.x);
}

// Function to find the closest pair of points using brute force.
pair<Point, Point> bruteForceClosestPair(vector<Point> &points, int start, int end)
{
    // Here I tried to follow similar notation to findClosestPairOrder function
    pair<Point, Point> closestPair; // Initialize the closest pair of points

    double minDistance = numeric_limits<double>::max(); // Initialize the minimum distance to the maximum possible value

    for (int i = start; i <= end; ++i) // iterate through each pair of points in the given range
    {
        for (int j = i + 1; j <= end; ++j)
        {
            double dis = distance(points[i], points[j]); // calculalte the distance between points[i] and points[j]

            if (dis < minDistance) // if the distance is less than the minimum distance at the time then  update minDistance and closestPair to match accordingly
            {
                minDistance = dis;
                closestPair = {points[i], points[j]};
            }
        }
    }
    return closestPair; // return the closest pair of points
}

// The main objective of the assignment. This is the function to find the closest pair of points recursively using divide and conquer.
pair<Point, Point> closestPair(vector<Point> &points, int start, int end)
{

    if (end - start <= 3) // Bruteforce used when the number of points is less than or equal to 3
    {
        return bruteForceClosestPair(points, start, end);
    }

    int center = (start + end) / 2; // Calculate the center index or the midpoint of the segment

    // Divide and conquer:
    // split the points into left and right  and find the closest pairs
    pair<Point, Point> closestOnLeft = closestPair(points, start, center);
    pair<Point, Point> closestOnRight = closestPair(points, center + 1, end);

    // Find the shorter distance between the two pairs that were found
    double shortestDistance = min(
        distance(closestOnLeft.first, closestOnLeft.second),
        distance(closestOnRight.first, closestOnRight.second));

    // Here assume that the pair with the shorttest distance is the closest pair
    pair<Point, Point> closest = shortestDistance == distance(closestOnLeft.first, closestOnLeft.second) ? closestOnLeft : closestOnRight;

    // Collect points that are close to the center line
    vector<Point> nearbyPoints;
    for (int i = start; i <= end; ++i)
    {
        // If the x-coordinate of the point is within the shortest distance from the center line, add it to the nearbyPoints vector
        if (abs(points[i].x - points[center].x) < shortestDistance)
        {
            nearbyPoints.push_back(points[i]);
        }
    }

    // Ordering the nearby points by the y-coordinate to find closer pairs
    sort(nearbyPoints.begin(), nearbyPoints.end(), compareY);

    // Check the pairs in the sorted list to see if there is a closer pair 
    for (size_t i = 0; i < nearbyPoints.size(); ++i)
    {
        for (size_t j = i + 1; j < nearbyPoints.size() && (nearbyPoints[j].y - nearbyPoints[i].y) < shortestDistance; ++j)
        {   // If the distance between the two points is less than the shortest distance, update the shortest distance and the closest pair
            double currDistance = distance(nearbyPoints[i], nearbyPoints[j]);
            if (currDistance < shortestDistance)
            {
                shortestDistance = currDistance;
                closest = {nearbyPoints[i], nearbyPoints[j]};
            }
        }
    }
    return closest;
}

// Function to remove a pair of points from the point vector. Returns the newly formed vector.
vector<Point> removePairFromVector(vector<Point> &point_vector, pair<Point, Point> point_pair)
{
    vector<Point> filteredPoints; // vector to hold the points that are not in the pair
    
    //loop over each point to see if it is in the pair
    for (const auto &point : point_vector)
    {
        // exclude the points that match the pair based on their coordinates
        if (!((point.x == point_pair.first.x && point.y == point_pair.first.y) || (point.x == point_pair.second.x && point.y == point_pair.second.y)))
        { // If the point is not part of the pair, add it to the resultant vector
            filteredPoints.push_back(point);    // Append the points that are distinct from the pair to the resultant vector
        }
    }
    return filteredPoints;
}

// This function should wrap up the entire algorithm. It should:
//    1- Find the closest pair
//    2- Add the pair to the "pairs" vector
//    3- Remove the paired result from the map
// at each iteration.
// Then it should set "unconnected" variable as the final, unconnected city (if it exists).
void findClosestPairOrder(vector<Point> points)
{
    vector<pair<Point, Point>> pairs; // add your pairs to this vector
    Point unconnected = {-1, -1};     // set this as the unconnected point if it exists, leave as is otherwise.

    // keep and sort the points by the x coordinate to facilitate closer pair identification
    sort(points.begin(), points.end(), compareX);

    // loop to fiind and remove the closest pair of points until there is only one point left
    while (points.size() > 1)
    {
        auto closestPairFound = closestPair(points, 0, points.size() - 1);  // Identify the nearest pair of points within the current set
        pairs.push_back(closestPairFound);                                  // save the discovered pair
        points = removePairFromVector(points, closestPairFound);            // remove the pair from the set of points
    }

    // make the last remaining point as 'unconnected' if not part of any pair
    if (points.size() == 1)
    {
        unconnected = points.front();
    }

    for (auto &pair : pairs)    // Ensure that the points in each pair are ordered by their y-coordinate
    {
        // If the second point is above the first point, or if they are at the same y-coordinate but the second point is to the left of the first point, swap them
        if (pair.second.y < pair.first.y || (pair.second.y == pair.first.y && pair.second.x < pair.first.x))
        {
            std::swap(pair.first, pair.second);
        }
    }

    // This part prints the output, don't modify.
    for (size_t i = 0; i < pairs.size(); i++)
    {
        cout << "Pair " << i + 1 << ": " << pairs[i].first.x << ", " << pairs[i].first.y << " - " << pairs[i].second.x << ", " << pairs[i].second.y << endl;
    }
    if (unconnected.x != -1)
    {
        cout << "Unconnected " << unconnected.x << ", " << unconnected.y;
    }
}

// Read the coordinates from the file and convert them to a vector. Return the vector as a "vector<Point>"
vector<Point> readCoordinatesFromFile(const string &filename)
{
    vector<Point> points;

    // Open the file
    ifstream file(filename);

    if (file)
    {                
        double x, y; // Variables to hold the coordinates that are read from the file
        // loop to read all the pairs of coordinates from the file
        while (file >> x >> y)
        {
            //make a  Point with these coordinates and put it in the set of points
            points.emplace_back(Point{x, y});
        }
        file.close(); // close the file
    }
    else
    {
        cerr << "Failed to open the file: " << filename << endl;
    }

    return points; 
}



// Main function. Ideally, you don't need to modify this.
int main(int argc, char* argv[]) {
    vector<Point> points = readCoordinatesFromFile(argv[1]);
    findClosestPairOrder(points);
    return 0;
}


/*

* To measurre the time I modified the main function like this
*   But I again left it in its original state above
* To measure the brutforce algorithm I changes the condition in closestPair 
* from 3 to a big number so that the brutforce algorithm is used for all cases

int main(int argc, char *argv[])
{
    auto start = std::chrono::high_resolution_clock::now(); 

    vector<Point> points = readCoordinatesFromFile(argv[1]);
    findClosestPairOrder(points);

    auto end = std::chrono::high_resolution_clock::now(); 
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start); 
    std::cout << "Total Program Execution Time: " << duration.count() << " ms" << std::endl;

    return 0;
}
*/