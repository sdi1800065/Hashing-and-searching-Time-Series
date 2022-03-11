#pragma once
#include "BTree.hpp"

typedef BTItem CBTItem;
typedef BTNode CBTNode;
typedef CBTNode CBTTree;



CBTNode CBTCreate();
CBTItem CBTGetItem(CBTTree tree,CBTNode node);
CBTNode CBTGetRoot(CBTTree tree);
CBTNode CBTGetParent(CBTTree tree,CBTNode node);
CBTNode CBTGetChildLeft(CBTTree tree,CBTNode node);
CBTNode CBTGetChildRight(CBTTree tree,CBTNode node);
int CBTIsNil(CBTNode node);
CBTNode CBTInsertLast(CBTTree tree, CBTItem item);
CBTNode CBTGetLast(CBTTree tree);
void CBTDestroy(CBTTree tree);
vector <double> PostOrder(CBTTree node, int size);
