#include "tree.h"
#include <iostream>

tree::tree() {}

bool is_power_two(int n)
{
  return !(n & (n - 1));
}

int get_power_two(int size)
{
  size--;
  size |= size >> 1;
  size |= size >> 2;
  size |= size >> 4;
  size |= size >> 8;
  size |= size >> 16;
  size++;

  return size >> 1;
}

vector<string> sort_hash(vector<string> base_leafs)
{
  for (int i = 0; i < base_leafs.size(); i++)
    base_leafs[i] = getleafHash(base_leafs[i]);
  sort(base_leafs.begin(), base_leafs.end());
  return base_leafs;
}

void tree::buildTree(vector<string> base_leafs)
{
  vector<node *> new_nodes;
  vector<node *> temp;
  
  base_leafs = sort_hash(base_leafs);
  this->sorted_leaf = (base_leafs);
  for(auto leaf:sorted_leaf)
    cout<<leaf<<endl;
  copy(base_leafs.begin(), base_leafs.end(), this->sorted_leaf.begin());
  int leaf_size = base_leafs.size();
  if (leaf_size & 1)
    leaf_size--;
  else if (leaf_size & (leaf_size - 1))
    leaf_size = get_power_two(leaf_size);
  for (int i = 0; i < leaf_size; i++)
  {
    node *new_node = new node;
    new_node->setHash(base_leafs[i]);
    new_nodes.push_back(new_node);
  }
  this->base.push_back(new_nodes);
  for (int i = leaf_size; i < base_leafs.size(); i++)
  {
    node *new_node = new node;
    new_node->setHash(base_leafs[i]);

    temp.push_back(new_node);
  }

  do
  {
    vector<node *> new_nodes;
    // makeBinary(this->base.end()[-1]);
    int size = this->base.end()[-1].size();
    if (size & 1)
      size--;
    else if (size & (size - 1))
      size = get_power_two(size);
    for (int i = 0; i < size; i += 2)
    {
      node *new_parent = new node;
      this->base.end()[-1][i]->setParent(new_parent);
      this->base.end()[-1][i + 1]->setParent(new_parent);

      new_parent->setHash(combine_hash(this->base.end()[-1][i]->getHash(), this->base.end()[-1][i + 1]->getHash()));
      new_parent->setChildren(this->base.end()[-1][i], this->base.end()[-1][i + 1]);
      new_nodes.push_back(new_parent);
      // cout << "combine  " << this->base.end()[-1][i]->getHash() << " and " << this->base.end()[-1][i + 1]->getHash() << endl
      //      << " parents: " << new_parent->getHash() << endl;
    }

    if (is_power_two(new_nodes.size() + temp.size()))
    {
      for (auto node : temp)
        new_nodes.push_back(node);
      temp.clear();
    }
    // printTreeLevel(new_nodes);

    this->base.push_back(new_nodes);

  } while (this->base.end()[-1].size() > 1);

  this->merkleRoot = this->base.end()[-1][0]->getHash();

  std::cout << "Merkle Root is: " << this->merkleRoot << endl
            << endl;
}

string tree::getMerkleRoot()
{
  return merkleRoot;
}

vector<node *> tree::generate_proof(string hash)
{

  node *el_node = NULL;

  string act_hash = hash;
  vector<node *> proof;
  for (int i = 0; i < this->base[0].size(); i++)
  {
    if (this->base[0][i]->getHash() == act_hash)
      el_node = this->base[0][i];
  }
  while ((el_node->getParent()) != NULL)
  {
    proof.push_back(el_node->getSibling());
    // proof.push_back(el_node->getParent());
    el_node = el_node->getParent();
  }
  return proof;
}

bool tree::verify(vector<node *> proof, string hash)
{

  node *el_node = NULL;
  string act_hash = hash;

  for (int i = 0; i < proof.size(); i++)
  {
    if (proof[i]->checkDir())
      act_hash = combine_hash(act_hash, proof[i]->getHash());
    else
      act_hash = combine_hash(proof[i]->getHash(), act_hash);
  }
  return act_hash == merkleRoot ? 1 : 0;
}

exclusion_proof tree::generate_Exclusionproof(string hash)
{
  printf("here\n");
  exclusion_proof proof;
  vector<string> sorted_list=this->sorted_leaf;
  sorted_list.push_back(hash);
  sort(sorted_list.begin(),sorted_list.end());
  int index=find(sorted_list.begin(),sorted_list.end(),hash)-sorted_list.begin();
  printf("index: %d\n",index);
  int index1,index2;
  cout<<"sorted_list[9]"<<sorted_list[9]<<endl;
  if(index==sorted_list.size()) 
  {
    index1=index-1;
    index2=0;
  }
  else if(index==0)
  {
    index1=sorted_list.size()-1;
    
    index2=0;
    printf("index1: %d  index2:   %d",index1,index2);
  }
  else
  {
    index1=index;
    index2=index;
    // while(sorted_list[index1]==hash) index1--;
    
    // while(sorted_list[index2]==hash) index2++;
  }
  printf("index1: %d  index2:   %d",index1,index2);
  proof.hash1 = (sorted_list[index1]);
  proof.proof1 = (generate_proof(sorted_list[index1]));
  
   proof.hash2 = (sorted_list[index2]);
  
   proof.proof2 = (generate_proof(sorted_list[index2]));

   //while (hash <= sorted_leaf[i]) i--;
    
  
  // printf("i: %d\n",i);
  // proof.hash1 = (this->sorted_leaf[i]);
  // proof.proof1 = (generate_proof(this->sorted_leaf[i]));

  return proof;
}

bool tree::verify_Exclusionproof(exclusion_proof proof)
{
  int i;
  for (i = 0; i < this->sorted_leaf.size(); i++)
  {
    if (proof.hash1 == this->sorted_leaf[i])
      break;
  }
  if (this->sorted_leaf[i + 1] != proof.hash2)
    return false;
  if (verify(proof.proof1, proof.hash1) && verify(proof.proof2, proof.hash2))
    return true;
  return false;
}

tree::~tree() {}
