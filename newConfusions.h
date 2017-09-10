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
