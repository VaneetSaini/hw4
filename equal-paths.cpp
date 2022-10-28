#include "equal-paths.h"
#include <iostream> 
using namespace std;


// You may add any prototypes of helper functions here
int height(Node* root)
{
    if (root == nullptr) return 0;
    return 1 + max(height(root->left), height(root->right));
}
bool equalPaths(Node * root)
{
    int r, l;
    if (root == nullptr) return true;
    l = height(root->left);
    r= height(root->right);
    if ((l == 0 && r == 1) || (r == 0 && l == 1)) return true;
    if (l == r) return true;
    return false;
}

