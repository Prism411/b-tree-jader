#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define T 3  // Grau mínimo da árvore B, ajuste conforme necessário

typedef struct BTreeNode {
    int *keys;
    int n;
    bool leaf;
    struct BTreeNode **children;
} BTreeNode;

BTreeNode* createNode(bool leaf);
void traverse(BTreeNode* root);
BTreeNode* insert(BTreeNode* root, int k);
void insertNonFull(BTreeNode* node, int k);
void splitChild(BTreeNode* parent, int idx, BTreeNode* child);
BTreeNode* search(BTreeNode* root, int k);

BTreeNode* createNode(bool leaf) {
    BTreeNode* newNode = (BTreeNode*)malloc(sizeof(BTreeNode));
    newNode->keys = (int*)malloc(sizeof(int) * (2 * T - 1));
    newNode->children = (BTreeNode**)malloc(sizeof(BTreeNode*) * 2 * T);
    newNode->n = 0;
    newNode->leaf = leaf;

    return newNode;
}

void traverse(BTreeNode* root) {
    int i;
    for (i = 0; i < root->n; i++) {
        if (!root->leaf) {
            traverse(root->children[i]);
        }
        printf(" %d", root->keys[i]);
    }

    if (!root->leaf) {
        traverse(root->children[i]);
    }
}

BTreeNode* search(BTreeNode* root, int k) {
    int i = 0;
    while (i < root->n && k > root->keys[i]) {
        i++;
    }

    if (i < root->n && root->keys[i] == k) {
        return root;
    }

    if (root->leaf) {
        return NULL;
    }

    return search(root->children[i], k);
}

BTreeNode* insert(BTreeNode* root, int k) {
    if (root == NULL) {
        root = createNode(true);
        root->keys[0] = k;
        root->n = 1;
        return root;
    }

    if (root->n == 2 * T - 1) {
        BTreeNode* newRoot = createNode(false);
        newRoot->children[0] = root;
        splitChild(newRoot, 0, root);

        int i = 0;
        if (newRoot->keys[0] < k) {
            i++;
        }
        insertNonFull(newRoot->children[i], k);

        return newRoot;
    } else {
        insertNonFull(root, k);
        return root;
    }
}

void insertNonFull(BTreeNode* node, int k) {
    int i = node->n - 1;

    if (node->leaf) {
        while (i >= 0 && node->keys[i] > k) {
            node->keys[i + 1] = node->keys[i];
            i--;
        }

        node->keys[i + 1] = k;
        node->n++;
    } else {
        while (i >= 0 && node->keys[i] > k) {
            i--;
        }

        if (node->children[i + 1]->n == 2 * T - 1) {
            splitChild(node, i + 1, node->children[i + 1]);

            if (node->keys[i + 1] < k) {
                i++;
            }
        }
        insertNonFull(node->children[i + 1], k);
    }
}

void splitChild(BTreeNode* parent, int idx, BTreeNode* child) {
    BTreeNode* newNode = createNode(child->leaf);
    newNode->n = T - 1;

    for (int j = 0; j < T - 1; j++) {
        newNode->keys[j] = child->keys[j + T];
    }

    if (!child->leaf) {
        for (int j = 0; j < T; j++) {
            newNode->children[j] = child->children[j + T];
        }
    }

    child->n = T - 1;

    for (int j = parent->n; j >= idx + 1; j--) {
        parent->children[j + 1] = parent->children[j];
    }

    parent->children[idx + 1] = newNode;

    for (int j = parent->n - 1; j >= idx; j--) {
        parent->keys[j + 1] = parent->keys[j];
    }

    parent->keys[idx] = child->keys[T - 1];
    parent->n++;
}

int main() {
    BTreeNode* root = NULL;

    root = insert(root, 10);
    root = insert(root, 20);
    root = insert(root, 5);
    root = insert(root, 6);
    root = insert(root, 12);
    root = insert(root, 30);
    root = insert(root, 7);
    root = insert(root, 17);

    printf("Traversal of the constructed tree is:");
    traverse(root);

    int key = 6;
    (search(root, key) != NULL) ? printf("\nPresent") : printf("\nNot Present");

    key = 15;
    (search(root, key) != NULL) ? printf("\nPresent") : printf("\nNot Present");

    return 0;
}

