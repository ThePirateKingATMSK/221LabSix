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
    std::vector<std::list<Key>> *table;
    int currentSize;
    int bucketCount;
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

// Default constructor, initializes a hash table with 11 buckets
template<class Key, class Hash>
HashTable<Key, Hash>::HashTable() {
    // Initialize our default values and create the hash table
    bucketCount = 11;
    currentSize = 0;
    maxLoad = 1;
    table = new std::vector<std::list<Key>>[bucketCount];

    // Initialize the lists within the vector
    for (int i = 0; i < bucketCount; i++) {
        table->push_back(std::list<Key>());
    }
}

// Copy constructor, makes one has table identical to the other
template<class Key, class Hash>
HashTable<Key, Hash>::HashTable(const HashTable &other){

    // Copy the variables over
    bucketCount = other.bucketCount;
    maxLoad = other.maxLoad;
    currentSize = other.currentSize;
    table = new std::vector<std::list<Key>>[bucketCount];

    for (int i = 0; i < other.bucketCount; i++) {
        // Use the list's copy assignment to copy the whole list
        table->push_back(other.table->at(i));
    }
}

template<class Key, class Hash>
HashTable<Key, Hash>::~HashTable() {
    // We created a vector of lists, so we need to delete[]
    delete[] table;
}

// Copy assignment operator, used to copy hashtables whilst also checking for self assignment
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

    for (int i = 0; i < other.bucketCount; i++) {
        // Use the list's copy assignment to copy the whole list
        table->push_back(other.table->at(i));
    }
    return *this;
}

// Paramaterized constructor that will allow the user to set the amount of buckets
template<class Key, class Hash>
HashTable<Key, Hash>::HashTable(HashTable::size_type buckets) {

    // Set our bucket size and initialize the other variables
    bucketCount = buckets;
    currentSize = 0;
    maxLoad = 1;
    table = new std::vector<std::list<Key>>[bucketCount];

    // Initialize the lists within the vector
    for (int i = 0; i < bucketCount; i++) {
        table->push_back(std::list<value_type>());
    }
}

// Function to see if the hashtable is empty
template<class Key, class Hash>
bool HashTable<Key, Hash>::is_empty() const {

    // Iterate through each bucket and see if it contains anything, if it does, return false
    for (int i = 0; i < bucketCount; i++){
        if (table->at(i).empty()) {
            return false;
        }
    }
    // If we've reached this code, then none of the buckets had a list
    return true;
}

// Function to return the number of values currently in the table
template<class Key, class Hash>
size_t HashTable<Key, Hash>::size() const {
    return currentSize;
}

// Function to completely empty out the hash table
template<class Key, class Hash>
void HashTable<Key, Hash>::make_empty() {

    // For all of the lists in our vector, clear that list
    for(int i = 0; i < table->size(); i++) {
        table->at(i).clear();
    }
    currentSize = 0;
}

// Inserts the given value into the hash table, and rehashes if the maximum load factor is exceeded
// WORKING
template<class Key, class Hash>
bool HashTable<Key, Hash>::insert(const value_type &value) {

    size_t hash_value = Hash{}(value);
    hash_value = hash_value % bucketCount;

    std::list<Key> hashList;

    // Check if the table exists at that index, and use an empty list if it doesn't.
    if (table->size() > hash_value) {
        hashList = table->at(hash_value);
    } else {
        hashList = std::list<Key>();
    }
    // Use the find function to check all elements from the beginning to the end of the list
    if (std::find(std::begin(hashList), std::end(hashList), value) != std::end(hashList)){
        // Return false if there's a duplicate item
        return false;
    }

    // If we've passed the loop, we can insert the item
    table->at(hash_value).push_back(value);
    currentSize += 1;

    // Check if we need to rehash
    if (load_factor() > maxLoad) {
        rehash(bucketCount * 2);
    }

    return true;
}

// Checks if an element exists in a hash table and removes it if it does, or does nothing if it's not present
template<class Key, class Hash>
size_t HashTable<Key, Hash>::remove(const key_type &key) {

    // Hash the key for use
    size_t hash_value = Hash{}(key);
    hash_value = hash_value % bucketCount;

    auto & hashList = table->at(hash_value);
    // Use the find function to check all elements from the beginning to the end of the list
    auto itr = std::find(std::begin(hashList), std::end(hashList), key);

    if (itr == std::end(hashList)) {
        // Return 0, since we didn't remove anything
        return 0;
    }

    // Erase the object and update the current size
    hashList.erase(itr);
    currentSize -= 1;
    return 1;
}

