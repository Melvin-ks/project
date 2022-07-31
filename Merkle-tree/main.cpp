#include <iostream>
#include "tree.h"
#include "sm3.h"
using namespace std;

const int N = 100000;

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
  

  proof = ntree.generate_proof(getleafHash(v[0]));
  if (ntree.verify(proof, getleafHash(v[0])))
    printf("inclusion verify success!\n");
  else
    printf("inclusion verify fail\n");
  exclusion_proof ex_proof;
  string test="2000460103";
  cout<<"exclusion_leaf_hash: "<<getleafHash(test)<<endl;
  ex_proof=ntree.generate_Exclusionproof(getleafHash(test));
  if(ntree.verify_Exclusionproof(ex_proof))
    printf("exclusion verify success!\n");
  else
    printf("exclusion verify fail\n");
}