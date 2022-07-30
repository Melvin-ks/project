#include "node.h"
#include"sm3.h"
using namespace std;

string combine_hash(string leftData,string rightData)
{

  unsigned char hash[33];
  unsigned char prefix[1]={0x01};
  char out_buff[32];
  string hash_value;
  sm3_ctx sm3;
  SM3_init(&sm3);
  SM3_update(&sm3,prefix,1);
  SM3_update(&sm3,(unsigned char*)leftData.c_str(),leftData.length()) ;
  SM3_update(&sm3,(unsigned char*)rightData.c_str(),rightData.length()) ;
  
  SM3_final(&sm3,hash);

  for(int i = 0; i < 32; i++)
  {
    sprintf(out_buff + (i * 2), "%02x", hash[i]);
  }
  out_buff[64] = 0;
  hash_value=out_buff;
  
	return hash_value;
}

node::node (){
  this->parent = NULL;
  this->children[0] = NULL;
  this->children[1] = NULL;
}

void node::setHash(string hash_str){
  this->hash_str=hash_str;
}

node* node::getParent(){
  return this->parent;
}

void node::setParent(node* parent){
  this->parent = parent;
}

void node::setChildren(node* children_l, node* children_r){
  this->children[0] = children_l;
  this->children[1] = children_r;
}

node* node::getSibling(){
  node* parent = this->getParent();
  return parent->getChildren(0) == this ? parent->getChildren(1) : parent->getChildren(0);

}

node* node::getChildren(int index){
  return index <= 1 ? children[index] : NULL;
}

string node::getHash(){
    return this->hash_str;
}

int node::checkDir(){
  return parent->getChildren(0) == this ? 0 : 1;
}



node::~node () {}


string getleafHash(string hash_str){
  
  unsigned char hash[33];
  unsigned char prefix[1]={0x00};
 
  char out_buff[32];
  sm3_ctx sm3;
  SM3_init(&sm3);
  SM3_update(&sm3,prefix,1);
  SM3_update(&sm3,(unsigned char*)hash_str.c_str(),hash_str.length() );
  SM3_final(&sm3,hash);

  for(int i = 0; i < 32; i++)
  {
    sprintf(out_buff + (i * 2), "%02x", hash[i]);
  }
  out_buff[64] = 0;
  
  hash_str=out_buff;
  return hash_str;
}