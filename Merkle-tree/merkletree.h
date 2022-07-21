#include "binarytree.h"
#include <string.h>
#include <iostream>
#include"../SM3/sm3.h"

#define SM3_SIZE 64
#define SM3_DIGEST_SIZE 32
using namespace std;



// combin_hash and hash by sm3
static void combin_hash(char* leftData,char* rightData,char out_buff[65]){

  unsigned char hash[SM3_DIGEST_SIZE];
  unsigned char prefix[1]={0x01};
  sm3_ctx sm3;
  SM3_init(&sm3);
  SM3_update(&sm3,prefix,1);
  SM3_update(&sm3,(unsigned char*)leftData,strlen((const char*)leftData) );
  SM3_update(&sm3,(unsigned char*)rightData,strlen((const char*)rightData) );
  SM3_final(&sm3,hash);

  for(int i = 0; i < SM3_DIGEST_SIZE; i++)
  {
    sprintf(out_buff + (i * 2), "%02x", hash[i]);
  }
  out_buff[64] = 0;
}



class merkletree{
public:
  vector<char*> tree;

  
  merkletree(){}
  merkletree(vector<char*> leaves){
    tree = computeTree(leaves);
  }

  size_t size(){return tree.size();}
  char* root(){return tree[0];}

  void calSM3(char* inp,char out_buff[65]){
    unsigned char hash[SM3_DIGEST_SIZE];
    sm3_ctx sm3;
    SM3_init(&sm3);
    SM3_update(&sm3, (unsigned char*)inp, strlen(inp));
    SM3_final(&sm3,hash);

    //char buffx[65];
    for(int i = 0; i < SM3_DIGEST_SIZE; i++)
    {
      sprintf(out_buff + (i * 2), "%02x", hash[i]);
    }
    out_buff[64] = 0;
  }



  vector<char*> computeTree(vector<char*> leaves){
   
    int nodeCount = leafCountToNodeCount(leaves.size());
    int delta = nodeCount - leaves.size();
    vector<char*> tree(nodeCount);

    // deep copy
    for(int i = 0 ;i<leaves.size();i++){
        tree[delta + i] = new char[65];
        memcpy(tree[delta + i],leaves[i],65);
        tree[delta + i][64] = 0;
    }

    int idx = nodeCount-1;
    printf("%s \n",tree[idx]);
    printf("%s ",tree[delta]);
    printf("%d \n",idx);
    while(idx > 0){
      int parent = (idx -1)/2;

      tree[parent] = new char[65];
      combin_hash(tree[idx-1],tree[idx],tree[parent]);
      tree[parent][64] = 0;
      // printf("tree[left]:%s \n",tree[idx-1]);
      // printf("tree[right]:%s \n",tree[idx]);
      // printf("tree[parent]:%s \n",tree[parent]);
      // printf("\n");
      idx-=2;
    }



    return tree;
  }



  

  vector<char*> getproof(char * leafData)
  {
    int idx=findLeaf(tree,leafData);
    vector<char*> proof;
    int proofIdx = 0;
    while(idx > 0 ){
      
      if(idx==getLeft(tree,getParent(tree,idx))) 
        {
          idx = getParent(tree,idx);
          proof.push_back(tree[getRight(tree,idx)]);
        }
      else 
      {
        idx = getParent(tree,idx);
        proof.push_back(tree[getLeft(tree,idx)]);
      }
    }

    //proof.resize(proofIdx);

      return proof;
    
  } 


  

	void pushleaf(char* leaf){
		pushleafworker(combin_hash,leaf);
	}


	void pushleafworker(void (*combineFn)(char*,char*,char*),char* leaf){

		
		tree.push_back(new char[65]);
		tree.push_back(new char[65]);

		int pidx = getParent(tree,tree.size()-1);

		// push parent and newleaf
		memcpy(tree[tree.size()-2],tree[pidx],65);
		memcpy(tree[tree.size()-1],leaf,65);

		
		int idx = tree.size()-1;
		while(idx > 0){
			idx = getParent(tree,idx);
			//cout<<&combineFn<<'\n';
      char *buff = new char[65];
      combineFn(tree[getLeft(tree,idx)],tree[getRight(tree,idx)],buff);
      tree[idx] = buff;
		}

		// done!
	}


};
