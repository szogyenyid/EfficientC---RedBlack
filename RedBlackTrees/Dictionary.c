#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> //tolower miatt

typedef struct DictNode {
	char sajatNyelv[32];
	char idegenNyelv[32];
	char color;
	struct DictNode *left, *right, *parent;
}DictNode;

DictNode root = NULL;

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

