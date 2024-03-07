/*Erblina Nivokazi
  150200917
  Project 2
  Introduction to Algorithms
*/
#include <iostream>
#include <vector>
#include <cmath>
#include <fstream>
#include <sstream>
#include <ctime>
#include <chrono>

// Struct to make the vector out of it
// I first implemented it differently by taking the value as a pair from the csv file but along the way I encountered many errors
// This way proved to be the safest
struct Population
{
    std::string city;
    int population;
};

// ****************** HEAP ************************
// MAX-HEAPIFY FUNCTION
void max_heapify(std::vector<Population> &A, int i, int heapSize);
// BUILD-MAX-HEAP FUNCTION
void build_max_heap(std::vector<Population> &A);
// HEAPSORT PROCEDURE
void heapsort(std::vector<Population> &A);

//******************** PRIORITY QUEUE OPERATIONS ************************

// MAX-HEAP-INSERT: Inserts a new element into the max-heap.
void max_heap_insert(std::vector<Population> &A, int num);

// HEAP-EXTRACT-MAX: Removes and returns the maximum element from the maxheap.
Population heap_extract_max(std::vector<Population> &A);

// HEAP-INCREASE-KEY: Increases the key of a specified element in the max-heap.
void heap_increase_key(std::vector<Population> &A, int i, int key);

// HEAP-MAXIMUM: Returns the maximum element from the max-heap without removing it.
Population heap_maximum(std::vector<Population> &A);

// ******************** D-ARY OPERATIONS ********************
// **Implementation of d-ary Heap Operations**
// **Extra Functions needed for d-ary implementations**
void daryMaxHeapify(std::vector<Population> &A, int i, int heapSize, int d);
void daryBuildMaxHeap(std::vector<Population> &A, int d);

// Height Calculation
int dary_calculate_height(int d, int n);
// EXTRACT-MAX Implementation
Population dary_extract_max(std::vector<Population> &A, int d);
// INSERT Implementation
void dary_insert_element(std::vector<Population> &A, Population new_element, int d);
// INCREASE-KEY Implementation
void dary_increase_key(std::vector<Population> &A, int i, int k, int d);

// File functions to read and write. These functions are the same as the ones I used in te first project
// But with some modifications because this time I'm using std::vector<Population> to take the data
std::vector<Population> readCsvFile(const std::string &name);
void writeCsvFile(const std::vector<Population> &data, const std::string &name);

// These functions help extract the values taken from the extra arguments given as i123/d123/k12 or parseK serves to extract city and k from input argument k_city_population
std::pair<char, int> parseInput(const std::string &input);
std::pair<std::string, int> parseK(const std::string &input);

