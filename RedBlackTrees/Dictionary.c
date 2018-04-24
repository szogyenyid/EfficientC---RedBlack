#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> //tolower miatt
#define WORDL 32

typedef struct DictNode {
	char sajatNyelv[WORDL];
	char idegenNyelv1[WORDL];
	char idegenNyelv2[WORDL];
	char idegenNyelv3[WORDL];
	char color;
	struct DictNode *left, *right, *parent;
}DictNode;

DictNode* root = NULL;

int isEqual(char a[WORDL], char b[WORDL]) {
	for (int i = 0; i < WORDL; i++) {
		if (a[i] != b[i]) return 0; //ha bármelyik betû nem egyezik, akkor hamis
	}
	return 1; //különben igaz
}
int isGreater(char a[WORDL], char b[WORDL]) {
	for (int i = 0; i < WORDL; i++) {
		if (a[i] > b[i]) return 1;
		if (a[i] < b[i]) return 0;
	}
	return 0; //különben hamis
}
int isLess(char a[WORDL], char b[WORDL]) {
	for (int i = 0; i < WORDL; i++) {
		if (a[i] < b[i]) return 1;
		if (a[i] > b[i]) return 0;
	}
	return 0;
}
int isEmpty(char a[WORDL]) {
	for (int i = 0; i < WORDL; i++) {
		if (a[i] != 0) return 0;
	}
	return 1;
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
void insert(char newSajat[WORDL], char newIdegen[WORDL]) {
	struct DictNode *a = NULL; //x = a
	struct DictNode *b = NULL; //y = b
	struct DictNode *c = (DictNode*)malloc(sizeof(DictNode)); //z = c

	for (int i = 0; i < WORDL; i++) {
		c->sajatNyelv[i] = newSajat[i];
		c->idegenNyelv1[i] = newIdegen[i];
		c->idegenNyelv2[i] = 0;
		c->idegenNyelv3[i] = 0;
	}

	c->left = NULL;
	c->right = NULL;
	c->color = 'r';
	a = root;

	if (isinDict(newSajat) == 1) {
		printf("\nA beirt szo mar szerepel a szotarban!\n");
		if (addMeaning(newSajat, newIdegen)) printf("Uj jelentes hozzaadva.\n");
		else printf("Nem sikerult a jelentest hozzaadni. :(\n");
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

int isinDict(char word[WORDL]){
	DictNode* temp = root;

	while (temp != NULL) {
		if (isEqual(temp->sajatNyelv,word)) return 1;
		else {
				if (isGreater(word, temp->sajatNyelv)) temp = temp->right;
				else temp = temp->left;
		}
	}
	return 0;
}
void search(char word[WORDL]) {
	DictNode* temp = root;
	if (isinDict(word)==1) {
		while (!isEqual(temp->sajatNyelv,word)) {
			if (isGreater(word, temp->sajatNyelv)) temp = temp->right;
			if (isLess(word, temp->sajatNyelv)) temp = temp->left;
		}
		printf("%s: %s, %s, %s", temp->sajatNyelv, temp->idegenNyelv1, temp->idegenNyelv2, temp->idegenNyelv3);
	}
	else printf("A keresett szo nincs a szotarban.\n");
}
int addMeaning(char sajat[WORDL], char idegen[WORDL]) {
	DictNode* temp = root;

	if (isinDict(sajat)) {
		while (temp != NULL) {
			if (isEqual(temp->sajatNyelv, sajat)) break;
			else {
				if (isGreater(sajat, temp->sajatNyelv)) temp = temp->right;
				else temp = temp->left;
			}
		}
		if ((isEqual(temp->idegenNyelv1, idegen)) || (isEqual(temp->idegenNyelv2, idegen)) || (isEqual(temp->idegenNyelv3, idegen))) printf("A szonak mar bent van ez a jelentese.\n");
		else {
			//printf("addMeaning isEqual(1) returns: %d\n", isEqual(temp->idegenNyelv1, ""));
			if (isEmpty(temp->idegenNyelv1)) {
				for (int i = 0; i < WORDL; i++) {
					temp->idegenNyelv1[i] = idegen[i];
				}
				return 1;
			}
			else {
				//printf("addMeaning isEqual(2) returns: %d\n", isEqual(temp->idegenNyelv2, ""));
				if (isEmpty(temp->idegenNyelv2)) {
					for (int i = 0; i < WORDL; i++) {
						temp->idegenNyelv2[i] = idegen[i];
					}
					return 1;
				}
				else {
					//printf("addMeaning isEqual(3) returns: %d\n", isEqual(temp->idegenNyelv3, ""));
					if (isEmpty(temp->idegenNyelv3)) {
						for (int i = 0; i < WORDL; i++) {
							temp->idegenNyelv3[i] = idegen[i];
						}
						return 1;
					}
				}
			}
		}
	}
	else printf("A szo meg nincs bent a szotarban.");
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

void wordTest(char test1[WORDL], char test2[WORDL]) {
	char sajat[WORDL];
	char idegen[WORDL];
	for (int i = 0; i < WORDL; i++) {
		sajat[i] = test1[i];
		idegen[i] = test2[i];
	}
	printf("\n%d %d %d", isLess(sajat, idegen), isEqual(sajat, idegen), isGreater(sajat, idegen));
}
void toLower(char a[WORDL]) {
	for (int i = 0; i < WORDL; i++) {
		a[i] = tolower(a[i]);
	}
}


int main() {
	int choice, var, fl = 0;
	char sajat[WORDL]="", idegen[WORDL]="";
	while (1) {
		printf("\nSzotar program\nValasztasod:\n1:Beszuras\n2:Torles\n3:Kereses\n4:Bejaras\n5:Kilepes\n");
		scanf("%d", &choice);
		switch (choice) {
		case 1:
			printf("A szo amit hozza akarsz adni [magyar angol]: ");
			scanf("%s %s", &sajat, &idegen);
			toLower(sajat);
			toLower(idegen);
			insert(sajat, idegen);
			break;
		case 2:
			printf("Enter the int you wanna delete: ");
			scanf("%d", &var);
			//delete(var);
			break;
		case 3:
			printf("Enter word to search: \n");
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
