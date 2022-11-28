#include "Configuration.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <ctype.h>
#include <deque>
#include <dirent.h>
#include <fstream>
#include <grp.h>
#include <iomanip>
#include <iostream>
#include <limits.h>
#include <memory>
#include <pwd.h>
#include <stack>
#include <stdexcept>
#include <stdlib.h>
#include <string>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>
#include <vector>
#include <fcntl.h>

using namespace std;
// Store the height and width of current terminal

/// GLOBAL VARIABLES
struct termios original_terminal;
int ter_rows, ter_cols;
string current_dir_path;
string label;
Configuration *x;

deque<string> histlist;
int maxhistsize;
int hist_currelem;
string hist_previous = "";

struct node {
  node *children[88] = {NULL};
  bool end = false;
  string word;
};

struct node *root;

vector<string>
    record_vector; // used to store the commands in record_start-record_end
/// GLOBAL VARIABLES END

void disableRawMode() {
  tcsetattr(STDIN_FILENO, TCSAFLUSH,
            &original_terminal); // Return to Canonical mode
}

void enableRawMode() {
  tcgetattr(STDIN_FILENO,
            &original_terminal); // Save original terminal configurations

  struct termios raw = original_terminal;
  raw.c_lflag &= ~(ECHO | ICANON);

  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
  atexit(disableRawMode); // Ensure return to canonical mode
}

int getWindowSize(int *r, int *c) {
  struct winsize ws;
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
    return -1;
  } else {
    *c = ws.ws_col;
    *r = ws.ws_row;
    return 0;
  }
}

// HISTORY
void setPrev() {
  int l = histlist.size();
  if (histlist.size() >= 2) {
    // if(previous != s)
    hist_previous = histlist[l - 2];
  } else {
    hist_previous = "";
  }
}

void addToHistory(string s) {

  histlist.push_back(s);

  setPrev();

  if (hist_previous == s) {
    histlist.pop_back();
    return;
  }

  if (histlist.size() > maxhistsize) {
    histlist.pop_front();
  }
  hist_currelem = histlist.size() - 1;
}

void printHistory() {
  if (histlist.size() == 0) {
    cout << "No history" << endl;
    return;
  }

  for (int i = 0; i < histlist.size(); i++) {
    cout << i << "  " << histlist[i] << endl;
  }
}

string printForward() {
  // currelem = histlist.size() - 1;
  if (hist_currelem != histlist.size() - 1) {
    hist_currelem++;
    return histlist[hist_currelem];
  } else {
    return "";
  }
}

string printBackward() {
  // currelem = histlist.size() - 1;
  if (hist_currelem != 0) {
    return histlist[hist_currelem--];
  } else {
    return histlist[hist_currelem];
  }
}
// HISTORY END

// COMMAND AUTOCOMPLETE
void insert(struct node *root, string s) {
  struct node *curr = root;
  for (int i = 0; i < s.length(); i++) {

    int index = s[i] - 35;

    if (curr->children[index] == NULL) {
      curr->children[index] = new node();
      curr = curr->children[index];
      if (i == (s.length() - 1)) {
        curr->end = true;
        curr->word = s;
      }
    } else {
      curr = curr->children[index];
      if (i == (s.length() - 1)) {
        curr->end = true;
        curr->word = s;
      }
    }
  }
}

struct node *reach(struct node *root, string s) {

  for (int i = 0; i < s.length(); i++) {
    if (root->children[s[i] - 35] != NULL) {
      root = root->children[s[i] - 35];
    } else
      return NULL;
  }

  return root;
}

void autocomp(struct node *root, string s, vector<string> &W) {
  struct node *curr = root;
  if (curr == NULL)
    return;
  if (curr->end) {
    W.push_back(curr->word);
  }

  for (int i = 0; i < 88; i++) {
    if (curr->children[i] != NULL) {
      autocomp(curr->children[i], s, W);
    }
  }
}

void autoComplete(string s, vector<string> &words) {
  struct node *p = reach(root, s);
  autocomp(p, s, words);
}

void autoCompleteInit() {
  root = new node();
  DIR *dir = opendir("/usr/bin");
  if (dir == NULL) {
    return;
  }
  struct dirent *c = readdir(dir);
  while (c != NULL) {
    insert(root, string(c->d_name));
    c = readdir(dir);
  }
}
// COMMAND AUTOCOMPLETE END

// PARSERS AND PARSING HELPERS
string ltrim(const string &str) {
  string s(str);
  s.erase(s.begin(),
          find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace))));
  return s;
}

