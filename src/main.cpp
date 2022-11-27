#include "Configuration.hpp"
#include <string>
#include <iostream>
#include <sys/stat.h>
#include <time.h>
#include <cstring>
#include <dirent.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <stdlib.h>
#include <pwd.h>
#include <grp.h>
#include <iomanip>
#include <cmath>
#include <stack>
#include <algorithm>
#include <array>
#include <bits/stdc++.h>
#include <cstdio>
#include <fstream>
#include <memory>
#include <stdexcept>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>
#include <limits.h>
#include <ctype.h>
#include <deque>

using namespace std;
// Store the height and width of current terminal

/// GLOBAL VARIABLES
struct termios original_terminal;
int ter_rows, ter_cols;
string current_dir_path;
string label;
Configuration *x;

deque<string> histlist;
int maxhistsize = 5;
int hist_currelem;
string hist_previous = "";

struct node
{
    node *children[88] = {NULL};
    bool end = false;
    string word;
};

struct node *root;
/// GLOBAL VARIABLES END

void disableRawMode()
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_terminal); // Return to Canonical mode
}

void enableRawMode()
{
    tcgetattr(STDIN_FILENO, &original_terminal); // Save original terminal configurations

    struct termios raw = original_terminal;
    raw.c_lflag &= ~(ECHO | ICANON);

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
    atexit(disableRawMode); // Ensure return to canonical mode
}

int getWindowSize(int *r, int *c)
{
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0)
    {
        return -1;
    }
    else
    {
        *c = ws.ws_col;
        *r = ws.ws_row;
        return 0;
    }
}

// HISTORY
void setPrev(){
    int l = histlist.size();
    if(histlist.size() >= 2){    
        // if(previous != s)
            hist_previous =  histlist[l-2];
    }
    else{
        hist_previous = "";
    }

}

void addToHistory(string s){
    
    histlist.push_back(s);

    setPrev();

    if(hist_previous == s){
        histlist.pop_back();
        return;
    }

    if(histlist.size() > maxhistsize){
        histlist.pop_front();
        
    }
    hist_currelem = histlist.size() - 1;
}

void printHistory(){
    if(histlist.size() == 0){
        cout << "No history" << endl;
        return;
    }

    for(int i = 0; i < histlist.size(); i++){
        cout << i << "  " <<  histlist[i] << endl;
    }    
}

string printForward(){
    // currelem = histlist.size() - 1;
    if(hist_currelem != histlist.size() - 1){
        hist_currelem++;
        return histlist[hist_currelem];       
    }
    else{
        return "";
    }
}

string printBackward(){
    // currelem = histlist.size() - 1;
    if(hist_currelem != 0){
        return histlist[hist_currelem--];
    }
    else{
        return histlist[hist_currelem];
    }
}
// HISTORY END

// COMMAND AUTOCOMPLETE
void insert(struct node *curr, string s)
{
    for (int i = 0; i < s.length(); i++)
    {

        int index = s[i] - 35;

        if (curr->children[index] == NULL)
        {
            curr->children[index] = new node();
            curr = curr->children[index];
            if (i == (s.length() - 1))
            {
                curr->end = true;
                curr->word = s;
            }
        }
        else
        {
            curr = curr->children[index];
            if (i == (s.length() - 1))
            {
                curr->end = true;
                curr->word = s;
            }
        }
    }
}

struct node* reach(struct node *root, string s){

        for(int i = 0; i < s.length(); i++){
            if(root->children[s[i]-35] != NULL)
            {
                root = root->children[s[i]-35];
            }
        }


    return root;
}

void autocomp(struct node *curr, string s, vector<string> &W){

    
        if(curr->end){
            W.push_back(curr->word);
        }
        
        for(int i = 0; i < 88; i++){
            if(curr->children[i] != NULL){
                autocomp(curr->children[i],s,W);
            }
        }
}
// COMMAND AUTOCOMPLETE END

// Function to copy behaviour of Python's Split function as much as possible
vector<string> psplit(string input, char delimiter = ' ')
{
    // Return a vector of strings, which are separated by delimiter in input
    string s = "";
    vector<string> tokens;
    for (size_t i = 0; i < input.size(); i++)
    {
        if (input.at(i) != delimiter)
            s += input.at(i);
        else
        {
            tokens.push_back(s);
            s = "";
        }
    }
    if (s.size() != 0)
    {
        tokens.push_back(s);
        s = "";
    }
    return tokens;
}

