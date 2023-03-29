#include <stdio.h>
#include <stdlib.h>

typedef struct NODE_s *NODE;
typedef struct NODE_s {
    NODE left;
    NODE right;
    int key;
    void *data;
    int height;
} NODE_t[1];

typedef struct {
    NODE root;
} BST_t[1], *BST;


BST bst_init(){
    BST tree = (BST)malloc(sizeof(BST_t));
    tree->root = NULL;
    return tree;
}

NODE bst_init_node( int key, void *data) {
    NODE node = (NODE)malloc(sizeof(NODE_t));
    node->left = NULL;
    node->right = NULL;
    node->key = key;
    node->data = data;
    return node;
}

int bst_get_height(NODE node)
{
    if(node == NULL)
    {
        return 0;
    }
    else {
        return node->height;
    }
}

NODE right_rotation(NODE node)
{
    NODE left_child = node->left;
    NODE right_of_left_child = left_child->right;
    node->left = right_of_left_child;
    left_child->right = node;

    node->height = bst_get_height(node);
    left_child->height = bst_get_height(left_child);
    return left_child;
}

NODE left_rotation(NODE node)
{
    NODE right_child = node->left;
    NODE left_of_right_child = right_child->right;
    node->right = left_of_right_child;
    right_child->left = node;

    node->height = bst_get_height(node);
    right_child->height = bst_get_height(right_child);
    return right_child;
}

int bst_insert(BST tree, int key, void *data) {
    if(tree == NULL) {
        printf("Error: The tree not initialized.\n");
    }
    else if(tree->root == NULL) {
        tree->root = bst_init_node(key, data);
    }
    else {
        NODE node = tree->root;
        while(node != NULL) {
            if(key < node->key) {
                if(node->left != NULL) {
                    node = node->left;
                }
                else{
                    node->left = bst_init_node(key, data);
                    node = NULL;
                }
            }
            else if(key > node->key) {
                if(node->right != NULL) {
                    node = node->right;
                }
                else{
                    node->right = bst_init_node(key, data);
                    node = NULL;
                }
            }
            else{
                printf("Error: Duplicate key Entry! No action is done. \n");
                node = NULL;
                return -1;
            }
        }
    }
    return 1;
}

int max(int a, int b) {
    if(a>b) {
        return a;
    }
    else {
        return b;
    }
}



int balance_factor(NODE node)
{
    return bst_get_height(node->left) - bst_get_height(node->right);
}

NODE bst_insert_rec(BST tree, NODE node, int key, void *data)
{
    if(node == NULL){
        node = bst_init_node(key, data);
    }
    else {
        if(key < node->key) {
            node->left = bst_insert_rec(tree, node->left, key, data);
        }
        else if(key > node->key) {
            node->right = bst_insert_rec(tree, node->right, key, data);
        }
        else {
            printf("Error duplicate key entered!\n");
        }
    }
    node->height = 1 + max(bst_get_height(node->left), bst_get_height(node->right));
    int balance = balance_factor(node);
    printf("Node Key: %d, Node Height: %d, Balance Factor: %d\n",node->key, node->height, balance);

    if(balance ==2) {
        if(node->left->right->height > node->left->left->height) {
            node->left = left_rotation(node->left);
            return right_rotation(node);
            //LR(tree, node, node->left, node->left->right);

        }
        else {
            return right_rotation(node);
            //RR(tree, node, node->left, node->left->left);
        }
    }
    else if(balance == -2) {
        if(node->right->left->height > node->right->right->height) {
            node->right = right_rotation(node->right);
            return left_rotation(node);

            //RR(tree, node, node->left, node->left->right);
            //LR();
        }
        else {
            return left_rotation(node);
            //LR(tree, node, node->left, node->left->left);
        }
    }
    return node;
}

void bst_insert_recursion(BST tree, int key, void *data)
{
    if(tree == NULL) {
        printf("Error: The tree not initialized.\n");
    }
    else if(tree->root == NULL) {
        tree->root = bst_init_node(key, data);
    }
    else {
        bst_insert_rec(tree, tree->root , key, data);
        tree->root = bst_init_node(key, data);
    }
}

void bst_print_side_way(int key, int height)
{
    int i =1;
    while (i < height){
        printf("----");
        i++;
    }
    printf("%d\n", key);
}

void bst_reverse_in_order(NODE node, int height)
{
    if(node != NULL) {
        height++;
        if(node->right != NULL){
            bst_reverse_in_order(node->right, height);
        }
        bst_print_side_way(node->key, height);
        if(node->left != NULL){
            bst_reverse_in_order(node->left, height);
        }
    }
}

void bst_unbalanced_deletion(BST tree, int key)
{
    NODE node = tree->root;
    NODE parent = (NODE)tree;

    while(node != NULL) {
        if( key < node->key) {
            parent = node;
            node = node->left;
        }
        else if(key > node->key) {
            parent = node;
            node = node->right;
        }
        else {
            break;
        }
    }

    if(node == NULL)
    {
        printf("Key does not exist.\n");
        return;
    }

    if((node->left == NULL) && (node->right == NULL)) {
        if(parent->left == node) {
            parent->left = NULL;
        }
        else {
            parent->right = NULL;
        }
    }

    else if((node->left == NULL) && (node->right != NULL)) {
        if(parent->left == node) {
            parent->left = node->right;
        }
        else {
            parent->right = node->right;
        }
    }

    else if((node->left != NULL) && (node->right == NULL)) {
        if(parent->left == node) {
            parent->left = node->left;
        }
        else {
            parent->right = node->left;
        }
    }

    else {
        NODE min = node->right;
        while (min->left != NULL) {
            min = min->left;
        }
        if(parent->right == node) {
            parent->right = node->right;
            min->left = node->left;
        }
        else {
            parent->left = node->right;
            min->left = node->left;
        }

    }
    free(node);
}

void bst_balanced_deletion(BST tree, int key)
{
    NODE node = tree->root;
    NODE parent = (NODE)tree;

    while(node != NULL) {
        if( key < node->key) {
            parent = node;
            node = node->left;
        }
        else if(key > node->key) {
            parent = node;
            node = node->right;
        }
        else {
            break;
        }
    }

    if(node == NULL)
    {
        printf("Key does not exist.\n");
        return;
    }

    if((node->left == NULL) && (node->right == NULL)) {
        if(parent->left == node) {
            parent->left = NULL;
        }
        else {
            parent->right = NULL;
        }
        free(node);
    }

    else if((node->left == NULL) && (node->right != NULL)) {
        if(parent->left == node) {
            parent->left = node->right;
        }
        else {
            parent->right = node->right;
        }
        free(node);
    }

    else if((node->left != NULL) && (node->right == NULL)) {
        if(parent->left == node) {
            parent->left = node->left;
        }
        else {
            parent->right = node->left;
        }
        free(node);
    }
s
    else {
        parent = node;
        NODE min = node->right;
        while (min->left != NULL) {
            parent = min;
            min = min->left;
        }
        free(node->data);
        node->key = min->key;
        node->data = min->data;
        parent->left = min->right;
        free(min);
    }
}



int main() {

    BST tree = bst_init();

    bst_insert_recursion(tree, 17, NULL);
    printf("\n");
    bst_insert_recursion(tree, 18, NULL);
    printf("\n");
    bst_insert_recursion(tree, 19, NULL);
    printf("\n");
    bst_insert_recursion(tree, 20, NULL);
    printf("\n");
    bst_insert_recursion(tree, 77, NULL);
    printf("\n");
    bst_reverse_in_order(tree->root, 0);
    free(tree);
    return 1;
}
