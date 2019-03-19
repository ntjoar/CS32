#include "provided.h"
#include "Trie.h"
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <map>
using namespace std;
/* Changes here WILL be turned in */
/* 127 lines */
class GenomeMatcherImpl
{
public:
    GenomeMatcherImpl(int minSearchLength); // Constructor
    ~GenomeMatcherImpl(); // Delete objects
    void addGenome(const Genome& genome); // Add a genome
    int minimumSearchLength() const; // Returns minSearchLength
    bool findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const; // Find genomes
    bool findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const; // Percentage
private:
    int m_minSearchLength; // Store constructor value
    Trie<pair<const Genome*, int>> m_gene; // Trie accessor
    vector<const Genome*> m_gen; // Genomes
    static bool sortByName(const pair<const Genome*,int> &a, const pair<const Genome*,int> &b) {return (a.first->name() < b.first->name());} // Sorting by name
};

GenomeMatcherImpl::GenomeMatcherImpl(int minSearchLength) : m_minSearchLength(minSearchLength)
{} // Simple constructor

GenomeMatcherImpl::~GenomeMatcherImpl()
{
    for(auto i = m_gen.begin(); i != m_gen.end();) // Delete all allocated memory
    {
        delete *i;
        m_gen.erase(i);
    }
}

void GenomeMatcherImpl::addGenome(const Genome& genome)
{
     m_gen.push_back(new Genome(genome)); // Push a new genome
    for(int i = 0; i + m_minSearchLength - 1 < genome.length(); i++)
    {
        string s = "";
        genome.extract(i, m_minSearchLength, s);
        m_gene.insert(s, pair<const Genome*, int> (m_gen.back(), i)); // Make sure vector access the back
    }
}

int GenomeMatcherImpl::minimumSearchLength() const // Trivial
{
    return m_minSearchLength;
}

bool GenomeMatcherImpl::findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const // A little difficult
{
    vector<pair<const Genome*, int>> gMat;
    if(fragment.size() < m_minSearchLength || minimumLength < m_minSearchLength) // Error checking
        return false;
    string subFrag = fragment.substr(0, m_minSearchLength); // We can assume this otherwise cannot be added
    string subGene = ""; // Get storing values ready
    gMat = m_gene.find(subFrag, exactMatchOnly);
    sort(gMat.begin(), gMat.end(), sortByName); // Sort all our gotten values
    bool added = false;
    if(gMat.empty()) // No values gotten?
        return false;
    else // Any other time
    {
        for(auto i = gMat.begin(); i != gMat.end(); i++) // Vectors
        {
            int len = m_minSearchLength - 1;
            int pos = -1;
            if(exactMatchOnly) // TRUE
            {
                for(int j = 0; j + m_minSearchLength - 1 < fragment.size(); j++)
                {
                    subFrag = fragment.substr(j, m_minSearchLength);
                    bool k = i->first->extract(i->second + j, m_minSearchLength, subGene);
                    if(k && subFrag == subGene) // Can be extracted
                    {
                        len++;
                        if(pos == -1)
                            pos = i->second - (len - m_minSearchLength);
                    }
                    else // Otherwise
                        break;
                }
            }
            else
            {
                len = 0;
                int diff = 0;
                for(int j = 0; j < fragment.size(); j++) // For each character
                {
                    subFrag = fragment[j];
                    bool k = i->first->extract(i->second + j, 1, subGene); // Extract data
                    if(!k) // Cannot be extracted i.e. EOF
                        break;
                    else if(subFrag != subGene) // Fragment is not gene
                    {
                        len++;
                        diff++;
                    }
                    else // Otherwise
                    {
                        len++;
                        pos = i->second;
                    }
                    if(diff > 1) // Difference is more than one
                    {
                        len--;
                        break;
                    }
                }
            }
            if(len < minimumLength) // Length
                continue;
            if(matches.size() > 0 && matches.back().length >= len &&
               matches.back().genomeName == i->first->name()) // If back length is more
                continue;
            else if(matches.size() > 0 && matches.back().genomeName == i->first->name() && // If back length is less
                    matches.back().length <= len)
                matches.pop_back();
            DNAMatch d; // Allocate data
            d.genomeName = i->first->name();
            d.length = len;
            d.position = pos;
            matches.push_back(d);
            added = true; // We can add something
        }
    }
    if(!added)
        matches.clear(); // For universality
    return added; // Return whether or not we added a dna
}

bool GenomeMatcherImpl::findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const // Follows algorithm given
{
    vector<DNAMatch> dna;
    vector<pair<const Genome*, int>> genes;
    int resSize = 0;
    for(int i = 0; i < query.length(); i += fragmentMatchLength)
    {
        string frag = "";
        query.extract(i, fragmentMatchLength, frag);
        findGenomesWithThisDNA(frag, fragmentMatchLength, exactMatchOnly, dna);
    }
    for(int i = 0; i < dna.size(); i++)
    {
        string frag = "";
        for(int j = 0; j < m_gen.size(); j++)
        {
            m_gen[j]->extract(dna[i].position, dna[i].length, frag);
            genes = m_gene.find(frag, exactMatchOnly);
            if((genes.size() * 100) / (query.length() / fragmentMatchLength) >= matchPercentThreshold)
            {
                GenomeMatch g;
                g.genomeName = m_gen[i]->name();
                g.percentMatch = (genes.size() * 100) / (query.length() / fragmentMatchLength);
                results.push_back(g);
                resSize++;
                break;
            }
        }
    }
    return resSize > 0; // We have added
}

//******************** GenomeMatcher functions ********************************

// These functions simply delegate to GenomeMatcherImpl's functions.
// You probably don't want to change any of this code.
/*  DON'T CHANGE ANYTHING BELOW THIS LINE */

GenomeMatcher::GenomeMatcher(int minSearchLength)
{
    m_impl = new GenomeMatcherImpl(minSearchLength);
}

GenomeMatcher::~GenomeMatcher()
{
    delete m_impl;
}

void GenomeMatcher::addGenome(const Genome& genome)
{
    m_impl->addGenome(genome);
}

int GenomeMatcher::minimumSearchLength() const
{
    return m_impl->minimumSearchLength();
}

bool GenomeMatcher::findGenomesWithThisDNA(const string& fragment, int minimumLength, bool exactMatchOnly, vector<DNAMatch>& matches) const
{
    return m_impl->findGenomesWithThisDNA(fragment, minimumLength, exactMatchOnly, matches);
}

bool GenomeMatcher::findRelatedGenomes(const Genome& query, int fragmentMatchLength, bool exactMatchOnly, double matchPercentThreshold, vector<GenomeMatch>& results) const
{
    return m_impl->findRelatedGenomes(query, fragmentMatchLength, exactMatchOnly, matchPercentThreshold, results);
}
