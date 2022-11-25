#include <array>
#include <bits/stdc++.h>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <vector>
#include <sys/wait.h>

using namespace std;

vector<string> binOutVec;
std::string exec(const char *cmd) {
  std::array<char, 128> buffer;
  std::string result;
  std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
  if (!pipe) {
    throw std::runtime_error("popen() failed!");
  }
  while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
    result += buffer.data();
    binOutVec.push_back(buffer.data());
  }

  return result;
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

int main() {

  const char *cmd =
      "cd / && cd /bin && ls >> /home/aadesh/Downloads/posixShell/binout.txt";

  int pid = fork();
  if (pid == -1) {
    return 1;
  }
  if (pid == 0) {
    // child here
    exec(cmd);
  } else {
    // parent here

    wait(NULL);

    std::ifstream fin("/home/aadesh/Downloads/posixShell/binout.txt");

    std::vector<string> data;

    string element;
    while (fin >> element) {
      data.push_back(element);
    }

    for (int i = 0; i < data.size(); i++) {
      cout << data[i] << endl;
    }

    // trie code here
  }
  // exec(cmd);

  return 0;
}