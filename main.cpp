// Kevin Yen Jan 24
#include "scanner.h"
#include <sstream>
#include <fstream>
#include <vector>

using namespace std;

struct node{ //
	int index; // current index
	string key; // content of the node
	int back; // back link to it's father/mother/whoever-adpoted-this-node.
	int left; // index number of left child
	int right; // index number of right child
};

struct TreeNode{
  string key;
  struct TreeNode *left;
  struct TreeNode *right;
};

struct TreeNode *root =0;
Scanner s;
stringstream ss;
vector<node> tree,tree2;
int focus,treeindex;

ofstream myfile;

void debug(){
	cout<<"QWERQWRE"<<endl;
	exit(0);
}

// functions for c++98
int to_int(string in){
	int out;
	istringstream(in) >> out;
	return out;
} 

string to_str(int in){
	string s;
	stringstream out;
	out << in;
	s = out.str();
	return s;
}

inline bool is_number(const std::string & s)
{
   if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false ;

   char * p ;
   strtol(s.c_str(), &p, 10) ;

   return (*p == 0) ;
}

// returns a node, for constructing the vector.
node anode(int index, string key, int back, int left, int right){
	node temp;
	temp.back = back;
	temp.index = index;
	temp.key = key;
	temp.left = left;
	temp.right = right;
	return temp;
}

// initialize tree for new Expr
void treeinit(){
	tree.clear();
	focus = 0;
	treeindex = 0;
	tree.push_back(anode(0,"",-1,-1,-1));
}

void newnode(){
	tree[focus].left = ++treeindex;
	tree.push_back(anode(treeindex,"",focus,-1,-1));
	tree[focus].right = ++treeindex;
	tree.push_back(anode(treeindex,"",focus,-1,-1));
	focus = tree[focus].left;
}

void focusright(){focus = tree[tree[focus].back].right;}

void fillnode(string key){tree[focus].key = key;}

void focusback(){focus = tree[focus].back;}



// main stuff. Start(), Expr(), and List().
void Expr();
void List();

// Check() checks for any error messages to show. If error appears, print error and exit the whole program.
void Check(){
	if(s.Current.Type=="ERROR" || !s.error.empty()){
		// returning token is ERROR or the string named error is written by something.
		cout << "ERROR: " << s.error << endl;
		exit(0);
	}else if (s.Current.Type=="EOF"){
		// this happens when file ended but the grammar is not finished.
		cout << "ERROR: [syntax error] sentence not finished?" << endl;;
		exit(0);
	}
}

void drawtreeDOT(int index){
	// not exist
	if(index == -1) return;
	// alone, not a list
	if(tree[index].left==-1 && tree[index].right==-1){
		myfile << "\t" <<  tree[index].key << tree[index].index;
		myfile << ";" << endl;
		return;
	}
	myfile << "\t" <<  tree[index].key << tree[index].index << " -> " << tree[tree[index].left].key << tree[tree[index].left].index << ";" << endl;
	myfile << "\t" <<  tree[index].key << tree[index].index << " -> " << tree[tree[index].right].key << tree[tree[index].right].index << ";" << endl;
	drawtreeDOT(tree[index].left);
	drawtreeDOT(tree[index].right);
}

struct TreeNode* NewNode(string data) { 
  struct TreeNode* leaf = new(struct TreeNode);    // "new" is like "malloc" 
  leaf->key = data; 
  leaf->left = NULL; 
  leaf->right = NULL;

  return(leaf); 
} 

struct TreeNode* insert(struct TreeNode* leaf, string data) { 
  // 1. If the tree is empty, return a new, single node 
  if (leaf == NULL) { 
    return(NewNode(data)); 
  } 
  else { 
    // 2. Otherwise, recur down the tree 
    if (data <= leaf->key) leaf->left = insert(leaf->left, data); 
    else leaf->right = insert(leaf->right, data);

    return(leaf); // return the (unchanged) node pointer 
  } 
} 