vector<string> parse(string input)
{
    // Return a Vector of strings, which are separated be " " in input
    string s = "";
    vector<string> tokens;
    // cout << endl;
    for (int i = 0; i < input.size(); i++)
    {
        // cout << input[i] << endl;
        if (input.at(i) != ' ')
            s += input[i];
        else
        {
            tokens.push_back(s);
            s = "";
        }
    }
    if (s.size() != 0)
    {
        tokens.push_back(s);
        s = "";
    }

    // tokens = pathify(tokens);
    return tokens;
}

string GetStdoutFromCommand(string cmd)
{

    string data;
    FILE *stream;
    const int max_buffer = 256;
    char buffer[max_buffer];
    cmd.append(" 2>&1");

    stream = popen(cmd.c_str(), "r");
    if (stream)
    {
        while (!feof(stream))
            if (fgets(buffer, max_buffer, stream) != NULL)
                data.append(buffer);
        pclose(stream);
    }
    return data;
}

void print_promt()
{
    string rel_path = current_dir_path;
    int i = rel_path.find(x->config["HOME"]);
    if (i != std::string::npos)
    {
        if (i == 0)
        {
            rel_path.replace(i, x->config["HOME"].length(), "~");
        }
    }
    // Added Colouring to our Prompt - Sharanya
    label = "\e[1;33m" + x->config["PS1"] + "\e[1;37m:\e[34m" + rel_path + "\e[1;37m$ \e[0m";
    write(STDOUT_FILENO, label.c_str(), label.length());
}

void init()
{
    write(STDOUT_FILENO, "\x1b[2J\x1b[3J", 8);
    write(STDOUT_FILENO, "\x1b[H", 3);

    getWindowSize(&ter_rows, &ter_cols);
    enableRawMode();

    string config_fp = "ppshrc";
    x = new Configuration(config_fp);
    current_dir_path = x->config["HOME"];
    chdir(current_dir_path.c_str());
    print_promt();
}

vector<string> pathify(vector<string> tokens)
{
    for (int i = 1; i < tokens.size(); i++)
    {
        if (tokens.at(i).at(0) == '~' || tokens.at(i).at(0) == '.' || tokens.at(i).at(0) == '/')
        {
            if (tokens.at(i).at(0) == '~')
            {
                if (tokens.at(i).length() > 1)
                    tokens.at(i) = x->config["HOME"] + "/" + tokens.at(i).substr(2, tokens.at(i).length());
                else
                    tokens.at(i) = x->config["HOME"];
            }
            else if (tokens.at(i).at(0) == '.')
                tokens.at(i) = current_dir_path + "/" + tokens.at(i);

            char name[PATH_MAX];
            bzero(name, PATH_MAX);
            realpath(const_cast<char *>(tokens.at(i).c_str()), name);
            tokens.at(i) = string(name);
        }
    }
    return tokens;
}

// Change Directory (cd) Implementation
void gotoDir(string dirname)
{
    // Must work with both Absolute and Relative paths
    string abs_path = "";
    char r_path[PATH_MAX];
    bzero(r_path, PATH_MAX);
    if (dirname.at(0) == '~')
    {
        if (dirname.size() > 1)
        {
            if (dirname.at(1) == '/')
            {
                abs_path += x->config["HOME"];
                abs_path += dirname.substr(1);
            }
        }
        else
        {
            abs_path += x->config["HOME"];
            abs_path += dirname.substr(1);
        }
    }
    else if (dirname.at(0) == '/')
    {
        abs_path += dirname;
    }
    else
    {
        abs_path += current_dir_path + "/" + dirname;
    }
    if (realpath(abs_path.c_str(), r_path) != NULL)
        abs_path = string(r_path);

    // UPDATE FILE TRIE HERE
    DIR *d = opendir(abs_path.c_str());
    if (d == NULL)
    {
        string error = "Error! Directory doesn't exist";
        // Raise Error
        cout << "\n"
             << error << endl;
        closedir(d);
        return;
    }
    closedir(d);
    // END OF UPDATE FILE TRIE

    // Update current_dir_path
    chdir(r_path);
    current_dir_path = abs_path;
    // cout <<  "\t" << current_dir_path;
}

