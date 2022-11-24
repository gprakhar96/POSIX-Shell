#include <bits/stdc++.h>
#include <iostream>
#include <unistd.h>
using namespace std;

void runCommand(vector<string> inpCommand) {

  char *arr[inpCommand.size() + 1];
  for (int i = 0; i < inpCommand.size(); i++) {
    arr[i] = inpCommand[i].data();
  }
  char *n = {NULL};
  arr[inpCommand.size()] = n;
  // for (int i = 0; i < inpCommand.size(); i++) {
  //   cout << arr[i] << endl;
  // }
  // char *arr[] = {"cat", "/home/aadesh/Downloads/posixShell/execTest.cpp",
  // NULL};

  execvp(arr[0], arr);
}

int main(int argc, char *argv[], char *envp[]) {

  //  string command;
  //  cin >> command;
  vector<string> inpCommand;
  // command = "ping www.google.com";
  // std::istringstream ss(command);
  // std::string token;

  // while (std::getline(ss, token, ' ')) {
  //   cout << token << endl;
  //   inpCommand.push_back(token);
  // }
  // int size = inpCommand.size();
  // for (int i = 0; i < size; i++) {
  //   cout << inpCommand[i];
  // }

  // inpCommand.push_back("printenv");
  // inpCommand.push_back("/home/aadesh/peer.cpp");
  // inpCommand.push_back("/home/aadesh/tracker.cpp");
  // inpCommand.push_back("testDirectory");
  inpCommand.push_back("ping");
  inpCommand.push_back("www.google.com");
  runCommand(inpCommand);
  return 0;
}