//
//  MyHash.hpp
//  Project4
//
//  Created by Amy_Zhao on 3/7/18.
//  Copyright © 2018 Amy_Zhao. All rights reserved.
//

#ifndef MyHash_h
#define MyHash_h

// MyHash.h

// Skeleton for the MyHash class template.  You must implement the first seven
// member functions; we have implemented the eighth.
const int M_SIZE = 100;
template<typename KeyType, typename ValueType>
class MyHash
{
public:
    MyHash(double maxLoadFactor = 0.5);
    ~MyHash();
    void reset();
    void associate(const KeyType& key, const ValueType& value);
    int getNumItems() const;
    double getLoadFactor() const;
    
    // for a map that can't be modified, return a pointer to const ValueType
    const ValueType* find(const KeyType& key) const;
    
    // for a modifiable map, return a pointer to modifiable ValueType
    ValueType* find(const KeyType& key)
    {
        return const_cast<ValueType*>(const_cast<const MyHash*>(this)->find(key));
    }
    
    // C++11 syntax for preventing copying and assignment
    MyHash(const MyHash&) = delete;
    MyHash& operator=(const MyHash&) = delete;
    
private:
private:
    struct Node
    {
        KeyType key;
        ValueType value;
    };
    Node *m_buckets;
    int m_size;
    int numBuckets;
    int m_maxLoadF;
};
template <class KeyType, class ValueType>
MyHash<KeyType, ValueType>::MyHash(double maxLoadFactor)
{
    numBuckets = 100;
    m_buckets = new Node[100];
    if(maxLoadFactor <= 0)
        m_maxLoadF = 0.5;
    else if(maxLoadFactor > 2)
        m_maxLoadF = 2.0;
    else
        m_maxLoadF = maxLoadFactor;
}
template <class KeyType, class ValueType>
MyHash<KeyType, ValueType>::~MyHash()
{
    delete [] m_buckets;
}
template <class KeyType, class ValueType>
double MyHash<KeyType, ValueType>::getLoadFactor() const
{
    return m_size / static_cast<double>(numBuckets);
}
template <class KeyType, class ValueType>
void MyHash<KeyType, ValueType>::reset()
{
    delete [] m_buckets;
    numBuckets = 100;
    m_buckets = new Node[100];
}
template <class KeyType, class ValueType>
void MyHash<KeyType, ValueType>::associate(const KeyType& key, const ValueType& value)
{
    if(getLoadFactor() > m_maxLoadF)
    {
        int size = 2 * numBuckets;
        numBuckets = size;
        Node *replacement = new Node[size];
        for(int i = 0; i < m_size - 1; i++)
        {
            replacement[i].key = m_buckets[i].key;
            replacement[i].value = m_buckets[i].value;
        }
        reset();
        m_size = size;
        delete [] m_buckets;
        m_buckets = replacement;
    }
    unsigned int hash(const KeyType &key);
    m_size++;
    int bucket = hash(key);
    m_buckets[bucket].key = key;
    m_buckets[bucket].value = value;
    
    
}
template <class KeyType, class ValueType>
const ValueType* MyHash<KeyType, ValueType>::find(const KeyType& key) const
{
    unsigned int hash(const KeyType &key);
    int bucket = hash(key);
    const ValueType *v = &(m_buckets[bucket].value);
    return v;
}
template <class KeyType, class ValueType>
int MyHash<KeyType, ValueType>::getNumItems() const
{
    return m_size;
}
#endif

