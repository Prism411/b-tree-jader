
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define T 3
typedef struct {
    char name[100];
    int rnn;
} Key;

typedef struct BTreeNode {
    int n;  // Número de chaves no nó
    Key keys[2 * T - 1];  // Chaves
    struct BTreeNode* children[2 * T];  // Filhos da página
    bool leaf;  // É um nó folha?
} BTreeNode;

typedef struct {
    BTreeNode* root;
} BTree;

typedef struct BTreePage {
    int pageNumber;  // Número da página no arquivo de disco
    int parentPageNumber;  // Número da página do pai no arquivo de disco
    int numKeys;  // Número de chaves na página
    Key keys[2 * T - 1];  // Chaves
    int children[2 * T];  // Posições dos filhos no arquivo
    bool isLeaf;  // É uma página folha?
} BTreePage;


BTree* createBTree();
BTreeNode* createBTreeNode(bool leaf);
void freeBTree(BTree* tree);
void freeBTreeNode(BTreeNode* node);
void insert(BTree* tree, Key k);
void insertNonFull(BTreeNode* node, Key k);
void splitChild(BTreeNode* x, int i, BTreeNode* y);
BTreeNode* search(BTreeNode* root, char* name);
void traverse(BTreeNode* root);

