#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> //tolower miatt
#define WORDL 32

/*TODO:
- Save and restore tree to/from file
*/

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

void colorDelete(DictNode *a) {
	while (a != root && a->color == 'b') {
		DictNode *d = NULL;
		if ((a->parent->left != NULL) && (a == a->parent->left)) {
			d = a->parent->right;
			if ((d != NULL) && (d->color == 'r')) {
				d->color = 'b';
				a->parent->color = 'r';
				rotateLeft(a->parent);
				d = a->parent->right;
			}

			if ((d != NULL) && (d->right != NULL) && (d->left != NULL) && (d->left->color == 'b') && (d->right->color == 'b')) {
				d->color = 'r';
				a = a->parent;
			}
			else if ((d != NULL) && (d->right->color == 'b')) {
				d->left->color = 'b';
				d->color = 'r';
				rotateRight(d);
				d = a->parent->right;
			}

			if (d != NULL) {
				d->color = a->parent->color;
				a->parent->color = 'b';
				d->right->color = 'b';
				rotateLeft(a->parent);
				a = root;
			}
		}

		else if (a->parent != NULL) {
			d = a->parent->left;
			if ((d != NULL) && (d->color == 'r')) {
				d->color = 'b';
				a->parent->color = 'r';
				rotateLeft(a->parent);
				if (a->parent != NULL) d = a->parent->left;

			}

			if ((d != NULL) && (d->right != NULL) && (d->left != NULL) && (d->right->color == 'b') && (d->left->color == 'b')) a = a->parent;
			else if ((d != NULL) && (d->right != NULL) && (d->left != NULL) && (d->left->color == 'b')) {
				d->right->color = 'b';
				d->color = 'r';
				rotateRight(d);
				d = a->parent->left;
			}

			if (a->parent != NULL) {
				d->color = a->parent->color;
				a->parent->color = 'b';
			}

			if (d->left != NULL) d->left->color = 'b';
			if (a->parent != NULL) rotateLeft(a->parent);
			a = root;
		}
	}
	a->color = 'b';
}
DictNode* delete(char word[WORDL]) {
	DictNode* a = NULL;
	DictNode* b = NULL;
	DictNode* c = root;

	if ((c->left == NULL) && (c->right == NULL) && (isEqual(c->sajatNyelv, word))) {
		root = NULL;
		printf("A szotar kiurult.\n");
		return;
	}

	while ((!isEqual(c->sajatNyelv, word)) && (c != NULL)) {
		if (isLess(word, c->sajatNyelv)) c = c->left;
		else c = c->right;
		if (c == NULL) return;
	}

	if ((c->left == NULL) || (c->right == NULL)) b = c;
	else b = successor(c);
	if (b->left != NULL) a = b->left;
	else if (b->right != NULL) a = b->right;
	if ((a != NULL) && (b->parent != NULL)) a->parent = b->parent;
	if ((b != NULL) && (a != NULL) && (b->parent == NULL)) root = a;
	else if (b == b->parent->left) b->parent->left = a;
	else b->parent->right = a;
	if (b != c) {
		for (int i = 0; i < WORDL; i++) {
			c->sajatNyelv[i] = b->sajatNyelv[i];
			c->idegenNyelv1[i] = b->idegenNyelv1[i];
			c->idegenNyelv2[i] = b->idegenNyelv2[i];
			c->idegenNyelv3[i] = b->idegenNyelv3[i];
		}
	}
	if ((b != NULL) && (a != NULL) && (b->color == 'b')) colorDelete(a);
	return b;
}

