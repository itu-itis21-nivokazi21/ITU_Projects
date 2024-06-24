/**
 * BLG 336E - Analysis of Algorithms II
 * Assignment 3
 * Erblina Nivokazi
 * 150200917
 */

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm> // Include sort, max_element for sorting and finding the maximum element
#include <map>       // Include map for priorityMap and floorSchedules
#include <numeric>   // For std::accumulate which I used in calculating the total value of items
#include <iomanip>   // For std::fixed, std::setprecision which I used in displaying the total value of items
#include <cmath>     // For ceil function which I used in rounding the total value of items

using namespace std;

// Struct for time intervals
struct TimeIntervals
{
    string floor, room, startTime, endTime; // I kept time as string for easier comparison and display
    int priority;
};

// Struct for priority
struct Priority
{
    // I used string for floor and room to make it easier to compare with the room in the TimeIntervals struct
    std::string floor;
    std::string room;
    int priorityValue; // Higher values mean higher priority

    // Constructor for easier initialization
    Priority(std::string fl, std::string rm, int value) : floor(fl), room(rm), priorityValue(value) {}
};

// Struct for items
struct Item
{
    // I used string for name to make it easier to display the name of the item
    std::string name;
    int cost;
    double value;

    Item(string n, int c, double v) : name(n), cost(c), value(v) {}
};

/* START: WEIGHTED INTERVAL SCHEDULING (for each floor) */

// I made a separate struct for Room because I wanted to keep the start and end times as strings
// This will make it easier to compare the times, and also easier to display the floor schedule
struct Room
{
    string name, startTime, endTime; // Keep start and end times as strings
    int priority;

    Room(string n, string s, string e, int p) : name(n), startTime(s), endTime(e), priority(p) {}
};

// Struct for the schedule of each floor, which includes the total priority gain and the rooms
struct Schedule
{
    string floor;
    int totalPriority;  // This should hold the sum of all room priorities
    vector<Room> rooms; // this should hold the rooms in the optimal schedule

    Schedule() : floor(""), totalPriority(0) {}
    Schedule(string flr, int prp) : floor(flr), totalPriority(prp) {}
};

// Function to convert time string to minutes
int timeToMinutes(const std::string &timeStr)
{
    // I used sscanf to parse the time string
    int hours, minutes;
    sscanf(timeStr.c_str(), "%d:%d", &hours, &minutes);

    return hours * 60 + minutes;
}

// Function to convert the time intervals to a vector of Schedule objects
// I needed this function to convert the input data to the format that the scheduling function expects
vector<Schedule> convertToIntervals(const vector<TimeIntervals> &intervals)
{
    map<string, Schedule> floorSchedules; // Map to store the schedules for each floor
    // I created a map to store the schedules for each floor
    for (const auto &interval : intervals)
    { // Iterate over the time intervals to create the schedules
        if (floorSchedules.find(interval.floor) == floorSchedules.end())
        {
            floorSchedules[interval.floor] = Schedule(interval.floor, 0);
        }
        // Aggregate the priority gain for each floor
        floorSchedules[interval.floor].totalPriority += interval.priority;
        // Create and add the room to the corresponding floor
        Room room(interval.room, interval.startTime, interval.endTime, interval.priority);
        floorSchedules[interval.floor].rooms.push_back(room);
    }

    // here I convert the map to a vector of Schedule objects
    vector<Schedule> schedules;
    for (auto &pair : floorSchedules)
    { // Iterate over the map to convert it to a vector
        schedules.push_back(pair.second);
    }
    return schedules;
}

// function to find the best schedule for each floor
bool compareRooms(const Room &one, const Room &two)
{
    if (one.endTime != two.endTime)
    {
        return one.endTime < two.endTime; // Sort primarily by end time
    }
    else if (one.priority != two.priority)
    {
        return one.priority > two.priority; // If end times are the same then the room with higher priority is chosen
    }
    else
    {
        return one.startTime < two.startTime; // If end time and priority are the same then choose the room that ends earlier
    }
}

