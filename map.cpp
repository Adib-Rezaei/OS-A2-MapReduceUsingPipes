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
#include <map>
#include <filesystem>
using namespace std;

int main(int argc, char** argv)
{   char filename[1024];
  //get file name by reading from pipe given
  read(atoi(argv[0]),filename,1024);
  fstream fs(filename);
  map<string, int> count_map;
  string line, word;
  fs >> line;
  stringstream ss;
  ss << line;

  while (getline(ss, word, ','))
  {
    if (count_map.find(word) != count_map.end())
    count_map[word] += 1;
    else
    count_map[word] = 1;
  }

  string results_of_one_file;
  map<string, int>::iterator it;
  for(it=count_map.begin(); it!=count_map.end(); ++it){
    results_of_one_file += it->first;
    results_of_one_file += ',';
    results_of_one_file += to_string(it->second);
    results_of_one_file += '\n';
  }
  int fd = open(argv[1], O_WRONLY); //named pipe to reduce
  write(fd, results_of_one_file.c_str(), 1024);
  close(fd);
  return 0;
}
