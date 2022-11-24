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
  execvp(arr[0], arr);
}

int main(int argc, char *argv[], char *envp[]) {

  vector<string> inpCommand;
  inpCommand.push_back("ping");
  inpCommand.push_back("www.google.com");
  runCommand(inpCommand);
  return 0;
}