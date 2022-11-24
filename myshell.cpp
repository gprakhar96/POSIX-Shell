#include<stdio.h>
#include<string>
#include<string.h>
#include <unistd.h> 
#include<ctype.h>
#include<vector>
#include<string.h>
#include<fstream>
#include<iostream>
#include<stdlib.h>
#include <fcntl.h>
#include  <sys/types.h>
#include <sys/wait.h>
#include <fstream>
#include <string>
#include <iostream>


#include <bits/stdc++.h>
#define TOK_BUFSIZE 64
#define TOK_DELIM " \t\r\n\a"
using namespace std;
extern char ** environ;

char  *_line,*_path,*_home,*_user,*_hostname,*_ps1, *_histsize,*PS1,*_mp4,*_mp3,*_text;
int background,histmaxsize,recordstatus;

int launchproc(char **args);
char **splitline(char *arr);
int execute(char **args);
void printpwd();
void initShell(){
	char ** newEnv;
	// printf("\n");
		int	 count =0;

		ifstream rc_store;
		    string line;
		    rc_store.open("myrc");
			while (getline(rc_store, line))
			{
				if (line.rfind("PATH=",0) != std::string::npos)
				{
		    		_path = strcpy(new char[line.length() + 1], line.c_str());
		   		}
				if (line.rfind("HOME=",0) != std::string::npos)
				{
		    		_home = strcpy(new char[line.length() + 1], line.c_str());
		    	}
				if (line.rfind("USER=",0) != std::string::npos)
				{
		    		_user = strcpy(new char[line.length() + 1], line.c_str());
		    	}
		    	if (line.rfind("HOSTNAME=",0) != std::string::npos)
				{
		    		_hostname = strcpy(new char[line.length() + 1], line.c_str());
		    	}
		    	if (line.rfind("mp4=",0) != std::string::npos)
				{
		    		_mp4 = strcpy(new char[line.length() + 1], line.c_str());
		    	}
		    	if (line.rfind("txt=",0) != std::string::npos)
				{
		    		_text = strcpy(new char[line.length() + 1], line.c_str());
		    	}
		    	if (line.rfind("mp3=",0) != std::string::npos)
				{
		    		_mp3 = strcpy(new char[line.length() + 1], line.c_str());
		    	}
		    	if (line.rfind("PS1=",0) != std::string::npos)
				{
		    		_ps1 = strcpy(new char[line.length() + 1], line.c_str());
		    		line.erase(0,4);
		    		PS1=strcpy(new char[line.length() + 1], line.c_str());
		    	}
		    	if (line.rfind("HISTSIZE=",0) != std::string::npos)
				{
		    		_histsize = strcpy(new char[line.length() + 1], line.c_str());
		    		//sscanf((line+9), "%d", &histmaxsize);
		    		line.erase(0,9);
		    		_line = strcpy(new char[line.length() + 1], line.c_str());
		    		sscanf(_line, "%d", &histmaxsize);
		    	}		    	
			}
			//cout<<endl<<"loop: "<<endl;
    while(environ[count] != NULL)
   {
   		string current;
   		current=environ[count]; 
   		if(current.find("PATH=")==0){
         strcpy(environ[count],_path);
     	}
     	if(current.find("USER=")==0){
         strcpy(environ[count],_user);
     	}
     	if(current.find("HOME=")==0){
         strcpy(environ[count],_home);
     	}
         count++;
   }
   // setenv("PATH",_path,1);
   // setenv("USER",_user,1);
   // setenv("HOME",_home,1);
  // setenv("NEWval",_home,1);
 //  cout<<_hostname<<endl<<_ps1<<endl<<_histsize<<endl;
   //environ[count]=NULL;
//   count=0;//printall
   // cout<<"printall"<<endl;
   // while(environ[count] != NULL)
   // {
   //       printf("%d [%s] \n",count+1, environ[count]);
   //       count++;
   // }

  // cout<<endl<<endl;
}

