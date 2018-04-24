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
		if (a[i] > b[i]) return 1;
		if (a[i] < b[i]) return 0;
	}
	return 0; //különben hamis
}
int isLess(char a[32], char b[32]) {
	for (int i = 0; i < 32; i++) {
		if (a[i] < b[i]) return 1;
		if (a[i] > b[i]) return 0;
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
void insert(char newSajat[32], char newIdegen[32]) {
	struct DictNode *a = NULL; //x = a
	struct DictNode *b = NULL; //y = b
	struct DictNode *c = (DictNode*)malloc(sizeof(DictNode)); //z = c

	for (int i = 0; i < 32; i++) {
		c->sajatNyelv[i] = newSajat[i];
		c->idegenNyelv1[i] = newIdegen[i];
	}

	c->left = NULL;
	c->right = NULL;
	c->color = 'r';
	a = root;

	if (search(newSajat) == 1) {
		printf("\nA beirt szo mar szerepel a szotarban!\n");
		//PRINT CURRENT MEANING -----------------------------------------------------------------------------------TODO----------------------------------------
		//INSERT NEW MEANING --------------------------------------------------------------------------------------TODO----------------------------------------
		//EDIT CURRENT MEANING ------------------------------------------------------------------------------------TODO----------------------------------------
		return;
	}

	if (root == NULL) {
		root = c;
		root->color = 'b';
		c->parent = NULL;
		return;
	}

	while (a != NULL) {
		b = a;
		if (isLess(c->sajatNyelv, a->sajatNyelv)) a = a->left;
		else a = a->right;
	}

	c->parent = b;
	if (b == NULL) root = c;
	else {
		if (isLess(c->sajatNyelv, b->sajatNyelv)) b->left = c;
		else b->right = c;
	}
	colorInsert(c);
}

void inorderTree(DictNode* root) {
	DictNode* temp = root;

	if (temp != NULL) {
		inorderTree(temp->left);
		printf(" %s-%c ", temp->sajatNyelv, temp->color);
		inorderTree(temp->right);
	}
}
void postorderTree(DictNode* root) {
	DictNode* temp = root;

	if (temp != NULL) {
		postorderTree(temp->left);
		postorderTree(temp->right);
		printf(" %s-%c ", temp->sajatNyelv, temp->color);
	}
}

void traversal(DictNode* root) {
	if (root != NULL) {
		printf("A gyoker: %s-%c", root->sajatNyelv, root->color);
		printf("\nInorder bejaras:\n");
		inorderTree(root);
		printf("\nPostorder bejaras:\n");
		postorderTree(root);
	}
	else printf("A szotarad ures!");
}

int search(char word[32]){
	DictNode* temp = root;

	while (temp != NULL){
		if (isGreater(word, temp->sajatNyelv)) temp = temp->right;
		else if (isLess(word, temp->sajatNyelv)) temp = temp->left;
		else{
			//printf("Megvan!\n");
			return 1;
		}
	}
	printf("A keresett szo nem talalhato a szotarban.\n");
	return 0;
}
DictNode* minim(DictNode *a) {
	while (a->left) a = a->left;
	return a;
}
DictNode* successor(DictNode *a){ //leszarmazott
	DictNode *b;
	if (a->right) return minim(a->right);
	b = a->parent;
	while (b && a == b->right) {
		a = b;
		b = b->parent;
	}
	return b;
}

void wordTest(char test1[32], char test2[32]) {
	char sajat[32];
	char idegen[32];
	for (int i = 0; i < 32; i++) {
		sajat[i] = test1[i];
		idegen[i] = test2[i];
	}
	printf("\n%d %d %d", isLess(sajat, idegen), isEqual(sajat, idegen), isGreater(sajat, idegen));
}

int main() {
	int choice, var, fl = 0;
	char sajat[32], idegen[32];
	while (1) {
		printf("\nSzotar program\nValasztasod:\n1:Beszuras\n2:Torles\n3:Kereses\n4:Bejaras\n5:Kilepes\n");
		scanf("%d", &choice);
		switch (choice) {
		case 1:
			printf("A szo amit hozza akarsz adni [magyar angol]: ");
			scanf("%s %s", &sajat, &idegen);
			insert(tolower(sajat), tolower(idegen));
			break;
		case 2:
			printf("Enter the int you wanna delete: ");
			scanf("%d", &var);
			//delete(var);
			break;
		case 3:
			printf("Enter int to search: \n");
			scanf("%s", &sajat);
			search(sajat);
			break;

		case 4:
			traversal(root);
			break;

		case 5:
			fl = 1;
			break;

		default:
			printf("\nInvalid Choice\n");
		}

		if (fl == 1) break;
	}

	return 0;
}
