#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
#include <unordered_map>
using namespace std;

string toslp1(string &str){
  return str;
}

bool isNonVowel(string ocrp){
    string a = "aeiouAEIOU";
    for(size_t t =0; t< a.size(); t ++){
    if (ocrp == a.substr(t,1)) return 0;
    }
    return 1;

}

void loadvectomap(vector<string> ConfP, map<string,int>& ConfPmap){
    for(size_t t = 0; t< ConfP.size(); t++) {
      ConfPmap[ConfP[t]]++;
    }
}

std::string removeSpaces(string input)
{ string output;
  int length = input.length();
  for (int i = 0; i < length; i++) {
     if(input[i] != ' ') output+= input[i];
  }
  return output;
}

typedef std::vector<int> lengths;

/*
  The "members" type is used as a sparse set for LCS calculations.
  Given a sequence, xs, and members, m, then
  if m[i] is true, xs[i] is in the LCS.
*/
typedef std::vector<bool> members;

/*
  Fill the LCS sequence from the members of a sequence, xs
  x - an iterator into the sequence xs
  xs_in_lcs - members of xs
  lcs - an output results iterator
*/
template <typename it, typename ot>
void set_lcs(it x, members const & xs_in_lcs, ot lcs)
{
    for (members::const_iterator xs_in = xs_in_lcs.begin();
         xs_in != xs_in_lcs.end(); ++xs_in, ++x)
    {
        if (*xs_in)
        {
            *lcs++ = *x;
        }
    }
}

/*
  Calculate LCS row lengths given iterator ranges into two sequences.
  On completion, `lens` holds LCS lengths in the final row.
*/
template <typename it>
void lcs_lens(it xlo, it xhi, it ylo, it yhi, lengths & lens)
{
    // Two rows of workspace.
    // Careful! We need the 1 for the leftmost column.
    lengths curr(1 + distance(ylo, yhi), 0);
    lengths prev(curr);

    for (it x = xlo; x != xhi; ++x)
    {
        swap(prev, curr);
        int i = 0;
        for (it y = ylo; y != yhi; ++y, ++i)
        {
            curr[i + 1] = *x == *y
                ? prev[i] + 1
                : std::max(curr[i], prev[i + 1]);
        }
    }
    swap(lens, curr);
}

/*
  Recursive LCS calculation.
  See Hirschberg for the theory!
  This is a divide and conquer algorithm.
  In the recursive case, we split the xrange in two.
  Then, by calculating lengths of LCSes from the start and end
  corners of the [xlo, xhi] x [ylo, yhi] grid, we determine where
  the yrange should be split.

  xo is the origin (element 0) of the xs sequence
  xlo, xhi is the range of xs being processed
  ylo, yhi is the range of ys being processed
  Parameter xs_in_lcs holds the members of xs in the LCS.
*/
template <typename it>
void calculate_lcs(it xo, it xlo, it xhi, it ylo, it yhi, members & xs_in_lcs)
{
    unsigned const nx = distance(xlo, xhi);

    if (nx == 0)
    {
        // empty range. all done
    }
    else if (nx == 1)
    {
        // single item in x range.
        // If it's in the yrange, mark its position in the LCS
        xs_in_lcs[distance(xo, xlo)] = find(ylo, yhi, *xlo) != yhi;
    }
    else
    {
        // split the xrange
        it xmid = xlo + nx / 2;

        // Find LCS lengths at xmid, working from both ends of the range
        lengths ll_b, ll_e;
        std::reverse_iterator<it> hix(xhi), midx(xmid), hiy(yhi), loy(ylo);

        lcs_lens(xlo, xmid, ylo, yhi, ll_b);
        lcs_lens(hix, midx, hiy, loy, ll_e);

        // Find the optimal place to split the y range
        lengths::const_reverse_iterator e = ll_e.rbegin();
        int lmax = -1;
        it y = ylo, ymid = ylo;

        for (lengths::const_iterator b = ll_b.begin();
             b != ll_b.end(); ++b, ++e)
        {
            if (*b + *e > lmax)
            {
                lmax = *b + *e;
                ymid = y;
            }
            // Care needed here!
            // ll_b and ll_e contain one more value than the range [ylo, yhi)
            // As b and e range over dereferenceable values of ll_b and ll_e,
            // y ranges over iterator positions [ylo, yhi] _including_ yhi.
            // That's fine, y is used to split [ylo, yhi), we do not
            // dereference it. However, y cannot go beyond ++yhi.
            if (y != yhi)
            {
                ++y;
            }
        }
        // Split the range and recurse
        calculate_lcs(xo, xlo, xmid, ylo, ymid, xs_in_lcs);
        calculate_lcs(xo, xmid, xhi, ymid, yhi, xs_in_lcs);
    }
}