void multiPipe(char ** argv,int count){
	//cout<<"Multipipe"<<endl;
	pid_t p0;
	p0=fork();
	if(p0==-1){
		perror("forking error");
		exit(1);		
	}
	if(p0==0){
		int i;
		 for( i=0; i<count; i++)
	    {
	        int pd[2];
	        pipe(pd);

	        if (!fork()) {
	        	
	        	char *str[1024];
				char temp[1024];
				strcpy(temp,argv[i]);
				int index=0;
				char *ptr= strtok(temp," \n");
				while(ptr != NULL)
				{
					str[index]=ptr;
					ptr = strtok(NULL, " \n");
					index++;
				}
				str[index]=NULL;
				// int j=0;
				// while(str[j]!=NULL)
				// printf("@%s\n",str[j++]);

	            dup2(pd[1], 1); 
	            //execlp(argv[i], argv[i], NULL);
	           // cout<<"STR: " <<str[0]<<" "<<str<<endl;
	            execvp(str[0],str);
	            perror("exec error");
	            abort();
	        }
	        dup2(pd[0], 0);
	        close(pd[1]);
	    }

	    char *str[1024];
		char temp[1024];
		strcpy(temp,argv[i]);
		int index=0;
		char *ptr= strtok(temp," ");
		while(ptr != NULL)
		{
			str[index]=ptr;
			ptr = strtok(NULL, " ");
			index++;
		}
		str[index]=NULL;
				// int j=0;
				// while(str[j]!=NULL)
				// printf("@%s\n",str[j++]);

	    execvp(str[0],str);
	}
	else{//parent
		wait(NULL);
	}
}
void multiPipeWithOpRed(char ** argv,int count){
	pid_t p0;
	p0=fork();
	if(p0==-1){
		perror("forking error");
		exit(1);		
	}
	if(p0==0){
		int i;
		 for( i=0; i<count; i++)
	    {
	        int pd[2];
	        pipe(pd);

	        if (!fork()) {
	        	
	        	char *str[1024];
				char temp[1024];
				strcpy(temp,argv[i]);
				int index=0;
				char *ptr= strtok(temp," \n");
				while(ptr != NULL)
				{
					str[index]=ptr;
					ptr = strtok(NULL, " \n");
					index++;
				}
				str[index]=NULL;
				// int j=0;
				// while(str[j]!=NULL)
				// printf("#%s\n",str[j++]);

	            dup2(pd[1], 1);
	            //execlp(argv[i], argv[i], NULL);
	            //execvp(str[0],str);
	            if (execvp(str[0], str) == -1) {
			      perror("error1");
			      exit(EXIT_FAILURE);
			    }
	            perror("exec error");
	            abort();
	        }
	        dup2(pd[0], 0);
	        close(pd[1]);
	    }


	    char *str[1024];
		char temp[1024];
		strcpy(temp,argv[i]);
		int index=0;
		char *ptr= strtok(temp,">");
		while(ptr != NULL)
		{
			str[index]=ptr;
			ptr = strtok(NULL, ">");
			index++;
		}
		str[index]=NULL;

		char *redirection=*str;
		char *red1;
		char *red2=*(str+index-1);
		//cout<<"Red val:"<<redirection<<endl;
		//cout<<"Red val2:"<<red2<<endl;
		char ** args2;
     	args2=splitline(redirection);

     	char * ofile=red2;
     	if (ofile != NULL) {
                int fd2;
                if ((fd2 = open(ofile, O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0) {
                    perror("output file error");
                    exit(0);
                }
                dup2(fd2, STDOUT_FILENO);
                close(fd2);
         }
		
		if (execvp(args2[0], args2) == -1) {
	      perror("error1");
	      exit(EXIT_FAILURE);
	    }
	    //execvp(str[0],str);
	}
	else{//parent
		wait(NULL);
	}
}

void multiPipeWithOpRedAppend(char ** argv,int count){
	pid_t p0;
	p0=fork();
	if(p0==-1){
		perror("forking error");
		exit(1);		
	}
	if(p0==0){
		int i;
		 for( i=0; i<count; i++)
	    {
	        int pd[2];
	        pipe(pd);

	        if (!fork()) {
	        	
	        	char *str[1024];
				char temp[1024];
				strcpy(temp,argv[i]);
				int index=0;
				char *ptr= strtok(temp," \n");
				while(ptr != NULL)
				{
					str[index]=ptr;
					ptr = strtok(NULL, " \n");
					index++;
				}
				str[index]=NULL;
				// int j=0;
				// while(str[j]!=NULL)
				// printf("#%s\n",str[j++]);

	            dup2(pd[1], 1); 
	            //execlp(argv[i], argv[i], NULL);
	            //execvp(str[0],str);
	            if (execvp(str[0], str) == -1) {
			      perror("error1");
			      exit(EXIT_FAILURE);
			    }
	            perror("exec error");
	            abort();
	        }
	        dup2(pd[0], 0);
	        close(pd[1]);
	    }


	    char *str[1024];
		char temp[1024];
		strcpy(temp,argv[i]);
		int index=0;
		char *ptr= strtok(temp,">>");
		while(ptr != NULL)
		{
			str[index]=ptr;
			ptr = strtok(NULL, ">>");
			index++;
		}
		str[index]=NULL;

		char *redirection=*str;
		char *red1;
		char *red2=*(str+index-1);
		//cout<<"Red val:"<<redirection<<endl;
		//cout<<"Red val2:"<<red2<<endl;
		char ** args2;
     	args2=splitline(redirection);

     	char * ofile=red2;
     	if (ofile != NULL) {
                int fd2;
                if ((fd2 = open(ofile, O_WRONLY| O_APPEND | O_CREAT, 0644)) < 0) {
                    perror("output file error");
                    exit(0);
                }
                dup2(fd2, STDOUT_FILENO);
                close(fd2);
         }
		
		if (execvp(args2[0], args2) == -1) {
	      perror("error1");
	      exit(EXIT_FAILURE);
	    }
	    //execvp(str[0],str);
	}
	else{//parent
		wait(NULL);
	}
}


void removefrontspace(char* arr){
	cout << arr;
	arr="ls";
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
// char *removefrontandbackspace(char * tok){
// 	return tok;
// }

char **splitline(char *arr)
{
	int bufsize = TOK_BUFSIZE, pos = 0;
	char **tokens = (char ** )malloc(bufsize * sizeof(char*));
	char *token;
	token = strtok(arr, TOK_DELIM);
	while (token != NULL) {
		tokens[pos] = token;
		pos++;
		if (pos >= bufsize) {
		  bufsize += TOK_BUFSIZE;
		  tokens = (char ** )realloc(tokens, bufsize * sizeof(char*));
		}
		token = (char *)strtok(NULL, TOK_DELIM);
	}
	tokens[pos] = NULL;
	return tokens;
}
char ** splitByPipe(char *arr){
	int bufsize = TOK_BUFSIZE, pos = 0;
	char **tokens = (char ** )malloc(bufsize * sizeof(char*));
	char *token;
	token = strtok(arr, "|");
	while (token != NULL) {
		tokens[pos] = token;
		pos++;
		if (pos >= bufsize) {
		  bufsize += TOK_BUFSIZE;
		  tokens = (char ** )realloc(tokens, bufsize * sizeof(char*));
		}
		token = (char *)strtok(NULL, "|");
	}
	tokens[pos] = NULL;
	return tokens;
}
char ** splitByDot(char *arr){
	int bufsize = TOK_BUFSIZE, pos = 0;
	char **tokens = (char ** )malloc(bufsize * sizeof(char*));
	char *token;
	token = strtok(arr, ".");
	while (token != NULL) {
		tokens[pos] = token;
		pos++;
		if (pos >= bufsize) {
		  bufsize += TOK_BUFSIZE;
		  tokens = (char ** )realloc(tokens, bufsize * sizeof(char*));
		}
		token = (char *)strtok(NULL, ".");
	}
	tokens[pos] = NULL;
	return tokens;
}

int launchproc(char **args)
{
  pid_t pid, wpid;
  int status;

  pid = fork();
  if (pid == 0) {
    // Child process
    if (execvp(args[0], args) == -1) {
      perror("error1");
      exit(EXIT_FAILURE);
    }
  } else if (pid < 0) {
    // Error forking
    perror("error in fork");
  } else { 
  	if(background==0){
	    // Parent process
	    do {
	    	wpid = wait(NULL);
	      //wpid = waitpid(pid, &status, WUNTRACED);
	    } while (!(WIFEXITED(status) || WIFSIGNALED(status)));
	}
  }

  return 1;
}

void launchIORedProc(char **args,char * arr){
	//cout<<"\nIored called\n";
	char line[81];
	int size=0,i = 0;
	while(args[size]!=NULL){ size++;}
	
	while (strcmp(args[i],">")!=0){
		//cout<<"args i: "<<i<<" "<<args[i]<<endl;
	    i++;
	}
	char * ofile=args[i+1];
	char ** dumargs=(char **)malloc(sizeof(char*) * i);

	i = 0;
	while (strcmp(args[i],">")!=0){
		dumargs[i]=args[i];
	    i++;
	}
	dumargs[i]=NULL;
	// i=0;
	// while (dumargs[i]!=NULL){
	// //cout<<"dumargs i: "<<i<<" "<<dumargs[i]<<endl;
	//     i++;
	// }
	//cout<<"ofile is : "<<ofile<<endl;

	i=0;


	pid_t pid;
	//char *ofile=args[3];//ofile=a
	        switch (pid = fork()) {
        case 0:
       // cout<<"out switch 0";
            if (ofile != NULL) {
                int fd2;
                if ((fd2 = open(ofile, O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0) {
                    perror("output file error");
                    exit(0);
                }
                dup2(fd2, STDOUT_FILENO);
                close(fd2);
            }
            //args[2]=NULL;args[3]=NULL;
            execvp(arr, dumargs);        /* child */
           // signal(SIGINT, SIG_DFL);
            exit(1);
            break;

        case -1:
            fprintf(stderr, "ERROR can't create child process!\n");
            break;

        default:
            wait(NULL);
        }
}
void launchIORedProcApp(char ** args, char * arr){
	//cout<<"IoAppend";
		char line[81];
	int size=0,i = 0;
	while(args[size]!=NULL){ size++;}
	
	while (strcmp(args[i],">>")!=0){
		//cout<<"args i: "<<i<<" "<<args[i]<<endl;
	    i++;
	}
	char * ofile=args[i+1];
	char ** dumargs=(char **)malloc(sizeof(char*) * i);

	i = 0;
	while (strcmp(args[i],">>")!=0){
		dumargs[i]=args[i];
	    i++;
	}
	dumargs[i]=NULL;
	i=0;
	while (dumargs[i]!=NULL){
	//cout<<"dumargs i: "<<i<<" "<<dumargs[i]<<endl;
	    i++;
	}
	//cout<<"ofile is : "<<ofile<<endl;

	i=0;


	pid_t pid;
	//char *ofile=args[3];//ofile=a
	        switch (pid = fork()) {
        case 0:
       // cout<<"out switch 0";
            if (ofile != NULL) {
                int fd2;
                if ((fd2 = open(ofile, O_WRONLY| O_APPEND | O_CREAT, 0644)) < 0) {
                    perror("output file error");
                    exit(0);
                }
                dup2(fd2, STDOUT_FILENO);
                close(fd2);
            }
            //args[2]=NULL;args[3]=NULL;
            execvp(arr, dumargs);        /* child */
            //signal(SIGINT, SIG_DFL);
            exit(1);
            break;

        case -1:
            fprintf(stderr, "ERROR can't create child process!\n");
            break;

        default:
            wait(NULL);
        }
}
void launchOIRedProc(char ** args, char * arr){
	//cout<<"output red"<<endl;
	char line[81];
	int size=0,i = 0;
	while(args[size]!=NULL){ size++;}
	
	while (strcmp(args[i],"<")!=0){
		//cout<<"args i: "<<i<<" "<<args[i]<<endl;
	    i++;
	}
	char * ifile=args[i+1];
	char ** dumargs=(char **)malloc(sizeof(char*) * i);

	i = 0;
	while (strcmp(args[i],"<")!=0){
		dumargs[i]=args[i];
	    i++;
	}
	dumargs[i]=NULL;
	i=0;
	while (dumargs[i]!=NULL){
	//cout<<"dumargs i: "<<i<<" "<<dumargs[i]<<endl;
	    i++;
	}
//	cout<<"ifile is : "<<ifile<<endl;

	i=0;


	pid_t pid;

	switch (pid = fork()) { 
        case 0:
            // open stdin
            if (ifile != NULL) {
                int fd = open(ifile, O_RDONLY);

                if (dup2(fd, STDIN_FILENO) == -1) {
                    fprintf(stderr, "dup2 failed");
                }

                close(fd);
            }

            execvp(arr, dumargs);        /* child */
           // signal(SIGINT, SIG_DFL);
            exit(1);
            break;

        case -1:
            /* unlikely but possible if hit a limit */
            fprintf(stderr, "ERROR can't create child process!\n");
            break;

        default:
            //printf("am I here");
            wait(NULL);
            //waitpid(pid, 0, 0);
        }

}
void launchOIRedProcApp(char ** args, char * arr){
	//cout<<"output red App"<<endl;
	char line[81];
	int size=0,i = 0;
	while(args[size]!=NULL){ size++;}
	
	while (strcmp(args[i],"<<")!=0){
		//cout<<"args i: "<<i<<" "<<args[i]<<endl;
	    i++;
	}
	char * ifile=args[i+1];
	char ** dumargs=(char **)malloc(sizeof(char*) * i);

	i = 0;
	while (strcmp(args[i],"<")!=0){
		dumargs[i]=args[i];
	    i++;
	}
	dumargs[i]=NULL;
	i=0;
	while (dumargs[i]!=NULL){
	//cout<<"dumargs i: "<<i<<" "<<dumargs[i]<<endl;
	    i++;
	}
	//cout<<"ifile is : "<<ifile<<endl;

	i=0;


	pid_t pid;

	switch (pid = fork()) { 
        case 0:
            // open stdin
            if (ifile != NULL) {
                int fd = open(ifile, O_RDONLY);

                if (dup2(fd, STDIN_FILENO) == -1) {
                    fprintf(stderr, "dup2 failed");
                }

                close(fd);
            }

            execvp(arr, dumargs);        /* child */
            //signal(SIGINT, SIG_DFL);
            exit(1);
            break;

        case -1:
            fprintf(stderr, "ERROR  child process!\n");
            break;

        default:
            //printf("am I here");
            wait(NULL);
            //waitpid(pid, 0, 0);
        }
}

int execute(char **args)
{
  int i=0;
  //printf("1.cd.. arg0: %s\n",args[0]);
  if (args[0] == NULL) {
    return 1;
  }
  if ( strcmp(args[0], "cd")==0 ) {
		{
			if(args[1]==NULL){
				//cout<<"move to home"<<_home<<endl;
				if(_home[0]=='/')
				{
					//cout<<"move to home2"<<_home<<endl;
					if (chdir((_home)) != 0) {
						  perror("error");
					}
				}
					else{
						if (chdir((_home+5)) != 0) {
						  perror("error");
						}	
					}
			}
			else{
				//cout<<"move to dir"<<args[1]<<endl;
				if (chdir(args[1]) != 0) {
				  perror("error");
				}
			}
		}
	   return 1;
  }

  return launchproc(args);
}

void printpwd()
{
	char buf[1000];
	if (getcwd(buf, sizeof(buf)) != NULL)
		printf("%s",buf);
	else
		perror("Error:");	
}
// void sig_chld_hdlr(int num)   Install new handler 
// {
//     //cout<<"Signchld received"<<endl;
// }


int main(){
	initShell();
	//init();
	//cout<<environ<<endl;
	char arr[50];
	map<string, string> aliases;
	char *ptr;
	int procstatus=1;
	//history
	FILE *fp ;
	int histlinecount=0;
    //signal(SIGCHLD, sig_chld_hdlr);

	//alarm(2);
	//cout<<"Max history size :"<<histmaxsize<<endl;
	while(procstatus)
	{
		int inputlen=0;	background=0;
		printf("%s: ",PS1);
		//printf("\nPath %s\n",PATH);
		//printf("shanu@hp: ");//read from bashrc later
		printpwd();
		printf("$ ");
		fgets(arr, sizeof(arr), stdin);
		removefrontspace(arr);

		fp=fopen("/home/shanu/Desktop/2019201014_assignment_1/history.txt", "a+"); 
		if (fp == NULL) 
	    { 
	        printf("Couldn't open file"); 
	    } 
	    else
	    { 
	    	char line[50];
	    	histlinecount=0;
			while (fgets(line,50, fp) != NULL)
			{
				histlinecount++;
			}
			//cout<<"histlinecount "<<histlinecount;
   			fputs(arr, fp); 
        	fclose(fp); 
        } 
        if(histlinecount>=histmaxsize){//changing history deleting first elemnt
	        		//cout<<"here"<<endl;
	        		 string line;
				    ifstream in("/home/shanu/Desktop/2019201014_assignment_1/history.txt");
				    if(!in.is_open())
				    {
				          cout << "Input file failed to open\n";
				          return 1;
				    }
				    ofstream out("/home/shanu/Desktop/2019201014_assignment_1/outfile.txt");
				    	getline(in,line);
				    	
				    while( getline(in,line) )
				    {
				         out <<line << "\n";
				    }
				    in.close();
				    out.close();    
				    remove("/home/shanu/Desktop/2019201014_assignment_1/history.txt");
				    rename("/home/shanu/Desktop/2019201014_assignment_1/outfile.txt","/home/shanu/Desktop/2019201014_assignment_1/history.txt");
	        	}

		ptr = strchr(arr, '\n');
		if (int(arr[0])==11){//check for space
			continue;
		}
		if (ptr != NULL)
		{	
			*ptr = '\0';
		}

		//

		//alias handling
		string arr2=arr;
		string arr3=arr;
		string arr4=arr;

		string firstWord = arr2.substr(0, arr2.find(" "));
		firstWord=firstWord+"=";
		int firstWordlen=firstWord.length();
		//find in dynamic map
		bool found=false;
		string key,value;
		for(auto it=aliases.begin(); it!=aliases.end(); ++it)
		   {
		   	//cout<<"key is "<<(*it).first<<"first word is"<<firstWord<<endl;
		   		key=(*it).first;value=(*it).second;
		   		key.append("=");
		       if(firstWord==key){
		       	found=true;
		       //	cout<<"key is "<< (*it).first<<"value is"<< (*it).second<<endl;
		       	break;
		       }
		   }
		   if(found==true){
		   		arr4.replace(0,firstWordlen,value);
		   		//cout<<"arr 4 is"<<arr4<<endl;
	    		int finalLen = arr4.length(); 
			    int i11;
			    for(i11 =0;i11<finalLen;i11++){
			    	arr[i11]=arr4[i11];
			    }
			    arr[i11]='\0';

		   }
			


		//cout << " The word is: " << firstWord <<" len is :"<<firstWordlen<< endl;
		if(found ==false){
			ifstream alias_store;
		    string line;
		    alias_store.open("alias.txt");
			while (getline(alias_store, line))
			{
				if (line.rfind(firstWord,0) != std::string::npos)
				{
		    //		cout << "Alias Found: " << line << endl;
		    		line.erase(0,firstWordlen);
		    //		cout<<"Alias value: "<<line<<endl;
		    		arr3.replace(0,firstWordlen,line);
		    //		cout<<"arr 3 is :"<<arr3<<endl;
		    		
		    		int finalLen = arr3.length(); 
				    int i11;
				    for(i11 =0;i11<finalLen;i11++){
				    	arr[i11]=arr3[i11];
				    }
				    arr[i11]='\0';
				}
				    
			}
		}
		//store alias
		 arr2=arr;
		if(arr2.rfind("alias ",0) != std::string::npos){
			arr2.erase(0,6);
			//cout<<"make alias :"<<arr2<<endl;
			int equalpos=0;
			while(char(arr2[equalpos])!='='){
				equalpos++;
			}
			//cout<<"equalpos: "<<equalpos<<endl;
			string key = arr2.substr(0, equalpos);
			int endpos=arr2.length();
			string value=arr2.substr(equalpos+2, endpos- equalpos-3);
			value.append(" ");
			//cout<<"key is"<<key<<"value is"<<value<<"!"<<endl;
			aliases.insert(std::pair<string,string>(key,value));

			continue;
		}

		if(strstr(arr,"~")!=NULL){//associating  with HOME
		char  copyarr[100],copy2arr[100];
		int i =0,j=0;
			while(arr[i]!='~'){
				//cout<<arr[i]<<endl;
				copyarr[i]=arr[i];
				i++;		
			}copyarr[i]='\0';
			i++;
			while(arr[i]){
				//cout<<arr[i]<<endl;
				copy2arr[j++]=arr[i];
				i++;		
			}			
			copy2arr[j]='\0';
			//cout<<"copyarr"<<copyarr<<":"<<endl;
			strcat(copyarr,getenv("HOME"));
			//cout<<"copyarr"<<copyarr<<":"<<endl;
			//cout<<"copy2arr"<<copy2arr<<":"<<endl;
			strcat(copyarr,copy2arr);
			strcpy(arr,copyarr);
			//cout<<"ARR"<<arr<<endl;

		}
		//handling $$
		removeDoubleDollar:
		if(strstr(arr,"$$")!=NULL){
			char  newarr3[1000],newarr4[1000],newarr5[1000];
			int i =0,j=0,k=0,len=0;
		
			while(!(arr[i]=='$'&& arr[i+1]=='$')){
				newarr3[i]=arr[i];
				i++;;
			}
			newarr3[i]='\0';
			len=i;
			i++;i++;
			while(arr[i]){
				newarr4[k]=arr[i];
				k++;i++;
				//cout<<arr[j]<<endl;
			}	newarr4[k]='\0';
			int pid = getpid();
			char * mypid = (char*)malloc(6);   // ex. 34567
			sprintf(mypid, "%d", pid);
			 strcat(newarr3,mypid);
			 strcat(newarr3,newarr4);
			 //cout<<"new arr"<<newarr3<<endl;
			 strcpy(arr,newarr3);
			
		}
		if(strstr(arr,"$$")!=NULL)
			goto removeDoubleDollar;
	{
		//changing $ variables
	removedollar:
		if(strstr(arr,"$")!=NULL){
			 char  newarr3[1000],newarr4[1000],newarr5[1000];
			int i =0,j=0,k=0,len=0;
		
			while(arr[i]!='$'){
				newarr3[i]=arr[i];
				i++;;
			}
			newarr3[i]='\0';
			len=i;
			i++;
			while(arr[i] && arr[i]!=' '){
				newarr5[j]=arr[i];
				j++;i++;
				//cout<<arr[j]<<endl;
			}
			newarr5[j]='\0';
			while(arr[i]){
				newarr4[k]=arr[i];
				k++;i++;
				//cout<<arr[j]<<endl;
			}	newarr4[k]='\0';
			if(strcmp(newarr5,"PS1")==0){
				strcat(newarr3,PS1);
				 strcat(newarr3,newarr4);
			 
				 strcpy(arr,newarr3);	

			}else 
			if(strcmp(newarr5,"HOSTNAME")==0){
				strcat(newarr3,(_hostname+9));
				 strcat(newarr3,newarr4);
			 
				 strcpy(arr,newarr3);	

			}
			else{
			 strcat(newarr3,getenv(newarr5));
			 strcat(newarr3,newarr4);
			 
			 strcpy(arr,newarr3);
			}
		
		 }
		 if(strstr(arr,"$")!=NULL)
		 	goto removedollar;
		
	}
	{//change variables
		string asign;
		asign=arr;
		if (asign.rfind("HOME=",0) != std::string::npos){
			string val=asign.substr(5);
			char *path2 = strcpy(new char[val.length() + 1], val.c_str());
			setenv("HOME",path2,1);
			strcpy(_home,path2);
			continue;
		}
		if (asign.rfind("PATH=",0) != std::string::npos){
			string val=asign.substr(5);
			char *path2 = strcpy(new char[val.length() + 1], val.c_str());
			setenv("PATH",path2,1);
			strcpy(_path,path2);
			continue;
		}
		if (asign.rfind("USER=",0) != std::string::npos){
			string val=asign.substr(5);
			char *path2 = strcpy(new char[val.length() + 1], val.c_str());
			setenv("USER",path2,1);
			strcpy(_user,path2);
			continue;
		}
		if (asign.rfind("HOSTNAME=",0) != std::string::npos){
			string val=asign.substr(9);
			char *path2 = strcpy(new char[val.length() + 1], val.c_str());
			setenv("HOSTNAME",path2,1);
			strcpy(_hostname,path2);
			continue;
		}
		if (asign.rfind("PS1=",0) != std::string::npos){
			string val=asign.substr(4);
			char *path2 = strcpy(new char[val.length() + 1], val.c_str());
			PS1=path2;
			strcpy(_ps1,path2);
			continue;
		}


	}
{//open applications
	if(strncmp(arr,"open",4)==0) 
     	{
     		cout<<"open file"<<endl;
     		string str=arr;
     		int j=4,k=0,size=0;
     		while(arr[size]){
     			size++;
     		}
     		while(arr[j]==' '){
     			j++;
     		}
     		k=j;
     		while(arr[k]!='.'){
     			k++;
     		}
     		 //cout<<"j is :"<<j<<endl;
     		 //cout<<"k is :"<<k<<endl;
     		 string name1 = str.substr(j, k-j);
     		 char *name=strcpy(new char[name1.length() + 1], name1.c_str());
     		 string ext1 = str.substr(k, size-k);
     		 char *ext=strcpy(new char[ext1.length() + 1], ext1.c_str());
     		 //cout<<"Name"<<name<<endl;
     		 //cout<<"Ext!"<<ext<<endl;
     		 if(strcmp(ext,".txt")==0){
     		 	//cout<<"text";
     		 	char *arr3;
     		 	arr3=(_text+4);//arr3=gedit
     		 	//cout<<"Arr3:"<<arr3<<endl;
     		 	strcat(arr3," ");
     		 	//cout<<"Arr3:"<<arr3<<"!"	<<endl;
     		 	strcat(arr3,name);//arr3=gedit filename
     		 	//cout<<"Arr3:"<<arr3<<"!"	<<endl;
     		 	strcat(arr3,ext);//arr3=gedit filename.txt
     		 	//cout<<"Arr3:"<<arr3<<"!"<<endl;
     		 	strcpy(arr,arr3);

     		 }
     		 if(strcmp(ext,".mp4")==0){
     		 	char *arr3;
     		 	arr3=(_mp4+4);//arr3=gedit
     		 	strcat(arr3," ");
     		 	strcat(arr3,name);//arr3=gedit filename
     		 	strcat(arr3,ext);//arr3=gedit filename.txt
     		 	strcpy(arr,arr3);

     		 }
     		 if(strcmp(ext,".mp3")==0){
     		 	char *arr3;
     		 	arr3=(_mp3+4);//arr3=gedit
     		 	strcat(arr3," ");
     		 	strcat(arr3,name);//arr3=gedit filename
     		 	strcat(arr3,ext);//arr3=gedit filename.txt
     		 	strcpy(arr,arr3);
     		 }
     	}

}	
	
		//handle background with &
		inputlen= strlen(arr);
		//cout<<"input length is:"<<inputlen<<endl;
		if(arr[inputlen-1]=='&'){
			//cout<<"BG handling";
			arr[inputlen-1]='\0';
			background=1;
		}
		

		if(strcmp(arr,"exit")==0) 
     	{
     		//Exiting
     		printf("Exiting\n");
     		break;
     	}
		if(strcmp(arr,"history")==0) //+i to show history
     	{
     		fp=fopen("/home/shanu/Desktop/2019201014_assignment_1/history.txt", "r"); 
			if (fp == NULL) 
		    { 
		        printf("Couldn't open file"); 
		    } 
		    else
		    {
		    	int lineno=1;
		    	char line[50];
		    	while (fgets(line,50, fp) != NULL)
	        	{		
	        		cout<<lineno<<" "<<line;
	        		lineno++;
	        	}
		        fclose(fp); 
		    } 
     		continue;
     	}     	
     	if(strstr(arr,">")!=NULL && strstr(arr,">>")==NULL && strstr(arr,"|")==NULL)
    	{
    		char ** args;
     		args=splitline(arr);
     		launchIORedProc(args, arr);
		    free(args);
		    continue;
     	}
     	if(strstr(arr,">>")!=NULL && strstr(arr,"|")==NULL)
    	{
    		char ** args;
     		args=splitline(arr);
     		launchIORedProcApp(args, arr);
		    free(args);
		    continue;
     	}
     	if(strstr(arr,"<")!=NULL && strstr(arr,"<<")==NULL)
    	{
    		char ** args;
     		args=splitline(arr);
     		launchOIRedProc(args, arr);
		    free(args);
		    continue;
     	}
     	if(strstr(arr,"<<")!=NULL)
    	{
    		char ** args;
     		args=splitline(arr);
     		launchOIRedProcApp(args, arr);
		    free(args);
		    continue;
     	}

     	if(strstr(arr,"|")!=NULL)//handle pipeline
	     	{
	     		char arr1[50],arr2[50];
	     		strcpy(arr1,arr);
	     		strcpy(arr2,arr);
	     		
	     		int pipecount=0;
	     		int len = strlen(arr);
	     		int i2;
	     		int v;
	     		for(v=0;v<len;v++)
	     		{
	     			if(arr[v]=='|')
	     				pipecount++;
	     		}
	     		//cout<<"Pipeline :"<<pipecount<<endl;
	     		
	     		if(strstr(arr,">")!=NULL && strstr(arr,">>")==NULL){
	     			//handle outredirection at end
	     			//cout<<"pipe with redirection"<<endl;
	     			char ** args=splitByPipe(arr);
	     			//int i1=0;
	     			// while(args[i1]!=NULL){
	     			// 	cout<<"Tokens:"<<args[i1]<<":"<<endl;i1++;
	     			// }
	     			multiPipeWithOpRed(args,pipecount);	     			
	     		}
	     		else if(strstr(arr,">>")!=NULL){
	     			char ** args=splitByPipe(arr);
	     			multiPipeWithOpRedAppend(args,pipecount);	     			
	     		}	     		
	     		else{
	     			//only pipe
	     			char ** args=splitByPipe(arr);
	     			//int i1=0;
	     			// while(args[i1]!=NULL){
	     			// 	cout<<"Tokens:"<<args[i1]<<":"<<endl;i1++;
	     			// }
	     			//pipeHandler(args);
	     			multiPipe(args,pipecount);

	     		}

	     	}

     	else{

     		//script
			string recrdstat;
			recrdstat=arr;
			if (recrdstat.rfind("record start",0) != std::string::npos)
			{
	    		cout<<"Record Start"<<endl;
	    		recordstatus=1;
	    		continue;
	    	}
			if (recrdstat.rfind("record stop",0) != std::string::npos)
			{
	    		cout<<"Record Stop"<<endl;
	    		recordstatus=0;
	    		continue;
	    	}    	
	  	if(recordstatus==1){
	  		//cout<<"execute1"<<endl;
	    		char ** args3;
	    		char arrcop[50];
	    		strcpy(arrcop,arr);
	     		args3=splitline(arrcop);
				if ( strcmp(args3[0], "cd")!=0 ){
				  pid_t pid, wpid;
				  int status2;
				  switch (pid = fork()) {
			        case 0:
			                int fd2;
			                if ((fd2 = open("/home/shanu/Desktop/2019201014_assignment_1/Records.txt", O_WRONLY| O_APPEND | O_CREAT, 0644)) < 0) {
			                    perror("output file error");
			                    exit(0);
			                }
			                dup2(fd2, STDOUT_FILENO);
			                close(fd2);
			            //}
			            //args[2]=NULL;args[3]=NULL;
			            execvp(args3[0], args3);        /* child */
			            //signal(SIGINT, SIG_DFL);
			            exit(1);
			            break;
			         default:
            				wait(NULL);
			        }

				}
			 free(args3);
	     	}


	     	//cout<<"execute2"<<endl;
     		char ** args;
     		args=splitline(arr);
     		procstatus = execute(args);
		    free(args);
     		
	    }
	}



}
