#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fstream>
#include <filesystem>
using namespace std;


int count_files(string path_);


int main(int argc, char* argv[]){

	fstream output_file;
	char final_string[4000];
	//count files
	int num_of_files = count_files("./testcases");
	for(int i=1 ; i <= num_of_files ; i++)
	{
		//create fifo for connecting each map with reduce seperately
		string fifo_addr = "./fifo_"+to_string(i);
		char * fifo = (char*)fifo_addr.c_str();
		mkfifo(fifo, 0666);

		//create unnamed pipes to the number of files for connection between OVERALL AND MAP
		int fd[2];
		if (pipe(fd)==-1){ 
			fprintf(stderr, "Pipe Failed" ); 
			exit(EXIT_FAILURE);
		}
		pipe(fd);
		pid_t pid = fork();

		if (pid == -1)
		{
			fprintf(stderr, "fork Failed"); 
			exit(EXIT_FAILURE);
		}
		else if (pid == 0) //means we're now the map proccess
		{
			close(fd[1]); // close write end of pipe flowing from overall to map
			char* map_executable_path = (char *)"./map.out";
			char* arg_list[] = {
				map_executable_path,
				(char *)to_string(fd[0]).c_str(),
				fifo,
				NULL
			};
			execv(map_executable_path, arg_list);
		}
		else   //we're parents
		{
			close(fd[0]); // close read end for when we're main
			string name = "./testcases/" + to_string(i)+".csv";
			write(fd[1], name.c_str(), 1024); //give name of related testcase file to each map
		}
	}

	//create unnamed pipe to recirve final result from reduce fuction
	int pipe_from_reduce[2];
	if (pipe(pipe_from_reduce))
	{
		fprintf(stderr, "Pipe Failed"); 
		exit(EXIT_FAILURE);
	}	
	//fork for reduce
	pid_t pid;
	pid = fork();
	if (pid == -1){
		fprintf(stderr, "fork Failed"); 
		exit(EXIT_FAILURE);
	}
	else if (pid == 0) //means now we're reduce
	{
		close(pipe_from_reduce[0]); // close read for reduce
		char* reduce_executable_path = (char *)"./reduce.out";
		char* argv_list[] = {
			(char *)"./reduce.out",
			(char *)to_string(pipe_from_reduce[1]).c_str(),
			(char *)to_string(num_of_files).c_str(),
			NULL
		};
		execv(argv_list[0], argv_list);
	}
	else{
		close(pipe_from_reduce[1]); // close write for main
		read(pipe_from_reduce[0], final_string,4000); //get final results
	}
	//write to file
	output_file.open("output.csv", ios::out | ios::app);
	output_file<<final_string;
	cout << "DEBUG: " << final_string << endl;


	return 0;
}


int count_files(string path_)
{
	int count{};
	filesystem::path p1{path_};
	for (auto& p : filesystem::directory_iterator(p1))
	++count;
	return count;
}