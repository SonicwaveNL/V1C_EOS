#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <sys/wait.h>
#include <fcntl.h>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <vector>
#include <syscall.h>

using namespace std;

void new_file() // ToDo: Implementeer volgens specificatie.
{
    //Set privileges
    mode_t mode = S_IRWXU | S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;

    //Ask for file name and Create file with given file name
    string getName;
    cout << "What's the name of the file?" << endl;
    getline(cin, getName);

    const char *fileName = getName.c_str();
    syscall(SYS_creat, fileName, mode);

    //Ask for file content and put it in the created file
    string getText;
    cout << "Content of your created file:" << endl;
    getline(cin, getText);

    const char *fileText = getText.c_str();

    int fd = syscall(SYS_open, fileName, O_RDWR, 0755);
    syscall(SYS_write, fd, fileText, getText.length());

    close(fd);

}

void list() // ToDo: Implementeer volgens specificatie.
{
    cout << "Forking and Executing 'ls -a' command" << endl;

    int pid = syscall(SYS_fork);

    if(pid == 0){
        char* args[] =  { (char*) "ls", (char*)"-la", 0};
	    execv("/bin/ls", args);

    } else if (pid > 0)
    {
        int wstatus;
        wait(&wstatus);
    } else {
        cout << "An error has occured, status: " << pid;
    }
    
}

void find() // ToDo: Implementeer volgens specificatie.
{
    string lookFor;
    cout << "What are you looking for?" << endl << endl;
    getline(cin, lookFor);

    pid_t pid;
    int pipefd[2];
	int status;
	int retu;

	retu = pipe(pipefd);

	if (retu == -1)
	{
		perror("pipe");
		exit(1);
	}

	pid = fork();

	if (pid == 0)
	{
        char* args[] = {(char*)"find", (char*)"./", 0};
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);

		execv("/usr/bin/find", args);

	} else if (pid > 0)
    {
        int wstatus;
        wait(&wstatus);

    } else {
        cout << "An error has occured, status: " << pid;
    }	

	pid = fork();

	if (pid == 0)
	{
        const char *findFile = lookFor.c_str();
        char* args[] = {(char*)"grep", (char*)findFile, 0};
		close(pipefd[1]);
		dup2(pipefd[0], STDIN_FILENO);

		execv("/bin/grep", args);

	} else if (pid > 0)
    {
        int wstatus;
        wait(&wstatus);

    } else {
        cout << "An error has occured, status: " << pid;
    }

	close(pipefd[0]);
	close(pipefd[1]);

}

void python() // ToDo: Implementeer volgens specificatie.
{
    cout << "ToDo: Implementeer hier python()" << endl;
}

void src()
{ 
    int fd = syscall(SYS_open, "shell.cpp", O_RDONLY, 0755);
    char byte[1];
    while(syscall(SYS_read, fd, byte, 1))
    cout << byte; 
}

int main()
{
    string input;

	vector<char> buff{ };
	buff.resize(512);

	int fd = syscall(SYS_open, "prompt.txt", O_RDONLY, 0755);
	ssize_t rfile = read(fd, buff.data(), 512);
	string prompt(buff.begin(), buff.end());

    while (true)
    {
        cout << prompt << endl;
        getline(cin, input);
        if (input == "new_file")
        {
            new_file();
        }
        else if (input == "ls")
        {
            list();
        }

        // else if (input == "src"){
        //     src();
        // }

        else if (input == "find")
        {
            find();
        }

        else if (input == "python")
        {
            python();
        }

        else if (input == "exit")
        {
            return 0;
        }

        else if (input == "quit")
        {
            return 0;
        }

        else if (input == "error")
        {
            return 1;
        }

        if (cin.eof())
        {
            return 0;
        }
    }
}
