// Example program
#include <bits/stdc++.h>
#include <iostream>
#include <unistd.h>

using namespace std;

int main()
{
    char *arr[]={
        "cd",
        "cd /home/aadesh/Downloads",
        NULL
    };

    // char *arr[]={
    //     "pwd",
    //     "pwd",
    //     NULL
    // };
    execvp(arr[0],arr);
}