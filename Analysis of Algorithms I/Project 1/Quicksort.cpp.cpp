#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>

//Declaring all the functions that will be used

int partition(std::vector<int> &a, int first, int last, int index, std::fstream &logFile);

void naiveQuickSort(std::vector<int> &a, int first, int last, std::fstream &logFile); // Part 1

int randomElement(int first, int last);

int medianOfThree(std::vector<int> &arr, int first, int last);

void quickSort(std::vector<int> &a, int first, int last, char strategy, std::fstream &logFile); // Part 2

void insertionSort(std::vector<int> &arr, int first, int last);

void hybridQuickSort(std::vector<int> &a, int first, int last, char strategy, int k, std::fstream &logFile); // Part 3

std::vector<std::pair<std::string, int>> readCsvFile(const std::string &name); // Function to read csv file

void writeCsvFile(const std::vector<std::pair<std::string, int>> &data, const std::string &name); // Function to write into csv file

std::vector<int> getPopulationValues(const std::vector<std::pair<std::string, int>> &data);                   // Dividing the vector pairs and getting only
                                                                                                              // the population data needed for sorting
void rearrangeDataVector(std::vector<std::pair<std::string, int>> &data, const std::vector<int> &population); // Function to sort the cities according to
                                                                                                              // the sorted populations

int main(int argc, char *argv[])
{
    std::fstream logFile;

    if ((argc < 5) || (argc > 6))   //The arguments can be both 6 and 5 but not less or more
    {                               //It can be 5 without the verbose condition and 6 with the verbose condition
        std::cout << "Provide: [DataSetName].csv [l|r|m] [threshold] [outputFileName].csv [v]" << std::endl;
        return 1;
    }

    std::string dataset = argv[1];
    char strategy = argv[2][0];
    int k = std::atoi(argv[3]);
    std::string outputFile = argv[4];
    if (argc == 6 && argv[5][0] == 'v')
        logFile.open("logfile.txt", std::ios::out | std::ios::app);

    std::vector<std::pair<std::string, int>> data = readCsvFile(dataset);
    std::vector<int> populations = getPopulationValues(data);

    auto startTime = std::chrono::high_resolution_clock::now(); //Measuring time

    //trying out both quicSort and hybridQuickSort
    // quickSort(populations, 0, populations.size() - 1, strategy, logFile);
    hybridQuickSort(populations, 0, populations.size() - 1, strategy, k, logFile);

    auto endTime = std::chrono::high_resolution_clock::now();
    auto elapsedTime = std::chrono::duration_cast<std::chrono::nanoseconds>(endTime - startTime);

    std::cout << "Time taken by QuickSort with pivot strategy '" << strategy << "' and threshold " << k
              << ": [" << elapsedTime.count() << " ns]." << std::endl;

    rearrangeDataVector(data, populations);

    writeCsvFile(data, outputFile);

    return 0;
}

//Implementation of the functions:

int partition(std::vector<int> &a, int first, int last, int index, std::fstream &logFile) // Partition function implemented
{
    int pivot = a[index]; // Pivot's index is given as a parameter to the function so I stored
                          // the pivot's value in int pivot to use for comparing later

    int temp = a[index]; // swapping pivot with the element in the end of the array
    a[index] = a[last];  // I added this functionality after the implementation of part 2 and part 3
    a[last] = temp;

    int i = first - 1;
    for (int j = first; j < last; ++j)
    {
        if (a[j] <= pivot)
        {
            ++i;

            temp = a[i];
            a[i] = a[j];
            a[j] = temp;
        }
    }

    temp = a[i + 1];    // Swapping the last element(pivot) to the position i+1 so its in between
    a[i + 1] = a[last]; // the numbers smaller than pivot and numbers larger than pivot
    a[last] = temp;

    if (logFile.is_open())                              //Here I check whether the logFile is open or not so that I can
    {                                                   //Write into it
        logFile << "Pivot: " << a[last] << " Array: ["; //I made the structure of the writing in logFile as it is mentioned in the project document
        
        int i = first;

        while (i <= last)
        {
            logFile << a[i];
            if (i != last)
            {
                logFile << ", ";
            }
            i++;
        }

        logFile << "]" << std::endl;
    }

    return i + 1; // returning the pivot position
}

void naiveQuickSort(std::vector<int> &a, int first, int last, std::fstream &logFile)
{
    if (first < last)
    {
        int pivotPos = partition(a, first, last, last, logFile);
        naiveQuickSort(a, first, pivotPos - 1, logFile);
        naiveQuickSort(a, pivotPos + 1, last, logFile);
    }
}

int randomElement(int first, int last) // Function that returns a random element index for the
{                                      // random pivot selection
    int random = first + rand() % (last - first + 1);

    return random;
}