void printTree(struct TreeNode* node) { 
  if (node == NULL) return;
  printTree(node->left); 
  cout << node->key; 
  printTree(node->right); 
} 

int treelength(struct TreeNode *leaf){
	if(leaf->key == "NIL") return 0;
	return 1 + treelength(leaf->right);
}

bool isLetter(struct TreeNode **leaf){
	if((*leaf)->key[0]>=65 && (*leaf)->key[0]<=90) return true;
	return false;
}

bool is_atom(struct TreeNode *leaf){
	if(leaf->left == NULL)
		return true;
	return false;
}

// Eval implementation
struct TreeNode* eval(struct TreeNode* leaf){
	//not a s-expression

// ******* RULE LEVEL 1 ********
// *******     ATOM     ********
// ******* RULE LEVEL 1 ********
	if(!leaf->key.empty()){
		if((leaf->key=="T")||(leaf->key=="NIL")||(is_number(leaf->key))) return leaf;
		else{
			cout << "ERROR : undefined atom " << leaf->key <<endl;
			exit(0);
		}
	}
// ******* RULE LEVEL 2 ********
// ******* RULE LEVEL 2 ********
// ******* RULE LEVEL 2 ********
	//CAR
	if(leaf->left->key=="CAR"){
		if (treelength(leaf)!=2)
		{
			cout << "ERROR : CAR undefined (treelength(leaf)!=2)" << endl;
			exit(0);
		}
		struct TreeNode *temp = 0;
		temp = eval(leaf->right->left);
		if(is_atom(temp)){
			cout << "ERROR : CAR undefined (eval(s1) is an atom)" << endl;
			exit(0);
		}
		return temp->left;
	}
	//CDR
	if(leaf->left->key=="CDR"){
		if (treelength(leaf)!=2)
		{
			cout << "ERROR : CDR undefined (treelength(leaf)!=2)" << endl;
			exit(0);
		}
		struct TreeNode *temp = 0;
		temp = eval(leaf->right->left);
		if(is_atom(temp)){
			cout << "ERROR : CDR undefined (eval(s1) is an atom)" << endl;
			exit(0);
		}
		return temp->right;
	}
	//CONS
	if(leaf->left->key=="CONS"){
		if (treelength(leaf)!=3)
		{
			cout << "ERROR : CONS undefined (treelength(leaf)!=3)" << endl;
			exit(0);
		}
		struct TreeNode *temp = NewNode("");
		temp->left = eval(leaf->right->left);
		temp->right = eval(leaf->right->right->left);
		return temp;
	}
	//ATOM
	if(leaf->left->key=="ATOM"){
		if (treelength(leaf)!=2)
		{
			cout << "ERROR : ATOM undefined (treelength(leaf)!=2)" << endl;
			exit(0);
		}
		if(is_atom(eval(leaf->right->left)))
			return NewNode("T");
		else
			return NewNode("NIL");
	}
	//INT
	if(leaf->left->key=="INT"){
		if (treelength(leaf)!=2)
		{
			cout << "ERROR : INT undefined (treelength(leaf)!=2)" << endl;
			exit(0);
		}
		if(is_number((eval(leaf->right->left)->key)))
			return NewNode("T");
		else
			return NewNode("NIL");
	}
	//NULL
	if(leaf->left->key=="NULL"){
		if (treelength(leaf)!=2)
		{
			cout << "ERROR : NULL undefined (treelength(leaf)!=2)" << endl;
			exit(0);
		}
		if((eval(leaf->right->left))->key == "NIL")
			return NewNode("T");
		else
			return NewNode("NIL");
	}
	//EQ
	if(leaf->left->key=="EQ"){
		if (treelength(leaf)!=3)
		{
			cout << "ERROR : EQ undefined (treelength(leaf)!=3)" << endl;
			exit(0);
		}else if(!is_atom(eval(leaf->right->left))){
			cout << "ERROR : eval(s1) is something other than an atom" << endl;
			exit(0);
		}else if(!is_atom(eval(leaf->right->right->left))){
			cout << "ERROR : eval(s2) is something other than an atom" << endl;
			exit(0);
		}
		if((eval(leaf->right->left))->key == (eval(leaf->right->right->left))->key)
			return NewNode("T");
		else
			return NewNode("NIL");
	}
	//PLUS
	if(leaf->left->key=="PLUS"){
		if (treelength(leaf)!=3)
		{
			cout << "ERROR : PLUS undefined (treelength(leaf)!=3)" << endl;
			exit(0);
		}else if(!is_number((eval(leaf->right->left))->key)){
			cout << "ERROR : PLUS - eval(s1) is something other than an numeric atom " << (eval(leaf->right->left))->key  << endl;
			exit(0);
		}else if(!is_number( (eval(leaf->right->right->left))->key )){
			cout << "ERROR : PLUS - eval(s2) is something other than an numeric atom " << (eval(leaf->right->right->left))->key << endl;
			exit(0);
		}
		return NewNode(to_str( to_int((eval(leaf->right->left))->key)+to_int((eval(leaf->right->right->left))->key) ));
	}
	//MINUS
	if(leaf->left->key=="MINUS"){
		if (treelength(leaf)!=3)
		{
			cout << "ERROR : MINUS undefined (treelength(leaf)!=3)" << endl;
			exit(0);
		}else if(!is_number((eval(leaf->right->left))->key)){
			cout << "ERROR : MINUS - eval(s1) is something other than an numeric atom " << (eval(leaf->right->left))->key << endl;
			exit(0);
		}else if(!is_number((eval(leaf->right->right->left))->key)){
			cout << "ERROR : MINUS - eval(s2) is something other than an numeric atom " << (eval(leaf->right->right->left))->key  << endl;
			exit(0);
		}
		return NewNode(to_str( to_int((eval(leaf->right->left))->key)-to_int((eval(leaf->right->right->left))->key) ));
	}
	//TIMES
	if(leaf->left->key=="TIMES"){
		if (treelength(leaf)!=3)
		{
			cout << "ERROR : TIMES undefined (treelength(leaf)!=3)" << endl;
			exit(0);
		}else if(!is_number((eval(leaf->right->left))->key)){
			cout << "ERROR : TIMES - eval(s1) is something other than an numeric atom " << (eval(leaf->right->left))->key << endl;
			exit(0);
		}else if(!is_number((eval(leaf->right->right->left))->key)){
			cout << "ERROR : TIMES - eval(s2) is something other than an numeric atom " << (eval(leaf->right->right->left))->key  << endl;
			exit(0);
		}
		return NewNode(to_str( to_int((eval(leaf->right->left))->key)*to_int((eval(leaf->right->right->left))->key) ));
	}
	//LESS
	if(leaf->left->key=="LESS"){
		if (treelength(leaf)!=3)
		{
			cout << "ERROR : LESS undefined (treelength(leaf)!=3)" << endl;
			exit(0);
		}else if(!is_number((eval(leaf->right->left))->key)){
			cout << "ERROR : LESS - eval(s1) is something other than an numeric atom " << (eval(leaf->right->left))->key << endl;
			exit(0);
		}else if(!is_number((eval(leaf->right->right->left))->key)){
			cout << "ERROR : LESS - eval(s2) is something other than an numeric atom " << (eval(leaf->right->right->left))->key  << endl;
			exit(0);
		}
		if(to_int((eval(leaf->right->left))->key)<to_int((eval(leaf->right->right->left))->key))
			return NewNode("T");
		else
			return NewNode("NIL");
	}
	//GREATER
	if(leaf->left->key=="GREATER"){
		if (treelength(leaf)!=3)
		{
			cout << "ERROR : GREATER undefined (treelength(leaf)!=3)" << endl;
			exit(0);
		}else if(!is_number((eval(leaf->right->left))->key)){
			cout << "ERROR : GREATER - eval(s1) is something other than an numeric atom " << (eval(leaf->right->left))->key << endl;
			exit(0);
		}else if(!is_number((eval(leaf->right->right->left))->key)){
			cout << "ERROR : GREATER - eval(s2) is something other than an numeric atom " << (eval(leaf->right->right->left))->key  << endl;
			exit(0);
		}
		if(to_int((eval(leaf->right->left))->key)>to_int((eval(leaf->right->right->left))->key))
			return NewNode("T");
		else
			return NewNode("NIL");
	}
	//QUOTE
	if(leaf->left->key=="QUOTE"){
		if (treelength(leaf)!=2)
		{
			cout << "ERROR : QUOTE length != 2" << endl;
			exit(0);
		}
		return leaf->right->left;
	}
	//COND
	// if(leaf->left->key=="COND"){
	// 	if (treelength(leaf)==1)
	// 	{
	// 		cout << "ERROR : COND length == 1" << endl;
	// 		exit(0);
	// 	}else if(){}
	// 	return leaf->right->left;
	// }


