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
void copyNode(DictNode* dest, DictNode* src) {
	for (int i = 0; i < WORDL; i++) {
		dest->sajatNyelv[i] = src->sajatNyelv[i];
		dest->idegenNyelv1[i] = src->idegenNyelv1[i];
		dest->idegenNyelv2[i] = src->idegenNyelv2[i];
		dest->idegenNyelv3[i] = src->idegenNyelv3[i];
	}
	dest->color = src->color;
}

void rotateLeft(DictNode* a) {
	DictNode* b;

	b = a->right;
	a->right = b->left;
	if (b->left) b->left->parent = a;
	b->parent = a->parent;

	if (!(a->parent)) root = b;
	else if ((a->parent->left) && isEqual(a->sajatNyelv, a->parent->left->sajatNyelv)) a->parent->left = b; //itt vizsgáljuk hogy a szülõjének õ melyik oldali gyereke
	else a->parent->right = b;
	b->left = a;
	a->parent = b;
}
void rotateRight(DictNode* b) {
	DictNode* a;

	a = b->left;
	b->left = a->right;
	if (a->right) a->right->parent = b;
	a->parent = b->parent;

	if (!(b->parent)) root = a;
	else if ((b->parent->left) && isEqual(b->sajatNyelv, b->parent->left->sajatNyelv)) b->parent->left = a;
	else b->parent->right = a;
	a->right = b;
	b->parent = a;
}

