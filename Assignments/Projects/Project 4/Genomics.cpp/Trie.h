#include <iostream>
#include <string>
#include <vector>
using namespace std;
/* 93 lines */
const int ABC = 26;

template<typename ValueType>
class Trie
{
public:
    Trie() // New Trie
    {
        m_root = new Node; // Set new node
    }
    
    ~Trie()
    {
        reset(); // Delete everything
        delete m_root;
    }
    
    void reset()
    {
        for(auto i = m_nodes.begin(); i != m_nodes.end();) // Delete all descendants
        {
            delete *i;
            i = m_nodes.erase(i);
        }
        delete m_root; // Delete root and children
        m_root = new Node; // Clear root
    }
    
    void insert(const std::string& key, const ValueType& value) // New insertion
    {
        Node* current = m_root; // Start at root
        for(int i = 0; i < key.size(); i++) // Go through Tree
        {
            int id = tolower(key[i]) - 'a';
            if(current->m_children[id] == nullptr) // No value there, add
            {
                current->m_children[id] = new Node;
                current->m_children[id]->ch = toupper(key[i]);
                m_nodes.push_back(current->m_children[id]);
            }
            current = current->m_children[id]; // Move through children
        }
        current->m_values.push_back(value);
    }
    
    std::vector<ValueType> find(const std::string& key, bool exactMatchOnly) const
    {
        std::vector<ValueType> retValue; // What we return
        std::vector<ValueType> temp; // What we want to add
        Node* current = m_root;
        int id = tolower(key[0]) - 'a'; // To lower
        if(current->m_children[id] != nullptr)
        {
            temp = findHelper(current->m_children[id], key.substr(1, key.size() - 1), exactMatchOnly);
            retValue.insert(retValue.end(), temp.begin(), temp.end());
        } // Add values
        return retValue; // Return
    }
    // C++11 syntax for preventing copying and assignment
    Trie(const Trie&) = delete;
    Trie& operator=(const Trie&) = delete;
private:
    struct Node // Node structure
    {
        Node()
        {
            for(int i = 0; i < ABC; i++) // Allocate all to nullptr
                m_children[i] = nullptr;
        }
        char ch; // Node label (i.e. what pointer "sees")
        std::vector<ValueType> m_values; // Values it holds
        Node* m_children[ABC]; // Child values
    };
    std::vector<ValueType> findHelper(Node* head, const std::string& key, bool exactMatchOnly) const // Find helper
    {
        std::vector<ValueType> retVal;
        std::vector<ValueType> temp;
        if(key.size() == 0) // Base case
            return head->m_values;
        if(exactMatchOnly) // Exact match
        {
            for(int i = 0; i < key.size(); i++) // Look through key
            {
                int ide = tolower(key[i]) - 'a'; // Go through and pick out values
                if(head->m_children[ide] != nullptr)
                    head = head->m_children[ide];
                else // Break when we can't go through anymores
                    break;
            }
            retVal = head->m_values;
        }
        else // Else
        {
            for(int i = 0; i < ABC; i++) // Look for values
            {
                if(head->m_children[i] != nullptr)
                {
                    temp = head->m_values;
                    if(head->m_children[i]->ch == toupper(key[0]))
                        temp = findHelper(head->m_children[i], key.substr(1,key.size() - 1), false); // Recurse val true
                    else
                        temp = findHelper(head->m_children[i], key.substr(1,key.size() - 1), true); // Recurse val false
                    retVal.insert(retVal.end(), temp.begin(), temp.end()); // Add values
                }
            }
        }
        return retVal; // Retvalue
    }
    Node* m_root; // Dummy Node, contains nothing and only children with string values
    std::vector<Node*> m_nodes; // No root node, make sure to delete
};
