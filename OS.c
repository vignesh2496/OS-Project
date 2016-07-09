#include <stdio.h>
#include <stdlib.h>

struct node
{
    int start;
    int height;
    int leftmax;
    int rightmax;
    int size;
    struct node* left;
    struct node* right;
};


int max(int a,int b)
{
    return a >= b ? a : b;
}

int geth(struct node* N)
{
    if(N == NULL)
        return 0;
    else
        return N->height;
}

int bal(struct node* N)
{
    if(N == NULL)
        return 0;
    else
        return geth(N->left) - geth(N->right);
}

struct node * inorderSucc(struct node* node)
{
    struct node* current = node;

    /* loop down to find the leftmost leaf */
    while (current->left != NULL)
        current = current->left;

    return current;
}

struct node * inorderPred(struct node* node)
{
    struct node* current = node;

    /* loop down to find the leftmost leaf */
    while (current->right != NULL)
        current = current->right;

    return current;
}

struct node* rightrot(struct node* piv)
{
    struct node* temp = piv->left;
    struct node* T = temp->right;
    temp->right = piv;
    piv->left = T;
    piv->height = max(geth(piv->left),geth(piv->right)) + 1;
    piv->leftmax = getleftmax(piv);
    piv->rightmax = getrightmax(piv);
    temp->height = max(geth(temp->left),geth(temp->right)) + 1;
    temp->leftmax = getleftmax(temp);
    temp->rightmax = getrightmax(temp);
    return temp;
}

struct node* leftrot(struct node* piv)
{
    struct node* temp = piv->right;
    struct node* T = temp->left;
    temp->left = piv;
    piv->right = T;
    piv->height = max(geth(piv->left),geth(piv->right)) + 1;
    piv->leftmax = getleftmax(piv);
    piv->rightmax = getrightmax(piv);
    temp->height = max(geth(temp->left),geth(temp->right)) + 1;
    temp->leftmax = getleftmax(temp);
    temp->rightmax = getrightmax(temp);
    return temp;
}

int getleftmax(struct node* root)
{
    if(root == NULL)
        return 0;

    if(root->left == NULL)
        return 0;

    return max(root->left->size, max(root->left->leftmax, root->left->rightmax));
}

int getrightmax(struct node* root)
{
    if(root == NULL)
        return 0;

    if(root->right == NULL)
        return 0;

    return max(root->right->size, max(root->right->leftmax, root->right->rightmax));
}

struct node* del(struct node* root, int key)
{

    if (root == NULL)
        return root;

    if ( key < root->start )
        root->left = del(root->left, key);

    else if( key > root->start )
        root->right = del(root->right, key);

    else
    {
        // node with only one child or no child
        if( (root->left == NULL) || (root->right == NULL) )
        {
            struct node *temp = root->left ? root->left : root->right;

            // No child case
            if(temp == NULL)
            {
                temp = root;
                root = NULL;
            }
            else // One child case
             *root = *temp; // Copy the contents of the non-empty child

            free(temp);
        }
        else
        {
            // node with two children: Get the inorder successor (smallest
            // in the right subtree)
            struct node* temp = inorderSucc(root->right);

            // Copy the inorder successor's data to this node
            root->start = temp->start;

            // Delete the inorder successor
            root->right = del(root->right, temp->start);
        }
    }

    // If the tree had only one node then return
    if (root == NULL)
      return root;

    root->height = max(geth(root->left), geth(root->right)) + 1;
    root->leftmax = getleftmax(root);
    root->rightmax = getrightmax(root);

    //  this node became unbalanced
    int B = bal(root);

    // If this node becomes unbalanced, then there are 4 cases

    // Left Left Case
    if (B > 1 && bal(root->left) >= 0)
        return rightrot(root);

    // Left Right Case
    if (B > 1 && bal(root->left) < 0)
    {
        root->left =  leftrot(root->left);
        return rightrot(root);
    }

