#include "provided.h"
#include "MyHash.h"
#include <string>
#include <vector>
#include <cctype>
#include <list>
#include <fstream>
#include <map>
using namespace std;

class WordListImpl
{
public:
    bool loadWordList(string filename);
    bool contains(string word) const;
    vector<string> findCandidates(string cipherWord, string currTranslation) const;
private:
    MyHash<string, vector<string>> m_hash_table;
    string letterPattern(string word) const;
    bool isWord(string word);
    bool sameLetterPattern(string word, string cipherWord, string currTranslation) const;
    string to_lower(string word) const;
};
//method returns the letter pattern
string WordListImpl::letterPattern(string word) const
{
    MyHash<string, string> wordToLetter;
    string pattern_letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int index = 0;
    string pattern = "";
    //use a hash table to associate a letter of the word with a pattern letter
    for(int i = 0; i < word.size(); i++)
    {
        if(wordToLetter.find(word.substr(i, 1)) == nullptr)
        {
            wordToLetter.associate(word.substr(i, 1), pattern_letters.substr(index, 1));
            index++;
        }
    }
    //now assemble the letter pattern using the hash table
    for(int i = 0; i < word.size(); i++)
    {
        if(!isalpha(word[i]))
            pattern += word[i];
        else
        {
            pattern += *wordToLetter.find(word.substr(i, 1));
        }
    }
    return pattern;
}
string WordListImpl::to_lower(string word) const
{
    string lower = "";
    for(int i = 0; i < word.size(); i++)
        lower += tolower(word[i]);
    return lower;
}
bool WordListImpl::isWord(string word)
{
    for(int i = 0; i < word.size(); i++)
    {
        if(!isalpha(word[i]) || word[i] == '\'')
            return false;
    }
    return true;
}
bool WordListImpl::loadWordList(string filename)
{
    m_hash_table.reset();
    ifstream infile(filename);
    if(!infile)
        return false;
    string s;
    while(getline(infile, s))
    {
        s = to_lower(s);
        if(isWord(s))
        {
            //use a hash table to associate a letter pattern and a vector of words
            if(m_hash_table.find(letterPattern(s)) == nullptr)
            {
                vector<string> words;
                words.push_back(s);
                m_hash_table.associate(letterPattern(s), words);
                
            }
            else
            {
                m_hash_table.find(letterPattern(s))->push_back(s);
            }
        }
    }
    return true;
}

bool WordListImpl::contains(string word) const
{
    string key = letterPattern(word);
    string copy = to_lower(word);
    if(m_hash_table.find(key) == nullptr)
        return false;
    else
    {
        //find the corresponding vector to the letter pattern
        //search the vector for the word
        vector<string> comparison = *m_hash_table.find(key);
        for(int i = 0; i < comparison.size(); i++)
        {
            if(copy == comparison[i])
                return true;
        }
    }
    return false; 
}
bool WordListImpl::sameLetterPattern(string word, string cipherWord, string currTranslation) const
{
    bool matches = true;
    for(int j = 0; j < currTranslation.size(); j++)
    {
        if(isalpha(currTranslation[j]))
        {
            if(currTranslation[j] != word[j])
                matches = false;
        }
        if(currTranslation[j] == '?')
        {
            if(!isalpha(word[j]))
               matches = false;
        }
        if(currTranslation[j] == '\'')
        {
            if(word[j] != '\'')
                matches = false;
        }
    }
    return matches;
}
vector<string> WordListImpl::findCandidates(string cipherWord, string currTranslation) const
{
    //use cipherWord's letter pattern to find possible candidates
    string cipher_pattern = letterPattern(cipherWord);
    vector<string> candidates= *m_hash_table.find(cipher_pattern);
    vector<string> viableCandidates;
    if(cipherWord.size() != currTranslation.size())
        return viableCandidates;
    //double check possible candidates against currTranslation
    for(int j = 0; j < currTranslation.size(); j++)
    {
        if(isalpha(currTranslation[j]))
        {
            if(!isalpha(cipherWord[j]))
            {
                return viableCandidates;
            }
        }
        if(currTranslation[j] == '?')
        {
            if(!isalpha(cipherWord[j]))
            {
                return viableCandidates;
            }
        }
        if(currTranslation[j] == '\'')
        {
            if(cipherWord[j] != '\'')
            {
                return viableCandidates;
            }
        }
    }
    for(int i = 0; i < candidates.size(); i++)
    {
        if(sameLetterPattern(candidates[i], cipherWord, currTranslation))
            viableCandidates.push_back(candidates[i]);
    }
    return viableCandidates;
}

//***** hash functions for string, int, and char *****

unsigned int hash(const std::string& s)
{
    return std::hash<std::string>()(s);
}

unsigned int hash(const int& i)
{
    return std::hash<int>()(i);
}

unsigned int hash(const char& c)
{
    return std::hash<char>()(c);
}

//******************** WordList functions ************************************

// These functions simply delegate to WordListImpl's functions.
// You probably don't want to change any of this code.

WordList::WordList()
{
    m_impl = new WordListImpl;
}

WordList::~WordList()
{
    delete m_impl;
}

bool WordList::loadWordList(string filename)
{
    return m_impl->loadWordList(filename);
}

bool WordList::contains(string word) const
{
    return m_impl->contains(word);
}

vector<string> WordList::findCandidates(string cipherWord, string currTranslation) const
{
    return m_impl->findCandidates(cipherWord, currTranslation);
}


