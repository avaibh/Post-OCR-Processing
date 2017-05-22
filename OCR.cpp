#include <iostream>
#include <sstream>
#include <fstream>
#include <cassert>
#include <string>
#include <string.h>
#include <vector>
#include <algorithm> //for using sorting algorithm
#include <map>
#include <unordered_map>
#include <iterator>
#include <utility>
#include "editDistance.h"
#include "confusions.h"
// #include "newConfusions.h"
using namespace std;

/*
Legends:
lw = left_word
tp = train pairs
Cmap = Confusion Map
*/

/*
  New findConfusions function
*/

vector<string> findNewConfusions(string ocr, string correct, vector<string>& vec){

    vector<string> v;
    size_t sz = ocr.size();
    string ocrp = "";
    size_t t = 0;
    while(1){
        string ocrn = "";
        string correctn = "";
        string s1 = ocr.substr(t,1), s2 = correct.substr(t,1);
        //cout << "t = " << t << " " << sz << endl;
        // deletion
        if(s2 == " ") {
            while(s1 != s2) {ocrn += s1; correctn += s2; t++;  if(t >= sz) break; s1 = ocr.substr(t,1); s2 = correct.substr(t,1);}
            if(ocrp != "") {
              vec.push_back(removeSpaces(ocrp+ocrn) + " " + removeSpaces(ocrp+correctn));
              v.push_back(removeSpaces(ocrp+ocrn) + " " + removeSpaces(ocrp+correctn));
          }
            else if(t < sz) {
              vec.push_back(removeSpaces(ocrn + s1) + " " + removeSpaces(correctn + s1));
              v.push_back(removeSpaces(ocrn + s1) + " " + removeSpaces(correctn + s1));
            }
            else {
              vec.push_back(removeSpaces(ocrn) + " " + removeSpaces(correctn));
              v.push_back(removeSpaces(ocrn) + " " + removeSpaces(correctn));
          }
        }
        // addition
        else if(s1 == " ") {
            while(s1 != s2) {ocrn += s1; correctn += s2; t++;  if(t >= sz) break; s1 = ocr.substr(t,1); s2 = correct.substr(t,1);}
            if((ocrp != "")&&(isNonVowel(ocrp))) {
              vec.push_back(removeSpaces(ocrp+ocrn) + " " + removeSpaces(ocrp+correctn));
              v.push_back(removeSpaces(ocrp+ocrn) + " " + removeSpaces(ocrp+correctn));
            }
            else if(t < sz) {
              vec.push_back(removeSpaces(ocrn + s1) + " " + removeSpaces(correctn + s1));
              v.push_back(removeSpaces(ocrn + s1) + " " + removeSpaces(correctn + s1));
          }
            else {
              vec.push_back(removeSpaces(ocrn) + " " + removeSpaces(correctn));
              v.push_back(removeSpaces(ocrn) + " " + removeSpaces(correctn));
            }
        }
        else if(s1 != s2) {
            while(s1 != s2) {ocrn += s1; correctn += s2; t++;  if(t >= sz) break; s1 = ocr.substr(t,1); s2 = correct.substr(t,1);}
            if((ocrp != "")&&(isNonVowel(ocrp))) {
              vec.push_back(removeSpaces(ocrp+ocrn) + " " + removeSpaces(ocrp+correctn));/*else if(t < sz) cout << "s " << ocrn + s1 << " " << correctn + s1<< endl;*/
              v.push_back(removeSpaces(ocrp+ocrn) + " " + removeSpaces(ocrp+correctn));
            }
              else {
                vec.push_back(removeSpaces(ocrn) + " " + removeSpaces(correctn));
                v.push_back(removeSpaces(ocrn) + " " + removeSpaces(correctn));
              }
        } else t++;

        ocrp = s1;
        if(t >= sz) break;
    }
return v;
}
void appendNewConfusionsPairs(string str1, string str2, vector<string>& vec, vector<string>& vec1){
    str1 = "@" + toslp1(str1) + "#"; str2 = "@" + toslp1(str2) + "#";
    string str3;
    lcs(str1,str2,str3);
    allignlcsnew(str1,str2,str3);
    removeEndCommonSpaces(str1,str2);
    vec1 = findNewConfusions(str1,str2,vec);
    //return vec;
}

//function to load confusions
void loadNewConfusions(string& left_str, vector< pair <int, string> >& v_editDistance ,map< string, map<string, float> >& lw_ConfPmap){
  vector<string> lw_ConfP, vec;
  string str1, str2;
  int edit_Distance;
  str1 = left_str;
  map<string, float> inner;

  for( vector< pair <int, string> >::const_iterator eptr=v_editDistance.begin(); eptr!=v_editDistance.end(); eptr++)
     {
      edit_Distance = eptr->first;
      str2 = eptr->second ;
      appendNewConfusionsPairs((str1),(str2),lw_ConfP, vec);//toslp1
      for(size_t t = 0; t< vec.size(); t++) {
        lw_ConfPmap[str2][vec[t]] = 1 ;
      }
    //  lw_ConfPmap.insert(make_pair(str2,inner));
   //   lw_ConfPmap[str2];
      //cout << str1 << " " << str2 << endl;
      //vec.clear();
     }
     //std::cout << "new confusions loop enetered " << '\n';
     //loadvectomap(lw_ConfP,lw_ConfPmap);
}/*
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
*/

