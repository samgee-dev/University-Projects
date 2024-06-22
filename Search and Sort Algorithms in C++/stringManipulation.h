#ifndef STRING_MANIPULATION_H
#define STRING_MANIPULATION_H

#include <string>
#include <stack>
#include <set>
#include <map>

using namespace std;

class StringManipulation {
public:
    int numberOfVowels(string& str);

    string reverseString(string& str);
    string reverseWords(string& str);
    string removeDuplications(const string& str);

    char mostRepeatedChar(const string& str);

    bool isRotation(string& str1, string& str2);
};

#endif // STRING_MANIPULATION_H