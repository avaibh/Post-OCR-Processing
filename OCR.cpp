#include <iostream>
#include <fstream>
#include <cassert>
#include <string>
#include <vector>
#include <algorithm> //for sorting
#include <map>
using namespace std;

/*Recursive function
void ocrword_to_correctword(std::string &left_str, std::string &right_str) {
}
*/
/*struct myclass {
  bool operator() (int i,int j) { return (i<j);}
} myobject;
*/
//Edit Distance
int editDistance(std::string word1, std::string word2)
{
    int i, j, l1, l2, m;
    l1 = word1.length();
    l2 = word2.length();
    vector< vector<int> > t(l1 + 1, vector<int>(l2 + 1));

    for (i = 0; i <= l1; i++)
        t[i][0] = i;
    for (i = 1; i <= l2; i++)
        t[0][i] = i;

    for (i = 1; i <= l1; i++)
    {
        for (j = 1; j <= l2; j++)
        {
            m = min(t[i-1][j], t[i][j-1]) + 1;
            t[i][j] = min(m, t[i-1][j-1] + (word1[i-1] == word2[j-1] ? 0 : 1));
        }
    }
    return t[l1][l2];
}

//function that takes dictionary and the left_word and return the top 10 edit distance as a vector
std::vector< pair < int,string > > ocrDict_sorting(std::ifstream &dict, std::string &left_str) {
  std::string line;
  std::vector< pair <int, string> > v_editDistance;
  while (getline(dict, line)){
      v_editDistance.push_back( make_pair ( editDistance(line,left_str), line ) );
  }
  //std::sort(v_editDistance.begin(), v_editDistance.end(), myobject);
  std::sort(v_editDistance.begin(), v_editDistance.end()); //sorting by edit distance

  while(v_editDistance.size() > 10) { // top 10 edit distances only
      v_editDistance.pop_back();
  }
  return v_editDistance;
}

int main () {

  std::string incorrect_word; //Incorrect word in SLP1 format
  std::string left_word, right_word;
/*
  for (int i = 0; i <= strlen(incorrect_word); i++) {
    left_word = incorrect_word.substr(0,i);
    right_word = incorrect_word.substr(i+1, strlen(incorrect_word));

    if (left_word) {
    }
    ocrword_to_correctword(left_word, right_word);
  }
*/
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

  //confusion file
  std::ifstream confusion("confusion.txt");

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
  std::ifstream sandhi("sandhi.txt");

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