int medianOfThree(std::vector<int> &arr, int first, int last) // Function that returns an index based on the median of three random numbers
{                                                             // definition as given in the homework pdf
    int r1 = first + rand() % (last - first + 1);             // Generating three random variables
    int r2 = first + rand() % (last - first + 1);
    int r3 = first + rand() % (last - first + 1);

    // Choosing in between the three randomly assigned indexs
    // choosing the median of them
    if ((r1 >= r2 && r1 <= r3) || (r1 >= r3 && r1 <= r2))
    {
        return r1;
    }
    else if ((r2 >= r1 && r2 <= r3) || (r2 >= r3 && r2 <= r1))
    {
        return r2;
    }
    else
    {
        return r3;
    }
}

void quickSort(std::vector<int> &a, int first, int last, char strategy, std::fstream &logFile) // Part 2 of quick sort implementation with pivoting strategy
{
    if (first < last)
    {
        int index;

        if (strategy == 'l') // Checking for pivoting strategy
        {
            index = last; // Pivot index takes the value of the Last Element
        }
        else if (strategy == 'r')
        {
            index = randomElement(first, last); // Pivot index takes value of a Random Element
        }
        else if (strategy == 'm')
        {
            index = medianOfThree(a, first, last); // Pivot index takes value of Median of Three random elements
        }

        int pivotPos = partition(a, first, last, index, logFile); // Continues th same way as in naive quick sort implementation
        quickSort(a, first, pivotPos - 1, strategy, logFile);
        quickSort(a, pivotPos + 1, last, strategy, logFile);
    }
}

void insertionSort(std::vector<int> &arr, int first, int last) // simple insertion sort implementation
{                                                              // Implemented it by refering to the pseudocode in the slides
    for (int i = first + 1; i <= last; ++i)
    {
        int key = arr[i];
        int j = i - 1;

        while (j >= first && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            j--;
        }

        arr[j + 1] = key;
    }
}

void hybridQuickSort(std::vector<int> &a, int first, int last, char strategy, int k, std::fstream &logFile) // Implementation of part 3 hybrid quick sort
{
    if (first < last)
    {
        if (last - first + 1 <= k) // if array contains a smaller value of elements than the
        {                          // number k then it gets sorted with insertion sort
            insertionSort(a, first, last);
        }
        else // otherwise it gets sorted with quick sort, this part contains the same code with quickSort
        {    // I didn't call the quickSort function here because the subarrays from hybridQuickSort will use insertion sort after their size gets smaller than k

            int index;

            if (strategy == 'l')
            {
                index = last;
            }
            else if (strategy == 'r')
            {
                index = randomElement(first, last);
            }
            else if (strategy == 'm')
            {
                index = medianOfThree(a, first, last);
            }

            int pivotPos = partition(a, first, last, index, logFile);
            hybridQuickSort(a, first, pivotPos - 1, strategy, k, logFile);
            hybridQuickSort(a, pivotPos + 1, last, strategy, k, logFile);
        }
    }
}

std::vector<std::pair<std::string, int>> readCsvFile(const std::string &name) // This function is implemented to read csv files and
{                                                                             // transfer them into pair vectors
                            
    std::ifstream file(name);
    std::vector<std::pair<std::string, int>> data;

    if (!file.is_open()) // Checks whether file is open or not
        throw std::runtime_error("Unable to read file");


    std::string line;
    while (std::getline(file, line)) //Loop to push the data into the vector pair
    {
        std::string city;
        int population;

        std::stringstream stream(line);
        std::getline(stream, city, ';'); //The data is differentiated by the ';' sign, first i get the city datta
        stream >> population;           //Then I get the population data

        data.push_back({city, population}); 
    }

    file.close();

    return data;
}

void writeCsvFile(const std::vector<std::pair<std::string, int>> &data, const std::string &name) // Function to write into csv file
{
    std::ofstream file(name);

    if (!file.is_open())
        throw std::runtime_error("Unable to write file");

    for (const auto &entry : data)
    {
        file << entry.first << ";" << entry.second << "\n";
    }

    file.close();
}

std::vector<int> getPopulationValues(const std::vector<std::pair<std::string, int>> &data) //Since the values I get are vector pairs
{                                                                                          //I cannot use those pairs to compare
    std::vector<int> population;                                                           //So with this function I create a vector of only the values
    for (const auto &entry : data)                                                         //that are population and cities
    {
        population.push_back(entry.second);
    }
    return population;
}

void rearrangeDataVector(std::vector<std::pair<std::string, int>> &data, const std::vector<int> &population) 
{//After sorting the vector holding the populations here I merge the sorted population values with their respective city names
    std::vector<std::pair<std::string, int>> sortedData(data.size());

    for (size_t i = 0; i < data.size(); ++i) //I start from 0 as to not ruin the order of how the cities are written in the original file
    {                                        //order is protected
        int j = 0;
        while (population[i] != data[j].second)
        {
            j++;
        }
        sortedData[i] = {data[j].first, population[i]};
    }

    data = sortedData;
}