// Calculate an LCS of (xs, ys), returning the result in an_lcs.
template <typename seq>
void lcs(seq const & xs, seq const & ys, seq & an_lcs)
{
    members xs_in_lcs(xs.size(), false);
    calculate_lcs(xs.begin(), xs.begin(), xs.end(),
                  ys.begin(), ys.end(), xs_in_lcs);
    set_lcs(xs.begin(), xs_in_lcs, back_inserter(an_lcs));
}

void findConfusions(string ocr, string correct, vector<string>& vec){
    //vector<string> vec;
    //ocrp = “”; correctp = “”;
    // for( t = 0 t< s.size) // load ocrp correctp if t > 0 with substr(t-1,1)
    //ocr == “”; correct = “”
    // while(s1 != s2) ocr += s1; correct += s2;  t++;// s1 s2 are chars from each word
    // of ocrp == “”load ocrnext in ocrp wiht substr(t,1)
    // subss add if ocr != “” :- // if ocrp is a non-vowel vec.push_back(ocrp+ocr, correctp+correct) else vec.push_back(ocr, correct)
    //ocrp = s1; correctp = s2;

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
            if(ocrp != "") vec.push_back(removeSpaces(ocrp+ocrn) + " " + removeSpaces(ocrp+correctn)); else if(t < sz) vec.push_back(removeSpaces(ocrn + s1) + " " + removeSpaces(correctn + s1)); else vec.push_back(removeSpaces(ocrn) + " " + removeSpaces(correctn));
        }
        // addition
        else if(s1 == " ") {
            while(s1 != s2) {ocrn += s1; correctn += s2; t++;  if(t >= sz) break; s1 = ocr.substr(t,1); s2 = correct.substr(t,1);}
            if((ocrp != "")&&(isNonVowel(ocrp))) vec.push_back(removeSpaces(ocrp+ocrn) + " " + removeSpaces(ocrp+correctn)); else if(t < sz) vec.push_back(removeSpaces(ocrn + s1) + " " + removeSpaces(correctn + s1)); else vec.push_back(removeSpaces(ocrn) + " " + removeSpaces(correctn));
        }
        else if(s1 != s2) {
            while(s1 != s2) {ocrn += s1; correctn += s2; t++;  if(t >= sz) break; s1 = ocr.substr(t,1); s2 = correct.substr(t,1);}
            if((ocrp != "")&&(isNonVowel(ocrp))) vec.push_back(removeSpaces(ocrp+ocrn) + " " + removeSpaces(ocrp+correctn));/*else if(t < sz) cout << "s " << ocrn + s1 << " " << correctn + s1<< endl;*/ else vec.push_back(removeSpaces(ocrn) + " " + removeSpaces(correctn));
        } else t++;

        ocrp = s1;
        if(t >= sz) break;
    }
//return vec;
}

void removeEndCommonSpaces(string& str1, string& str2){
    size_t t = str1.size();
    string s1 = str1.substr(t-1,1), s2 = str1.substr(t-1,1);
    while((s1 == " ") && (s2 == " ")) {str1 = str1.substr(0,t-1); str2 = str2.substr(0,t-1); t = str1.size(); s1 = str1.substr(t-1,1), s2 = str1.substr(t-1,1);}

}

