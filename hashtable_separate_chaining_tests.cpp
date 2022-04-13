/*****************************************
** File:    hashtable_separate_chaining_tests.cpp
** Project: CSCE 221 Lab 6 Spring 2022
** Author:  Joshua Hillis
** Date:    04/12/2022
** Section: 512
** E-mail:  joshuahillis292002@tamu.edu
**
** This file contains the test suite for the separate chaining header file, it tests all of
** the functions for 90% coverage.
**
***********************************************/

#include <iostream>
#include <sstream>
#include "hashtable_separate_chaining.h"

using std::cout, std::endl;

int main() {
    // Example test case in lab document
    std::cout << "make an empty hash table with 11 buckets for strings" << std::endl;
    HashTable<std::string> table(11);

    std::cout << "initial size is " << table.size() << std::endl;
    std::cout << "initial bucket count is " << table.bucket_count() << std::endl;
    std::cout << "initial load factor is " << table.load_factor() << std::endl;
    std::cout << "initial max load factor is " << table.max_load_factor() << std::endl;

    // A bunch of standard insertions
    std::cout << "insert several strings" << std::endl;
    table.insert("And them who hold High Places");
    table.insert("Must be the ones to start");
    table.insert("To mold a new Reality");
    table.insert("Closer to the Heart");
    table.insert("The Blacksmith and the Artist");
    table.insert("Reflect it in their Art");
    table.insert("Forge their Creativity");
    table.insert("Closer to the Heart");
    table.insert("Philosophers and Plowmen");
    table.insert("Each must know their Part");
    table.insert("To sow a new Mentality");
    table.insert("Closer to the Heart");
    table.insert("You can be the Captain");
    table.insert("I will draw the Chart");
    table.insert("Sailing into Destiny");
    table.insert("Closer to the Heart");

    // Test contains with positive and negative casese
    std::cout << "Table contains 'Closer to the Heart'";
    std::cout << " " << table.contains("Closer to the Heart") << std::endl;

    table.remove("Closer to the Heart");
    std::cout << "Table contains 'Closer to the Heart'";
    std::cout << " " << table.contains("Closer to the Heart") << std::endl;

    // Test size, bucket count, load factor, max load factor
    std::cout << "size is " << table.size() << std::endl;
    std::cout << "bucket count is " << table.bucket_count() << std::endl;
    std::cout << "load factor is " << table.load_factor() << std::endl;
    std::cout << "max load factor is " << table.max_load_factor() << std::endl;


    // Test print
    {
        std::cout << "print the table" << std::endl;
        std::stringstream ss;
        table.print_table(ss);
        std::cout << ss.str() << std::endl;
    }

    std::cout << "Table contains Philosop and Plowmen: " << table.contains("Philosophers and Plowmen") << std::endl;
    std::cout << "remove \"Philosophers and Plowmen\"" << std::endl;
    table.remove("Philosophers and Plowmen");
    std::cout << "remove \"Each must know their Part\"" << std::endl;
    table.remove("Each must know their Part");

    std::cout << "size is " << table.size() << std::endl;
    std::cout << "bucket count is " << table.bucket_count() << std::endl;
    std::cout << "load factor is " << table.load_factor() << std::endl;
    std::cout << "max load factor is " << table.max_load_factor() << std::endl;

    {
        std::cout << "print the table" << std::endl;
        std::stringstream ss;
        table.print_table(ss);
        std::cout << ss.str() << std::endl;
    }

    std::cout << "set max load factor to 2" << std::endl;
    table.max_load_factor(2);
    std::cout << "rehash the table to size 7" << std::endl;
    table.rehash(7);

    std::cout << "size is " << table.size() << std::endl;
    std::cout << "bucket count is " << table.bucket_count() << std::endl;
    std::cout << "load factor is " << table.load_factor() << std::endl;
    std::cout << "max load factor is " << table.max_load_factor() << std::endl;

    {
        std::cout << "print the table" << std::endl;
        std::stringstream ss;
        table.print_table(ss);
        std::cout << ss.str() << std::endl;
    }

    std::cout << "find \"The Blacksmith and the Artist\"" << std::endl;
    size_t index = table.bucket("The Blacksmith and the Artist");
    std::cout << " ==> bucket " << index << std::endl;
    std::cout << "     which has " << table.bucket_size(index) << " elements" << std::endl;

    std::cout << "make the table empty" << std::endl;
    table.make_empty();

    std::cout << "size is " << table.size() << std::endl;
    std::cout << "bucket count is " << table.bucket_count() << std::endl;
    std::cout << "load factor is " << table.load_factor() << std::endl;
    std::cout << "max load factor is " << table.max_load_factor() << std::endl;

    {
        std::cout << "print the table" << std::endl;
        std::stringstream ss;
        table.print_table(ss);
        std::cout << ss.str() << std::endl;
    }
    return 0;
}