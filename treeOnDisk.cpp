#define T 3  // Grau da árvore B. Escolha um valor adequado.
#include "file.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


BTree* createBTree() {
    BTree* tree = (BTree*)malloc(sizeof(BTree));
    if (tree == NULL) {
        fprintf(stderr, "Erro de alocação de memória ao criar uma nova árvore B.\n");
        exit(EXIT_FAILURE);
    }

    // A árvore é inicialmente vazia, então a raiz é uma folha (nó folha)
    tree->root = createBTreeNode(true);
	
	printf("Arvore Criada com Sucesso!");
    return tree;
}

BTreeNode* createBTreeNode(bool leaf) {
    BTreeNode* newNode = (BTreeNode*)malloc(sizeof(BTreeNode));
    if (newNode == NULL) {
        fprintf(stderr, "Erro de alocação de memória ao criar um novo nó da árvore B.\n");
        exit(EXIT_FAILURE);
    }

    newNode->leaf = leaf;
    newNode->n = 0; // Inicializando com 0 chaves

    // Inicializando as chaves e os filhos
    for (int i = 0; i < 2 * T - 1; i++) {
        newNode->keys[i].rnn = 0; // ou outra valor que indique uma chave vazia ou não utilizada
        // Inicializar newNode->keys[i].name se necessário
    }
    for (int i = 0; i < 2 * T; i++) {
        newNode->children[i] = NULL;
    }

    return newNode; printf("Criado com Sucesso");
}

void freeBTreeNode(BTreeNode* node) {
    // Verificação de segurança; se o nó já é NULL, não há nada para liberar.
    if (node == NULL) {
        return;
    }

    // Se o nó não é uma folha, libere todos os seus filhos primeiro.
    if (!node->leaf) {
        for (int i = 0; i <= node->n; i++) {  // Note que usamos `<=` porque há n+1 filhos
            freeBTreeNode(node->children[i]);
        }
    }

    // Agora que os filhos foram liberados, podemos liberar o próprio nó.
    free(node);
}

void freeBTree(BTree* tree) {
    if (tree != NULL) {
        // Comece liberando todos os nós a partir da raiz.
        freeBTreeNode(tree->root);
        
        // Depois de todos os nós serem liberados, libere a estrutura da árvore.
        free(tree);
    }
}

// Função para dividir o filho y do nó x. i é o índice de y em
// filho de x.  O filho deve estar cheio quando esta função é chamada
void splitChild(BTreeNode* x, int i, BTreeNode* y) {
    // Crie um novo nó para armazenar (t-1) chaves de y
    BTreeNode* z = createBTreeNode(y->leaf);
    z->n = T - 1;

    // Copia as últimas (t-1) chaves de y para z
    for (int j = 0; j < T - 1; j++) {
        z->keys[j] = y->keys[j + T];
    }

    // Copia os últimos t filhos de y para z se y não for folha
    if (!y->leaf) {
        for (int j = 0; j < T; j++) {
            z->children[j] = y->children[j + T];
        }
    }

    y->n = T - 1; // Reduzir o número de chaves em y

    // Como o nó x vai ter um novo filho,
    // criamos espaço para o novo filho
    for (int j = x->n; j >= i + 1; j--) {
        x->children[j + 1] = x->children[j];
    }

    // Conecte o novo filho ao nó
    x->children[i + 1] = z;

    // Uma chave de y vai subir para o nó x para encontrar o novo filho
    for (int j = x->n - 1; j >= i; j--) {
        x->keys[j + 1] = x->keys[j];
    }

    // Copie a chave do meio de y para x
    x->keys[i] = y->keys[T - 1];

    // Incrementa a contagem de chaves em x
    x->n = x->n + 1;
}

// Função para inserir uma chave em árvore. Assume-se que a árvore não está cheia ao chamar esta função
void insertNonFull(BTreeNode* node, Key k) {
    // Inicializa o índice como o índice do elemento mais à direita
    int i = node->n - 1;

    // Se este é um nó folha
    if (node->leaf) {
        // O seguinte loop faz duas coisas:
        // a) Encontra a localização da nova chave a ser inserida
        // b) Move todas as chaves maiores para a direita
        while (i >= 0 && node->keys[i].rnn > k.rnn) {
            node->keys[i + 1] = node->keys[i];
            i--;
        }

        // Insere a nova chave no local encontrado
        node->keys[i + 1] = k;
        node->n = node->n + 1;
    } else { // Se este nó não é folha
        // Encontra o filho que terá a nova chave
        while (i >= 0 && node->keys[i].rnn > k.rnn) {
            i--;
        }

        // Verifica se o filho encontrado está cheio
        if (node->children[i + 1]->n == 2 * T - 1) {
            // Se o filho está cheio, então o divide
            splitChild(node, i + 1, node->children[i + 1]);

            // Após dividir, a chave do meio de children[i] sobe e children[i] é dividido em dois.
            // Verifica qual dos dois terá a nova chave
            if (node->keys[i + 1].rnn < k.rnn) {
                i++;
            }
        }
        insertNonFull(node->children[i + 1], k);
    }
}

// Função para inserir uma chave na árvore
void insert(BTree* tree, Key k) {
    // Se a árvore está vazia
    if (tree->root == NULL) {
        // Aloca memória para a raiz
        tree->root = createBTreeNode(true);
        tree->root->keys[0] = k;  // Inserir chave
        tree->root->n = 1;  // Atualizar número de chaves em root
    } else {  // Se a árvore não está vazia
        // Se a raiz está cheia, então a árvore cresce em altura
        if (tree->root->n == 2 * T - 1) {
            // Aloca memória para nova raiz
            BTreeNode* s = createBTreeNode(false);

            // Faz a antiga raiz virar filha da nova raiz
            s->children[0] = tree->root;

            // Divide a antiga raiz e move 1 chave para a nova raiz
            splitChild(s, 0, tree->root);

            // Nova raiz tem dois filhos agora. Decide qual dos dois filhos terá a nova chave
            int i = 0;
            if (s->keys[0].rnn < k.rnn) {
                i++;
            }
            insertNonFull(s->children[i], k);

            // Troca a raiz
            tree->root = s;
        } else {  // Se a raiz não está cheia, chama insertNonFull para a raiz
            insertNonFull(tree->root, k);
        }
    }
}
BTreeNode* search(BTreeNode* root, char* name) {
    // Base case: se o root é NULL, a chave não foi encontrada.
    if (root == NULL) {
        return NULL;
    }

    int i = 0;
    // Encontre a primeira chave maior ou igual a 'name'
    while (i < root->n && strcmp(name, root->keys[i].name) > 0) {
        i++;
    }

    // Se a chave encontrada é igual a 'name', retorna este nó
    if (i < root->n && strcmp(root->keys[i].name, name) == 0) {
        return root;
    }

    // Se este é um nó folha, a chave não está presente
    if (root->leaf) {
        return NULL;
    }

    // Vá para o filho apropriado
    return search(root->children[i], name);
}
// Função para percorrer todos os nós em uma subárvore enraizada com este nó
void traverse(BTreeNode* root) {
    int i;
    for (i = 0; i < root->n; i++) {
        // Se este não é um nó folha, antes de imprimir a chave[i],
        // percorra a subárvore enraizada com o filho children[i].
        if (!root->leaf) {
            traverse(root->children[i]);
        }
        printf(" %s", root->keys[i].name);
    }

    // Imprima as chaves do subárvore enraizada com o último filho
    if (!root->leaf) {
        traverse(root->children[i]);
    }
}


