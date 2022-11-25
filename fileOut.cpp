#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

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

int main() {

  // inpCommand.push_back("printenv");
  // inpCommand.push_back("diff");
  // inpCommand.push_back("/home/aadesh/peer.cpp");
  // inpCommand.push_back("/home/aadesh/tracker.cpp");
  // inpCommand.push_back("mkdir");
  // inpCommand.push_back("/home/aadesh/testDirectory");
  // inpCommand.push_back("pwd");
  // inpCommand.push_back("mkdir");
  // inpCommand.push_back("/home/aadesh/Downloads/posixShell/demoDirectory11");
  // inpCommand.push_back("ping");
  // inpCommand.push_back("www.google.com");
  // inpCommand.push_back("ifconfig");
  // cd / && cd /bin && ls >> /home/aadesh/Downloads/posixShell/binout.txt
    string command;
  // string ls = GetStdoutFromCommand("sdhbjzfvkgzsf");
  // string ls = GetStdoutFromCommand("cd");
  // string ls = GetStdoutFromCommand("mkdir /home/aadesh/Downloads/abcd");
  // string ls = GetStdoutFromCommand("pwd");
  string ls = GetStdoutFromCommand("ifconfig");
  cout << "COMMAND o/p: " << endl << ls << endl;
  return 0;
}