string rtrim(const string &str) {
  string s(str);
  s.erase(
      find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(),
      s.end());
  return s;
}

// Function to copy behaviour of Python's Split function as much as possible
vector<string> psplit(string input, char delimiter = ' ') {
  // Return a vector of strings, which are separated by delimiter in input
  string s = "";
  vector<string> tokens;
  for (size_t i = 0; i < input.size(); i++) {
    if (input.at(i) != delimiter)
      s += input.at(i);
    else {
      if (s != "")
        tokens.push_back(rtrim(ltrim(s)));
      s = "";
    }
  }
  if (s.size() != 0) {
    tokens.push_back(rtrim(ltrim(s)));
    s = "";
  }
  return tokens;
}

vector<string> parse(string input) {
  // // Return a Vector of strings, which are separated be " " in input
  // string s = "";
  // vector<string> tokens;
  // // cout << endl;
  // for (int i = 0; i < input.size(); i++)
  // {
  //     // cout << input[i] << endl;
  //     if (input.at(i) != ' ')
  //         s += input[i];
  //     else
  //     {
  //         tokens.push_back(s);
  //         s = "";
  //     }
  // }
  // if (s.size() != 0)
  // {
  //     tokens.push_back(s);
  //     s = "";
  // }

  // // tokens = pathify(tokens);
  // return tokens;
  return psplit(input, ' ');
}

vector<string> parsePipes(string input) {
  // Return a Vector of strings, which are separated be "|" in input
  return psplit(input, '|');
}

vector<string> parseSingleIORedirect(string input){
    return (psplit(input, '>'));
}
bool checkPipe(string input) {
  if (input.find("|") != std::string::npos) {
    return true;
  }
  return false;
}

bool checkSingleIORedirect(string input) {
  if (count(input.begin(), input.end(), '>') == 1) {
    return true;
  }
  return false;
}

bool checkDoubleIORedirect(string input) {
  if (count(input.begin(), input.end(), '>') == 2) {
    return true;
  }
  return false;
}
// PARSERS AND PARSING HELPERS END
string GetStdoutFromCommand(string cmd) {

  string data;
  FILE *stream;
  const int max_buffer = 256;
  char buffer[max_buffer];
  cmd.append(" 2>&1");

  stream = popen(cmd.c_str(), "r");
  if (stream) {
    while (!feof(stream))
      if (fgets(buffer, max_buffer, stream) != NULL)
        data.append(buffer);
    pclose(stream);
  }
  return data;
}

void print_promt() {
  string rel_path = current_dir_path;
  int i = rel_path.find(x->config["HOME"]);
  if (i != std::string::npos) {
    if (i == 0) {
      rel_path.replace(i, x->config["HOME"].length(), "~");
    }
  }
  // Added Colouring to our Prompt - Sharanya
  label = "\e[1;33m" + x->config["PS1"] + "\e[1;37m:\e[34m" + rel_path +
          "\e[1;37m$ \e[0m";
  write(STDOUT_FILENO, label.c_str(), label.length());
}

void init() {
  write(STDOUT_FILENO, "\x1b[2J\x1b[3J", 8);
  write(STDOUT_FILENO, "\x1b[H", 3);

  getWindowSize(&ter_rows, &ter_cols);
  enableRawMode();

  string config_fp = "ppshrc";
  x = new Configuration(config_fp);
  current_dir_path = x->config["HOME"];
  chdir(current_dir_path.c_str());
  maxhistsize = stoi(x->config["HISTSIZE"]);
  autoCompleteInit();
  print_promt();
}