    // Right Right Case
    if (B < -1 && bal(root->right) <= 0)
        return leftrot(root);

    // Right Left Case
    if (B < -1 && bal(root->right) > 0)
    {
        root->right = rightrot(root->right);
        return leftrot(root);
    }

    return root;
}

struct node* insert(struct node* root, int startAdd, int chunkSize)
{
    if(root == NULL)
    {
        struct node* temp = (struct node*)malloc(sizeof(struct node));
        temp->start = startAdd;
        temp->left = NULL;
        temp->right = NULL;
        temp->height = 1;
        temp->leftmax = 0;
        temp->rightmax = 0;
        temp->size = chunkSize;
        root = temp;
    }

    else if(root->start + root->size == startAdd)
    {
        root->size = root->size + chunkSize;
    }

    else if(startAdd + chunkSize == root->start)
    {
        root->start = startAdd;
        root->size = root->size + chunkSize;
    }

    else if(root->start > startAdd)
        root->left = insert(root->left, startAdd, chunkSize);

    else
        root->right = insert(root->right, startAdd, chunkSize);

    root->height = max(geth(root->left), geth(root->right)) + 1;
    root->leftmax = getleftmax(root);
    root->rightmax = getrightmax(root);

    int B = bal(root);

    if(B > 1 && root->left->start > startAdd)
        return rightrot(root);

    else if(B < -1 && root->right->start < startAdd)
        return leftrot(root);

    else if(B > 1 && root->left->start < startAdd)
    {
        root->left = leftrot(root->left);
        return rightrot(root);
    }

    else if(B <- 1 && root->right->start > startAdd)
    {
        root->right = rightrot(root->right);
        return leftrot(root);
    }
    return root;
}

struct node* worstFit(struct node* root, int procSize, int* worstFitRes)
{
    if(root == NULL)
    {
        *worstFitRes = -1;
        return NULL;
    }

    int M = max(root->size,max(root->leftmax, root->rightmax));
    if(M == root->size)
    {
        if(M < procSize)
        {
            *worstFitRes = -1;
        }

        else if(M == procSize)
        {
            root = del(root, root->start);
        }

        else
        {
            root->size = root->size - procSize;
        }

        return root;
    }

    else if(M == root->leftmax)
        root->left = worstFit(root->left, procSize, worstFitRes);
    
    else
        root->right = worstFit(root->right, procSize, worstFitRes);

    root->leftmax = getleftmax(root);
    root->rightmax = getrightmax(root);
    return root;
}

void inorder(struct node* root)
{
    if(root == NULL)
        return;
    else
    {
        inorder(root->left);
        printf("Chunk Start : %d\nChunk Size : %d KB\n",root->start,root->size);
        inorder(root->right);
    }
}

void main()
{
    struct node* R = (struct node*)malloc(sizeof(struct node));
    R=NULL;
    while(1)
    {
        int ch;
        printf("MENU:\n1.Worst Fit\n2.Release Memory\n3.Display Free Memory Chunks\n4.Exit\nEnter your choice:");
        scanf("%d",&ch);
        switch(ch)
        {
            case 1:{
                        int s;
                        printf("Enter the size of the process in KB : ");
                        scanf("%d",&s);
                        int worstFitRes = 0;
                        R = worstFit(R, s, &worstFitRes);
                        if(worstFitRes == -1)
                            printf("Sorry could not find any free space to be allocated the process.\n");

                        printf("\n\n");
                        break;
                   }
            case 2:{
                        int st,si;
                        printf("Enter the Starting address of the block : ");
                        scanf("%d",&st);
                        printf("Enter the size of the block : ");
                        scanf("%d",&si);
                        R = insert(R,st,si);
                        printf("\n\n");
                        break;
                   }
            case 3:{
                        inorder(R);
                        printf("\n\n");
                        break;
                   }
            case 4:{
                        exit(0);
                        break;
                   }
        }
    }
}
