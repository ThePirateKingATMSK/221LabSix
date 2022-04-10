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

public:
    HashTable();
    HashTable(const HashTable& other);
    ~HashTable();
    HashTable& operator=(const HashTable& other);
    HashTable(size_type buckets);

    bool is_empty() const;
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

template<class Key, class Hash>
HashTable<Key, Hash>::HashTable() {

}

template<class Key, class Hash>
HashTable<Key, Hash>::HashTable(const HashTable &other) {

}

template<class Key, class Hash>
HashTable<Key, Hash>::~HashTable() {

}

#endif  // HASHTABLE_SEPARATE_CHAINING_H