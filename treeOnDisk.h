
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define T 3
typedef struct {
    char name[100];
    int rnn;
} Key;

typedef struct BTreeNode {
    int n;  // N�mero de chaves no n�
    Key keys[2 * T - 1];  // Chaves
    struct BTreeNode* children[2 * T];  // Filhos da p�gina
    bool leaf;  // � um n� folha?
} BTreeNode;

typedef struct {
    BTreeNode* root;
} BTree;

typedef struct BTreePage {
    int pageNumber;  // N�mero da p�gina no arquivo de disco
    int parentPageNumber;  // N�mero da p�gina do pai no arquivo de disco
    int numKeys;  // N�mero de chaves na p�gina
    Key keys[2 * T - 1];  // Chaves
    int children[2 * T];  // Posi��es dos filhos no arquivo
    bool isLeaf;  // � uma p�gina folha?
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

