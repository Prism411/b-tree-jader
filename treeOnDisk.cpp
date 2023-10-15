#define T 3  // Grau da �rvore B. Escolha um valor adequado.
#include "file.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


BTree* createBTree() {
    BTree* tree = (BTree*)malloc(sizeof(BTree));
    if (tree == NULL) {
        fprintf(stderr, "Erro de aloca��o de mem�ria ao criar uma nova �rvore B.\n");
        exit(EXIT_FAILURE);
    }

    // A �rvore � inicialmente vazia, ent�o a raiz � uma folha (n� folha)
    tree->root = createBTreeNode(true);
	
	printf("Arvore Criada com Sucesso!");
    return tree;
}

BTreeNode* createBTreeNode(bool leaf) {
    BTreeNode* newNode = (BTreeNode*)malloc(sizeof(BTreeNode));
    if (newNode == NULL) {
        fprintf(stderr, "Erro de aloca��o de mem�ria ao criar um novo n� da �rvore B.\n");
        exit(EXIT_FAILURE);
    }

    newNode->leaf = leaf;
    newNode->n = 0; // Inicializando com 0 chaves

    // Inicializando as chaves e os filhos
    for (int i = 0; i < 2 * T - 1; i++) {
        newNode->keys[i].rnn = 0; // ou outra valor que indique uma chave vazia ou n�o utilizada
        // Inicializar newNode->keys[i].name se necess�rio
    }
    for (int i = 0; i < 2 * T; i++) {
        newNode->children[i] = NULL;
    }

    return newNode; printf("Criado com Sucesso");
}

void freeBTreeNode(BTreeNode* node) {
    // Verifica��o de seguran�a; se o n� j� � NULL, n�o h� nada para liberar.
    if (node == NULL) {
        return;
    }

    // Se o n� n�o � uma folha, libere todos os seus filhos primeiro.
    if (!node->leaf) {
        for (int i = 0; i <= node->n; i++) {  // Note que usamos `<=` porque h� n+1 filhos
            freeBTreeNode(node->children[i]);
        }
    }

    // Agora que os filhos foram liberados, podemos liberar o pr�prio n�.
    free(node);
}

void freeBTree(BTree* tree) {
    if (tree != NULL) {
        // Comece liberando todos os n�s a partir da raiz.
        freeBTreeNode(tree->root);
        
        // Depois de todos os n�s serem liberados, libere a estrutura da �rvore.
        free(tree);
    }
}

// Fun��o para dividir o filho y do n� x. i � o �ndice de y em
// filho de x.  O filho deve estar cheio quando esta fun��o � chamada
void splitChild(BTreeNode* x, int i, BTreeNode* y) {
    // Crie um novo n� para armazenar (t-1) chaves de y
    BTreeNode* z = createBTreeNode(y->leaf);
    z->n = T - 1;

    // Copia as �ltimas (t-1) chaves de y para z
    for (int j = 0; j < T - 1; j++) {
        z->keys[j] = y->keys[j + T];
    }

    // Copia os �ltimos t filhos de y para z se y n�o for folha
    if (!y->leaf) {
        for (int j = 0; j < T; j++) {
            z->children[j] = y->children[j + T];
        }
    }

    y->n = T - 1; // Reduzir o n�mero de chaves em y

    // Como o n� x vai ter um novo filho,
    // criamos espa�o para o novo filho
    for (int j = x->n; j >= i + 1; j--) {
        x->children[j + 1] = x->children[j];
    }

    // Conecte o novo filho ao n�
    x->children[i + 1] = z;

    // Uma chave de y vai subir para o n� x para encontrar o novo filho
    for (int j = x->n - 1; j >= i; j--) {
        x->keys[j + 1] = x->keys[j];
    }

    // Copie a chave do meio de y para x
    x->keys[i] = y->keys[T - 1];

    // Incrementa a contagem de chaves em x
    x->n = x->n + 1;
}

