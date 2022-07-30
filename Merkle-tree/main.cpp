#include <iostream>
#include "tree.h"
#include "sm3.h"
using namespace std;

const int N = 10;

string rand_str(const int len)
{
     
  string str;
  char c;
  for (int i = 0; i < len; i++)
  {
    c = 'a' + rand() % 26;
    str.push_back(c);
  }
  return str;
}

int main()
{
  srand(time(0)); 
  string check_str = "";
  check_str = rand();
  std::vector<string> v;
  for (int i = 0; i < N; i++)
    v.push_back(rand_str(20));

  vector<node *> proof;
  tree ntree;
  ntree.buildTree(v);
  // ntree.buildBaseLeafes(v);

  proof = ntree.generate_proof(getleafHash(v[1]));
  cout<<"tets: "<<getleafHash(v[1])<<endl;
  if (ntree.verify(proof, getleafHash(v[1])))
    printf("verfify success!\n");
  else
    printf("verfify fail\n");
  exclusion_proof ex_proof;
  string test="202000460103";
  cout<<"generate_Exclusionproof: "<<getleafHash(v[1])<<endl;
  ex_proof=ntree.generate_Exclusionproof(getleafHash(v[1]));
  if(ntree.verify_Exclusionproof(ex_proof))
    printf("verfify success!\n");
  else
    printf("verfify fail\n");
}