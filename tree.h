// Kevin Yen Jan 24
#include <string>
#include <iostream>
#include <sstream>

#ifndef TREE_H
#define TREE_H

using namespace std;


struct TreeNode {
	int indexnumber;
	string item;         // The data in this node.
	TreeNode *left;   // Pointer to the left subtree.
	TreeNode *right;  // Pointer to the right subtree.

	TreeNode(string str, int index) {
	    // Constructor.  Make a node containing str.
	    indexnumber = index;
		item = str;
		left = NULL;
		right = NULL;
	}
};


#endif