	cout << "ERROR: eval undefined " << leaf->left->key << endl;
	exit(0);
}

void pretty(int index){
	if(index==-1)return;
	if(tree[index].key!=""){
		cout << tree[index].key;
		return;
	}
	cout << "(";
	pretty(tree[index].left);
	cout << " . ";
	pretty(tree[index].right);
	cout << ")";
}

void printtree2(struct TreeNode *leaf){
	if(leaf == NULL)return;
	if(leaf->key!=""){
		cout << leaf->key;
		return;
	}
	cout << "(";
	printtree2(leaf->left);
	cout << " . ";
	printtree2(leaf->right);
	cout << ")";
}

void treetotree(struct TreeNode* leaf, int index){
	if(tree[index].left!=-1){
		leaf->left = NewNode(tree[tree[index].left].key);
		treetotree(leaf->left, tree[tree[index].left].index);
	}
	if(tree[index].right!=-1){
		leaf->right = NewNode(tree[tree[index].right].key);
		treetotree(leaf->right, tree[tree[index].right].index);
	}
}

void Start(){
	Check();
	while(s.Current.Type != "EOF"){
		// instead of simply cout the output, store them at a buff called ss. If there's no error, print out ss. This prevents error message appear in the middle of parsing/scanning.
		treeinit();
		Expr();
		// if the program manages to reach here, means there's no error. Thus print out legal results.
		// pretty(0); // same as cout << ss.str() << endl;
		// cout << endl;

		// ******** Raw tree ********
		//printtree(0);// uncomment to see tree. (my_index,content,back_index,left_index,right_index)
		root = NewNode(tree[0].key);
		treetotree(root, 0);
		drawtreeDOT(0);
		cout << "pretty : ";
		pretty(0);
		cout<<endl;

		cout << "input  : ";
		printtree2(root);
		cout << endl;

		root = eval(root);

		cout << "output : ";
		printtree2(root);
		cout << endl;
	}
}

void Expr(){
	Check();
	if(s.Current.Type == "Atom"){
		fillnode(s.Current.Content);
		s.MoveToNext();
	}
	else if(s.Current.Type == "OpenParenthesis"){
		s.MoveToNext();
		if(s.Current.Type == "ClosingParenthesis"){
			fillnode("NIL");
		}
		while(s.Current.Type != "ClosingParenthesis"){
			List();
		}
		s.MoveToNext();
	}
	else
		s.error = "[syntax error] not expecting \"" + s.Current.Content + "\"";
}

void List(){
	Check();
	int x = 0; // x is for counting how many closing parenthesis needs to be printed according to open parenthesis.
	while(s.Current.Type == "Atom" || s.Current.Type == "OpenParenthesis"){
		x++;
		newnode();
		Expr();
		focusright();
	}
	fillnode("NIL");
	while(x!=0){ // fill up all the forgotten closing parenthesises.
		x--;
		focusback();
	}
}

int main(){
	myfile.open("graph.dot");
	myfile << "digraph G {"<<endl;
	s.Init();
	Start();
	myfile << "}" << endl;
	myfile.close();
	return 0;
}