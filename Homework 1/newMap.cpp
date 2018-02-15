
#include "newMap.h"
#include <iostream>
using namespace std;

Map::Map(int pairs)
{
    max_size = pairs;
    m_size = 0;
    m_array = new map_content[max_size];
}

Map::Map(const Map &copy)
{
    m_size = copy.m_size;
    max_size = copy.max_size;
    delete [] m_array;
    m_array = new map_content[max_size];
    for(int i = 0; i != m_size; i++)
        m_array[i] = copy.m_array[i];
    
    
}

Map::~Map()
{
    delete [] m_array;
}

Map& Map::operator=(const Map& src)
{
    if(this != &src)
    {
     
         delete [] m_array;
         m_size = src.m_size;
         max_size = src.max_size;
         m_array = new map_content[max_size];
         for(int i = 0; i != m_size; i++)
         m_array[i] = src.m_array[i];
 
    }
    return *this;
}

bool Map::empty() const  // Return true if the map is empty, otherwise false.
{
    return (m_size == 0);
}

int Map::size() const    // Return the number of key/value pairs in the map.
{
    return m_size;
}

bool Map::insert(const KeyType& key, const ValueType& value)
// If key is not equal to any key currently in the map, and if the
// key/value pair can be added to the map, then do so and return true.
// Otherwise, make no change to the map and return false (indicating
// that either the key is already in the map, or the map has a fixed
// capacity and is full).
{
    if(m_size == max_size || contains(key))
    {
        return false;
    }
    else
    {
        m_array[m_size].key = key;
        m_array[m_size].value = value;
        m_size++;
        return true;
    }
}

bool Map::update(const KeyType& key, const ValueType& value)
// If key is equal to a key currently in the map, then make that key no
// longer map to the value it currently maps to, but instead map to
// the value of the second parameter; return true in this case.
// Otherwise, make no change to the map and return false.
{
    for(int i = 0; i < m_size; i++)
    {
        if(m_array[i].key == key)
        {
            m_array[i].value = value;
            return true;
        }
    }
    return false;
}

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value)
// If key is equal to a key currently in the map, then make that key no
// longer map to the value it currently maps to, but instead map to
// the value of the second parameter; return true in this case.
// If key is not equal to any key currently in the map and if the
// key/value pair can be added to the map, then do so and return true.
// Otherwise, make no change to the map and return false (indicating
// that the key is not already in the map and the map has a fixed
// capacity and is full).
{
    if(update(key, value))
    {
        return true;
    }
    if(insert(key, value))
    {
        return true;
    }
    return false;
}

bool Map::erase(const KeyType& key)
// If key is equal to a key currently in the map, remove the key/value
// pair with that key from the map and return true.  Otherwise, make
// no change to the map and return false.
{
    int index = 0;
    for(int i = 0; i < m_size; i++)
    {
        if(m_array[i].key == key)
        {
            index = i;
            break;
        }
    }
    if(index == m_size)
        return false;
    for(int i = index; i < m_size; i++)
    {
        m_array[i] = m_array[i + 1];
    }
    m_size--;
    return true;
}

bool Map::contains(const KeyType& key) const {
    // Return true if key is equal to a key currently in the map, otherwise
    // false.
    
    for(int i = 0; i < m_size; i++)
    {
        if(m_array[i].key == key)
        {
            return true;
        }
    }
    return false;
}

bool Map::get(const KeyType& key, ValueType& value) const
// If key is equal to a key currently in the map, set value to the
// value in the map that that key maps to, and return true.  Otherwise,
// make no change to the value parameter of this function and return
// false.
{
    for(int i = 0; i < m_size; i++)
    {
        if(m_array[i].key == key)
        {
            value = m_array[i].value;
            return true;
        }
    }
    return false;
}

bool Map::get(int i, KeyType& key, ValueType& value) const
// If 0 <= i < size(), copy into the key and value parameters the
// key and value of one of the key/value pairs in the map and return
// true.  Otherwise, leave the key and value parameters unchanged and
// return false.  (See below for details about this function.)
{
    if(i >= 0 && i < m_size)
    {
        key = m_array[i].key;
        value = m_array[i].value;
        return true;
    }
    return false;
}

void Map::swap(Map& other)
// Exchange the contents of this map with the other one.
{
    Map temp;
    temp = *this;
    *this = other;
    other = temp;
    
}



