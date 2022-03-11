#include <iostream>
#include <vector>
using namespace std;

#pragma once


typedef struct tnode *BTNode;	//Pointer to a tree's node
typedef int BTItem;			//The data type of the tree's nodes
typedef void (*FunPtr)(BTNode a);
void VisitTheNode(BTNode node);
typedef BTNode BTTree;




BTNode BTCreate();
BTNode BTGetRoot(BTTree tree);
BTNode BTGetParent(BTTree tree,BTNode node);
BTNode BTGetChildLeft(BTTree tree,BTNode node);
BTNode BTGetChildRight(BTTree tree,BTNode node);
int BTIsNil(BTNode node);
BTItem BTGetItem(BTTree tree,BTNode node);
BTNode BTInsertRoot(BTTree tree,BTItem item);
void BTInsertLeft(BTTree tree, BTNode node, BTItem item);
void BTInsertRight(BTTree tree, BTNode node, BTItem item);
void BTDestroy(BTTree tree);
