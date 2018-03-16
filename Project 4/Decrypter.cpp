#include "provided.h"
#include <string>
#include <vector>
using namespace std;

class DecrypterImpl
{
public:
    bool load(string filename);
    vector<string> crack(const string& ciphertext);
private:
    WordList w;
    Translator trans;
    //private methods
    vector<string> validSolutions;
    int containsMapping(string mapping1, string mapping2, string w, string p);
    bool fullyTranslated(string ciphertext);
    int numUntranslatedLetters(string cipher);
    string mostUntranslatedLetters(vector<string> ciphertext);
    void recursiveHelper(string inputMessage);
};

bool DecrypterImpl::load(string filename)
{
    if(w.loadWordList(filename))
        return true;
    else
        return false;
}
int DecrypterImpl::containsMapping(string mapping1, string mapping2, string w, string p)
{
    //returns 0 if doesn't contain mapping
    //returns 1 if contains same mapping
    //returns 2 if contains conflicting mapping
    for(int i = 0; i < mapping1.size(); i++)
    {
        if(w == mapping1.substr(i, 1) && p == mapping2.substr(i, 1))
        {
            return 1;
        }
        else if(w == mapping1.substr(i, 1) && p != mapping2.substr(i, 1))
        {
            return 2;
        }
        else if(w != mapping1.substr(i, 1) && p == mapping2.substr(i, 1))
        {
            return 2;
        }
    }
    return 0;
}
bool DecrypterImpl::fullyTranslated(string ciphertext)
{
    for(int i = 0; i < ciphertext.size(); i++)
    {
        if(ciphertext.substr(i, 1) == "?")
            return false;
    }
    return true;
}
int DecrypterImpl::numUntranslatedLetters(string cipher)
{
    int sum = 0;
    for(int i = 0; i < cipher.size(); i++)
    {
        if(cipher.substr(i, 1) == "?")
            sum++;
    }
    return sum;
}
string DecrypterImpl::mostUntranslatedLetters(vector<string> ciphertext)
{
    vector<string>::iterator it = ciphertext.begin();
    vector<string>::iterator itCopy = it;
    int most = numUntranslatedLetters(*it);
    it++;
    while(it != ciphertext.end())
    {
        if(numUntranslatedLetters(*it) > most)
        {
            most = numUntranslatedLetters(*it);
            itCopy = it;
        }
        it++;
    }
    return *itCopy;
}
//recursive helper function
void DecrypterImpl::recursiveHelper(string inputMessage)
{
    Tokenizer token(" 123456789,;:.!()[]{}-\"#$%^&");
    vector<string> tokenized_words = token.tokenize(inputMessage);
    string ciphertextW = mostUntranslatedLetters(tokenized_words);
    //step 3: translate the word
    string partialTrans = trans.getTranslation(ciphertextW);
    //step 4: locate candidates
    vector<string> C;
    C = w.findCandidates(ciphertextW, partialTrans);
    if(C.empty())
    {
        trans.popMapping();
        return;
    }
    vector<string>::iterator itC = C.begin();
    while(itC != C.end())
    {
        string mapping1 = "";
        string mapping2 = "";
        string p = *itC;
        bool goBack = false;
        for(int i = 0; i < ciphertextW.size(); i++)
        {
            if(containsMapping(mapping1, mapping2, ciphertextW.substr(i, 1), p.substr(i, 1)) == 0)
            {
                mapping1 += ciphertextW.substr(i, 1);
                mapping2 += p.substr(i, 1);
            }
            if(containsMapping(mapping1, mapping2, ciphertextW.substr(i, 1), p.substr(i, 1)) == 2)
            {
                goBack = true;
                break;
            }
        }
        if(goBack)
            continue;
        trans.pushMapping(mapping1, mapping2);
        string newTranslation = trans.getTranslation(inputMessage);
        vector<string> new_transWords = token.tokenize(newTranslation);
        bool fully_translated = true;
        bool wrong = false;
        for(int i = 0; i < new_transWords.size(); i++)
        {
            if(!fullyTranslated(new_transWords[i]))
               fully_translated = false;
            else
            {
                if(!w.contains(new_transWords[i]))
                {
                    wrong = true;
                    break;
                }
            }
        }
        if(wrong)
        {
            trans.popMapping();
            continue;
        }
        if(fully_translated)
        {
            validSolutions.push_back(newTranslation);
            trans.popMapping();
        }
        else
        {
            recursiveHelper(newTranslation);
        }
        itC++;
    }
}
vector<string> DecrypterImpl::crack(const string& ciphertext)
{
    recursiveHelper(ciphertext);
    return validSolutions;
}

//******************** Decrypter functions ************************************

// These functions simply delegate to DecrypterImpl's functions.
// You probably don't want to change any of this code.

Decrypter::Decrypter()
{
    m_impl = new DecrypterImpl;
}

Decrypter::~Decrypter()
{
    delete m_impl;
}

bool Decrypter::load(string filename)
{
    return m_impl->load(filename);
}

vector<string> Decrypter::crack(const string& ciphertext)
{
    return m_impl->crack(ciphertext);
}

