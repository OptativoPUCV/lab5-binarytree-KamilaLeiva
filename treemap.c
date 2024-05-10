#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap * new = (TreeMap *)malloc(sizeof(TreeMap));
    new->root = NULL;
    new->current = NULL;
    new->lower_than = lower_than;
    return new;
}

void insertTreeMap(TreeMap * tree, void* key, void * value) {
    if (tree == NULL || key == NULL || value == NULL) return;

    if (tree->root == NULL){
        tree->root = createTreeNode(key, value);
        TreeNode* aux = tree->root;
        tree->current = aux;
        return;
    } else{
        TreeNode* aux = tree->root;
        while(aux != NULL){
            if(is_equal(tree,key,aux->pair->key)){
                aux->pair->value = value;
                return;
            }
            if(tree->lower_than(key,aux->pair->key) == 1){
                if(aux->left == NULL){
                    TreeNode* new = createTreeNode(key, value);
                    new->parent = aux;
                    aux->left = new;
                    tree->current = new;
                    return;
                }
                aux = aux->left;    
                }
            else{
                if(aux->right == NULL){
                    TreeNode* new = createTreeNode(key, value);
                    new->parent = aux;
                    aux->right = new;
                    tree->current = new;
                    return;
                }
                aux = aux->right;
            }
        }
    }
}

TreeNode * minimum(TreeNode * x){
    while(x->left != NULL){
        x = x->left;
    }
    return x;
}


void removeNode(TreeMap * tree, TreeNode* node){
    if(node == NULL) return;
    
    if(node->left == NULL && node->right == NULL){
        if(node->parent == NULL){
            tree->root = NULL;
        }
        else if(node->parent->left == node){
            node->parent->left = NULL;
        }
        else{
            node->parent->right = NULL;
        }
        free(node);
    }
    else if (node->left != NULL && node->right != NULL) {
        
        TreeNode* sucesor = minimum(node->right);
        node->pair->key = sucesor->pair->key;
        node->pair->value = sucesor->pair->value;
        removeNode(tree, sucesor);
    } else {
        TreeNode* hijo = (node->left != NULL) ? node->left : node->right;
        if (node->parent == NULL) {
            tree->root = hijo;
        } else if (node->parent->left == node) {
            node->parent->left = hijo;
        } else {
            node->parent->right = hijo;
        }
        hijo->parent = node->parent;
        free(node);
    }
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}

Pair * searchTreeMap(TreeMap * tree, void* key) {
    if (tree == NULL || tree->root == NULL) return NULL;
    TreeNode* aux = tree->root;
    while(aux != NULL){
        if(is_equal(tree,key,aux->pair->key)){
            tree->current = aux;
            return aux->pair;
        }
        if(tree->lower_than(key,aux->pair->key) == 1){
            aux = aux->left;
        }
        else{
            aux = aux->right;
        }
    }
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    if (tree == NULL || tree->root == NULL) return NULL;
    TreeNode* aux = tree->root;
    Pair *resultado = NULL;
    while(aux != NULL){
        if(tree->lower_than(aux->pair->key, key) == 0){
            aux = aux->right;
        }
        else{
            resultado = aux->pair;
            aux = aux->left;
        }
    }
    return resultado;
}

Pair * firstTreeMap(TreeMap * tree) {
    if (tree == NULL || tree->root == NULL) return NULL;
    TreeNode* aux = minimum(tree->root);
    tree->current = aux;
    return aux->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
    if (tree == NULL || tree->current == NULL) return NULL;
    TreeNode* aux = tree->current;
    if(aux->right != NULL){
        tree->current = minimum(aux->right);
        return tree->current->pair;
    }
    TreeNode* aux2 = aux->parent;
    while(aux2 != NULL && aux == aux2->right){
        aux = aux2;
        aux2 = aux->parent;
    }
    tree->current = aux2;
    if(tree->current != NULL) return tree->current->pair;
    
    return NULL;
}
