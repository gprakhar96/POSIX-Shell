#include "Configuration.hpp"
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <limits.h>
#include <pwd.h>

Configuration::Configuration(string config_file_path){

    Configuration::config["USER"] = string(getlogin());
    
    char hostname[HOST_NAME_MAX + 1];
    gethostname(hostname, HOST_NAME_MAX + 1);
    Configuration::config["HOSTNAME"] = string(hostname);

    struct passwd *pw = getpwuid(getuid());
    Configuration::config["HOME"] = string(pw->pw_dir);
    
    Configuration::config["PS1"] = Configuration::config["USER"] + "@" +Configuration::config["HOSTNAME"];
    
    fstream ppshrc_file;
    ppshrc_file.open(config_file_path, ios::in);
    
    string env_string;
    if(ppshrc_file.is_open()){
        while(getline(ppshrc_file, env_string)){
            Configuration::addEnvironmentVariable(env_string);
        }
        ppshrc_file.close();
    }
}

void Configuration::addEnvironmentVariable(string env_string){
    int equals_delim_i = env_string.find_first_of('=');
    string env_var_name, env_var_value;
    if(equals_delim_i != std::string::npos){
        env_var_name = env_string.substr(0, equals_delim_i);
        env_var_value = (equals_delim_i == env_string.size()-1) ? "" : env_string.substr(equals_delim_i+1);

        Configuration::config[env_var_name] = env_var_value; 
    }
}