vector<string> pathify(vector<string> tokens) {
  for (int i = 1; i < tokens.size(); i++) {
    if (tokens.at(i).at(0) == '~' || tokens.at(i).at(0) == '.' ||
        tokens.at(i).at(0) == '/') {
      if (tokens.at(i).at(0) == '~') {
        if (tokens.at(i).length() > 1)
          tokens.at(i) = x->config["HOME"] + "/" +
                         tokens.at(i).substr(2, tokens.at(i).length());
        else
          tokens.at(i) = x->config["HOME"];
      } else if (tokens.at(i).at(0) == '.')
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
void gotoDir(string dirname) {
  // Must work with both Absolute and Relative paths
  string abs_path = "";
  char r_path[PATH_MAX];
  bzero(r_path, PATH_MAX);
  if (dirname.at(0) == '~') {
    if (dirname.size() > 1) {
      if (dirname.at(1) == '/') {
        abs_path += x->config["HOME"];
        abs_path += dirname.substr(1);
      }
    } else {
      abs_path += x->config["HOME"];
      abs_path += dirname.substr(1);
    }
  } else if (dirname.at(0) == '/') {
    abs_path += dirname;
  } else {
    abs_path += current_dir_path + "/" + dirname;
  }
  if (realpath(abs_path.c_str(), r_path) != NULL)
    abs_path = string(r_path);

  // UPDATE FILE TRIE HERE
  DIR *d = opendir(abs_path.c_str());
  if (d == NULL) {
    string error = "Error! Directory doesn't exist";
    // Raise Error
    cout << "\n" << error << endl;
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

void runCommand(vector<string> inpCommand) {
  // Handle case where the inpCommand is empty vector
  inpCommand = pathify(inpCommand);

  char *arr[inpCommand.size() + 1];
  for (int i = 0; i < inpCommand.size(); i++) {
    arr[i] = inpCommand[i].data();
  }
  char *n = {NULL};
  arr[inpCommand.size()] = n;
//   int status;
  int pid = fork();
  if (pid < 0) {
    return;
  }

  if (pid == 0) {
    // child process
    execvp(arr[0], arr);
    kill(getpid(), SIGKILL);
  }
  if (pid > 0) {
    // parent process
    wait(&pid);
    // kill(pid, SIGKILL);
    //  return 0;
  }
  // execvp(arr[0], arr);
//    exit(0);
}

// PIPING
int spawn_proc(int in, int out, vector<string> cmd) {
  cout << "print7" << endl;
  pid_t pid;
//   int status;
  pid = fork();
  if (pid == 0) {
    cout << "print8" << endl;
    if (in != 0) {
      cout << "print 8.0.1"
           << "in: " << in << endl;
      dup2(in, 0);
      cout << "print 8.0.2" << endl;
      close(in);
      cout << "print 8.0.3" << endl;
    }
    cout << "print8.1" << endl;
    //sleep(5);
    if (out != 1) {
      cout << "print8.1.1"
           << "out:" << out << endl;
      dup2(out, 1);
    //   sleep(2);
      cout << "print8.1.2" << endl;
      close(out);
      cout << "print8.1.3" << endl;
    }
    cout << "print8.2" << endl;
    char *arr[cmd.size() + 1];
    for (int i = 0; i < cmd.size(); i++) {
      arr[i] = cmd[i].data();
      cout << "print00" << endl;
    }
    char *n = {NULL};
    arr[cmd.size()] = n;
    cout << "print10" << endl;
    int ret = execvp(arr[0], arr);
    return ret;
    // kill(getpid(), SIGKILL);
  }
  if (pid > 0) {
    cout << "print9" << endl;
    // wait(&pid);
    // kill(pid, SIGKILL);
  }
  if (pid < 0) {
    return 1;
  }
  cout << "print11" << endl;
  return pid;
  // exit(0);
}

int fork_pipes(vector<string> cmd) {
  cout << "print1" << endl;
  int n = cmd.size();
  int i;
  pid_t pid;
  int in, fd[2];

  /* The first process should get its input from the original file descriptor 0.
   */
  in = 0;

  /* Note the loop bound, we spawn here all, but the last stage of the pipeline.
   */
  for (i = 0; i < n - 1; ++i) {
    cout << "print2" << endl;
    vector<string> local_command = parse(cmd[i]);

    pipe(fd);

    /* fd[1] is the write end of the pipe, we carry `in` from the prev
     * iteration.  */
    cout << "print3" << endl;
    pid_t pd = spawn_proc(in, fd[1], local_command);
    // sleep(1);
    cout << "print12" << endl;
    /* No need for the write end of the pipe, the child will write here.  */
    wait(&pd);
    close(fd[1]);

    /* Keep the read end of the pipe, the next child will read from there.  */
    in = fd[0];
  }
  cout << "print4" << endl;
  /* Last stage of the pipeline - set stdin be the read end of the previous pipe
     and output to the original file descriptor 1. */
  if (in != 0)
    dup2(in, 0);
  cout << "print6" << endl;
  vector<string> last_command = parse(cmd[i]);

  char *arr[last_command.size() + 1];
  for (int i = 0; i < last_command.size(); i++) {
    arr[i] = last_command[i].data();
  }
  char *nn = {NULL};
  arr[last_command.size()] = nn;
  cout << "print5" << endl;
  /* Execute the last stage with the current process. */
//   sleep(2);
  execvp(arr[0], arr);
  //   return ret;
  // kill(getpid(), SIGKILL);
}
// RECORD
void record() {
  string input = "";
  int i = 0;
  char c;
  record_vector.clear();
  write(STDOUT_FILENO, ">>> ", 5);
  while (read(STDIN_FILENO, &c, 1) == 1) {

    // ENTER/RETURN key
    if ((int)c == 10) {
      if (input.size() == 0) {
        cout << endl;
        write(STDOUT_FILENO, ">>> ", 5);
        continue;
      } else {
        // vector<string> input_tokens = parse(input);
        // executeCommand(input_tokens);
        // cout << endl;

        // print_promt();

        if (input != "record_end") {
          record_vector.push_back(
              input); // pushing non-parsed string; parsing will be handled in
                      // record_run function
        } else {
          return;
        }
        cout << endl;
        write(STDOUT_FILENO, ">>> ", 5);
      }

      input.clear();
    }
    // DELETE (BACKSPACE in INDIA) key
    else if ((int)c == 127) {
      if (i > 0) {
        for (int j = 0; j < input.size(); j++)
          write(STDOUT_FILENO, "\x1b[D", 3);
        write(STDOUT_FILENO, "\x1b[0J", 4);
        i--;
        input.pop_back();
        write(STDOUT_FILENO, input.c_str(), input.size());
      } else
        continue;
    } else {
      write(STDOUT_FILENO, &c, 1);
      input.push_back(c);
      i++;
    }
  }
}

void record_run() {
  for (int i = 0; i < record_vector.size(); i++) {
    vector<string> vct =
        parse(record_vector[i]); // take out each command from record_vector,
                                 // parse it and run them invidually
    runCommand(vct);
    vct.clear();
  }
}
// RECORD END

//IO REDIRECTION
int singleRedirect(vector<string> cmd) {

  // look for command on the left side of the > operator
  // else just create file with name of RHS of > operator
  vector<string> parsed_command = parse(cmd[0]);
  string filename = cmd[1];
  const char *fname = filename.c_str();
  int pid = fork();
  if (pid < 0) {
    return 1;
  }
  if (pid == 0) {
    // child
    int file = open(fname, O_WRONLY | O_CREAT, 0777);
    if (file == -1) {
      return 2;
    }
    dup2(file, STDOUT_FILENO); // from here onwards all the stdout
                               // will be stored into the file
    close(file);

    char *arr[parsed_command.size() + 1];
    for (int i = 0; i < parsed_command.size(); i++) {
      arr[i] = parsed_command[i].data();
    }
    char *nn = {NULL};
    arr[parsed_command.size()] = nn;
    int err = execvp(arr[0], arr);
    if (err == -1) {
      printf("Could not find the program to execute\n");
      return 2;
    }
  }
  if (pid > 0) {
    // parent
    wait(NULL);
    kill(pid, SIGKILL);
  }
}
//IO REDIRECTION END
void executeCommand(vector<string> input) {
  if (input.at(0) == "cd") {
    if (input.size() > 2) {
      cout << "\n"
           << "Wrong Format for cd" << endl;
      return;
    }
    gotoDir(input.at(1));
    cout << endl;
    return;
  }
  if (input.at(0) == "exit") {
    cout << endl; //<<"Came Here" <<endl;
    while (1) {
      exit(0);
    }
  }
  if (input.at(0) == "pwd") {
    cout << "\n" << current_dir_path << endl;
    return;
  }
  if (input.at(0) == "history") {
    cout << endl;
    printHistory();
    return;
  }
  if (input.at(0) == "record_start") {
    cout << endl;
    record();
  }
  if (input.at(0) == "record_run") {
    cout << endl;
    record_run();
    cout << endl;
  }
//   int status;
  int pid = fork();
  if (pid < 0) {
    return;
  }
  if (pid == 0) {
    // child here
    cout << endl;
    runCommand(input);
    kill(getpid(), SIGKILL);
  }
  if (pid > 0) {
    // parent here
    wait(&pid);
    // kill(pid, SIGKILL);
    //  cout << "Output from Aadesh : " << GetStdoutFromCommand(input[0]) <<
    //  endl;
  }
//   exit(0);
}



void run() {
  string input = "";
  int i = 0;
  char c;
  while (read(STDIN_FILENO, &c, 1) == 1) {
    // Process Arrow Keys --> History Showing
    if (c == '\x1b') {
      if (read(STDIN_FILENO, &c, 1) && c == '[') {
        if (read(STDIN_FILENO, &c, 1) == 1) {
          string new_input;
          switch (c) {
          // Up Arrow - Move Cursor Up/Scroll Up
          case 'A':
            new_input = printBackward(); // Implement a Function for Scrolling
                                         // Up in History
            break;
          // Down Arrow - Move Cursor Down/Scroll Down
          case 'B':
            new_input = printForward(); // Implement a Function for Scrolling
                                        // Down in History
            break;
          // Incase we have time, can update input taking capabilites, but left
          // for now Right Arrow
          case 'C':
            // rightArrowProcess();
            break;
          // Left Arrow
          case 'D':
            // leftArrowProcess();
            break;
          }
          if (new_input != input) {
            for (int j = 0; j < input.size(); j++)
              write(STDOUT_FILENO, "\x1b[D", 3);
            write(STDOUT_FILENO, "\x1b[0J", 4);
            input = new_input;
            i = input.size();
            write(STDOUT_FILENO, input.c_str(), input.size());
          }
        }
      }
    }
    // TAB key -- AutoComplete
    else if ((int)c == 9) {
      vector<string> words;
      autoComplete(input, words);
      if (words.size() == 0)
        continue;
      else if (words.size() == 1) {
        for (int j = 0; j < input.size(); j++)
          write(STDOUT_FILENO, "\x1b[D", 3);
        write(STDOUT_FILENO, "\x1b[0J", 4);
        input = words.at(0);
        i = input.size();
        write(STDOUT_FILENO, input.c_str(), input.size());
      } else {
        cout << endl;
        int v_pos = 0;
        string print = "";
        // cout << words.size() << " " << input << " " << v_pos << endl;
        // while(v_pos < (words.size() - 4)){
        //     print = words.at(v_pos) + " " + words.at(v_pos+1) + " " +
        //     words.at(v_pos+2) + " " + words.at(v_pos+3)
        //             + " " + words.at(v_pos+4);
        //     v_pos += 5;
        //     cout << print << endl;
        // }
        while (v_pos < words.size()) {
          write(STDOUT_FILENO, (words.at(v_pos) + " ").c_str(),
                words.at(v_pos).size() + 2);
          v_pos++;
          if (v_pos % 5 == 0)
            write(STDOUT_FILENO, "\n", 2);
        }
        cout << endl;
        print_promt();
        write(STDOUT_FILENO, input.c_str(), input.size());
      }
    }
    // ENTER/RETURN key
    else if ((int)c == 10) {
      if (input.size() == 0) {
        cout << endl;
        print_promt();
        continue;
      } 
      else {
        addToHistory(input);
        // if (checkPipe(input)) {
        //   vector<string> input_tokens = parsePipes(input);
        //   cout << endl;
        //   for (auto x : input_tokens) {
        //     cout << x << endl;
        //   }
        //   fork_pipes(input_tokens);
        //   cout << "After Pipe" << endl;
        // } else {
        //   vector<string> input_tokens = parse(input);
        //   executeCommand(input_tokens);
        //   // cout << endl;
        //   // for(auto x: input_tokens){
        //   //     cout << x << endl;
        //   // }
        // }
        // cout << endl;
        if (checkSingleIORedirect(input)){
            vector<string> input_tokens = parseSingleIORedirect(input);
            input_tokens = pathify(input_tokens);
            cout << endl;
            for (auto x: input_tokens){
                cout << x << endl;
            }
            singleRedirect(input_tokens);
        }
        else {
          vector<string> input_tokens = parse(input);
          executeCommand(input_tokens);
          // cout << endl;
          // for(auto x: input_tokens){
          //     cout << x << endl;
          // 
        }
        print_promt();
      }
      i = 0;
      input.clear();
    }
    // DELETE (BACKSPACE in INDIA) key
    else if ((int)c == 127) {
      if (i > 0) {
        for (int j = 0; j < input.size(); j++)
          write(STDOUT_FILENO, "\x1b[D", 3);
        write(STDOUT_FILENO, "\x1b[0J", 4);
        i--;
        input.pop_back();
        write(STDOUT_FILENO, input.c_str(), input.size());
      } else
        continue;
    } else {
      write(STDOUT_FILENO, &c, 1);
      input.push_back(c);
      i++;
    }
  }
}

int main() {
  init();
  run();
  return 0;
}