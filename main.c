#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int validtrips1[280][3];
int validtrips2[280][3];
int nvalidtrips;

int validfirstlast2[21][2];

const int siztrip = 3 * sizeof(int);

int compare1(const void *t1, const void *t2) {
	int* a = (int*)t1;
	int* b = (int*)t2;
	return a[0] - b[0];
}

int compare2(const void *t1, const void *t2) {
	int* a = (int*)t1;
	int* b = (int*)t2;
	return a[1] - b[1];
}


void init(int n, int s) {
	int maxn = 2 * n;
	nvalidtrips = 0;

	for (int i = 1; i <= maxn; i++) {
		if (i > s) break;
		for (int j = i + 1; j <= maxn; j++) {
			int k = s - i - j;
			if (k <= j) break;
			if (k > maxn) continue;

			validtrips1[nvalidtrips][0] = i;
			validtrips1[nvalidtrips][1] = j;
			validtrips1[nvalidtrips][2] = k;
			nvalidtrips++;


			validtrips1[nvalidtrips][0] = i;
			validtrips1[nvalidtrips][1] = k;
			validtrips1[nvalidtrips][2] = j;
			nvalidtrips++;


			validtrips1[nvalidtrips][0] = j;
			validtrips1[nvalidtrips][1] = i;
			validtrips1[nvalidtrips][2] = k;
			nvalidtrips++;


			validtrips1[nvalidtrips][0] = j;
			validtrips1[nvalidtrips][1] = k;
			validtrips1[nvalidtrips][2] = i;
			nvalidtrips++;


			validtrips1[nvalidtrips][0] = k;
			validtrips1[nvalidtrips][1] = i;
			validtrips1[nvalidtrips][2] = j;
			nvalidtrips++;


			validtrips1[nvalidtrips][0] = k;
			validtrips1[nvalidtrips][1] = j;
			validtrips1[nvalidtrips][2] = i;
			nvalidtrips++;
		}
	}

	memcpy(validtrips2, validtrips1, nvalidtrips*siztrip);

	qsort(validtrips1, nvalidtrips, siztrip, compare1);
	qsort(validtrips2, nvalidtrips, siztrip, compare2);

	int lasv2 = validtrips2[0][1];

	validfirstlast2[lasv2][0] = 0;

	for (int i = 1; i < nvalidtrips; i++) {

		if (lasv2 != validtrips2[i][1]) {
			validfirstlast2[lasv2][1] = i;
			lasv2 = validtrips2[i][1];
			validfirstlast2[lasv2][0] = i;
		}


	}

	validfirstlast2[lasv2][1] = nvalidtrips;

}

char solutionsbuf[3000][50];
int isolbuf;

int n, s;

int ngon[5][3];

char *strdupli(char *str) {
	strcpy(solutionsbuf[isolbuf], str);
	isolbuf++;
	return solutionsbuf[isolbuf - 1];
}

struct tree {
	char *sol;
	struct tree *left;
	struct tree *right;
};

struct tree* insert(struct tree* root, char* sol) {
	if (!root) {
		root = (struct tree*)calloc(1, sizeof(struct tree));
		root->sol = strdupli(sol);
	}
	else {
		int cmp = strcmp(sol, root->sol);
		if (cmp < 0)
			root->left = insert(root->left, sol);
		else if (cmp > 0)
			root->right = insert(root->right, sol);
	}

	return root;
}

void printtree(struct tree *stree) {
	if (!stree) return;
	printtree(stree->left);
	puts(stree->sol);
	printtree(stree->right);
}

char solution[50];
int isol;

struct tree* sortedsolutions;
void savesolution() {
	isol = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < 3; j++) {
			int v = ngon[i][j];
			if (v > 9) {
				solution[isol++] = (v / 10) + '0';
				solution[isol++] = (v % 10) + '0';
			}
			else {
				solution[isol++] = v + '0';
			}
		}
	}

	solution[isol++] = 0;
	sortedsolutions = insert(sortedsolutions, solution);
}

void searchsolutions(int signature, int ingon) {
	if (ingon == n) {
		if (ngon[ingon - 1][2] == ngon[0][1]) {
			savesolution();
			return;
		}
	}

	int ifirst = validfirstlast2[ngon[ingon - 1][2]][0];
	int ilast = validfirstlast2[ngon[ingon - 1][2]][1];

	for (int i = ifirst; i < ilast; i++) {
		if (validtrips2[i][0] <= ngon[0][0]) continue;
		int mask = (1 << validtrips2[i][0]);
		if (n> ingon + 1)
			mask |= (1 << validtrips2[i][2]);

		if (signature&mask) continue;
		if (validtrips2[i][1] != ngon[ingon - 1][2]) continue;

		memcpy(ngon[ingon], validtrips2[i], siztrip);
		searchsolutions(signature | mask, ingon + 1);
	}
}

int main()
{
	n = 5;
	//minsum: 1 + 2 + 2*ngon sides: maxsum: 2*n + 2*n-1 + 2*n -2 =6n - 3 (gross limit)  
	for (s = 13; s < 25; s++) {
		init(n, s);

		for (int i = 0; i < nvalidtrips; i++) {
			memcpy(ngon[0], validtrips1[i], siztrip);

			int signature = (1 << ngon[0][0]);
			signature |= (1 << ngon[0][1]);
			signature |= (1 << ngon[0][2]);

			searchsolutions(signature, 1);
		}

	}

	printtree(sortedsolutions);

	return 0;
}