// Function to compare schedules based on total priority, needed in weighted_interval_scheduling function
bool compareSchedules(const Schedule &one, const Schedule &two)
{
    return one.totalPriority > two.totalPriority; // Higher total priority first
}

// Function to find the optimal schedule for each floor
vector<Schedule> weighted_interval_scheduling(vector<Schedule> &schedules)
{
    vector<Schedule> finalSchedules; // make vector to store the final schedules

    // The schedules are sorted based on total priority
    for (auto &schedule : schedules)
    {
        sort(schedule.rooms.begin(), schedule.rooms.end(), compareRooms);   // Sort the rooms with the function above

        vector<int> dp(schedule.rooms.size());                              // Vector to store the maximum priority gain
        vector<vector<Room>> optimalSelections(schedule.rooms.size());      // Vector to store the optimal selection of rooms

        for (size_t i = 0; i < schedule.rooms.size(); ++i)
        { // Iterate over the rooms of the floor to find the optimal schedule
            int newProfit = schedule.rooms[i].priority; // Initialize the new profit with the priority of the current room
            int lastIndex = -1;                         // Initialize the last index with -1

            
            for (size_t j = 0; j < i; ++j)              // Find the last interval that doesnt overlap with the current interval
            { // Iterate over the rooms before the current room
                if (timeToMinutes(schedule.rooms[j].endTime) <= timeToMinutes(schedule.rooms[i].startTime))
                {
                    if (dp[j] + schedule.rooms[i].priority > newProfit)
                    {
                        newProfit = dp[j] + schedule.rooms[i].priority;
                        lastIndex = j;
                    }
                }
            }

            dp[i] = newProfit;
            if (lastIndex != -1)
            {
                optimalSelections[i] = optimalSelections[lastIndex];
            }
            optimalSelections[i].push_back(schedule.rooms[i]);
        }

        // Find the optimal schedule with maximum priority
        int maxIndex = max_element(dp.begin(), dp.end()) - dp.begin();
        Schedule optimizedSchedule; // this variable will keep the optimized schedule for the current floor

        // Fill the optimized schedule with the floor and the optimal selection of rooms
        optimizedSchedule.floor = schedule.floor;
        optimizedSchedule.rooms = optimalSelections[maxIndex];
        optimizedSchedule.totalPriority = dp[maxIndex];

        finalSchedules.push_back(optimizedSchedule);    // Add the optimized schedule to the final schedules vectors
    }

    return finalSchedules;
}

// *** END ***

/* START: KNAPSACK (for items) */

// Function to select the most valuable items that can be purchased with a certain budget
vector<Item> knapsack(const vector<Item> &items, int budget)
{
    int num = items.size(); // Number of items to choose from
    vector<vector<double>> dp(num + 1, vector<double>(budget + 1, 0.0)); // DP table which stores the maximum value that can be obtained with the given budget

    // Filling the DP table with the maximum value that can be obtained with the given budget
    for (int i = 1; i <= num; ++i)
    {
        // Iterate over the items to find the maximum value that can be obtained with the given budget
        for (int j = 1; j <= budget; ++j)
        {
            // If the current item can be included in the knapsack then choose the maximum value between including and excluding the item
            if (items[i - 1].cost <= j)
            {
                //choosing the maximum value between including and excluding the item
                dp[i][j] = max(dp[i - 1][j], items[i - 1].value + dp[i - 1][j - items[i - 1].cost]);
            }
            else    // If the current item cannot be included in the knapsack then exclude the itemfrom the knapsack
            {
                dp[i][j] = dp[i - 1][j];
            }
        }
    }

    // Here I find the selected items that can be purchased with the given budget
    //`selected_items` vector will store the selected items
    vector<Item> selected_items;
    // I start from the last item and go back to find the selected items
    int bd = budget;

    for (int i = num; i > 0; --i) // Iterate over the items to find the selected items
    {
        // If the maximum value that can be obtained with the given budget is different from the maximum value that can be obtained without the current item
        if (dp[i][bd] != dp[i - 1][bd])
        {
            // Add the selected item to the selected_items vector
            selected_items.push_back(items[i - 1]);
            bd -= items[i - 1].cost;
        }
    }


    return selected_items;  // Return the selected items that can be purchased with the given budget
}

