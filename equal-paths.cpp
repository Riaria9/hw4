#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;

// You may add any prototypes of helper functions here
void checkOther(Node* root, int total_length, int& desired_length, bool& result);

bool equalPaths(Node * root)
{
    // Add your code below
    // int desired_length = oneLength(root, 0);
    int desired_length = -1;
    bool comparison_result = true;
    checkOther(root,0 ,desired_length, comparison_result);
    return comparison_result;
}

void checkOther(Node* root, int total_length, int& desired_length, bool& result)
{   
    //only compare the length if it is a leaf node
    if(root == nullptr){
        return;
    }
    if(root->left != nullptr)
        checkOther(root->left,total_length+1, desired_length, result);
    if(root->right != nullptr)
        checkOther(root->right,total_length+1, desired_length, result);

    if(desired_length == -1){
        //finding the first leaf node, set it to the desired_length which would be later compared to others
            desired_length = total_length;
    }
    if(root->left == nullptr && root -> right == nullptr){
        if(total_length != desired_length){
        result = false;
        }
    }
    
    
    

}

