#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include <algorithm>

std::mutex mtx; // Mutex for synchronizing access to the standard output stream

// Class to represent data
class Person {
public:
    int id;
    std::string name;

    // Constructor
    Person(int _id, std::string _name) : id(_id), name(_name) {}
};

// Function to sort data by id
void sortById(std::vector<Person>& data) {
    // Simulating processing of a large amount of data
    std::this_thread::sleep_for(std::chrono::seconds(2));

    // Sorting data by id
    std::sort(data.begin(), data.end(), [](const Person& a, const Person& b) {
        return a.id < b.id;
    });

    // Locking the mutex before output
    std::lock_guard<std::mutex> lock(mtx);
    std::cout << "Thread " << std::this_thread::get_id() << " sorted the data by id" << std::endl;
}

// Function to sort data by name
void sortByName(std::vector<Person>& data) {
    // Simulating processing of a large amount of data
    std::this_thread::sleep_for(std::chrono::seconds(2));

    // Sorting data by name
    std::sort(data.begin(), data.end(), [](const Person& a, const Person& b) {
        return a.name < b.name;
    });

    // Locking the mutex before output
    std::lock_guard<std::mutex> lock(mtx);
    std::cout << "Thread " << std::this_thread::get_id() << " sorted the data by name" << std::endl;
}

// Function to load data from a file
std::vector<Person> loadDataFromFile(const std::string& filename) {
    std::ifstream file(filename);
    std::vector<Person> data;

    int id;
    std::string name;

    // Reading data from the file
    while (file >> id >> name) {
        data.emplace_back(id, name);
    }

    return data;
}

// Function to print data
void printData(const std::vector<Person>& data) {
    for (const auto& person : data) {
        std::cout << "ID: " << person.id << ", Name: " << person.name << std::endl;
    }
}

int main() {
    std::string filename = "../data.txt"; // Path to the file with data

    // Loading data from the file
    std::vector<Person> data = loadDataFromFile(filename);

    // Creating threads to sort data by different fields
    std::thread threadByName(sortByName, std::ref(data));
    std::thread threadById(sortById, std::ref(data));

    // Waiting for the threads to finish their work
    threadById.join();
    threadByName.join();

    // Printing the sorted data
    printData(data);

    return 0;
}