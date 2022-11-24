#include<iostream>
#include<bits/stdc++.h>
using namespace std;

void removefrontspace(char* arr){
	cout << arr;
	char * newarr;
	int i =0;
	while(arr[i]==' '){
		i++;
	} int j=0;
	while(arr[i]){
		newarr[j]=arr[i];i++;j++;
	}
	arr=newarr;
}

int main(){

    string s;
    cin >> s;
    char *t;
    t="ls";
    removefrontspace(t);
    return 0;
}