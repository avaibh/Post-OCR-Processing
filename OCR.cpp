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

/*
    MAKE A FUNCTION TO FIND PROBABILITY OF TOP 10 EDIT DISTANCES BY COMPARING WITH COMFUSION MAP OF TRAIN PAIRS

*/

//function to load confusions
void loadNewConfusions(string& left_str, vector< pair <int, string> >& v_editDistance ,map<string,int>& lw_ConfPmap){
    vector<string> lw_ConfP;
    string str1, str2;
    int edit_Distance;
    str1 = left_str;

    for( vector< pair <int, string> >::const_iterator eptr=v_editDistance.begin(); eptr!=v_editDistance.end(); eptr++)
       {
            edit_Distance = eptr->first;
            str2 = eptr->second ;
            appendConfusionsPairs((str1),(str2),lw_ConfP);//toslp1
            //cout << str1 << " " << str2 << endl;
           //vec.clear();
       }
       //std::cout << "new confusions loop enetered " << '\n';

    loadvectomap(lw_ConfP,lw_ConfPmap);
}

//function that takes dictionary and the left_word and return the top 10 edit distance as a vector
void load_editDistance(ifstream& dict, string& left_str, map<string,int>& lw_ConfPmap)
{
    string line, word;
    vector< pair <int, string> > v_editDistance;
    while (getline(dict, line)){
        dict >> word;
        v_editDistance.push_back( make_pair ( editDistance(word,left_str), word ) );
    }
    sort(v_editDistance.begin(), v_editDistance.end()); //sorting by edit distance
    while (v_editDistance.size() > 10) { // top 10 edit distances only
        v_editDistance.pop_back();
    }
    //std::cout << "I'm in vector function " << '\n';
    loadNewConfusions( left_str,v_editDistance, lw_ConfPmap );
}

//Recursive function
string ocrword_to_correctword(string &incorrect_word, ifstream &dict)
{
  string left_word, right_word, line, word;
  map<string,int> lw_ConfPmap;
  int flag = 0;

  while (getline(dict, line)){
    dict >> word;
    if (incorrect_word.compare(word) == 0) flag = 1; else flag =0;
  }

  if (flag == 1) { //if incorrect_word is in dictionary
    return incorrect_word;
  }else{
      flag = 0;
      for (size_t i = 0; i <= strlen(incorrect_word.c_str()); i++) {
          left_word = incorrect_word.substr(0,i);
          right_word = incorrect_word.substr(i, strlen(incorrect_word.c_str()));

          while (getline(dict, line)) {
            dict >> word;
            if ( left_word.compare(word) == 0 ) {
              //std::cout << "I'm in left_word == word if " << '\n';
              flag = 1;
            //  return a string to main
            }else flag =0;
          }
          if (flag == 1) {
            //ocrword_to_correctword(right_word, dict);
          }else {
          load_editDistance(dict, left_word, lw_ConfPmap);
        }
      }
   }
}


//main function
int main () {
  map<string, int> dict_map, confusion_map, sandhi_map;
  string word, line;
  int freq_word;
  float totalFreq_Dict = 0;
  //dictionary file
  std::ifstream dict("Data/Dict.txt");

  if (dict.is_open())
  {
    while (getline(dict, line))
    {
      dict >> word;
      dict_map[word]++;
    }
  }
  else{
    std::cout << "file is not open" << '\n';
  }

  for( map<string,int>::const_iterator eptr=dict_map.begin(); eptr!=dict_map.end(); eptr++)
        {
          totalFreq_Dict += (eptr->second) ;
        }
        std::cout << totalFreq_Dict << '\n';
        std::cout << 1/totalFreq_Dict << '\n';
/*
 *printing map of dictionary
 * for (std::map<string,int>::iterator it=dict_map.begin(); it!=dict_map.end(); ++it)
 * std::cout << it->first << " => " << it->second << '\n';
 */

 //for confusions.h file. Load confusions from train pairs
 map<string, int> ConfPmap1;
 //loadConfusions("DataForVaibhav/TrainPairs.txt",ConfPmap1);
 //printmapWFreq(ConfPmap1);

 string ocrWord, correctWord; //Incorrect word in SLP1 format
 correctWord = ocrword_to_correctword(ocrWord, dict);
 std::cout << correctWord << '\n';
/*
  //confusion file
  size_t totalFreq_confusion = 0;
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

  for( map<string,int>::const_iterator eptr=confusion_map.begin(); eptr!=confusion_map.end(); eptr++)
        {
          totalFreq += (eptr->second) ;
        }

  //sandhi file
  size_t totalFreq_sandhi = 0;
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
  for( map<string,int>::const_iterator eptr=sandhi_map.begin(); eptr!=sandhi_map.end(); eptr++)
        {
          totalFreq += (eptr->second) ;
        }
*/
  return 0;
}
