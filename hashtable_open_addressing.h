/*****************************************
** File:    hashtable_open_addressing.h
** Project: CSCE 221 Lab 6 Spring 2022
** Author:  Joshua Hillis
** Date:    04/12/2022
** Section: 512
** E-mail:  joshuahillis292002@tamu.edu
**
** This is the header file for the hashtable with open addressing. It allows the user
** to implement an open addressing hash table for efficient data storage, with functionality to
** add items, remove items, etc.
**
***********************************************/

#ifndef HASHTABLE_OPEN_ADDRESSING_H
#define HASHTABLE_OPEN_ADDRESSING_H

#include <functional>
#include <iostream>
#include <vector>

template<class Key, class Hash=std::hash<Key>>
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

        // Constructor for the cell, initializes to empty
        cell() {
            state = 0;
        }
    };

    int cellCount;
    int currentSize;
    float maxLoad;
    std::vector<cell> table;

    bool isActive(int index);

    void rehash(size_type count);

    float loadFactor() const;

    bool isPrime(int count);

    int nextPrime(int count);

public:
    HashTable();

    HashTable(const HashTable &other);

    ~HashTable();

    HashTable &operator=(const HashTable &other);

    HashTable(size_type cells);

    bool is_empty() const;

    size_t size() const;

    size_t table_size() const;

    void make_empty();

    bool insert(const value_type &value);

    size_t remove(const key_type &key);

    bool contains(const key_type &key);

    size_t position(const key_type &key) const;

    void print_table(std::ostream &os = std::cout) const;

    // Optional
    // HashTable(HashTable&& other);
    // HashTable& operator=(HashTable&& other);
    // bool insert(value_type&& value);
};

//-------------------------------------------------------
// Name: Default Constructor
// Initializes a new hashtable
//---------------------------------------------------------
template<class Key, class Hash>
HashTable<Key, Hash>::HashTable() {
    // Initialize our default values and create the hash table
    cellCount = 11;
    currentSize = 0;
    maxLoad = 0.5;
    table = std::vector<cell>();

    // Occupy the table with empty cells
    for (int i = 0; i < cellCount; i++) {
        table.push_back(cell());
    }
}

//-------------------------------------------------------
// Name: Copy Constructor
// Creates a new hashtable that is identical to another one
//---------------------------------------------------------
template<class Key, class Hash>
HashTable<Key, Hash>::HashTable(const HashTable &other) {

    // Copy the variables over
    cellCount = other.cellCount;
    maxLoad = other.maxLoad;
    currentSize = other.currentSize;
    table = std::vector<cell>();

    for (int i = 0; i < cellCount; i++) {
        table.push_back(cell());
    }

    // No further action required if the other table is empty
    if (!other.is_empty()) {
        // Copy the values over
        for (int i = 0; i < other.cellCount; i++) {
            cell tmpCell;
            tmpCell.data = other.table.at(i).data;
            tmpCell.state = other.table.at(i).state;
            table.push_back(tmpCell);
        }
    }
}

//-------------------------------------------------------
// Name: Destructor
// Deletes the hashtables
//---------------------------------------------------------
template<class Key, class Hash>
HashTable<Key, Hash>::~HashTable() {}

//-------------------------------------------------------
// Name: Equals operator
// Allows us to set hashtables equal to one another and copy them that way.
//---------------------------------------------------------
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
    table = std::vector<cell>(cellCount);
    table = other.table;

    return *this;
}

//-------------------------------------------------------
// Name: Parameterized Constructor
// Initializes a hashtable to have the given size for it's cell count.
//---------------------------------------------------------
template<class Key, class Hash>
HashTable<Key, Hash>::HashTable(HashTable::size_type cells) {

    // Set our cell size and initialize the other variables
    cellCount = cells;
    currentSize = 0;
    maxLoad = 0.5;
    table = std::vector<cell>(cellCount);

}

//-------------------------------------------------------
// Name: is_empty
// Returns true or false depending on whether the hashtable is empty or not
//---------------------------------------------------------
template<class Key, class Hash>
bool HashTable<Key, Hash>::is_empty() const {

    // Check to see if the size is 0
    if (currentSize != 0) {
        return false;
    }

    // Return true if all cells are empty
    return true;
}

//-------------------------------------------------------
// Name: size
// Returns the number of items currently stored inside of the hashtable
//---------------------------------------------------------
template<class Key, class Hash>
size_t HashTable<Key, Hash>::size() const {
    return currentSize;
}

//-------------------------------------------------------
// Name: table_size
// Returns the number of cells in the hashtable
//---------------------------------------------------------
template<class Key, class Hash>
size_t HashTable<Key, Hash>::table_size() const {
    return cellCount;
}

//-------------------------------------------------------
// Name: make_empty()
// Sets all of the cells in the hashtable to empty
//---------------------------------------------------------
template<class Key, class Hash>
void HashTable<Key, Hash>::make_empty() {

    // Mark all cells as empty
    for (unsigned int i = 0; i < table.size(); i++) {
        table.at(i).state = 0;
    }
    currentSize = 0;
}