void allignlcsnew(string& str1,string& str2,string& str3){
string str1new, str2new;
size_t t1 = 0;
size_t t2 =0;
size_t t3 =0;
string s1,s2,s3;
// while1
// a) addition if s1 == s3, and s2 differs add space to s1n, append t2 only so that we can reach close to s1 == s3 == s2
// b) deletion if s2 == s3, and s1 differs add space to s2n, append t1 only so that we can reach close to s2 == s3 == s1
// substitution if s1 != s2 != s3 s1n += s1, s2n += s2 append t1 and t2
// nothing if s1 == s2 == s3 s1n += s1, s2n += s2 append t1 and t2 and t3; if t3 == str3.size() Up3Flag = 1 break;

// if Up3Flag find which is smaller, add spaces in end of smaller smallstr = min(str1.size() - t1,str2.size() - t2);

while(1){
    s1 = str1.substr(t1,1); s2 = str2.substr(t2,1); s3 = str3.substr(t3,1);
    /*
    std::cout << str1 << '\n';
    std::cout << str2 << '\n';
    std::cout << str3 << '\n';
    std::cout << "/---------/" << '\n';
    */
    //a) add
    //cout << t1 << " " << t2 << " " << t3<< endl;
    //cout << s1 << " " << s2 << " " << s3<< endl;
    if((s1 == s3) && (s2 != s3)) {str1new += " "; str2new += s2; t2++; /*cout << "a growing str1new " << str1new <<endl;*/  }
    if((s2 == s3) && (s1 != s3)) {str2new += " ";  str1new += s1; t1++; /*cout << "d growing str2new " << str2new <<endl;*/ } // b)
    if((s2 != s3) && (s1 != s3)) {str1new += s1; str2new += s2; t1++; t2++; /*cout << "s growing str12new " << str1new<<" " <<str2new <<endl;*/ } //c)
    if((s2 == s3) && (s1 == s3))
    {
      str1new += s1; str2new += s2; t1++; t2++;t3++;
      /*
      cout << "n growing str12new " << str1new<<" " <<str2new <<endl;
      */
      if(t3 == str3.size()) {break;}
  } //d)
}
//cout << " " << str1new << " " << str2new << endl;
uint max = str1.size() ; if(max < str2.size() ) max = str2.size() ;

//cout << t1 << " " << t2 << " " << max<< endl;
while(1)
{   if((t1>=max) && (t2>=max)) break;
    if(t1<str1.size()) s1 = str1.substr(t1,1); else s1 = " "; if(t2<str2.size()) s2 = str2.substr(t2,1); else s2 = " ";
    //cout << t1 << " " << t2 << " " << max<< endl;
    //cout << s1 << " " << s2 << " " << endl;
    if(t1 < max) str1new += s1; else str1new += " ";
    if(t2 < max) str2new += s2; else str2new += " ";
    t1++; t2++;
}
str1 = str1new;
str2 = str2new;
//cout << " " << str1new << " " << str2new << endl;

}

void appendConfusionsPairs(string str1, string str2, vector<string>& vec){
    str1 = "@" + toslp1(str1) + "#"; str2 = "@" + toslp1(str2) + "#";
    string str3;
    lcs(str1,str2,str3);
    /*
    std::cout << str1 << '\n';
    std::cout << str2 << '\n';
    std::cout << str3 << '\n';
    std::cout << "/---------/" << '\n';
    */
    allignlcsnew(str1,str2,str3);
    removeEndCommonSpaces(str1,str2);
/*
    std::cout << str1 << '\n';
    std::cout << str2 << '\n';
    std::cout << str3 << '\n';
    std::cout << "/---------/" << '\n';
*/
    findConfusions(str1,str2,vec);
    //return vec;

}

void loadConfusions(std::string fileName,map<string,int>& ConfPmap){
    vector<string> ConfP;
    string str1, str2;
    ifstream myfile(fileName);

    if (myfile.is_open())
    {
       while(myfile >> str1)
       {
            //std::cout << str1 << '\n';
            //std::cout << str2 << '\n';
            myfile >> str2;
           //std::cout << str2 << '\n';
           //std::cout << "/* message */" << '\n';
            appendConfusionsPairs((str1),(str2),ConfP);//toslp1
            //cout << str1 << " " << str2 << endl;
           //vec.clear();
       }
       cout << ConfP.size() << " Confusions Loaded" << endl;
    }
    else cout <<"Error Confusions NOT Loaded" << endl;
    loadvectomap(ConfP,ConfPmap);
}

void printmapWFreq(map<string,int>& m1){
    for( map<string,int>::const_iterator eptr=m1.begin(); eptr!=m1.end(); eptr++)
          {
             cout << (eptr->first) << " " <<(eptr->second) << endl;
          }
}

int main() {
  map<string, int> ConfPmap1;
  loadConfusions("code.txt",ConfPmap1);
  printmapWFreq(ConfPmap1);
  return 0;

}
