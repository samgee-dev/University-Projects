#include "stringManipulation.h"

int StringManipulation::numberOfVowels(string &str) {
    int count = 0;
    string vowels = "aeiou";
    for (char ch : str) {
        if (vowels.find(tolower(ch)) != vowels.npos) {
            count++;
        }
    }

    return count;
}

string StringManipulation::reverseString(string &str) {
    string reversed;
    for (int i = str.length() - 1; i >= 0; i--) {
        reversed += str.at(i);
    }

    return reversed;
}

string StringManipulation::reverseWords(string &str) {
    stack<string> words;
    string currentWord;

    for (char ch : str) {
        currentWord += ch;
        if (ch == ' ') {
            words.push(currentWord);
            currentWord = "";
        }
    }

    currentWord += ' ';
    words.push(currentWord);

    string reversed;
    while (!words.empty()) {
        reversed += words.top();
        words.pop();
    }

    reversed.erase(reversed.size() - 1);

    return reversed;
}

string StringManipulation::removeDuplications(const string &str) {
    set<char> foundDuplications;
    string dupsGone;
    for (char ch : str) {
        if (foundDuplications.count(ch)) {
            continue;
        }

        foundDuplications.insert(ch);
        dupsGone += ch;
    }

    return dupsGone;
}

char StringManipulation::mostRepeatedChar(const string &str) {
    map<char, int> characterCounts;

    for (char ch : str) {
        if (!characterCounts.count(ch)) {
            characterCounts.insert(ch, 0);
        }
    }
}

bool StringManipulation::isRotation(string &str1, string &str2) {
    if (str1.length() != str2.length()) {
        return false;
    }

    string checkString = str1 + str1;

    if (checkString.find(str2) != checkString.npos) {
        return true;
    }

    return false;
}
