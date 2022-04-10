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
    int buckets;
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
    buckets = 11;
    currentSize = 0;
    maxLoad = 1;
    table = new std::vector<std::list<Key>>[buckets];
}

// Copy constructor, makes one has table identical to the other
template<class Key, class Hash>
HashTable<Key, Hash>::HashTable(const HashTable &other) {

    // Copy the variables over
    buckets = other.buckets;
    maxLoad = other.maxLoad;
    currentSize = other.currentSize;

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
    buckets = other.buckets;
    maxLoad = other.maxLoad;
    currentSize = other.currentSize;

    for (int i = 0; i < other.buckets; i++) {
        // Use the list's copy assignment to copy the whole list
        table[i].operator=(other.table[i]);
    }
}

// Paramaterized constructor that will allow the user to set the amount of buckets
template<class Key, class Hash>
HashTable<Key, Hash>::HashTable(HashTable::size_type buckets) {
    // Set our bucket size and initialize the other variables
    buckets = buckets;
    currentSize = 0;
    maxLoad = 1;
    table = new std::vector<std::list<Key>>[buckets];

}

// Function to see if the hashtable is empty
template<class Key, class Hash>
bool HashTable<Key, Hash>::is_empty() const {

    // Iterate through each bucket and see if it contains anything, if it does, return false
    for (int i = 0; i < buckets; i++){
        if (table[i] == NULL) {
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
    for (int i = 0; i < buckets; i++) {
        // Destroy the list
        std::destroy(table[i].begin(), table[i].end());
        // Set each linked list equal to null as we go
        table[i] = NULL;
    }

}

// Inserts the given value into the hash table, and rehashes if the maximum load factor is exceeded
template<class Key, class Hash>
bool HashTable<Key, Hash>::insert(const value_type &value) {
    return false;
}

// Removes the given value from the hash table
template<class Key, class Hash>
size_t HashTable<Key, Hash>::remove(const key_type &key) {
    return 0;
}

// Returns true or false depending on whether the hashtable contains the given value or not
template<class Key, class Hash>
bool HashTable<Key, Hash>::contains(const key_type &key) {
    return false;
}

#endif  // HASHTABLE_SEPARATE_CHAINING_H