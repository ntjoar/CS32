//
//  Set.cpp
//  CS32_HW1
//
//  Created by Nathan Tjoar on 1/16/19.
//  Copyright © 2019 Nathan Tjoar. All rights reserved.
//

#include "Set.h"

Set::Set() : m_size(0)
{}

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
        if(m_item[i] == value)
            return false;
    
    m_item[m_size] = value;
    m_size++;
    return true;
}

bool Set::erase(const ItemType& value)
{
    if(size() < 0 || size() >= DEFAULT_MAX_ITEMS)
        return false;
    
    for(int i = 0; i < size(); i++)
        if(m_item[i] == value)
        {
            m_item[i] = m_item[m_size-1];
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
        if(m_item[i] == value)
            return true;
    return false;
}

bool Set::get(int i, ItemType& value) const
{
    ItemType temp[DEFAULT_MAX_ITEMS];
    
    if(i < 0 || i >= size())
        return false;
    
    for(int j = 0; j < m_size; j++)
        temp[j] = m_item[j];

    for (int k = 0; k < size()-1; k++)
        for (int j = 0; j < size()-k-1; j++)
            if (temp[j] > temp[j+1])
                std::swap(temp[j], temp[j+1]);
    
    value = temp[i];
    return true;
}

void Set::swap(Set& other)
{
    ItemType tempItem[DEFAULT_MAX_ITEMS];
    int tempSize = m_size;
    for(int i = 0; i < size(); i++)
        tempItem[i] = m_item[i];
    for(int j = 0; j < other.size(); j++) //Replace m_items with other's items
        m_item[j] = other.m_item[j];
    for(int k = 0; k < size(); k++) //Replace other items with m_items items
        other.m_item[k] = tempItem[k];
    
    m_size = other.m_size;
    other.m_size = tempSize;
}

void Set::dump() const //Print out the Set
{
    for (int k = 0; k < size(); k++)
        std::cerr << m_item[k] << ", ";
}
