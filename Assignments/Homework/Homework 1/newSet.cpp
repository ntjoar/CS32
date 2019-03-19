//
//  newSet.cpp
//  CS32_HW1
//
//  Created by Nathan Tjoar on 1/17/19.
//  Copyright © 2019 Nathan Tjoar. All rights reserved.
//

#include "newSet.h"

Set::Set() : m_size(0)
{
    m_set = new ItemType[DEFAULT_MAX_ITEMS];
    m_maxSize = DEFAULT_MAX_ITEMS;
}

Set::~Set()
{
    delete[] m_set;
}

Set& Set::operator=(const Set& val)
{
    if (this == &val)
        return *this;
    delete[] m_set;
    m_size = val.m_size;
    m_set = new ItemType[size()];
    m_maxSize = val.m_maxSize;
    for (int i = 0; i < size(); i++)
        m_set[i] = val.m_set[i];
    return *this;
}

Set::Set(int val) : m_size(0)
{
    m_set = new ItemType[val];
    m_maxSize = val;
}

Set::Set(const Set& other)
{
    m_size = other.m_size;
    m_set = other.m_set;
    for(int i = 0; i < size(); i++)
        m_set[i] = other.m_set[i];
}

bool Set::empty() const
{
    if(m_size == 0)
        return true;
    return false;
}

int Set::size() const
{
    return m_size;
}

bool Set::insert(const ItemType& value)
{
    if(size() < 0 || size() >= DEFAULT_MAX_ITEMS)
        return false;
    
    for(int i = 0; i < size(); i++)
        if(m_set[i] == value)
            return false;
    
    m_set[m_size] = value;
    m_size++;
    return true;
}

bool Set::erase(const ItemType& value)
{
    if(size() < 0 || size() >= DEFAULT_MAX_ITEMS)
        return false;
    
    for(int i = 0; i < size(); i++)
        if(m_set[i] == value)
        {
            m_set[i] = m_set[m_size-1];
            m_size--;
            return true;
        }
    return false;
}

bool Set::contains(const ItemType& value) const
{
    if(size() < 0 || size() >= DEFAULT_MAX_ITEMS)
        return false;
    
    for(int i = 0; i < m_size; i++)
        if(m_set[i] == value)
            return true;
    return false;
}

bool Set::get(int i, ItemType& value) const
{
    ItemType temp[DEFAULT_MAX_ITEMS];
    
    if(i < 0 || i >= size())
        return false;
    
    for(int j = 0; j < m_size; j++)
        temp[j] = m_set[j];
    
    for (int k = 0; k < size()-1; k++)
        for (int j = 0; j < size()-k-1; j++)
            if (temp[j] > temp[j+1])
                std::swap(temp[j], temp[j+1]);
    
    value = temp[i];
    return true;
}

void Set::swap(Set& other)
{
    ItemType* tempItem = other.m_set;
    int tempSize = other.m_size;
    int tempItems =  other.m_maxSize;
    
    other.m_size = m_size;
    other.m_maxSize = m_maxSize;
    other.m_set = m_set;
    
    m_size = tempSize;
    m_maxSize = tempItems;
    m_set = tempItem;
}

void Set::dump() const //Print out the Set
{
    for (int k = 0; k < size(); k++)
        std::cerr << m_set[k] << ", ";
}