int main(int argc, char *argv[])
{
    if (argc < 4) // The arguments cant be less than 4
    {
        std::cout << "Provide: [DataSetName].csv [wanted operation] [output.csv] [outputFileName].csv [optional arguments i/k/d]" << std::endl;
        return 1;
    }
    std::string dataset = argv[1];   // The first argument will give us the name of the dataset that will be used
    std::string operation = argv[2]; // Second argument from the command line gives us the operation that will be used
    std::string output = argv[3];    // Third argument gives us the name of the file to write our output into

    std::vector<Population> data = readCsvFile(dataset);
    int heapSize = data.size(); // Needed later in the main function

    auto startTime = std::chrono::high_resolution_clock::now(); // Measuring time
    // I created an if function to be able to get all the possible inputs from command line
    if (argc == 4)
    { // I decided to write these if statements based on how many inputs get received, if 4 then that means there are no optional arguments
      // So inside of this i made another if statement to go through the functions that dont need the optional values
        if (operation == "build_max_heap")
        {
            build_max_heap(data);
        }
        else if (operation == "heapsort")
        {
            heapsort(data);
        }
        else if (operation == "heap_extract_max")
        {
            Population max = heap_extract_max(data);
            std::cout << "Max of heap: " << max.city << " ; " << max.population << std::endl; // For max heap I thought it'd be necessary to also output the max value
            
            std::vector<Population> data_to_be_written;
            data_to_be_written.push_back(max);
            writeCsvFile(data_to_be_written,output);

            //I added these afterwards becuase before I was only outputting to the terminal the max and not writing it in the out1.csv
            //So now it writes to out1.csv
            auto endTime = std::chrono::high_resolution_clock::now();
                auto elapsedTime = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);

                std::cout << "Time taken to run operation '" << operation << "'"
                << ": [" << elapsedTime.count() << " ns]." << std::endl;
            return 0;
        }
        else if (operation == "heap_maximum")
        {
            Population max = heap_maximum(data);
            std::cout << "Max of heap: " << max.city << " ; " << max.population << std::endl;
            
            //I added these afterwards becuase before I was only outputting to the terminal the max and not writing it in the out1.csv
            //So now it writes to out1.csv
            std::vector<Population> data_to_be_written;
            data_to_be_written.push_back(max);
            writeCsvFile(data_to_be_written,output);

            auto endTime = std::chrono::high_resolution_clock::now();
                auto elapsedTime = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);

                std::cout << "Time taken to run operation '" << operation << "'"
                << ": [" << elapsedTime.count() << " ns]." << std::endl;
            return 0;
        }
        else
        {
            std::cout << "Please provide a valid operation name" << std::endl;
            std::cout << "Other possible error: Do not provide extra arguments for this operation";
            return 1;
        }
    }
    else if (argc == 5)
    { // This other if goes through the functions that take one optional value [i/d/k]
        if (operation == "max_heapify")
        {
            if (argv[4][0] == 'i')
            {                                                 // For extra protection I check the first letter in each optional value just to ensure that its the correct optional value for the function selected
                std::pair<char, int> i = parseInput(argv[4]); // In the case the correct optional value for max_heapify is i
                max_heapify(data, i.second, heapSize);
            }
            else
            {
                std::cout << "For this operation you must provide an extra optional argument of the form i{num}" << std::endl;
                return 1;
            }
        }
        else if (operation == "max_heap_insert")
        {
            if (argv[4][0] == 'k')
            {
                std::pair<std::string, int> k = parseK(argv[4]);
                Population new_key;
                new_key.city = k.first;
                new_key.population = k.second;
                data.push_back(new_key);
                max_heap_insert(data, k.second);
            }
            else
            {
                std::cout << "For this operation you must provide an extra optional argument of the form k_city_num" << std::endl;
            }
        }
        else if (operation == "dary_calculate_height")
        {
            if (argv[4][0] == 'd')
            {
                std::pair<char, int> d = parseInput(argv[4]);
                int height = dary_calculate_height(d.second, heapSize);
                std::cout << "Height of d-ary tree: " << height << std::endl;

                //I added these afterwards becuase before I was only outputting to the terminal the max and not writing it in the out1.csv
                //So now it writes to out1.csv
                Population height_write;
                height_write.city = "dary height";
                height_write.population = height;
                std::vector<Population> data_to_be_written;
                data_to_be_written.push_back(height_write);
                writeCsvFile(data_to_be_written,output);

                auto endTime = std::chrono::high_resolution_clock::now();
                auto elapsedTime = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);

                std::cout << "Time taken to run operation '" << operation << "'"
                << ": [" << elapsedTime.count() << " ns]." << std::endl;
                return 0;
            }
            else
            {
                std::cout << "For this operation you must provide an extra optional argument of the form d{num}" << std::endl;
                return 1;
            }
        }
        else if (operation == "dary_extract_max")
        {
            if (argv[4][0] == 'd')
            {
                std::pair<char, int> d = parseInput(argv[4]);
                Population max = dary_extract_max(data, d.second);
                std::cout << "Max of d-ary tree: " << max.city << " ; " << max.population << std::endl;
                
                //I added these afterwards becuase before I was only outputting to the terminal the max and not writing it in the out1.csv
                //So now it writes to out1.csv
                std::vector<Population> data_to_be_written;
                data_to_be_written.push_back(max);
                writeCsvFile(data_to_be_written,output); 

                auto endTime = std::chrono::high_resolution_clock::now();
                auto elapsedTime = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);

                std::cout << "Time taken to run operation '" << operation << "'"
                << ": [" << elapsedTime.count() << " ns]." << std::endl;
                return 0;
            }
            else
            {
                std::cout << "For this operation you must provide an extra optional argument of the form d{num}" << std::endl;
                return 1;
            }
        }
        else
        {
            std::cout << "Please provide a valid operation name" << std::endl;
            std::cout << "Other possible error: If the function name is correct the you must know that it takes only 1 extra optional argument";
            return 1;
        }
    }
    else if (argc == 6)
    { // For 6 arguments I made another if statement containing the functions or operation that require 6 arguments from the command line
        if (operation == "heap_increase_key")
        {
            std::pair<char, int> i = parseInput(argv[4]);
            std::pair<char, int> k = parseInput(argv[5]);
            if ((i.first == 'i') && (k.first == 'k'))
            {
                heap_increase_key(data, i.second, k.second);
            }
            else
            {
                std::cout << "For this operation you must provide extra optional arguments in the form of i{num} k{num}" << std::endl;
                return 1;
            }
        }
        else if (operation == "dary_insert_element")
        {
            std::pair<std::string, int> k = parseK(argv[4]);
            Population new_element;
            new_element.city = k.first;
            new_element.population = k.second;
            std::pair<char, int> d = parseInput(argv[5]);
            if ((d.first == 'd'))
            {
                dary_insert_element(data, new_element, d.second);
            }
            else
            {
                std::cout << "For this operation you must provide extra optional arguments in the form of k_city_num d{num}" << std::endl;
                return 1;
            }
        }
        else
        {
            std::cout << "Please provide a valid operation name";
            return 1;
        }
    }
    else if (argc == 7)
    {
        if (operation == "dary_increase_key")
        {
            if ((argv[4][0] == 'i') && (argv[5][0] == 'k') && (argv[6][0] == 'd'))
            {
                std::pair<char, int> i = parseInput(argv[4]);
                std::pair<std::string, int> k = parseK(argv[5]);
                std::pair<char, int> d = parseInput(argv[6]);
                dary_increase_key(data, i.second, k.second, d.second);
            }
            else
            {
                std::cout << "Please provide a correct input in the form i{num} k_city_num d{num}" << std::endl;
                return 1;
            }
        }
        else
        {
            std::cout << "Please provide a valid operation name";
            return 1;
        }
    }
    else
    {
        std::cout << "You must not provide more than 8 arguments" << std::endl;
        return 1;
    }

    auto endTime = std::chrono::high_resolution_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);

    std::cout << "Time taken to run operation '" << operation << "'"
              << ": [" << elapsedTime.count() << " ns]." << std::endl; // I output the elapsed time

    writeCsvFile(data, "out1.csv"); // Writing to the csv file

    return 0;
}

