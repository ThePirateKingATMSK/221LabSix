#ifndef HASHTABLE_OPEN_ADDRESSING_H
#define HASHTABLE_OPEN_ADDRESSING_H

#include <functional>
#include <iostream>
#include <vector>

template <class Key, class Hash=std::hash<Key>>
class HashTable {
public:
    // Member Types - do not modify
    using key_type = Key;
    using value_type = Key;
    using hash = Hash;
    using size_type = size_t;
    // you can write your code below this

private:
    struct cell {
        // Three states a cell can be, 0 = empty, 1 = occupied, 2= deleted
        int state;
        // Store the information from the key
        Key data;
        cell() {
            state = 0;
            data;
        }
    };

    int cellCount;
    int currentSize;
    float maxLoad;
    std::vector<cell> *table;

    bool isActive(int index);
    int findIndex(const key_type& key);

public:
    HashTable();
    HashTable(const HashTable& other);
    ~HashTable();
    HashTable& operator=(const HashTable& other);
    HashTable(size_type cells);

    bool is_empty() const;
    size_t size() const;
    size_t table_size() const;

    void make_empty();
    bool insert(const value_type& value);
    size_t remove(const key_type& key);

    bool contains(const key_type& key);
    size_t position(const key_type& key) const;

    void print_table(std::ostream& os=std::cout) const;

    // Optional
    // HashTable(HashTable&& other);
    // HashTable& operator=(HashTable&& other);
    // bool insert(value_type&& value);
};

// Default constructor, initializes a hash table with 11 buckets
template<class Key, class Hash>
HashTable<Key, Hash>::HashTable() {
    // Initialize our default values and create the hash table
    cellCount = 11;
    currentSize = 0;
    maxLoad = 1;
    table = new std::vector<cell>[cellCount];

    // Occupy the table with empty cells
    for (int i = 0; i < cellCount; i++) {
        table->push_back(cell());
    }
}

// Copy constructor, makes one has table identical to the other
template<class Key, class Hash>
HashTable<Key, Hash>::HashTable(const HashTable &other) {

    // Copy the variables over
    cellCount = other.cellCount;
    maxLoad = other.maxLoad;
    currentSize = other.currentSize;
    table = new std::vector<cell>[cellCount];

    for (int i = 0; i < cellCount; i++) {
        table->push_back(cell());
    }

    // No further action required if the other table is empty
    if (!other.is_empty()) {
        // Copy the values over
        for (int i = 0; i < other.cellCount; i++) {
            table->at(i).data = other.table->at(i).data;
            table->at(i).state = other.table->at(i).state;
        }
    }
}

// Destructor, deallocates any memory in use by the program
template<class Key, class Hash>
HashTable<Key, Hash>::~HashTable() {
    // We created a new vector, so we need to delete[]
    delete[] table;
}

// Copy assignment operator, used to copy hashtables whilst also checking for self assignment
template<class Key, class Hash>
HashTable<Key, Hash> &HashTable<Key, Hash>::operator=(const HashTable &other) {

    // Check for self assignment
    if (this == &other) {
        return *this;
    }

    // Copy the variables over
    cellCount = other.cellCount;
    maxLoad = other.maxLoad;
    currentSize = other.currentSize;
    table = new std::vector<cell>[cellCount];

    for (int i = 0; i < cellCount; i++) {
        table->push_back(cell());
    }

    // No further action required if the other table is empty
    if (!other.is_empty()) {
        // Copy the values over
        for (int i = 0; i < other.cellCount; i++) {
            table->at(i).data = other.table->at(i).data;
            table->at(i).state = other.table->at(i).state;
        }
    }
}

// Paramaterized constructor that will allow the user to set the amount of buckets
template<class Key, class Hash>
HashTable<Key, Hash>::HashTable(HashTable::size_type buckets) {

    // Set our bucket size and initialize the other variables
    cellCount = buckets;
    currentSize = 0;
    maxLoad = 1;
    table = new std::vector<cell>[cellCount];
}

// Function to see if the hashtable is empty
template<class Key, class Hash>
bool HashTable<Key, Hash>::is_empty() const {

    // Check that each cell in the table is set to empty
    for (int i = 0; i < table->size(); i++) {
        if (table->at(i).state != 0) {
            return false;
        }
    }

    // Return true if all cells are empty
    return true;
}

// Function to return the number of values currently in the table
template<class Key, class Hash>
size_t HashTable<Key, Hash>::size() const {
    return currentSize;
}

template<class Key, class Hash>
size_t HashTable<Key, Hash>::table_size() const {
    return cellCount;
}

// Function to completely empty out the hash table
template<class Key, class Hash>
void HashTable<Key, Hash>::make_empty() {

    // Mark all cells as empty
    for (int i = 0; i < table->size(); i++) {
        table->at(i).state = 0;
    }
}

// Function to insert a new value into the hashtable
template<class Key, class Hash>
bool HashTable<Key, Hash>::insert(const value_type &value) {

    // Get the index we should insert to
    int currentIndex = findIndex(value);

    // Check if the cell is active, because we can't insert duplicates
    if (isActive(currentIndex)) {
        return false;
    }

    // Update the cell's data and state
    table->at(currentIndex).data = value;
    table->at(currentIndex).state = 1;

    // Insert code to rehash here

}

template<class Key, class Hash>
bool HashTable<Key, Hash>::contains(const key_type &key) {

    // Iterate through the table to find the index of the given key, and check if it's active
    int index = -1;
    for (int i = 0; i < table->size(); i++) {
        if (table->at(i).data == key) {
            index = i;
        }
    }

    if (index != -1) {
        return isActive(index);
    } else {
        return false;
    }
}

// Helper function to determine if a given cell is active or not
template<class Key, class Hash>
bool HashTable<Key, Hash>::isActive(int index) {

    // Check if the given cell is active, and return true or false accordingly
    if (table->at(index).state != 1) {
        return false;
    }

    return true;
}

// Helper function to find the index to insert an object at.
template<class Key, class Hash>
int HashTable<Key, Hash>::findIndex(const key_type &key) {

    int offset = 1;
    size_t hashVal = Hash{}(key);
    hashVal = hashVal % cellCount;

    int currentIndex = hashVal;

    // Check to ensure the currentIndex isn't empty, nor does it contain the value we're looking to insert
    while(table->at(currentIndex).state != 0 && table->at(currentIndex).data != key) {
        currentIndex += offset;
        offset += 2;

        // When we need to loop back around to the beginning of the table
        if (currentIndex >= table->size()) {
            currentIndex -= table->size();
        }

        return currentIndex;
    }


    return 0;
}

#endif  // HASHTABLE_OPEN_ADDRESSING_H