// Returns true or false depending on whether the hashtable contains the given value or not
template<class Key, class Hash>
bool HashTable<Key, Hash>::contains(const key_type &key) {

    // Hash the key for use
    size_t hash_value = Hash{}(key);
    hash_value = hash_value % bucketCount;

    key_type other_value;
    std::list<Key> hashList = table->at(hash_value);

    while (!hashList.empty()) {
        if (key == hashList.front()) {
            return true;
        }
        hashList.pop_front();
    }
    // If we make it past the above expression, then we've found the element
    return false;
}

// Function to return the number of buckets in a table
// WORKING
template<class Key, class Hash>
size_t HashTable<Key, Hash>::bucket_count() const {
    return bucketCount;
}

// Function to return the number of items in a given bucket
template<class Key, class Hash>
size_t HashTable<Key, Hash>::bucket_size(size_t n) const {
    // Check if the index is within bounds
    if (n >= bucketCount) {
        throw std::out_of_range("Bucket index is out of range!");
    }

    // Return the size (amount of items) in that bucket
    return table->at(n).size();
}

// Function that returns the index of the bucket containing the key, or the bucket that would contain it if it existed.
template<class Key, class Hash>
size_t HashTable<Key, Hash>::bucket(const key_type &key) const {
    // Hash the key for use
    size_t hash_value = Hash{}(key);
    hash_value = hash_value % bucketCount;

    std::list<key_type> hashList = table->at(hash_value);

    key_type other_value;

    // Grab the key from each index of the list and check it against the parameter
    for (int i = 0; i < hashList.size(); i++) {

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

// Function to calculate and return the current load factor
template<class Key, class Hash>
float HashTable<Key, Hash>::load_factor() const {
    // bucketCount / currentSize to calculate the current load factor
    if (currentSize == 0) {
        return 0;
    }

    // Return the calculation if we're not dividing by zero.
    return (float(currentSize) / float(bucketCount));
}

// Function to return the maximum load for that hashtable
template<class Key, class Hash>
float HashTable<Key, Hash>::max_load_factor() const {
    return maxLoad;
}

// Function to set a new maximum load factor, and rehash if necessary
template<class Key, class Hash>
void HashTable<Key, Hash>::max_load_factor(float mlf) {
    maxLoad = mlf;

    // Check if we've exceeded our new load factor
    if (load_factor() > maxLoad) {
        rehash(bucketCount * 2);
    }
}

// Function to rehash the table when necessary
template<class Key, class Hash>
void HashTable<Key, Hash>::rehash(HashTable::size_type count) {

    // Temp vector to store the lists with keys, and a list containing each value from the hashTable
    std::vector<std::list<Key>> copyVector;
    std::list<Key> masterList;
    std::list<Key> tmpList;

    // Loop to create the master list containing all of our elements
    for (unsigned int i = 0; i < table->size(); i++) {
        copyVector.push_back(table->at(i));
        tmpList = table->at(i);
        for (unsigned int j = 0; j < tmpList.size(); j++) {
            masterList.push_front(tmpList.front());
            tmpList.pop_front();
        }
    }
}

template<class Key, class Hash>
void HashTable<Key, Hash>::print_table(std::ostream &os) const {

    if (is_empty()) {
        std::cout << "<empty>\n";
    }

    std::list<Key> tmpList;
    // Loop through the array, and print the contents of each cell, if it's active
    for (unsigned int i = 0; i < table->size(); i++) {
        tmpList = table->at(i);
        if ((!tmpList.empty())) {
            os << "[ " << toascii(i) << " ]\n";
            os << "{ \n";
            while (!tmpList.empty()) {
                os << "   " << tmpList.front() << "\n";
                tmpList.pop_front();
            }
            os << "} \n";

        }
    }
}


template<class Key, class Hash>
int HashTable<Key, Hash>::nextPrime(int count) {

    // Handle base case / negative inputs
    if (count <= 1) {
        return 2;
    }

    int returnPrime = count;
    bool foundPrime = false;

    while (true) {
        if (isPrime(returnPrime)) {
            return returnPrime;
        }
        returnPrime += 1;
    }
}

template<class Key, class Hash>
bool HashTable<Key, Hash>::isPrime(int count) {

    if (count % 2 == 0 || count <= 1) {
        return false;
    }

    // Already checked for divisibility by 2, so we can iterate through odd numbers now
    for (int i = 3; i < (count / 2); i+=2) {
        if (count % i == 0) {
            // If this is true, we've found a factor, meaning the number isn't prime
            return false;
        }
    }

    // If we make it through the loop, then the number is prime
    return true;
}

#endif  // HASHTABLE_SEPARATE_CHAINING_H