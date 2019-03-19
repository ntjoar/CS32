//
//  SSNSet.cpp
//  CS32_HW1
//
//  Created by Nathan Tjoar on 1/16/19.
//  Copyright © 2019 Nathan Tjoar. All rights reserved.
//

#include "SSNSet.h"

SSNSet::SSNSet() : m_ssn()
{}

bool SSNSet::add(unsigned long ssn)
{
    return m_ssn.insert(ssn);
}

int SSNSet::size() const
{
    return m_ssn.size();
}

void SSNSet::print() const
{
    unsigned long printLn;
    for(int i = 0; i < m_ssn.size(); i++)
    {
        m_ssn.get(i, printLn);
        cout << printLn << endl;
    }
}
