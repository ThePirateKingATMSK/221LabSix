/*****************************************
** File:    hashtable_separate_chaining.h
** Project: CSCE 221 Lab 6 Spring 2022
** Author:  Joshua Hillis
** Date:    04/12/2022
** Section: 512
** E-mail:  joshuahillis292002@tamu.edu
**
** This is the header file for the hashtable with separate chaining. It allows the user
** to implement a separate chaining hash table for efficient data storage, with functionality to
** add items, remove, etc.
**
***********************************************/

#ifndef HASHTABLE_SEPARATE_CHAINING_H
#define HASHTABLE_SEPARATE_CHAINING_H

#include <vector>
#include <list>
#include <stdexcept>
#include <functional>
#include <iostream>


template <class Key, class Hash=std::hash<Key>>
class HashTable {
public:
    // Member Types - do not modify
    using key_type = Key;
    using value_type = Key;
    using hash = Hash;
    using size_type = size_t;

private:
    // A vector containing the lists
    std::vector<std::list<Key>> table;
    int currentSize;
    size_t bucketCount;
    int maxLoad;

    // Helper functions to determine if a number is prime, or find the next prime number
    int nextPrime(int count);
    bool isPrime(int count);

public:
    HashTable();
    HashTable(const HashTable& other);
    ~HashTable();
    HashTable& operator=(const HashTable& other);
    explicit HashTable(size_type buckets);
    [[nodiscard]] bool is_empty() const;
    size_t size() const;
    void make_empty();
    bool insert(const value_type& value);
    size_t remove(const key_type& key);
    bool contains(const key_type& key);
    size_t bucket_count() const;
    size_t bucket_size(size_t n) const;
    size_t bucket(const key_type& key) const;
    float load_factor() const;
    float max_load_factor() const;
    void max_load_factor(float mlf);
    void rehash(size_type count);
    void print_table(std::ostream& os=std::cout) const;

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
    bucketCount = 11;
    currentSize = 0;
    maxLoad = 1;
    table = std::vector<std::list<Key>>(11);
}


//-------------------------------------------------------
// Name: Copy Constructor
// Creates a new hashtable that is identical to another one
//---------------------------------------------------------
template<class Key, class Hash>
HashTable<Key, Hash>::HashTable(const HashTable &other){

    // Copy the variables over
    bucketCount = other.bucketCount;
    maxLoad = other.maxLoad;
    currentSize = other.currentSize;
    table = other.table;

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

    // Copy the variables and all of the lists
    bucketCount = other.bucketCount;
    maxLoad = other.maxLoad;
    currentSize = other.currentSize;

    table = other.table;
    return *this;
}


//-------------------------------------------------------
// Name: Parameterized Constructor
// Initializes a hashtable to have the given size for it's bucket count.
//---------------------------------------------------------
template<class Key, class Hash>
HashTable<Key, Hash>::HashTable(HashTable::size_type buckets) {

    buckets = nextPrime(buckets);
    // Set our bucket size and initialize the other variables
    bucketCount = buckets;
    currentSize = 0;
    maxLoad = 1;
    table = std::vector<std::list<Key>>(bucketCount);

}


