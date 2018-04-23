#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> //tolower miatt

typedef struct DictNode {
	char sajatNyelv[32];
	char idegenNyelv1[32];
	char idegenNyelv2[32];
	char idegenNyelv3[32];
	char color;
	struct DictNode *left, *right, *parent;
}DictNode;

DictNode* root = NULL;

int isEqual(char a[32], char b[32]) {
	for (int i = 0; i < 32; i++) {
		if (a[i] != b[i]) return 0; //ha bármelyik betû nem egyezik, akkor hamis
	}
	return 1; //különben igaz
}
int isGreater(char a[32], char b[32]) {
	for (int i = 0; i < 32; i++) {
		if (a[i] > b[i]) return 1; //ha bármelyik betû nagyobb (kihasználjuk hogy C-ben a char egy szám), akkor igaz
	}
	return 0; //különben hamis
}
int isLess(char a[32], char b[32]) {
	for (int i = 0; i < 32; i++) {
		if (a[i] < b[i]) return 1;
	}
	return 0;
}

void rotateLeft(DictNode* a) {
	DictNode* b;

	b = a->right;
	a->right = b->left;
	if (b->left != NULL) b->left->parent = a;
	b->parent = a->parent;

	if (a->parent == NULL) root = b;
	else if ((a->parent->left != NULL) && isEqual(a->sajatNyelv, a->parent->left->sajatNyelv)) a->parent->left = b; //itt vizsgáljuk hogy a szülõjének õ melyik oldali gyereke
	else a->parent->right = b;
	b->left = a;
	a->parent = b;
}
void rotateRight(DictNode* b) {
	DictNode* a;

	a = b->left;
	b->left = a->right;
	if (a->right != NULL) a->right->parent = b;
	a->parent = b->parent;

	if (b->parent == NULL) root = a;
	else if ((b->parent->left != NULL) && isEqual(b->sajatNyelv, b->parent->left->sajatNyelv)) b->parent->left = a;
	else b->parent->right = a;
	a->right = b;
	b->parent = a;
}

void colorInsert(DictNode* c) {
	DictNode *b = NULL;


	while ((c->parent != NULL) && (c->parent->color == 'r')) { //amíg van szülõ, és az piros
		if ((c->parent->parent->left != NULL) && isEqual(c->parent->sajatNyelv, c->parent->parent->left->sajatNyelv)) { //ha balgyerek
			if (c->parent->parent->right != NULL) b = c->parent->parent->right; 
			if ((b != NULL) && (b->color == 'r')) {
				c->parent->color = 'b';
				b->color = 'b';
				c->parent->parent->color = 'r';
				if (c->parent->parent != NULL) c = c->parent->parent;
			}
			else {
				if ((c->parent->right != NULL) && isEqual(c->sajatNyelv,c->parent->right->sajatNyelv)) {
					c = c->parent;
					rotateLeft(c);
				}
				c->parent->color = 'b';
				c->parent->parent->color = 'r';
				rotateRight(c->parent->parent);
			}
		}
		else {
			if (c->parent->parent->left != NULL) b = c->parent->parent->left;
			if ((b != NULL) && (b->color == 'r')) {
				c->parent->color = 'b';
				b->color = 'b';
				c->parent->parent->color = 'r';
				if (c->parent->parent != NULL) c = c->parent->parent;
			}
			else {
				if ((c->parent->left != NULL) && isEqual(c->sajatNyelv, c->parent->left->sajatNyelv)) {
					c = c->parent;
					rotateRight(c);
				}
				c->parent->color = 'b';
				c->parent->parent->color = 'r';
				rotateLeft(c->parent->parent);
			}
		}

	}
	root->color = 'b';
}
void insert(char val[32]) {
	struct DictNode *a = NULL; //x = a
	struct DictNode *b = NULL; //y = b
	struct DictNode *c = (DictNode*)malloc(sizeof(DictNode)); //z = c

	*c->sajatNyelv = val;   //IDE KELLETT BESZURNI EGY CSILLAGOT AZ EGESZ KIFEJEZES ELÉ
	c->left = NULL;
	c->right = NULL;
	c->color = 'r';
	a = root;

	if (search(val) == 1) {
		printf("\nWord is already in the tree\n");
		return;
	}

	if (root == NULL) {
		root = z;
		root->color = 'b';
		z->parent = NULL;
		return;
	}

	while (x != NULL) {
		y = x;
		if (z->key < x->key) x = x->left;
		else x = x->right;
	}

	z->parent = y;
	if (y == NULL) root = z;
	else {
		if (z->key < y->key) y->left = z;
		else y->right = z;
	}
	colorInsert(z);
}