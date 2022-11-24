#include <iostream>
#include <vector>
#include <cstring>
#include <fstream>

using namespace std;

//Function to copy behaviour of Python's Split function as much as possible
vector<string> psplit(string input, char delimiter = ' '){
    // Return a vector of strings, which are separated by delimiter in input
    string s = "";
    vector<string> tokens;
    for(size_t i=0;i<input.size();i++){
        if(input.at(i) != delimiter)
            s += input.at(i);
        else{
            tokens.push_back(s);
            s = "";
        }
    }
    if (s.size() != 0){
        tokens.push_back(s);
        s = "";

    }
    return tokens;
}

vector<string> parse(string input){
    // Return a Vector of strings, which are separated be " " in input
    string s = "";
    vector<string> tokens;
    // cout << endl;
    for(int i=0;i<input.size();i++){
        // cout << input[i] << endl;
        if (input.at(i) != ' ')
            s += input[i];
        else{
            tokens.push_back(s);
            s="";
        }
    }
    if (s.size() != 0){
        tokens.push_back(s);
        s="";
    }
    return tokens;
}