/*
  Funcition that computes total probabilityof the incorrect_word in recursion
*/
void computeProb(float totalProb, map< string, map<string, float> >& m1, std::map<string, float> finalMap) {
  for (map< string, map<string, float> >::const_iterator i=m1.begin(); i!=m1.end(); i++) {
    for( map<string,float>::const_iterator j=i->second.begin();j!=i->second.end(); j++){
            totalProb *= m1[(i->first)][(j->first)];
    }
    finalMap[(i->first)] = totalProb ;
  }
}

//function that takes dictionary and the left_word and return the top 10 edit distance as a vector
void load_editDistance(ifstream& dict, string& left_str,vector< pair <int, string> > v_editDistance)
{
  string line, word;
  while (getline(dict, line)){
      dict >> word;
      v_editDistance.push_back( make_pair ( editDistance(word,left_str), word ) );
  }
  sort(v_editDistance.begin(), v_editDistance.end()); //sorting by edit distance
  while (v_editDistance.size() > 10) { // top 10 edit distances only
      v_editDistance.pop_back();
  }
  //std::cout << "I'm in vector function " << '\n';
}
/*
A Funtion that finds the corresponding probability for the confusions of the pairs taken from top 10 edit distances
with respect to the train pairs' confusions
*/

void loadProbMap(map< string, map<string, float> >& lw_map, map<string,float>& tp_Cmap1){
  for (map< string, map<string, float> >::const_iterator eptr=lw_map.begin(); eptr!=lw_map.end(); eptr++) {
    for( map<string,float>::const_iterator i=eptr->second.begin();i!=eptr->second.end(); i++){
      for( map<string,float>::const_iterator j=tp_Cmap1.begin(); j!=tp_Cmap1.end(); j++ ) {

        if ((j->first).compare((i->first)) == 0) {
            lw_map[(eptr->first)][(i->first)] = (j->second);
            //std::cout << (i->second) << '\n';
            //std::cout << "/*-------------------------------- message */" << '\n';
           //(i->second) = (j->second);
          //((i->[second).second) = (j->second);
          //for( map<string,float>::const_iterator eptr=i->second.begin();eptr!=i->second.end(); eptr++){
          //  (eptr->second) = (j->second);
        }
      }
    }
  }
}
//Recursive function
string ocrword_to_correctword(string &incorrect_word, ifstream &dict, map<string,float>& tp_Cmap)
{
  vector< pair <int, string> > v_editDistance;
  string left_word, right_word, line, word, correctWord;
  map< string, map<string, float> > lw_ConfPmap; // Main map can be accessed as
                                                 // mainMap[string1][string2] = "float Value";
  map<string, float> finalMap;
  float totalProb=1;
  int flag = 0;

  while (getline(dict, line)){
    dict >> word;
    if (incorrect_word.compare(word) == 0) flag = 1; else flag =0;
  }

  if (flag == 1) { //if incorrect_word is in dictionary
    return incorrect_word;
  }else{
  flag = 0;
  for (size_t i = 2; i <= strlen(incorrect_word.c_str()); i++) {
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
        ocrword_to_correctword(right_word, dict, tp_Cmap);
      }else {
      load_editDistance(dict, left_word, v_editDistance);
      loadNewConfusions(left_word, v_editDistance, lw_ConfPmap);
      loadProbMap(lw_ConfPmap, tp_Cmap);
      computeProb(totalProb, lw_ConfPmap, finalMap);
      }
  }
}
return correctWord;
}

float load_totalFreq(map<string, int>& m2){
float totalFreq;
  for( map<string,int>::const_iterator eptr=m2.begin(); eptr!=m2.end(); eptr++)
        {
          totalFreq += (eptr->second);
        }
  return totalFreq;
}

void mapProbability (map<string,int>& m3, map<string, float>& m4, float totalFreq) {

for( map<string,int>::const_iterator eptr=m3.begin(); eptr!=m3.end(); eptr++)
  {
   m4[(eptr->first)]= (float)(eptr->second)/totalFreq;
  }
//for( map<string,float>::const_iterator eptr=m4.begin(); eptr!=m4.end(); eptr++)
//      {
//         cout <<"(" << (eptr->first) << " " <<(eptr->second)<<")" << "   ";
//      }
}

//main function
int main ()
{
map<string, int> dict_map, confusion_map, sandhi_map, ConfPmap1;
map<string, float> mProb_dict, mProb_confusion;
string word, line;
int freq_word;
float totalFreqDict = 0, totalFreqConfusion= 0;
//dictionary file
std::ifstream dict("/Users/vaibhavagrawal/Desktop/OCR word correction/Data/Dict.txt");

if (dict.is_open())
{
  while (getline(dict, line))
  {
    dict >> word;
    dict_map[word]++;
  }
}
else{
  std::cout << "file is not open" << endl;
}
totalFreqDict = load_totalFreq(dict_map);
//std::cout << totalFreqDict << '\n';
//std::cout << 1/totalFreqDict << '\n';
mapProbability(ConfPmap1, mProb_dict, totalFreqDict);

//for confusions.h file. Load confusions from train pairs
loadConfusions("/Users/vaibhavagrawal/Desktop/OCR word correction/Data/TrainPairs.txt",ConfPmap1);
totalFreqConfusion = load_totalFreq(ConfPmap1);
//std::cout << 1/totalFreqConfusion << '\n';
//printmapWFreq(ConfPmap1);

mapProbability(ConfPmap1, mProb_confusion, totalFreqConfusion);

string ocrWord, correctWord; //Incorrect word in SLP1 format
correctWord = ocrword_to_correctword(ocrWord, dict, mProb_confusion);
std::cout << correctWord << endl;
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
