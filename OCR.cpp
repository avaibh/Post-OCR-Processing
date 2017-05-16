#include <iostream>
#include <fstream>
#include <string>
#include <map>
using namespace std;

//Recursive function
void ocrword_to_correctword(/* arguments */) {

  std::string incorrect_word; //Incorrect word in SLP1 format
  std::string left_str, right_str;

  for (int i = 0; i <= strlen(incorrect_word); i++) {
    left_str = incorrect_word.substr(0,i);
    right_str = incorrect_word.substr(i+1, strlen(incorrect_word));

  }
}

int main () {

  std::string line;
  std::map<string, int> dict_map, confusion_map, sandhi_map;
  std::string word;
  int freq_word;

  //dictionary file
  ifstream dict("dictionary.txt");
  if (dict.is_open()) {   //if file is open
    while (getline(dict, line))
    {
      dict >> word;
      dict >> freq_word;
      dict_map[word] = freq_word;
    }
    dict.close();
  }
  else{
    std::cout << "file is not open" << '\n';
  }

  //confusion file
  ifstream confusion("confusion.txt");

  if (dict.is_open()) {   //if file is open
    while (getline(confusion, line))
    {
      confusion >> word;
      confusion >> freq_word;
      confusion_map[word] = freq_word ;
    }
    dict.close();
  }
  else{
    std::cout << "file is not open" << '\n';
  }

  //sandhi file
  ifstream sandhi("sandhi.txt");

  if (dict.is_open()) {   //if file is open
    while (getline(sandhi, line))
      {
        sandhi >> word;
        sandhi >> freq_word;
        sandhi_map[word] = freq_word ;
      }
      dict.close();
  }
  else{ //if file is not open
      std::cout << "file is not open" << '\n';
  }

  return 0;
}
