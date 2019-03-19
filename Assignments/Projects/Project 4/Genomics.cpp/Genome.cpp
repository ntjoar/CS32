#include "provided.h"
#include <string>
#include <vector>
#include <iostream>
#include <istream>
using namespace std;
/* Changes here WILL be turned in */
/* 68 lines */
class GenomeImpl
{
public:
    GenomeImpl(const string& nm, const string& sequence); // Build a genome
    static bool load(istream& genomeSource, vector<Genome>& genomes); // Load a genome
    int length() const; // Return sequence length
    string name() const; // Return name
    bool extract(int position, int length, string& fragment) const; // Pull a chunk of data from the sequence
private:
    string m_sequence; // Our sequence, but never returned, only for access
    string m_name; // Name of the genome
    int m_length; // Length of the sequence
};

GenomeImpl::GenomeImpl(const string& nm, const string& sequence) : m_sequence(sequence),
m_name(nm), m_length((int)sequence.size()) {} // Simple constructor

bool GenomeImpl::load(istream& genomeSource, vector<Genome>& genomes) 
{
    vector<Genome> temp = genomes;
    string nm = "";
    string sq = "";
    int m_nGenomes = 0;
    string l;
    bool postName = false;
    while(getline(genomeSource, l)) // Get the lines
    {
        if(l.size() == 0) // Check the size, i.e. not EOF or a null byte
        {
            genomes = temp;
            return false;
        }
        if(m_nGenomes == 0 &&  l[0] != '>') // Check first line value must be...
        {
            genomes = temp;
            return false;
        } // If improperly formatted
        if(l[0] == '>') // Is a name line
        {
            if(l.size() == 1) // Only the name
            {
                genomes = temp;
                return false;
            }
            if(postName == true) // Postname checking
            {
                genomes = temp;
                return false;
            }
            nm = l.substr(1);
            if(m_nGenomes > 0) // push sequence data
            {
                genomes.push_back(*new Genome(nm, sq));
                sq = "";
            }
            postName = true;
            m_nGenomes++;
        }
        else // Otherwise
        {
            sq += l;
            for(int i = 0; i < l.size(); i++) // Check
            {
                if(toupper(l[i]) != 'A' && toupper(l[i]) != 'T' && toupper(l[i]) != 'G' &&
                   toupper(l[i]) != 'C' && toupper(l[i]) != 'N') // Genome is improperly formatted
                {
                    genomes = temp;
                    return false;
                }
            } // If no genome properly
            postName = false;
            m_nGenomes++;
        }
    }
    if(m_nGenomes <= 1) // Set genome
    {
        genomes = temp;
        return false;
    } // If only name given
    genomes.push_back(*new Genome(nm, sq));
    return true;
}

int GenomeImpl::length() const // Trivial
{
    return m_length;
}

string GenomeImpl::name() const // Trivial
{
    return m_name;
}

bool GenomeImpl::extract(int position, int length, string& fragment) const // Pull data
{
    if(position + length > m_length) // Undef seq
        return false;
    else if (position < 0 || length < 0) // Undef seq
        return false;
    fragment = m_sequence.substr(position, length); // Get the string
    return true;
}

//******************** Genome functions ************************************

// These functions simply delegate to GenomeImpl's functions.
// You probably don't want to change any of this code.

Genome::Genome(const string& nm, const string& sequence)
{
    m_impl = new GenomeImpl(nm, sequence);
}

Genome::~Genome()
{
    delete m_impl;
}

Genome::Genome(const Genome& other)
{
    m_impl = new GenomeImpl(*other.m_impl);
}

Genome& Genome::operator=(const Genome& rhs)
{
    GenomeImpl* newImpl = new GenomeImpl(*rhs.m_impl);
    delete m_impl;
    m_impl = newImpl;
    return *this;
}

bool Genome::load(istream& genomeSource, vector<Genome>& genomes)
{
    return GenomeImpl::load(genomeSource, genomes);
}

int Genome::length() const
{
    return m_impl->length();
}

string Genome::name() const
{
    return m_impl->name();
}

bool Genome::extract(int position, int length, string& fragment) const
{
    return m_impl->extract(position, length, fragment);
}
