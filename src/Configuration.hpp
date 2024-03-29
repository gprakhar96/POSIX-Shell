#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

class Configuration{  
    
    public:
        unordered_map<string, string> config;
        Configuration(string config_file_path);
        void addEnvironmentVariable(string env_string);

};

#endif