void modifyWord(char word[WORDL]) {
	search(word);
	printf("\n1: Uj jelentes hozzaadasa\n2: Jelentes modositasa\n3: Jelentes torlese\n4: Szo torlese\n");
	int choice;
	int num;
	char newm[WORDL];
	scanf("%d", &choice);
	switch (choice) {
	case 1:
		printf("Ird ide az uj jelentest: ");
		scanf("%s", &newm);
		addMeaning(word, newm);
		break;
	case 2:
		printf("Melyik jelentest szeretned modositani? [1-2-3]\n");
		scanf("%d", &num);
		if (num > 3) {
			printf("Sajnos haromnal tobb jelentest nem tudok tarolni :(\n");
			break;
		}
		printf("Mi legyen az uj jelentes?\n");
		scanf("%s", &newm);
		editMeaning(num, word, newm);
		break;
	case 3:
		printf("Melyik jelentest szeretned torolni? [1-2-3]\n");
		scanf("%d", &num);
		if (num > 3) {
			printf("Sajnos haromnal tobb jelentest nem tudok tarolni :(\n");
			break;
		}
		editMeaning(num, word, "");
		break;
	case 4:
		delete(word);
		printf("Szo torolve.");
		break;
	default:
		printf("Nem tudom hogy mire gondolsz :(");
	}
}
int editMeaning(int nth, char word[WORDL], char newm[WORDL]) {
	DictNode* temp = root;
	if (isinDict(word)) {
		while (!isEqual(temp->sajatNyelv, word)) {
			if (isLess(word, temp->sajatNyelv)) temp = temp->left;
			else temp = temp->right;
		}
		for (int i = 0; i < WORDL; i++) {
			if (nth == 1) temp->idegenNyelv1[i] = newm[i];
			if (nth == 2) temp->idegenNyelv2[i] = newm[i];
			if (nth == 3) temp->idegenNyelv3[i] = newm[i];
		}
	}
	else printf("A szo nem szerepel a szotarban.");
	if ((isEmpty(temp->idegenNyelv1) && (isEmpty(temp->idegenNyelv2)) && (isEmpty(temp->idegenNyelv3)))) {
		delete(word);
		printf("A szo osszes jelentese elfogyott, igy toroltem.");
	}
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

void saveDict(DictNode* root, FILE* fp) {
	if (root != NULL) {
		fwrite(root, sizeof(DictNode), 1, fp);
		for (int i = 0; i < 2; i++) {
			if (i == 0) saveDict(root->left, fp);
			else saveDict(root->right, fp);
		}
	}
}
void loadDict(DictNode** root, FILE* fp) {
	*root = malloc(sizeof(DictNode));
	fread(*root, sizeof(DictNode), 1, fp);
	for (int i = 0; i < 2; i++) {
		if ((*root)->left) {
			if(i==0) loadDict((*root)->left, fp);
			else loadDict((*root)->right, fp);
		}
	}
}



int main() {
	int choice, var, fl = 1;
	char sajat[WORDL]="", idegen[WORDL]="";
	while (0) {
		printf("\nSzotar program\nValasztasod:\n1:Beszuras\n2:Kereses\n3:Bejaras\n4:Modositas\n5:Kilepes\n");
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
			printf("Enter word to search: \n");
			scanf("%s", &sajat);
			search(sajat);
			break;

		case 3:
			traversal(root);
			break;

		case 4:
			printf("Enter word to modify: ");
			scanf("%s", &sajat);
			modifyWord(sajat);
			break;

		case 5:
			fl = 0;
			break;
		default:
			printf("\nInvalid Choice\n");
		}
	}

	insert("kaki", "poop");
	insert("punci", "pussy");
	insert("pina", "cunt");
	insert("ablak", "window");
	insert("ajto", "door");
	insert("segg", "ass");
	insert("lo", "horse");
	insert("ondo", "semen");
	insert("lyuk", "hole");
	traversal(root);
	FILE* fp = fopen("wordtree.dict", "wb");
	saveDict(root, fp);
	fclose(fp);
	delete("lyuk");
	delete("punci");
	delete("pina");
	delete("ablak");
	delete("ajto");
	delete("segg");
	delete("lo");
	delete("ondo");
	delete("kaki");
	traversal(root);
	fp = fopen("wordtree.dict", "rb");
	loadDict(root, fp);
	traversal(root);
	fclose(fp);
	return 0;
}
