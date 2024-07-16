#include "GameEngine/util/Tree.h"

int main()
{
	Gen::Tree<int> tree;
	tree.addNode(1, 2, 3);
	tree.addNode(1, 3, 4);
	tree.addNode(6, 4, 3);
	tree.addNode(6, 4, 3);
}