// *** END ***

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cout << "Usage: " << argv[0] << " <test case number>" << endl;
        return 1;
    }

    string case_no = argv[1];
    string case_name = "case_" + case_no;
    string base_path = "./inputs/" + case_name + "/";

    int total_budget = 200000;

    // Read the input files and store the data in the corresponding vectors
    string time_intervals_path = base_path + "room_time_intervals.txt";
    string priorities_path = base_path + "priority.txt";
    string items_path = base_path + "items.txt";

    ifstream time_intervals_file(time_intervals_path);
    ifstream priorities_file(priorities_path);
    ifstream items_file(items_path);

    // Check if the files are opened successfully
    if (!time_intervals_file || !priorities_file || !items_file)
    {
        cerr << "Error opening one of the input files." << endl;
        return 1;
    }

    string line;                            // Variable to store the current line
    vector<TimeIntervals> timeIntervals;    // Vector to store the time intervals read from the file
    vector<Item> items;                     // Vector to store the items which will be read from the items file
    std::map<std::string, int> priorityMap; // Map to store priority for each room

    getline(priorities_file, line); // Skip the header line because it is not needed

    // Read priorities and store in a map
    while (getline(priorities_file, line))
    {
        stringstream ss(line);  // I used stringstream to parse the line
        string floor, room;
        int priority;
        ss >> floor >> room >> priority;
        priorityMap[floor + " " + room] = priority;  // Store the priority in the map with the key as floor and room
    }

    getline(time_intervals_file, line); // Skip the header line again

    // Read time intervals and construct TimeInterval objects
    while (getline(time_intervals_file, line))
    {
        // I used stringstream to parse the line as before
        stringstream ss(line);
        string floor, room, startStr, endStr;
        ss >> floor >> room >> startStr >> endStr;

        //only add this interval if a priority value exists for this room
        if (priorityMap.find(floor + " " + room) != priorityMap.end())
        {
            timeIntervals.push_back({floor, room, startStr, endStr, priorityMap[floor + " " + room]});
        }
    }

    getline(items_file, line); // Skip the header line again for the items file
    // Read items and construct Item objects
    while (getline(items_file, line))
    {
        stringstream ss(line);
        string name;
        int cost;
        double value;
        ss >> name >> cost >> value;
        // Store the items in the items vector
        items.push_back({name, cost, value});
    }

    // Close the files
    time_intervals_file.close();
    priorities_file.close();
    items_file.close();

    vector<Schedule> schedules = convertToIntervals(timeIntervals);              // Convert the time intervals to the format that the scheduling function expects

    vector<Schedule> optimalSchedules = weighted_interval_scheduling(schedules); // Find the optimal schedule for each floor

    cout << "Best Schedule for Each Floor" << endl;                             // Display the best schedule for each floor as wanted in the calico file
    for (const auto &s : optimalSchedules)
    {
        cout << s.floor << " --> Priority Gain: " << s.totalPriority << endl;
        for (const auto &room : s.rooms)
        {
            cout << s.floor << "\t" << room.name << "\t" << room.startTime << "\t" << room.endTime << endl;
        }
    }

    auto selectedItems = knapsack(items, total_budget);                         //call the knapsack function to find the selected items that can be purchased with the given budget
    cout << "Best Use of Budget" << endl;
    double totalValue = accumulate(selectedItems.begin(), selectedItems.end(), 0.0,
                                   [](double sum, const Item &item)
                                   { return sum + item.value; });               // Calculate the total value of the selected items

     
    double roundVal = ceil(totalValue * 10)/10;     // Round the total value to the nearest 0.1 to match calico
    cout << "Total Value --> " << roundVal << endl;

    // Display the selected items
    for (auto it = selectedItems.begin(); it != selectedItems.end(); ++it)
    {
        cout << it->name << endl;
    }

    return 0;
}
