#include <iostream>
#include <fstream>
#include <cassert>
#include <string>
#include <string.h>
#include <vector>
#include <algorithm> //for using sorting algorithm
#include <map>
#include "editDistance.h"
#include "confusions.h"
using namespace std;

//function that takes dictionary and the left_word and return the top 10 edit distance as a vector
vector< pair < int,string > > ocrDict_sorting(ifstream &dict, string &left_str)
{ //function start
    std::string line, word;
    std::vector< pair <int, string> > v_editDistance;
    while (getline(dict, line)){
        dict >> word;
        v_editDistance.push_back( make_pair ( editDistance(word,left_str), word ) );
    }
    std::sort(v_editDistance.begin(), v_editDistance.end()); //sorting by edit distance
    while (v_editDistance.size() > 10) { // top 10 edit distances only
        v_editDistance.pop_back();
    }
    //std::cout << "I'm in vector function " << '\n';
    return v_editDistance;
} //function end

//Recursive function
void ocrword_to_correctword(string &incorrect_word, ifstream &dict)
{ //function start
  std::string left_word, right_word, line, word;
  std::vector< pair <int, string> > v_editDistance;

  for (int i = 0; i <= strlen(incorrect_word.c_str()); i++) {
    left_word = incorrect_word.substr(0,i);
    right_word = incorrect_word.substr(i+1, strlen(incorrect_word.c_str()));
      while (getline(dict, line)) {
        dict >> word;
        if ( left_word.compare(word) == 0 ) {
          //std::cout << "I'm in left_word == word if " << '\n';
          ocrword_to_correctword(right_word, dict);
          return;
        }
      }
       v_editDistance = ocrDict_sorting(dict, left_word);
  }
} //function end

//main function
int main () {

  std::string incorrect_ocrWord; //Incorrect word in SLP1 format
  std::string line;
  std::map<string, int> dict_map, confusion_map, sandhi_map;
  std::string word;
  int freq_word;

  //dictionary file
  std::ifstream dict("dictionary.txt");
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

/* printing map of dictionary
 * for (std::map<string,int>::iterator it=dict_map.begin(); it!=dict_map.end(); ++it)
 * std::cout << it->first << " => " << it->second << '\n';
 */

  ocrword_to_correctword(incorrect_ocrWord, dict);

  //confusion file
  std::ifstream confusion("confusion.txt");

  if (confusion.is_open()) {   //if file is open
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
  std::ifstream sandhi("sandhi.txt");

  if (sandhi.is_open()) {   //if file is open
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
