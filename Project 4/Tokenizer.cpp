#include "provided.h"
#include <string>
#include <vector>
using namespace std;
class TokenizerImpl
{
    public:
        TokenizerImpl(std::string separators);
        vector<string> tokenize(const std::string& s) const;
    private:
        vector<std::string> m_separators;
};
//put all the separators into a vector
TokenizerImpl::TokenizerImpl(string separators)
{
    for(int i = 0; i < separators.size(); i++)
    {
        m_separators.push_back(separators.substr(i, 1));
    }
}
vector<string> TokenizerImpl::tokenize(const std::string& s) const
{
    //create a vector to hold the words
    std::vector<std::string> tokens;
    int length = 1;
    int index = 0;
    for(int i = 0; i < s.size(); i++)
    {
        for(int j = 0; j < m_separators.size(); j++)
        {
            //if a character is equal to a separator, take the substring and push to the tokens vector
            if(s.substr(i, 1) == m_separators[j])
            {
                if(s.substr(index, length - 1) != "")
                    tokens.push_back(s.substr(index, length - 1));
                index += length;
                length = 0;
                break;
            }
        }
        length++;
    }
    //if there's no separator at the end, push the last word to tokens vector
    tokens.push_back(s.substr(index, length));
    return tokens;
}
//******************** Tokenizer functions ************************************

// These functions simply delegate to TokenizerImpl's functions.
// You probably don't want to change any of this code.

Tokenizer::Tokenizer(string separators)
{
    m_impl = new TokenizerImpl(separators);
}

Tokenizer::~Tokenizer()
{
    delete m_impl;
}

vector<string> Tokenizer::tokenize(const std::string& s) const
{
    return m_impl->tokenize(s);
}

