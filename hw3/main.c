#include<stdio.h>
#include<stdlib.h>

typedef struct node
{
    int data;
    struct node *left;
    struct node *right;
}TreeNode;

TreeNode *AddNode(TreeNode *tree,int add)
{
    if(!tree)
    {
        tree=(TreeNode *)malloc(sizeof(TreeNode));
        tree->data=add;
        tree->left=NULL;
        tree->right=NULL;
    }
    else
    {
        if(tree->data>add)
            tree->left=AddNode(tree->left,add);
        else  // Since it is guaranteed that point x will not be in the BST before the insertion, there's no case of "if(tree->data==add)"
            tree->right=AddNode(tree->right,add);
    }
    return tree;
}

TreeNode *DelNode(TreeNode *tree,int del)
{
    /* Since it is guaranteed that point x is always in the BST before the deletion,
       there's no case of not finding any node to delete, which means there's no case of "if(tree==NULL)" */
    if(tree->data>del)
        tree->left=DelNode(tree->left,del);  // Find the node in the left-subtree
    else if(del>tree->data)
        tree->right=DelNode(tree->right,del);  // Find the node in the right-subtree
    else // The node which is going to be deleted is found
    {
        if(!tree->left&&!tree->right)
        {
            free(tree);
            tree=NULL;
        }
        else if(tree->left&&!tree->right)
        {
            TreeNode *temp=tree;
            tree=tree->left;
            free(temp);
        }
        else if(!tree->left&&tree->right)
        {
            TreeNode *temp=tree;
            tree=tree->right;
            free(temp);
        }
        else
        {
            TreeNode *temp=tree->left;  // Start from left-subtree, find the node whose data is the largest
            while(temp->right)
                temp=temp->right;
            tree->data=temp->data;      // Substitute the data of tree with which of temp
            tree->left=DelNode(tree->left,temp->data);  // Continue to delete the node whose data is temp->data (tree->data) in the left-subtree of tree
        }
    }
    return tree;
}

void QueryNode(TreeNode *tree,int search)
{
    TreeNode *ptr=tree;
    int min=abs(tree->data-search),temp;
    while(ptr!=NULL)  // My algorithm is to find the smallest gap between all nodes and the searching number first
    {
        temp=abs(ptr->data-search);
        if(temp<min)
            min=temp;
        if(ptr->data>search)
            ptr=ptr->left;
        else if(search>ptr->data)
            ptr=ptr->right;
        else  // The node does exist in the BST, just output its data and return to main
        {
            printf("%d\n",search);
            return;
        }
    }
    ptr=tree;
    int store[2]; // at most two nodes are in same distances with the query node
    int n=0;
    while(ptr!=NULL)  // Run through the path again and store the data of the node if(temp==min) in the array
    {
        temp=abs(ptr->data-search);
        if(temp==min)
        {
            store[n]=ptr->data;
            n++;
        }
        if(ptr->data>search)
            ptr=ptr->left;
        else
            ptr=ptr->right;
    }
    /* Assume that there are n nodes in the BST, then the worst case is to run through all n nodes(when the BST is actually a line)twice;
       therefore, the time complexity is 2*n=O(n) */
    switch(n)
    {
        case 1:
            printf("%d\n",store[0]);
            break;
        case 2:  // print in ascending order
            if(store[0]<store[1])
                printf("%d %d\n",store[0],store[1]);
            else
                printf("%d %d\n",store[1],store[0]);
    }
}

int main()
{
    TreeNode *tree=NULL;
    char command[10];
    int x,T;
    scanf("%d",&T);  // number of operations
    for(;T>0;T--)
    {
        scanf("%s",command);
        scanf("%d",&x);
        switch(*command)
        {
            case 'i':  // to insert a new node into the BST
                tree=AddNode(tree,x);
                break;
            case 'd':  // to delete a node in the BST
                tree=DelNode(tree,x);
                break;
            case 'q':  // to query a node in the BST
                QueryNode(tree,x);
        }
    }
    return 0;
}
