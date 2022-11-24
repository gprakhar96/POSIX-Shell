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
  // for(int i=0; i<inpCommand.size(); i++){
  //   cout << arr[i] << endl;
  // }
  // char *arr[] = {"cat", "/home/aadesh/Downloads/posixShell/execTest.cpp", NULL};

  execvp(arr[0], arr);
}

int main(int argc, char *argv[], char *envp[]) {

  vector<string> inpCommand;
  inpCommand.push_back("printenv");
  // inpCommand.push_back("/home/aadesh/peer.cpp");
  // inpCommand.push_back("/home/aadesh/tracker.cpp");
  // inpCommand.push_back("testDirectory");

  runCommand(inpCommand);
  return 0;
}