//
//  Set.cpp
//  CS32_Project2
//
//  Created by Nathan Tjoar on 1/26/19.
//  Copyright © 2019 Nathan Tjoar. All rights reserved.
//

#include "Set.h"

Set::Set() : m_size(0) //Default Constructor
{
    head = new Node;
    head->next = head;
    head->prev = head;
}

Set::~Set() //Destructor
{
    while(head->next != head) //Allocate memory and then delete
    {
        Node* temp = head->next;
        temp->next->prev = head;
        head->next = temp->next;
        delete temp;
    }
    delete head;
}

Set& Set::operator=(const Set& val) //Assignment constructor
{
    if (this != &val)
    {
        Set temp(val);
        swap(temp);
    }
    return *this;
}

Set::Set(const Set& other) : m_size(other.m_size) //Copy constructor
{
    head = new Node;
    head->next = head;
    head->prev = head;
    for(Node* temp = other.head->next; temp != other.head; temp = temp->next)
    {
        Node* newVal = new Node;
        newVal->m_data = temp->m_data;
        newVal->next = head;
        newVal->prev = head->prev;
        head->prev->next = newVal;
        head->prev = newVal;
    }
}

bool Set::empty() const //Returns true if set is empty
{
    if(m_size == 0)
        return true;
    return false;
}

int Set::size() const //Returns size of the set
{
    return m_size;
}

bool Set::insert(const ItemType& value) //Insert a new element in the set given that it is not there yet
{
    if(! this->contains(value))
    {
        Node* newVal = new Node;
        newVal->m_data = value;
        newVal->next = head;
        newVal->prev = head->prev;
        head->prev->next = newVal;
        head->prev = newVal;
        m_size++;
        return true;
    }
    return false;
}

bool Set::erase(const ItemType& value) //Erase value from set and return true if done
{
    Node* temp = head->next;
    while(temp != head)
    {
        if(temp->m_data == value)
        {
            temp->prev->next = temp->next;
            temp->next->prev = temp->prev;
            delete temp;
            m_size--;
            return true;
        }
        temp = temp->next;
    }
    return false;
}

bool Set::contains(const ItemType& value) const //Return true if value is in the set
{
    Node* temp = head->next;
    if(m_size != 0)
    {
        while(temp != head)
        {
            if(temp->m_data == value)
                return true;
            temp = temp->next;
        }
    }
    return false;
}

bool Set::get(int pos, ItemType& value) const //Returns true if 0 <= pos, copy into value item that is greater that pos values
{
    if(pos < 0 || pos >= size())
        return false;
    int rank;
    for(Node* s1 = head->next; s1 != head; s1 = s1->next)
    {
        rank = 0;
        Node* s2 = head->next;
        while(s2 != head)
        {
            if(s1->m_data > s2->m_data)
                rank++;
            s2 = s2->next;
        }
        if(rank == pos)
        {
            value = s1->m_data;
            return true;
        }
    }
    return false;
}

void Set::swap(Set& other) //Swaps the two heads
{
    Node* temp = other.head;
    other.head = this->head;
    this->head = temp;
    
    int tempSize = other.m_size;
    other.m_size = this->m_size;
    this->m_size = tempSize;
}

void Set::dump() const //Print out the Set
{
    for (Node* pres = head->next; pres != head; pres = pres->next)
        cerr << pres->m_data << endl;
}

void unite(const Set& s1, const Set& s2, Set& result)//Union between two sets
{
    ItemType item;
    for(int i = 0; i < s1.size(); i++)//Use insert function for set 1 to insert
    {
        s1.get(i, item);
        result.insert(item);
    }
    
    for(int i = 0; i < s2.size(); i++)//Use insert function for set 2 to insert
    {
        s2.get(i, item);
        result.insert(item);
    }
}

void subtract(const Set& s1, const Set& s2, Set& result)
{
    ItemType item;
    for(int i = 0; i < s1.size(); i++)//Use insert function for set 1 to insert
    {
        s1.get(i, item);
        result.insert(item);
    }
    
    for(int i = 0; i < s2.size(); i++)//Use remove function for set 2 to remove
    {
        s2.get(i, item);
        result.erase(item);
    }
}
