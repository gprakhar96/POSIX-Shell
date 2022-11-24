#include <iostream>
#include "Configuration.hpp"
#include <string>
#include <unistd.h>
#include <iostream>
#include <sys/stat.h>
#include <time.h>
#include <cstring>
#include <dirent.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <pwd.h>
#include <grp.h>
#include <iomanip>
#include <cmath>
#include <stack>
#include <vector>
#include <algorithm>
#include <limits.h>

using namespace std;
// Store the height and width of current terminal
struct termios original_terminal;
int ter_rows, ter_cols;
void disableRawMode(){
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_terminal); // Return to Canonical mode
}

void enableRawMode(){
    tcgetattr(STDIN_FILENO, &original_terminal); // Save original terminal configurations

    struct termios raw = original_terminal;
    raw.c_lflag &= ~(ECHO | ICANON);

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
    atexit(disableRawMode); //Ensure return to canonical mode
}

int getWindowSize(int *r, int *c ){
    struct winsize ws;
    if(ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
        return -1;
    } else {
        *c = ws.ws_col;
        *r = ws.ws_row;
        return 0;
    }
}

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

class App{

    string current_dir_path;
    string relative_current_dir_path;
    string label;
    
    public:
    App(){
        write(STDOUT_FILENO,"\x1b[2J\x1b[3J",8);
        write(STDOUT_FILENO,"\x1b[H",3);

        getWindowSize(&ter_rows, &ter_cols);
        enableRawMode();

        string config_fp = "ppshrc"; 
        Configuration x(config_fp);

        current_dir_path = x.config["HOME"]; 
        relative_current_dir_path = "";
        label = x.config["PS1"] + ":~"+ relative_current_dir_path + "$ "; 
        print_promt();       
    }
     
    void print_promt(){
        write(STDOUT_FILENO, label.c_str(), label.length());
    }

    void run(){
        string input="";
        int i = 0;
        char c;
        while (read(STDIN_FILENO, &c, 1) == 1){
            if (c == '\x1b'){
                return;
            }
            else if ((int)c == 10){
                if (input.size() == 0){
                    cout << endl;
                    print_promt();
                    continue;
                }
                else
                {
                    vector<string> input_tokens = parse(input);
                    // executeCommand(input);
                    cout << endl;
                    print_promt();  
                }
                
                input.clear();
            }
            else if ((int)c == 127){
                if (i>0){
                    for(int j=0;j<input.size();j++)
                        write(STDOUT_FILENO,"\x1b[D",3);
                    write(STDOUT_FILENO,"\x1b[0J",4);
                    i--;
                    input.pop_back();
                    write(STDOUT_FILENO,input.c_str(),input.size());
                }
                else
                    continue;
            }
            else{
                write(STDOUT_FILENO,&c,1);
                input.push_back(c);
                i++;
            }
        }
        
    }
};

int main(){
    App po;
    po.run();
    return 1;
}