/********************************** Writing the functions *************************************/

void max_heapify(std::vector<Population> &A, int i, int heapSize)
{
    int leftNode = 2 * i + 1;  // 2i+1 gives me the left child of the node that we're currently using(its 2i in the slide but i adjusted it to my code)
    int rightNode = 2 * i + 2; // 2i+2 gives me the right child of the node that we're currently using(2i+1 in the slides)
    // int heapSize = A.size();  I decided to get heapSize as a parameter, since sometimes not the whole array is taken as a heap
    int largest = i; // i is assigned first as the largest and only changed if any of the children are bigger in size (check LeftNode/rightNode)
    if (leftNode <= heapSize && A[leftNode].population > A[largest].population)
    {
        largest = leftNode;
    }
    if (rightNode <= heapSize && A[rightNode].population > A[largest].population)
    {
        largest = rightNode;
    }
    if (largest != i)           // Checking whether any of the children is actually larger than i
    {                           // and if soswapping those numbers and applying the same function recursively to the
                                // child who inhereted the value of its parent
        Population temp = A[i]; // Swapping the objects accordingly with the child that has a larger population value
        A[i] = A[largest];
        A[largest] = temp;
        max_heapify(A, largest, heapSize); // Recursively calling the function
    }
}

void build_max_heap(std::vector<Population> &A)
{
    int heapSize = A.size() - 1; // taking heapSize helps us find the parent of the child
    // Applying max_heapify from bottom up
    for (int i = (heapSize / 2) - 1; i >= 0; i--) //(size of heap)/2 - 1 gives us the position of the childs parent
    {                                             // I cal max_heapify for all the parents until it reaches the position 0 in the heap, or root
        max_heapify(A, i, heapSize);
    }
}

