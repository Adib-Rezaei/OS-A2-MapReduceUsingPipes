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
{
  vector<string> words;
  vector<int> counts;
  vector<string> all_files_result;
  //get results in a loop from each map function
  for(int i = 0 ; i < atoi(argv[2]); i++)
  {
    char result_of_one_file[1024];
    string fifo_address = "./fifo_"+to_string(i+1);
    char * myfifo = (char*)fifo_address.c_str();
    int fd = open(myfifo,O_RDONLY);
    read(fd, result_of_one_file, 1024);
    all_files_result.push_back(result_of_one_file);
    close(fd);
  }

  vector<string> splitted_by_new_line;
  for(int j = 0 ; j < atoi(argv[2]) ; j++) //for each file
  {
    stringstream ss(all_files_result[j]); //split result of one file by new_line
    string to;
    while(getline(ss,to,'\n'))
    {
      splitted_by_new_line.push_back(to);
    }
  }

  for(int i = 0 ; i < splitted_by_new_line.size() ; i++)
  {
    string word;
    string num_str;
    string entry = splitted_by_new_line[i];
    bool comma_found = false;
    for(int j = 0 ; j < entry.size() ; j++)
    {
      if(entry[j]!= ',')
      {
        if(comma_found == false)
        word += entry[j];
        else num_str += entry[j];
      }
      else if(entry[j] == ',')
      {
        comma_found = true;
      }
    }
    words.push_back(word);
    counts.push_back(stoi(num_str));
  }

  map<string, int> count_map;
  string final_string;
  for(int i = 0 ; i < words.size() ; i++)
  {
    if (count_map.find(words[i]) != count_map.end())
    count_map[words[i]] += counts[i];
    else
    count_map[words[i]] = counts[i];
  }

  string results_of_one_file;
  map<string, int>::iterator it;
  for(it=count_map.begin(); it!=count_map.end(); ++it){
    final_string += it->first;
    final_string += ": ";
    final_string += to_string(it->second);
    final_string += '\n';
  }
  write(stoi(argv[1]), final_string.c_str(),4000);
  return 0;
}