//-------------------------------------------------------
// Name: is_empty
// Returns true or false depending on whether the hashtable is empty or not
//---------------------------------------------------------
template<class Key, class Hash>
bool HashTable<Key, Hash>::is_empty() const {

    if (currentSize == 0) {
        return true;
    }
    // If we've reached this code, then there are elements in the vector
    return false;
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
// Name: make_empty()
// Make the hash table empty, containing zero objects
//---------------------------------------------------------
template<class Key, class Hash>
void HashTable<Key, Hash>::make_empty() {

    // For all of the lists in our vector, clear that list
    for(unsigned int i = 0; i < table.size(); i++) {
        table.at(i).clear();
    }

    // Update our size
    currentSize = 0;
}


//-------------------------------------------------------
// Name: insert
// As long as a value is not a duplicate, this function inserts a given value into the hashtable
//---------------------------------------------------------
template<class Key, class Hash>
bool HashTable<Key, Hash>::insert(const value_type &value){

    size_t hash_value = Hash{}(value);
    hash_value = hash_value % bucketCount;

    std::list<Key> hashList = table.at(hash_value);

    // Use the find function to check all elements from the beginning to the end of the list
    if (std::find(std::begin(hashList), std::end(hashList), value) != std::end(hashList)){
        // Return false if there's a duplicate item
        return false;
    }

    // If we've passed the loop, we can insert the item
    table.at(hash_value).push_back(value);
    currentSize += 1;

    // Check if we need to rehash
    if (load_factor() > maxLoad) {
        rehash(bucketCount * 2);
    }

    return true;
}


//-------------------------------------------------------
// Name: remove
// removes a cell with the given key from the hashtable, or does nothing if the key doesn't exist.
//---------------------------------------------------------
template<class Key, class Hash>
size_t HashTable<Key, Hash>::remove(const key_type &key) {

    // Hash the key for use
    size_t hash_value = Hash{}(key);
    hash_value = hash_value % bucketCount;

    auto & hashList = table.at(hash_value);
    // Use the find function to check all elements from the beginning to the end of the list
    auto itr = std::find(std::begin(hashList), std::end(hashList), key);

    if (itr == std::end(hashList)) {
        // Return 0, since we didn't remove anything
        return 0;
    }

    // Erase the object and update the current size
    currentSize -= 1;
    hashList.erase(itr);
    return 1;
}


//-------------------------------------------------------
// Name: contains
// Returns true or false depending on whether the key exists in the hashtable or not
//---------------------------------------------------------
template<class Key, class Hash>
bool HashTable<Key, Hash>::contains(const key_type &key) {

    // Hash the key for use
    size_t hash_value = Hash{}(key);
    hash_value = hash_value % bucketCount;

    key_type other_value;
    std::list<Key> hashList = table.at(hash_value);


    if (std::find(std::begin(hashList), std::end(hashList), key) != std::end(hashList)){
        // Return true if we find the item in that list
        return true;
    }
    // If we make it past the above expression, then we've found the element
    return false;
}


//-------------------------------------------------------
// Name: bucket_count
// Returns the number of buckets in the hashtable
//---------------------------------------------------------
template<class Key, class Hash>
size_t HashTable<Key, Hash>::bucket_count() const {
    return bucketCount;
}


//-------------------------------------------------------
// Name: bucket_size
// Returns the number of elements inside of a bucket
//---------------------------------------------------------
template<class Key, class Hash>
size_t HashTable<Key, Hash>::bucket_size(size_t n) const {
    // Check if the index is within bounds
    if (n >= bucketCount || n < 0) {
        throw std::out_of_range("Bucket index is out of range!");
    }

    std::list<Key> hashList = table.at(n);

    // Return the size (amount of items) in that bucket
    return hashList.size();
}


//-------------------------------------------------------
// Name: bucket
// Return the bucket a key belongs to, or would belong to
//---------------------------------------------------------
template<class Key, class Hash>
size_t HashTable<Key, Hash>::bucket(const key_type &key) const {

    // Hash the key for use
    size_t hash_value = Hash{}(key);
    hash_value = hash_value % bucketCount;

    std::list<key_type> hashList = table.at(hash_value);

    key_type other_value;

    // Grab the key from each index of the list and check it against the parameter
    while (!hashList.empty()) {

        // Use the keys from our shallow copies
        other_value = hashList.front();
        // Pop the front after we evaluate it to iterate through our local copy
        hashList.pop_front();

        if (key == other_value) {
            return hash_value;
        }
    }

    // If we make it past the above expression, then we've found the element
    return hash_value;
}

//-------------------------------------------------------
// Name: load_factor
// Return the current load factor of the hash table
//---------------------------------------------------------
template<class Key, class Hash>
float HashTable<Key, Hash>::load_factor() const {
    // bucketCount / currentSize to calculate the current load factor
    if (currentSize == 0) {
        return 0;
    }

    // Return the calculation if we're not dividing by zero.

    return float(currentSize) / float(bucketCount);
}

//-------------------------------------------------------
// Name: max_load_factor
// Return the maximum load factor for the table
//---------------------------------------------------------
template<class Key, class Hash>
float HashTable<Key, Hash>::max_load_factor() const {
    return maxLoad;
}

//-------------------------------------------------------
// Name: max_load_factor
// A function to set a new maximum load factor for the hashtable
//---------------------------------------------------------
template<class Key, class Hash>
void HashTable<Key, Hash>::max_load_factor(float mlf) {

    if (mlf == 0 || mlf < 0) {
        throw std::invalid_argument("Invalid maximum load factor");
    }
    maxLoad = mlf;

    // Check if we've exceeded our new load factor
    if (load_factor() > maxLoad) {
        rehash(bucketCount * 2);
    }
}

//-------------------------------------------------------
// Name: rehash
// Adjusts the size and re-inserts cells from the original hashmap, to reduce the load factor.
//---------------------------------------------------------
template<class Key, class Hash>
void HashTable<Key, Hash>::rehash(HashTable::size_type count) {

    std::vector<std::list<Key>> oldTable = table;

    table.resize(nextPrime(count));
    bucketCount = nextPrime(count);
    /*
    make_empty();

    for (unsigned int i = 0; i < oldTable.size(); i++) {
        int tmpSize = oldTable.at(i).size();
        for (int j = 0; j < tmpSize; j++) {
            insert(oldTable.at(i).front());
            oldTable.at(i).pop_front();
        }
    }
*/
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

    std::list<Key> tmpList;

    // Loop through the array, and print the contents of each cell, if it's active
    for (unsigned int i = 0; i < table.size(); i++) {
        tmpList = table.at(i);
        if (!tmpList.empty()) {
            os << "[ " << toascii(i) << " ]\n";
            os << "{ \n";
            while (!tmpList.empty()) {
                os << "   " << (tmpList.front()) << "\n";
                tmpList.pop_front();
            }
            os << "} \n";
        }
    }
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
    if (isPrime(count)) {
        return count;
    }

    int returnPrime = count;

    // Increment until we find the next prime number
    while (true) {
        returnPrime += 1;
        if (isPrime(returnPrime)) {
            return returnPrime;
        }
    }
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

#endif  // HASHTABLE_SEPARATE_CHAINING_H