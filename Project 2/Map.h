//
//  Map.hpp
//  ActualProject2
//
//  Created by Amy_Zhao on 1/26/18.
//  Copyright © 2018 Amy_Zhao. All rights reserved.
//

#ifndef Map_h
#define Map_h

#include <string>

using KeyType = std::string;
using ValueType = double;

const int DEFAULT_MAX_ITEMS = 250;

class Map
{
    public:
        Map();
        Map(const Map &copy);
        ~Map();
        Map &operator=(const Map &src);
        bool empty() const;
        int size() const;
        bool insert(const KeyType& key, const ValueType& value);
        bool update(const KeyType& key, const ValueType& value);
        bool insertOrUpdate(const KeyType& key, const ValueType& value);
        bool erase(const KeyType& key);
        bool contains(const KeyType& key) const;
        bool get(const KeyType& key, ValueType& value) const;
        bool get(int i, KeyType& key, ValueType& value) const;
        void swap(Map& other);
    private:
        struct Node {
            KeyType key;
            ValueType value;
            Node *next;
            Node *prev;
        };
        Node *head;
};

bool combine(const Map& m1, const Map& m2, Map& result);
void subtract(const Map& m1, const Map& m2, Map& result);

#endif /* Map_h */
