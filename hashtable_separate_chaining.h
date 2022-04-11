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
    // you can write your code below this

private:
    // A vector containing the lists
    std::vector<std::list<Key>> *table;
    int currentSize;
    int bucketCount;
    int maxLoad;

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
        table[i] = std::list<Key>();
    }
}

// Copy constructor, makes one has table identical to the other
template<class Key, class Hash>
HashTable<Key, Hash>::HashTable(const HashTable &other) {

    // Copy the variables over
    bucketCount = other.buckets;
    maxLoad = other.maxLoad;
    currentSize = other.currentSize;
    table = new std::vector<std::list<Key>>[bucketCount];

    for (int i = 0; i < other.buckets; i++) {
        // Use the list's copy assignment to copy the whole list
        table[i].operator=(other.table[i]);
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

    for (int i = 0; i < other.buckets; i++) {
        // Use the list's copy assignment to copy the whole list
        table[i].operator=(other.table[i]);
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
        if (table[i].empty()) {
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
}

// Inserts the given value into the hash table, and rehashes if the maximum load factor is exceeded
// WORKING
template<class Key, class Hash>
bool HashTable<Key, Hash>::insert(const value_type &value) {

    size_t hash_value = Hash{}(value);
    hash_value = hash_value % bucketCount;

    // Need to initialize all of the lists in the constructors too
    auto & hashList = table->at(hash_value);
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
        rehash(currentSize);
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

    // Check the list in each bucket for the given key
    for (int i = 0; i < bucketCount; i++) {
        if (table[i].find(table[i].begin(), table[i].end(), key)) {
            return true;
        }
    }
    // If we make it out of that loop, we didn't find the element so return false
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
    return table[n].size();
}

// Function that returns the index of the bucket containing the key, or the bucket that would contain it if it existed.
template<class Key, class Hash>
size_t HashTable<Key, Hash>::bucket(const key_type &key) const {
    return 0;
}

// Function to calculate and return the current load factor
template<class Key, class Hash>
float HashTable<Key, Hash>::load_factor() const {
    // bucketCount / currentSize to calculate the current load factor
    if (currentSize == 0) {
        return 0;
    }

    // Return the calculation if we're not dividing by zero.
    return (float(bucketCount) / currentSize);
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
    if (((float(bucketCount)) / currentSize) > maxLoad) {
        rehash(currentSize);
    }
}

// Function to rehash the table when necessary
template<class Key, class Hash>
void HashTable<Key, Hash>::rehash(HashTable::size_type count) {
}

template<class Key, class Hash>
void HashTable<Key, Hash>::print_table(std::ostream &os) const {
}

#endif  // HASHTABLE_SEPARATE_CHAINING_H