#include "provided.h"
#include "MyHash.h"
#include <string>
using namespace std;

class TranslatorImpl
{
public:
    TranslatorImpl();
    bool pushMapping(string ciphertext, string plaintext);
    bool popMapping();
    string getTranslation(const string& ciphertext) const;
    ~TranslatorImpl();
private:
    //private node with hash tables
    struct Table
    {
        MyHash<char, char> m_table;
        MyHash<char, char> opposite;
        Table *next;
    };
    Table *top;
};

TranslatorImpl::TranslatorImpl()
{
    //initialize empty mapping table
    Table* m_head = new Table;
    for(int i = 1; i < 27; i++)
    {
        char letter = 'A' + i - 1;
        m_head->m_table.associate(letter, '?');
    }
    m_head->next = nullptr;
    top = m_head;
}

bool TranslatorImpl::pushMapping(string ciphertext, string plaintext)
{
    //create a new node and add it to linked list
    if(ciphertext.size() != plaintext.size())
        return false;
    Table *temp = new Table;
    for(int i = 1; i < 27; i++)
    {
        char letter = 'A' + i - 1;
        char val = *(top->m_table.find(letter));
        temp->m_table.associate(letter, val);
        temp->opposite.associate(*(top->m_table.find(letter)), letter);
    }
    for(int i = 0; i < ciphertext.size(); i++)
    {
        if(isalpha(ciphertext[i]))
        {
            if(*(temp->m_table.find(ciphertext[i])) != '?' || temp->opposite.find(plaintext[i]) != nullptr)
                return false;
            temp->m_table.associate(ciphertext[i], plaintext[i]);
            temp->opposite.associate(plaintext[i], ciphertext[i]);
        }
        else
            return false;
    }
    temp->next = top;
    top = temp;
    return true;
}

bool TranslatorImpl::popMapping()
{
    //pop top node off of linked list
    if(top == nullptr)
        return false;
    Table *kill = top;
    top = top->next;
    delete kill;
    return true;
}

string TranslatorImpl::getTranslation(const string& ciphertext) const
{
    //use hash table in linked list's top node to get translation
    string translation = "";
    for(int i = 0; i < ciphertext.size(); i++)
    {
        if(!isalpha(ciphertext[i]))
            translation += ciphertext[i];
        else
        {
            if(top->m_table.find(toupper(ciphertext[i])) != nullptr)
            {
                char letter = *(top->m_table.find(toupper(ciphertext[i])));
                if(isupper(ciphertext[i]))
                    translation += toupper(letter);
                else if(islower(ciphertext[i]))
                    translation += tolower(letter);
                else
                    translation += letter;
            }
               
        }
    }
    return translation;
}
TranslatorImpl::~TranslatorImpl()
{
    Table *ptr = top;
    while(ptr != nullptr)
    {
        Table *kill = ptr;
        ptr = ptr->next;
        delete kill;
    }
}

//******************** Translator functions ************************************

// These functions simply delegate to TranslatorImpl's functions.
// You probably don't want to change any of this code.

Translator::Translator()
{
    m_impl = new TranslatorImpl;
}

Translator::~Translator()
{
    delete m_impl;
}

bool Translator::pushMapping(string ciphertext, string plaintext)
{
    return m_impl->pushMapping(ciphertext, plaintext);
}

bool Translator::popMapping()
{
    return m_impl->popMapping();
}

string Translator::getTranslation(const string& ciphertext) const
{
    return m_impl->getTranslation(ciphertext);
}