//-------------------------------------------------------
// Name: insert
// As long as a value is not a duplicate, this function inserts a given value into the hashtable
//---------------------------------------------------------
template<class Key, class Hash>
bool HashTable<Key, Hash>::insert(const value_type &value) {

    // Get the index we should insert to
    int currentIndex = position(value);

    // Check if the cell is active, because we can't insert duplicates
    if (isActive(currentIndex)) {
        return false;
    }

    // Update the cell's data and state
    table.at(currentIndex).data = value;
    table.at(currentIndex).state = 1;
    currentSize += 1;

    if (loadFactor() > maxLoad) {
        rehash(cellCount * 2);
    }

    return true;
}


//-------------------------------------------------------
// Name: remove
// removes a cell with the given key from the hashtable, or does nothing if the key doesn't exist.
//---------------------------------------------------------
template<class Key, class Hash>
size_t HashTable<Key, Hash>::remove(const key_type &key) {

    // Check to ensure the element isn't already deleted / not present
    int currentIndex = position(key);
    if (!isActive(currentIndex)) {
        return 0;
    }

    // Set the cell's state to deleted
    table.at(currentIndex).state = 2;
    currentSize -= 1;

    // Return 1, since we've removed one element
    return 1;
}


//-------------------------------------------------------
// Name: contains
// Returns true or false depending on whether the key exists in the hashtable or not
//---------------------------------------------------------
template<class Key, class Hash>
bool HashTable<Key, Hash>::contains(const key_type &key) {

    if (is_empty()) {
        return false;
    }
    // Iterate through the table to find the index of the given key, and check if it's active
    for (unsigned int i = 0; i < table.size(); i++) {
        // If we've found the data, we need to check if it's active or not
        if (table.at(i).state == 1 && table.at(i).data == key) {
            return true;
        }
    }

    return false;
}


//-------------------------------------------------------
// Name: isActive
// Returns true or false depending on whether the given cell is active or not
//---------------------------------------------------------
template<class Key, class Hash>
bool HashTable<Key, Hash>::isActive(int index) {

    // Check if the given cell is active, and return true or false accordingly
    if (table.at(index).state != 1) {
        return false;
    }

    return true;
}


//-------------------------------------------------------
// Name: position
// Either returns the index of the key's position, or where the key should be inserted.
//---------------------------------------------------------
template<class Key, class Hash>
size_t HashTable<Key, Hash>::position(const key_type &key) const {

    int offset = 1;
    size_t hashVal = Hash{}(key);
    hashVal = hashVal % cellCount;

    size_t currentIndex = hashVal;

    // Check to ensure the currentIndex isn't empty, nor does it contain the value we're looking to insert
    while (table.at(currentIndex).state != 0 && table.at(currentIndex).data != key) {
        currentIndex += offset;
        offset += 2;

        // When we need to loop back around to the beginning of the table
        if (currentIndex >= table.size()) {
            currentIndex -= table.size();
        }
    }
    return currentIndex;
}


//-------------------------------------------------------
// Name: rehash
// Adjusts the size and re-inserts cells from the original hashmap, to reduce the load factor.
//---------------------------------------------------------
template<class Key, class Hash>
void HashTable<Key, Hash>::rehash(size_type count) {

    std::vector<cell> oldTable = table;

    make_empty();
    table.resize(nextPrime(count));
    currentSize = 0;
    cellCount = nextPrime(count);

    while (!oldTable.empty()) {
        if (oldTable.back().state == 1) {
            insert(oldTable.back().data);
        }
        oldTable.pop_back();
    }
}

//-------------------------------------------------------
// Name: loadFactor
// Calculates the load factor on the current hashtable
//---------------------------------------------------------
template<class Key, class Hash>
float HashTable<Key, Hash>::loadFactor() const {
    if (cellCount == 0) {
        return 0.0;
    }
    return (float(currentSize) / float(cellCount));
}


//-------------------------------------------------------
// Name: isPrime
// Determines whether a given integer is prime or not
//---------------------------------------------------------
template<class Key, class Hash>
bool HashTable<Key, Hash>::isPrime(int count) {

    // Check to get simple cases out of the way first
    if (count == 2 || count == 3) {
        return true;
    }

    // Check for divisibility / count being one before we enter the loop, to save time
    if (count <= 1 || count % 2 == 0 || count % 3 == 0) {
        return false;
    }

    // Iterate through the square root of count, checking for factors
    for (int i = 5; i * i <= count; i += 6) {
        if (count % i == 0 || count % (i + 2) == 0) {
            return false;
        }
    }

    // If we're here, then the number is prime.
    return true;
}


//-------------------------------------------------------
// Name: nextPrime
// Finds the next prime number above a given integer
//---------------------------------------------------------
template<class Key, class Hash>
int HashTable<Key, Hash>::nextPrime(int count) {

    // Handle base case / negative inputs
    if (count <= 1) {
        return 2;
    }

    int returnPrime = count;

    while (true) {
        if (isPrime(returnPrime)) {
            return returnPrime;
        }
        returnPrime += 1;
    }

    return returnPrime;
}

//-------------------------------------------------------
// Name: print_table
// Outputs the contents of the hashmap to the terminal
//---------------------------------------------------------
template<class Key, class Hash>
void HashTable<Key, Hash>::print_table(std::ostream &os) const {
    if (is_empty()) {
        std::cout << "<empty>\n";
    }

    // Loop through the array, and print the contents of each cell, if it's active
    for (unsigned int i = 0; i < table.size(); i++) {
        if ((!table.empty()) && table.at(i).state == 1) {
            os << toascii(i) << ": " << (table.at(i).data) << "\n";
        }
    }
}

#endif  // HASHTABLE_OPEN_ADDRESSING_H