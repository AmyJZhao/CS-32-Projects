//
//  Map.cpp
//  ActualProject2
//
//  Created by Amy_Zhao on 1/26/18.
//  Copyright © 2018 Amy_Zhao. All rights reserved.
//

#include "Map.h"
#include <iostream>

Map::Map()
{
    //set head to null pointer
    head = nullptr;
}

Map::Map(const Map &copy)
{
    //set head to null pointer
    head = nullptr;
    //create a pointer that holds copy's head pointer
    Node *p = copy.head;
    //iterate along copy's linked list and use insert() method to add nodes
    while(p != nullptr)
    {
        insert(p->key, p->value);
        p = p->next;
    }
}

Map::~Map()
{
    //start with head pointer
    /*
    Node *p = head;
    while(p != nullptr)
    {
        //set new node pointer to current node
        Node *x = p;
        //set current node's pointer to next node
        p = x->next;
        //delete current node
        delete x;
    }
     */
    Node *p = head;
    while(p != nullptr)
    {
    Node *x = p->next;
    //set current node's pointer to next node
    delete p;
    //delete current node
    p = x;
    }
}
    

Map& Map::operator=(const Map& src)
{
    if(this != &src)
    {
        if(src.size() == 0)
            head = nullptr;
        else
        {
            //create an identical copy of src
            Map temp(src);
            //swap temporary map and this map
            swap(temp);
        }
        
    }
    return *this;
}

bool Map::empty() const
{
    return (head == nullptr);
}

int Map::size() const
{
    Node *p = head;
    int size = 0;
    while(p != nullptr)
    {
        size++;
        p = p->next;
    }
    return size;
}

bool Map::insert(const KeyType& key, const ValueType& value)
{
    if(contains(key))
        return false;
    //Case 1: list is empty, and we're inserting the first node
    //prev = nullptr, next = nullptr
    Node *p = new Node;
    p->key = key;
    p->value = value;
    //if the list is empty
    if(head == nullptr)
    {
        //set head pointer's prev and next pointer to null pointer
        p->prev = nullptr;
        p->next = nullptr;
        head = p;
    }
    else
    {
        //if there are items in the list, insert key into the rear of the Map
        Node *q = head;
        //navigate to the last node
        while(q->next != nullptr)
        {
            q = q->next;
        }
        //insert key by updating last node's next pointer
        q->next = p;
        p->prev = q; 
        p->next = nullptr;
        
    }
    return true;
}

bool Map::update(const KeyType& key, const ValueType& value)
{
    Node *p = head;
    while(p != nullptr)
    {
        //if key exists in the map
        if(p->key == key)
        {
            //update value and return true
            p->value = value;
            return true;
        }
        p = p->next;
    }
    //if key has not been found, return false
    return false;
}

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value)
{
    //try updating
    if(update(key, value))
    {
        return true;
    }
    //try inserting
    if(insert(key, value))
    {
        return true;
    }
    //if neither work (which should never happen), return false
    return false;
}

bool Map::erase(const KeyType& key)
{
    //three cases
    //erase the head
    //erase a Node in the middle
    //erase the last pointer (the tail)
    if(!contains(key))
        return false;
    if(head == nullptr)
        return false;
    Node *kill = head;
    if(head->key == key)
    {
        head = kill->next; //make the head pointer point to the second node
        head->prev = nullptr; //set the second node's prev pointer to nullptr
        delete kill;
        return true;
    }
    while(kill != nullptr)
    {
        if(kill->key == key)
        {
            if(kill->next != nullptr) //if the node to be erased is not the tail pointer
            {
                kill->prev->next = kill->next; //set previous node's next pointer to the node's
                //next pointer
                kill->next->prev = kill->prev; //set the next node's previous pointer to the
                //node's previous pointer
                delete kill;
                return true;
            }
            else //erase the last pointer
            {
                kill->prev->next = nullptr; //set the previous node's next pointer to nullptr
                delete kill;
                return true;
            }
        }
        kill = kill->next;
    }
    return false;
}

bool Map::contains(const KeyType& key) const
{
    Node *p = head;
    while(p != nullptr)
    {
        if(p->key == key)
            return true;
        p = p->next;
    }
    return false;
}

bool Map::get(const KeyType& key, ValueType& value) const
{
    Node *p = head;
    while(p != nullptr)
    {
        if(p->key == key)
        {
            value = p->value;
            return true;
        }
        p = p->next;
    }
    return false;
}

bool Map::get(int i, KeyType& key, ValueType& value) const
{
    if(i < 0 || i > size() - 1)
        return false;
    Node *p = head;
    int j = 0;
    while(p != nullptr && j <= i)
    {
        if(j == i)
        {
            key = p->key;
            value = p->value;
            return true;
        }
        p = p->next;
        j++;
    }
    return false;
}

void Map::swap(Map& other)
{
    //swap head pointers
    Node *temp = head;
    head = other.head;
    other.head = temp;
}

bool combine(const Map& m1, const Map& m2, Map& result)
{
    Map end; //temporary map intended to be identical to result
    int size1 = m1.size();
    int size2 = m2.size();
    bool f_y = true;
    //iterate through m1
    for(int i = 0; i < size1; i++)
    {
        ValueType v;
        KeyType k;
        //get key and value of each node
        m1.get(i, k, v);
        //if m2 does not contain key, it's safe to insert it into the combined
        //map
        if(!m2.contains(k))
            end.insert(k, v);
        else
        {
            //if m2 also contains the key
            ValueType temp;
            //if m2's identical key has the same value
            if(m2.get(k, temp) && v == temp)
            {
                //if the combined map doesn't already contain the key, insert it
                if(!end.contains(k))
                    end.insert(k, v);
            }
            else if(m2.get(k, temp) && v != temp) //if m2's identical key has a different value
                f_y = false; //set boolean variable to false
                
        }
    }
    //do the same for m2
    for(int i = 0; i < size2; i++)
    {
        ValueType v;
        KeyType k;
        m2.get(i, k, v);
        if(!m1.contains(k))
            end.insert(k, v);
        else
        {
            ValueType temp;
            if(m1.get(k, temp) && v == temp)
            {
                if(!end.contains(k))
                    end.insert(k, v);
            }
            else if(m1.get(k, temp) && v != temp)
                f_y = false;
            
        }
    }
    result = end; //set result to end (temporary map)
    return f_y; //return boolean variable
    
}

void subtract(const Map& m1, const Map& m2, Map& result)
{
    Map end; //temporary map intended to be identical to result
    int size1 = m1.size();
    //iterate through m1
    for(int i = 0; i < size1; i++)
    {
        ValueType v;
        KeyType k;
        //get key and value of each node
        m1.get(i, k, v);
        //if m2 does not contain key, it's safe to insert it into the combined
        //map
        if(!m2.contains(k))
            end.insert(k, v);
    }
    result = end; //set result to end (temporary map)
}