void heapsort(std::vector<Population> &A)
{
    build_max_heap(A); // Making the max heap at first so that we have a max heap structure

    int length = A.size();
    for (int i = length; i >= 1; i--) // loop to take max value out of heap and put it in the end of the array
    {                                 // the max value is swapped with the leaf that is in the most right
        Population temp = A[i];
        A[i] = A[0];
        A[0] = temp;
        length--;
        max_heapify(A, 0, length); // max_heapify is called whenever he first position, which normally is the max value, is swapped with a smaller value from the end of the heap
    }
}

void max_heap_insert(std::vector<Population> &A, int num)
{
    Population new_element;
    new_element.city = " ";
    new_element.population = num;
    A.push_back(new_element); // Since I am using vector as my heap, to add to it I used push_back
    build_max_heap(A);        // Adjusting the heap accordingly after the new addition
}

Population heap_extract_max(std::vector<Population> &A)
{
    int heapSize = A.size();
    if (heapSize < 1) // Heap size cannot be under 0, this way we cannot extract anything from it
    {
        throw std::runtime_error("Heap Underflow");
    }
    build_max_heap(A);     // Building the max heap to ensure that the structure is in max heap position so as to extract A[0] (the maximum value)
    Population max = A[0]; // Extraction of the max value
    int last = heapSize - 1;
    A[0] = A[last]; // Replacing the max object with the last object of the heap
    A.pop_back();   // Removing the last value in the heap , in this case the maximum value

    heapSize--;                  // Heaps size changes due to the deletion of the maximum value
    max_heapify(A, 0, heapSize); // To fix the structure of the heap max_heapify is called for the first element of the heap
    return max;
}

void heap_increase_key(std::vector<Population> &A, int i, int key)
{

    build_max_heap(A); // To get the correct position of the heap, I build the max heap
    if (key < A[i].population) // Key cannot be smaller than the last value, then it would be decreasing and increasing
    {
        throw std::runtime_error("New key is smaller than the current key");
    }
    A[i].population = key; // Key changes its value
    while (i > 0)          // Loop to check whether or not the parent of this node is bigger or smaller, this way I readjust the position of the new key accordingly
    {
        int parent = (i - 1) / 2;
        if (A[i].population > A[parent].population)
        {
            Population temp = A[i]; // Swapping of the objects
            A[i] = A[parent];
            A[parent] = temp;
            i = parent;
        }
        else
        {
            break;
        }
    }
}

Population heap_maximum(std::vector<Population> &A)
{ // Implementation similar to heap_extract_max but here max is not erased
    int heapSize = A.size();
    if (heapSize < 1)
    {
        throw std::runtime_error("Heap Underflow");
    }
    build_max_heap(A);
    return A[0]; // In this function I only the return the max object of the heap, nothing changes from the structure
}

// I implemented the same maxHeapify function just with some modifications
// This was needed to implement the other functions for the d-ary part
void daryMaxHeapify(std::vector<Population> &A, int i, int heapSize, int d)
{
    int largest = i;
    for (int j = 1; j <= d; ++j)
    {                          // Instead of doing only two checks for left and right child now the function checks
        int child = d * i + j; // for however many children that the d parameter says the tree has
        if (child < heapSize && A[child].population > A[largest].population)
        {
            largest = child;
        }
    }

    if (largest != i) // This function stayed the same
    {
        Population temp = A[i];
        A[i] = A[largest];
        A[largest] = temp;
        daryMaxHeapify(A, largest, heapSize, d);
    }
}

// Another necessary function for implementation of the d-ary functions
void daryBuildMaxHeap(std::vector<Population> &A, int d)
{
    int heapSize = A.size();

    for (int i = ((heapSize - 1) - 1) / d; i >= 0; --i) // Here instead of 2 I used the value d
    {
        daryMaxHeapify(A, i, heapSize, d);
    }
}

int dary_calculate_height(int d, int n) // As mentioned in the homework I only take d and n as inputs/parameters
{
    int height = std::log(n * (d - 1) + 1) / std::log(d); // I got this formula online from the James Storer's An introduction to Data Structures and Algorithms

    return height;
}

Population dary_extract_max(std::vector<Population> &A, int d)
{
    int heapSize = A.size(); // In this function I used similar logic to the heap_extract_max function just made some readjustments max_heapify -> daryMaxHeapify
    if (heapSize < 1)
    {
        throw std::runtime_error("Heap Underflow");
    }
    daryBuildMaxHeap(A, d); // Ensuring the heap is a MaxHeap
    Population max = A[0];
    int last = heapSize - 1;
    A[0] = A[last];
    A.pop_back();

    daryMaxHeapify(A, 0, last, d);

    return max;
}

