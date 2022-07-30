#include "node.h"
#include<algorithm>

using namespace std;


struct exclusion_proof
{
  vector<node*> proof1;
  vector<node*> proof2;
  string hash1;
  string hash2;
};


class tree{
private:
  string merkleRoot;
  vector<vector<node*>> base;
  

public:
  tree();
  vector<string> sorted_leaf;
  void buildTree(vector<string> base_leafs);
  string getMerkleRoot();
  vector<node*> generate_proof(string hash);
  void buildBaseLeafes(vector<string> base_leafs);
  bool verify(vector<node*> proof,string hash);
  exclusion_proof generate_Exclusionproof(string hash);
  bool verify_Exclusionproof(exclusion_proof proof);
  ~tree();
};

int get_power_two(int size);