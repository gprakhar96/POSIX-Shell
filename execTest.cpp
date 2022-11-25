#include <array>
#include <bits/stdc++.h>
#include <cstdio>
#include <errno.h>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string.h>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <vector>
using namespace std;

int errStatus = 0;
string commandOut;
std::string join(std::vector<std::string> const &strings, std::string delim) {
  std::stringstream ss;
  std::copy(strings.begin(), strings.end(),
            std::ostream_iterator<std::string>(ss, delim.c_str()));
  return ss.str();
}

int runCommand(vector<string> inpCommand) {

  char *arr[inpCommand.size() + 1];
  for (int i = 0; i < inpCommand.size(); i++) {
    arr[i] = inpCommand[i].data();
  }
  char *n = {NULL};
  arr[inpCommand.size()] = n;

  int err = execvp(arr[0], arr);

  // string data;
  // FILE *stream;
  // const int max_buffer = 256;
  // char buffer[max_buffer];

  // std::string delim = " ";

  // inpCommand.erase(inpCommand.end());
  // std::string str = join(inpCommand, delim);

  // str.append(" 2>&1");

  // stream = popen(str.c_str(), "r");
  // if (stream) {
  //   while (!feof(stream))
  //     if (fgets(buffer, max_buffer, stream) != NULL)
  //       data.append(buffer);
  //   pclose(stream);
  // }
  // commandOut = data;
  // return data;

  return err;
}

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

int main(int argc, char *argv[], char *envp[]) {

  vector<string> inpCommand1;
  vector<string> inpCommand2;
  vector<string> inpCommand3;
  vector<string> inpCommand4;
  vector<string> inpCommand5;
  inpCommand1.push_back("sdhbjzfvkgzsf");
  inpCommand2.push_back("cd /home/aadesh/Downloads");
  inpCommand3.push_back("mkdir /home/aadesh/Downloads/abcd");
  inpCommand4.push_back("pwd");
  inpCommand5.push_back("ifconfig");

  
  int pid = fork();
  if (pid == -1) {
    return 1;
  }
  if (pid == 0) {
    // child here

    // errStatus = runCommand(inpCommand1);
    // errStatus = runCommand(inpCommand2);
    //x errStatus = runCommand(inpCommand3);
    errStatus = runCommand(inpCommand4);
    // errStatus = runCommand(inpCommand5);
    exit(errno);
  } else {
    // parent here

    // wait(NULL);

    // cout << "OutPut from my function: "
    //      << GetStdoutFromCommand("mkdir /home/aadesh/Downloads/abcd") <<
    //      endl;

    // cout << "errStatus from exec(): " << errStatus << endl;
    // cout << "ERRNO: " << strerror(errno) << endl;

    
    
    int status;
    wait(&status); /*you made a exit call in child you
                     need to wait on exit status of child*/
    // // cout << "OutPut from my function: " <<
    // // GetStdoutFromCommand("sdhbjzfvkgzsf") << endl; cout << "Error Staus: "
    // <<
    // // errStatus << endl;

    // commandOut = GetStdoutFromCommand("sdhbjzfvkgzsf");
    // commandOut = GetStdoutFromCommand("cd /home/aadesh/Downloads");
    // commandOut = GetStdoutFromCommand("mkdir /home/aadesh/Downloads/abcd");
    // commandOut = GetStdoutFromCommand("pwd");
    commandOut = GetStdoutFromCommand("ifconfig");
    // wait(NULL);
    string typeout;
    // typeout = GetStdoutFromCommand("sdhbjzfvkgzsf");
    // typeout = GetStdoutFromCommand("cd /home/aadesh/Downloads");
    // typeout = GetStdoutFromCommand("mkdir /home/aadesh/Downloads/abcd");
    // typeout = GetStdoutFromCommand("pwd");
    typeout = GetStdoutFromCommand("ifconfig");

    cout << "errStatus from exec(): " << errStatus << endl;
    cout << "ERRNO: " << strerror(errno) << endl;
    cout << "COMMAND OUT: " << commandOut << endl;
    cout << "typeout(): " << typeout << endl;
    cout << "WEXITSTATUS(): " << WEXITSTATUS(status) << endl;

    if (WIFEXITED(status))
      printf("child exited with = %d\n", WEXITSTATUS(status));
    // // you should see the errno here
  }

  return 0;
}