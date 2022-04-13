#include <iostream>
#include "hashtable_open_addressing.h"

class Hashable {
    std::string str;
    int i;

public:
    Hashable() : str{}, i{} {}
    Hashable(std::string str, int i) : str{str}, i{i} {}

    bool operator==(const Hashable& rhs) const { return str == rhs.str && i == rhs.i; }
    bool operator!=(const Hashable& rhs) const { return !(*this == rhs); }

    friend std::ostream& operator<<(std::ostream&, const Hashable&);
};

std::ostream& operator<<(std::ostream& os, const Hashable& hashable) {
    os << "{\""<<hashable.str<<"\", "<<hashable.i<<"}";
    return os;
}

struct HashableHash {
    size_t operator()(const Hashable& value) const noexcept {
        return sizeof value;
    }
};

int main() {
    // Test default, paramaterized, and copy constructors
    HashTable<Hashable, HashableHash> defaultTable;
    HashTable<Hashable, HashableHash> table(11);
    HashTable<Hashable, HashableHash> copy = table;

    // Test copying empty hashtable
    copy.make_empty();
    defaultTable = copy;

    std::cout << "is empty is " << std::boolalpha << table.is_empty() << std::endl;
    std::cout << "size is " << table.size() << std::endl;
    table.make_empty();
    // Test insertion
    table.insert(Hashable("hey there", 3));

    // Test copying non-empty hashtable
    defaultTable = table;

    // Testing self assignment
    defaultTable.operator=(defaultTable);

    // Test copying via copy constructor
    HashTable<Hashable, HashableHash> copyTable(table);

    // Test more insertions
    table.insert(Hashable("Big test energy", 4));
    table.insert(Hashable("If I could escape", 5));
    defaultTable.insert(Hashable("If I could escape", 5));

    // Test remove
    table.remove(Hashable("i know it's hard to feel", 2));
    table.remove(Hashable("If I could escape", 5));
    std::cout << table.contains(Hashable("like i don't care at all", 7)) << std::endl;
    std::cout << table.contains(Hashable("hey there", 3)) << std::endl;
    std::cout << "table size is " << table.table_size() << std::endl;
    //table.print_table();
}