void runCommand(vector<string> inpCommand)
{
    // Handle case where the inpCommand is empty vector
    inpCommand = pathify(inpCommand);

    char *arr[inpCommand.size() + 1];
    for (int i = 0; i < inpCommand.size(); i++)
    {
        arr[i] = inpCommand[i].data();
    }
    char *n = {NULL};
    arr[inpCommand.size()] = n;

    execvp(arr[0], arr);
}

void executeCommand(vector<string> input)
{
    if (input.at(0) == "cd")
    {
        if (input.size() > 2)
        {
            cout << "\n"
                 << "Wrong Format for cd" << endl;
            return;
        }
        gotoDir(input.at(1));
        cout << endl;
        return;
    }
    if (input.at(0) == "exit")
    {
        cout << endl ; //<<"Came Here" <<endl;
        while(1){
            exit(0);
        }
    }
    if (input.at(0) == "pwd")
    {
        cout << "\n" << current_dir_path << endl;
        return;
    }
    if (input.at(0) == "history"){
        cout << endl;
        printHistory();
        return;
    }
    int pid = fork();
    if (pid == -1)
    {
        return;
    }
    if (pid == 0)
    {
        // child here
        cout << endl;
        runCommand(input);
    }
    else
    {
        // parent here
        wait(NULL);
        // cout << "Output from Aadesh : " << GetStdoutFromCommand(input[0]) << endl;
    }
}

void run()
{
    string input = "";
    int i = 0;
    char c;
    while (read(STDIN_FILENO, &c, 1) == 1)
    {
        // Process Arrow Keys --> History Showing
        if (c == '\x1b')
        {
            if (read(STDIN_FILENO, &c, 1) && c == '[')
            {
                if (read(STDIN_FILENO, &c, 1) == 1)
                {
                    string new_input;
                    switch (c)
                    {
                    // Up Arrow - Move Cursor Up/Scroll Up
                    case 'A':
                        new_input = printBackward(); // Implement a Function for Scrolling Up in History
                        break;
                    // Down Arrow - Move Cursor Down/Scroll Down
                    case 'B':
                        new_input = printForward(); // Implement a Function for Scrolling Down in History
                        break;
                    // Incase we have time, can update input taking capabilites, but left for now
                    // Right Arrow
                    case 'C':
                        // rightArrowProcess();
                        break;
                    // Left Arrow
                    case 'D':
                        // leftArrowProcess();
                        break;
                    }
                    if (new_input != input){
                        for (int j = 0; j < input.size(); j++)
                            write(STDOUT_FILENO, "\x1b[D", 3);
                        write(STDOUT_FILENO, "\x1b[0J", 4);
                        input = new_input;
                        i = input.size() - 1;
                        write(STDOUT_FILENO, input.c_str(), input.size());
                    }
                }

            }
        }
        // TAB key -- AutoComplete
        else if ((int)c == 9)
        {
            cout << "\nTAB key Recognized -- Implement AutoComplete" << endl;
            print_promt();
        }
        // ENTER/RETURN key
        else if ((int)c == 10)
        {
            if (input.size() == 0)
            {
                cout << endl;
                print_promt();
                continue;
            }
            else
            {
                addToHistory(input);
                vector<string> input_tokens = parse(input);
                executeCommand(input_tokens);
                // cout << endl;
                print_promt();
            }
            i = 0;
            input.clear();
        }
        // DELETE (BACKSPACE in INDIA) key
        else if ((int)c == 127)
        {
            if (i > 0)
            {
                for (int j = 0; j < input.size(); j++)
                    write(STDOUT_FILENO, "\x1b[D", 3);
                write(STDOUT_FILENO, "\x1b[0J", 4);
                i--;
                input.pop_back();
                write(STDOUT_FILENO, input.c_str(), input.size());
            }
            else
                continue;
        }
        else
        {
            write(STDOUT_FILENO, &c, 1);
            input.push_back(c);
            i++;
        }
    }
}

int main()
{
    init();
    run();
    return 0;
}