void dary_insert_element(std::vector<Population> &A, Population new_element, int d)
{
    // Insert the new element at the end
    A.push_back(new_element);
    daryBuildMaxHeap(A, d); // To ensure max structure of heap daryBuildMaxHeap is called
}

void dary_increase_key(std::vector<Population> &A, int i, int k, int d)
{
    // Similar to the heap_increase_key just with some adjustments
    daryBuildMaxHeap(A, d);
    if (k < A[i].population) // Key cannot be smaller than its previous value
    {
        throw std::runtime_error("New key is smaller than the current key");
    }

    A[i].population = k;
    while (i > 0)
    { // This for loop checks whether the parent of this key are now smaller than the new key
        int parent = (i - 1) / d;

        if (A[i].population > A[parent].population)
        {
            Population temp = A[i]; // Swapping the values if needed
            A[i] = A[parent];
            A[parent] = temp;
            i = parent;
        }
        else
        {
            break;
        }
    }
}

std::vector<Population> readCsvFile(const std::string &name)
{
    std::vector<Population> data; // Vector to return in the end

    std::ifstream file(name);
    if (!file.is_open())
    {                                                                 // Checking whether or not the file is open, this is a bit unnecessary since I manually open it, but it is good practice to write it in case errors arise
        std::cerr << "Error opening the file: " << name << std::endl; // Error to display
        return data;
    }

    std::string line;

    while (std::getline(file, line))
    {
        std::istringstream iss(line); // I'm using stream to make the data extraction
        std::string city;             // Initializing variables to store the extracted values
        int population;
        //
        if (std::getline(iss, city, ';') && (iss >> population))
        { // Extracting by differentiating the city and population by ;
            Population parse_element;
            parse_element.city = city;
            parse_element.population = population;
            data.push_back(parse_element);
        }
        else
        { // Good practice to write error to check if there is problem while parsing/extracting some lines of data form the csv
            std::cerr << "Error parsing the line: " << line << std::endl;
            continue;
        }
    }
    file.close();
    return data;
}

void writeCsvFile(const std::vector<Population> &data, const std::string &name)
{
    std::ofstream file(name);
    if (!file.is_open())
    { //Good practice to display error, if there is an unopen file, and we're trying to write into it

        std::cerr << "Could not open file: " << name << std::endl; 
        return;
    }
    for (const auto &entry : data) //Loop to enter the the data from the Population object vector
    {
        file << entry.city << " ; " << entry.population << std::endl;
    }
    file.close();
}

std::pair<char, int> parseInput(const std::string &input) // This function I created so as to extract the values given as extra input such as d123, k123 or i345
{
    std::pair<char, int> toot; // I created this pair to return it in the end as the extracted values that I'm looking for
    char character;            // I initialize the pair values so as to add into the char
    int number = 0;            // And also to add into 0, if I don't put 0 then I'll be adding to a strange number

    for (char ch : input) // This loop is used to go through the string and extract the needed parts
    {
        if (ch >= '0' && ch <= '9') // Here I check for the numeric part of the inpute
        {
            number = number * 10 + (ch - '0'); // Adding onto the already initialized number integer
            toot.second = number;
        }
        else // This is the character part and it's only one character
        {
            character = ch;
            toot.first = character;
        }
    }

    return toot; // return the pair
}

std::pair<std::string, int> parseK(const std::string &input) // This function is used to extract the values from the optiona input k_city_number
{
    std::pair<std::string, int> toot; // Similar to the extracting function here I initialize a pair to return in the end
    std::string city;
    int population = 0;

    int i = 2; // I start at the third character so I skip 'k_'
    while (input[i] != '\0')
    { // Conditional to check when I reach the end of the string

        if (input[i] >= '0' && input[i] <= '9') // Extracting the numerical part
        {
            population = population * 10 + (input[i] - '0');
        }
        else // Extracting the city name
        {
            if (input[i] != '_')
            { // If I don't put this condition then it will take the '_' character as part of the city name too
                city = city + input[i];
            }
        }
        i++;
    }
    toot.second = population;
    toot.first = city;
    return toot;
}
