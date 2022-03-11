#include <iostream>
#include <cstdlib>
#include "BTree.hpp"


struct tnode {
  BTItem data;
  BTNode left;                  //Points to the left child
  BTNode right;                 //Points to the right child
  BTNode parent;
};

BTNode BTCreate()
{
	return NULL;
}


BTNode BTGetRoot(BTTree tree)	//This function returns the root of a tree
{
	return tree;
}


BTNode BTGetParent(BTTree tree,BTNode node)
{
	if (tree==NULL) return NULL;
        if (node!=NULL) return node->parent;
        else exit(-1);
}

BTNode BTGetChildLeft(BTTree tree,BTNode node)	//This function returns the left child of a node
{
	if (node==NULL||tree==NULL) return NULL;
        return node->left;

}

BTNode BTGetChildRight(BTTree tree,BTNode node)	//This function returns the right child of a node
{
	if (node==NULL||tree==NULL) return NULL;
	return node->right;
}

int BTIsNil(BTNode node)	//This function checks if a tree is empty
{
	if (node==NULL) return 1;
	else return 0;
}

BTItem BTGetItem(BTTree tree,BTNode node)	//This function returns the value of a node
{
	if (tree==NULL) {				//If tree is NULL terminate the program;
		printf("The tree is NULL and the program just got terminated\n");
		exit(-1);
	}
	else return node->data;
}

BTNode BTInsertRoot(BTTree tree,BTItem item)	//This function Inserts a root in an empty tree
{
	BTNode root;
	if (tree!=NULL){				//If it is not empty return the same tree
		printf("Tree is not empy\n");
		return tree;
	}
	root = new struct tnode;
	root->left=NULL;
	root->parent=NULL;
								//Create the root
	root->right=NULL;
	root->data=item;
	return root;
}



void BTInsertLeft(BTTree tree, BTNode node, BTItem item)		//This function inserts a node as left child of another node
{
	BTNode newnode;

	if (node->left!=NULL){
		 return;		//If node has a left child just return this in order not to change anything
	}
	else {
		newnode=new struct tnode;
		newnode->left=NULL;
		newnode->parent=node;
								//Create the node
		newnode->right=NULL;
		newnode->data=item;
		node->left =newnode;
	}
}

void BTInsertRight(BTTree tree, BTNode node, BTItem item)		//This function inserts a node as right child of another node
{
	BTNode newnode;
	if (node->right!=NULL) return;								//If node has a right child just return this in order not to change anything
	else {
		newnode=new struct tnode;
		newnode->left=NULL;
		newnode->parent=node;
								//Create the node
		newnode->right=NULL;
		newnode->data=item;
		node->right =newnode;
	}
}



void BTDestroy(BTTree tree)			//This function frees all nodes in a binary tree
{

    if (tree==NULL) return;			//Base case
    BTDestroy(tree->left);		//Recur on left subtree
    BTDestroy(tree->right);			//Recur on right subtree
    free(tree);			//Free the node
}