void colorInsert(DictNode* c) {
	DictNode *b = NULL;


	while ((c->parent) && (c->parent->color == 'r')) { //amíg van szülõ, és az piros
		if ((c->parent->parent->left) && isEqual(c->parent->sajatNyelv, c->parent->parent->left->sajatNyelv)) { //ha balgyerek
			if (c->parent->parent->right) b = c->parent->parent->right; 
			if ((b) && (b->color == 'r')) {
				c->parent->color = 'b';
				b->color = 'b';
				c->parent->parent->color = 'r';
				if (c->parent->parent) c = c->parent->parent;
			}
			else {
				if ((c->parent->right) && isEqual(c->sajatNyelv,c->parent->right->sajatNyelv)) {
					c = c->parent;
					rotateLeft(c);
				}
				c->parent->color = 'b';
				c->parent->parent->color = 'r';
				rotateRight(c->parent->parent);
			}
		}
		else {
			if (c->parent->parent->left) b = c->parent->parent->left;
			if ((b) && (b->color == 'r')) {
				c->parent->color = 'b';
				b->color = 'b';
				c->parent->parent->color = 'r';
				if (c->parent->parent) c = c->parent->parent;
			}
			else {
				if ((c->parent->left) && isEqual(c->sajatNyelv, c->parent->left->sajatNyelv)) {
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
	DictNode *a = NULL;
	DictNode *b = NULL;
	DictNode *c = (DictNode*)malloc(sizeof(DictNode));

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
		return;
	}

	if (!root) {
		root = c;
		root->color = 'b';
		c->parent = NULL;
		return;
	}

	while (a) {
		b = a;
		if (isLess(c->sajatNyelv, a->sajatNyelv)) a = a->left;
		else a = a->right;
	}

	c->parent = b;
	if (!b) root = c;
	else {
		if (isLess(c->sajatNyelv, b->sajatNyelv)) b->left = c;
		else b->right = c;
	}
	printf("Szo hozzaadva.\n\n");
	colorInsert(c);
}

void inorderTree(DictNode* root) {
	DictNode* temp = root;

	if (temp) {
		inorderTree(temp->left);
		printf(" %s-%c ", temp->sajatNyelv, temp->color);
		inorderTree(temp->right);
	}
}
void postorderTree(DictNode* root) {
	DictNode* temp = root;

	if (temp) {
		postorderTree(temp->left);
		postorderTree(temp->right);
		printf(" %s-%c ", temp->sajatNyelv, temp->color);
	}
}

void traversal(DictNode* root) {
	if (root) {
		printf("A gyoker: %s-%c", root->sajatNyelv, root->color);
		printf("\nInorder bejaras:\n");
		inorderTree(root);
		printf("\nPostorder bejaras:\n");
		postorderTree(root);
		printf("\n");
	}
	else printf("\nA szotarad ures!\n");
}

int isinDict(char word[WORDL]){
	DictNode* temp = root;

	while (temp) {
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
		printf("%s: %s", temp->sajatNyelv, temp->idegenNyelv1);
		if (!isEmpty(temp->idegenNyelv2)) printf(", %s", temp->idegenNyelv2);
		if (!isEmpty(temp->idegenNyelv3)) printf(", %s", temp->idegenNyelv3);
	}
	else printf("A keresett szo nincs a szotarban.\n");
}
int addMeaning(char sajat[WORDL], char idegen[WORDL]) {
	DictNode* temp = root;

	if (isinDict(sajat)) {
		while (temp) {
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
		if ((a->parent->left) && (a == a->parent->left)) {
			d = a->parent->right;
			if ((d) && (d->color == 'r')) {
				d->color = 'b';
				a->parent->color = 'r';
				rotateLeft(a->parent);
				d = a->parent->right;
			}

			if ((d) && (d->right) && (d->left) && (d->left->color == 'b') && (d->right->color == 'b')) {
				d->color = 'r';
				a = a->parent;
			}
			else if ((d) && (d->right->color == 'b')) {
				d->left->color = 'b';
				d->color = 'r';
				rotateRight(d);
				d = a->parent->right;
			}

			if (d) {
				d->color = a->parent->color;
				a->parent->color = 'b';
				d->right->color = 'b';
				rotateLeft(a->parent);
				a = root;
			}
		}

		else if (a->parent) {
			d = a->parent->left;
			if ((d) && (d->color == 'r')) {
				d->color = 'b';
				a->parent->color = 'r';
				rotateLeft(a->parent);
				if (a->parent) d = a->parent->left;

			}

			if ((d) && (d->right) && (d->left) && (d->right->color == 'b') && (d->left->color == 'b')) a = a->parent;
			else if ((d) && (d->right) && (d->left) && (d->left->color == 'b')) {
				d->right->color = 'b';
				d->color = 'r';
				rotateRight(d);
				d = a->parent->left;
			}

			if (a->parent) {
				d->color = a->parent->color;
				a->parent->color = 'b';
			}

			if (d->left) d->left->color = 'b';
			if (a->parent) rotateLeft(a->parent);
			a = root;
		}
	}
	a->color = 'b';
}
DictNode* delete(char word[WORDL]) {
	DictNode* a = NULL;
	DictNode* b = NULL;
	DictNode* c = root;

	if ((!(c->left)) && !(c->right) && (isEqual(c->sajatNyelv, word))) {
		root = NULL;
		free(c);
		printf("A szotar kiurult.\n");
		return;
	}

	while ((!isEqual(c->sajatNyelv, word)) && (c)) {
		if (isLess(word, c->sajatNyelv)) c = c->left;
		else c = c->right;
		if (!c) return;
	}

	if (!(c->left) || !(c->right)) b = c;
	else b = successor(c);
	if (b->left) a = b->left;
	else if (b->right) a = b->right;
	if ((a) && (b->parent)) a->parent = b->parent;
	if ((b) && (a) && !(b->parent)) root = a;
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
	if ((b) && (a) && (b->color == 'b')) colorDelete(a);
	return b;
}

void modifyWord(char word[WORDL]) {
	search(word);
	if (isinDict(word)) {
		printf("\n1: Uj jelentes hozzaadasa\n2: Jelentes modositasa\n3: Jelentes torlese\n4: Szo torlese\n5: Megse\n");
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
			printf("Jelentes modositva.\n\n");
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
			printf("Szo torolve.\n");
			break;
		case 5:
			break;
		default:
			printf("Nem tudom hogy mire gondolsz :(\n");
		}
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

void preorderSave(DictNode* root, FILE* fp) {
	if (!root) return;
	fwrite(root, sizeof(DictNode), 1, fp);
	if (root->left) preorderSave(root->left, fp);
	if (root->right) preorderSave(root->right, fp);
}
DictNode* preorderLoad(DictNode** root, FILE* fp) { // root = preorderLoad(root, fp);
	if (!root) { //ha semmi nem fogja a gyökeret  === (root == NULL)
		printf("KURVA NAGY HIBA TORTENIK EPPEN!");
		return NULL;
	}
	DictNode* temp = malloc(sizeof(DictNode));
	fread(temp, sizeof(DictNode), 1, fp); 
	if (!(temp->color == 'b' || temp->color == 'r')) {
		printf("\nHiba a szotar betoltese kozben!\n");
		return NULL;
	}
	if (temp->left != NULL) {
		temp->left = preorderLoad(temp->left, fp);
		temp->left->parent = temp;
	}
	if (temp->right != NULL) {
		temp->right = preorderLoad(temp->right, fp);
		temp->right->parent = temp;
	}
	return temp;
}

void printToTxt(DictNode* root, FILE* fp) {
	DictNode* temp = root;
	if (temp) {
		printToTxt(temp->left, fp);
		fprintf(fp, "%s: %s", temp->sajatNyelv, temp->idegenNyelv1);
		if (!isEmpty(temp->idegenNyelv2)) fprintf(fp, ", %s", temp->idegenNyelv2);
		if (!isEmpty(temp->idegenNyelv3)) fprintf(fp, ", %s", temp->idegenNyelv3);
		fprintf(fp, "\n");
		printToTxt(temp->right, fp);
	}
}

void treeTest() {
	insert("kaki", "poop");
	insert("kutya", "dog");
	insert("ablak", "window");
	insert("ajto", "door");
	insert("narancs", "orange");
	insert("lo", "horse");
	insert("level", "letter");
	insert("lyuk", "hole");
	traversal(root);
	FILE* fp = fopen("wordtree.dict", "wb");
	preorderSave(root, fp);
	fclose(fp);
	delete("lyuk");
	delete("kutya");
	delete("ablak");
	delete("ajto");
	delete("level");
	delete("lo");
	delete("narancs");
	delete("kaki");
	traversal(root);
	fp = fopen("wordtree.dict", "rb");
	root = preorderLoad(&root, fp);
	traversal(root);
	fclose(fp);
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
	int choice, running = 1;
	char sajat[WORDL] = "" , idegen[WORDL] = "";
	FILE* fp = fopen("wordtree.dict", "rb");
	if (fp) {
		fseek(fp, 0, SEEK_END);
		int size = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		if (size > 0) root = preorderLoad(&root, fp);
		fclose(fp);
	}
	while (running) {
		printf("  1: Beszuras   2: Kereses   3: Modositas   4: Kiiras   5: Kilepes es mentes\n\t");
		scanf("%d", &choice);
		switch (choice) {
		case 1:
			printf("\nA szo amit hozza akarsz adni [magyar angol]: ");
			scanf("%s %s", &sajat, &idegen);
			toLower(sajat);
			toLower(idegen);
			insert(sajat, idegen);
			break;
		case 2:
			printf("\nKeresendo szo [magyar]: ");
			scanf("%s", &sajat);
			toLower(sajat);
			search(sajat);
			printf("\n");
			break;
		case 3:
			printf("\nModositando szo [magyar]: ");
			scanf("%s", &sajat);
			toLower(sajat);
			modifyWord(sajat);
			break;
		case 4:
			fp = fopen("szotar.txt", "wt");
			if(fp) printToTxt(root, fp);
			fclose(fp);
			printf("Szotar sikeresen kiirva .txt fajlba.\n\n");
			break;
		case 5:
			running = 0;
			break;
		case 6:
			traversal(root);
			break;
		default:
			printf("Nem tudom hogy mire gondolsz :/\n");
			break;
		}
	}
	fp = fopen("wordtree.dict", "wb");
	preorderSave(root, fp);
	fclose(fp);
	return 0;
}