// Fun��o para inserir uma chave em �rvore. Assume-se que a �rvore n�o est� cheia ao chamar esta fun��o
void insertNonFull(BTreeNode* node, Key k) {
    // Inicializa o �ndice como o �ndice do elemento mais � direita
    int i = node->n - 1;

    // Se este � um n� folha
    if (node->leaf) {
        // O seguinte loop faz duas coisas:
        // a) Encontra a localiza��o da nova chave a ser inserida
        // b) Move todas as chaves maiores para a direita
        while (i >= 0 && node->keys[i].rnn > k.rnn) {
            node->keys[i + 1] = node->keys[i];
            i--;
        }

        // Insere a nova chave no local encontrado
        node->keys[i + 1] = k;
        node->n = node->n + 1;
    } else { // Se este n� n�o � folha
        // Encontra o filho que ter� a nova chave
        while (i >= 0 && node->keys[i].rnn > k.rnn) {
            i--;
        }

        // Verifica se o filho encontrado est� cheio
        if (node->children[i + 1]->n == 2 * T - 1) {
            // Se o filho est� cheio, ent�o o divide
            splitChild(node, i + 1, node->children[i + 1]);

            // Ap�s dividir, a chave do meio de children[i] sobe e children[i] � dividido em dois.
            // Verifica qual dos dois ter� a nova chave
            if (node->keys[i + 1].rnn < k.rnn) {
                i++;
            }
        }
        insertNonFull(node->children[i + 1], k);
    }
}

// Fun��o para inserir uma chave na �rvore
void insert(BTree* tree, Key k) {
    // Se a �rvore est� vazia
    if (tree->root == NULL) {
        // Aloca mem�ria para a raiz
        tree->root = createBTreeNode(true);
        tree->root->keys[0] = k;  // Inserir chave
        tree->root->n = 1;  // Atualizar n�mero de chaves em root
    } else {  // Se a �rvore n�o est� vazia
        // Se a raiz est� cheia, ent�o a �rvore cresce em altura
        if (tree->root->n == 2 * T - 1) {
            // Aloca mem�ria para nova raiz
            BTreeNode* s = createBTreeNode(false);

            // Faz a antiga raiz virar filha da nova raiz
            s->children[0] = tree->root;

            // Divide a antiga raiz e move 1 chave para a nova raiz
            splitChild(s, 0, tree->root);

            // Nova raiz tem dois filhos agora. Decide qual dos dois filhos ter� a nova chave
            int i = 0;
            if (s->keys[0].rnn < k.rnn) {
                i++;
            }
            insertNonFull(s->children[i], k);

            // Troca a raiz
            tree->root = s;
        } else {  // Se a raiz n�o est� cheia, chama insertNonFull para a raiz
            insertNonFull(tree->root, k);
        }
    }
}
BTreeNode* search(BTreeNode* root, char* name) {
    // Base case: se o root � NULL, a chave n�o foi encontrada.
    if (root == NULL) {
        return NULL;
    }

    int i = 0;
    // Encontre a primeira chave maior ou igual a 'name'
    while (i < root->n && strcmp(name, root->keys[i].name) > 0) {
        i++;
    }

    // Se a chave encontrada � igual a 'name', retorna este n�
    if (i < root->n && strcmp(root->keys[i].name, name) == 0) {
        return root;
    }

    // Se este � um n� folha, a chave n�o est� presente
    if (root->leaf) {
        return NULL;
    }

    // V� para o filho apropriado
    return search(root->children[i], name);
}
// Fun��o para percorrer todos os n�s em uma sub�rvore enraizada com este n�
void traverse(BTreeNode* root) {
    int i;
    for (i = 0; i < root->n; i++) {
        // Se este n�o � um n� folha, antes de imprimir a chave[i],
        // percorra a sub�rvore enraizada com o filho children[i].
        if (!root->leaf) {
            traverse(root->children[i]);
        }
        printf(" %s", root->keys[i].name);
    }

    // Imprima as chaves do sub�rvore enraizada com o �ltimo filho
    if (!root->leaf) {
        traverse(root->children[i]);
    }
}


