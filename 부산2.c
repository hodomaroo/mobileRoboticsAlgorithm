#include <stdio.h>

int bogoal[12] = {
	2,0,1,3,
	0,0,0,0,
	1,3,0,2
};
int bomap[12] = {
	1,0,3,0,
	2,0,3,0,
	0,2,0,1
};
int bob[12] = {
	0,0,0,0,
	-1,0,-1,0,
	0,0,0,0
};

int bgoal[12] = { 0 };
int bmap[12] = { 0 };
int blink[12][4] = {
   -1, -1, 4, 1,
   -1, 0, 5, 2,
   -1, 1, 6, 3,
   -1, 2, 7, -1,
   0, -1, 8, 5,
   1, 4, 9, 6,
   2, 5, 10, 7,
   3, 6, 11, -1,
   4, -1, -1, 9,
   5, 8, -1, 10,
   6, 9, -1, 11,
   7, 10, -1, -1
};
int agoal[12] = { 0 };
int amap[12] = {
   1, 3,
   0, 2,
   2, 0,
   3, 1
};
int afix[12] = { 0 };
int aexit = 5;
int alink[8][4] = {
   -1, -1, 2, 1,
   -1, 0, 3, -1,
   0, -1, 4, 3,
   1, 2, 5, -1,
   2, -1, 6, 5,
   3, 4, 7, -1,
   4, -1, -1, 7,
   5, 6, -1, -1
};

int BGetCase();
void BSetGoal(int mapCase, int step);
void BSetGoalPart(int isTop, int isLeft, int cnt);
int BGetCanPlacePos(int p, int* result, int* count);
int BGetScore();
int BSearch(int mode, int* fromPath, int* toPath, int* pathLen);
//int BSearchDfs(int mode, int* fromPath, int* toPath, int maxDepth, int prevPos, int depth, int jmode);
int BSearchDfs(int mode, int* fromPath, int* toPath, int lp, int maxDepth, int prevPos, int depth);
void BAddPath(int* totalFromPath, int* totalToPath, int* totalPathLen, int* fromPath, int* toPath, int pathLen);
void BExecutePath(int* fromPath, int* toPath, int pathLen);
void BPrintMap();
void BPrintTotalPath(int* fromPath, int* toPath, int pathLen);

int ARunStep(int mode);
int AGetCanPlacePos(int p, int* result, int* count);
int AGetConditionResult(int mode);
int ASearch(int mode, int* fromPath, int* toPath, int* pathLen);
int ASearchDfs(int mode, int* fromPath, int* toPath, int matxDepth, int prevPos, int depth);
void AFixMap(int mode);
void AExecutePath(int* fromPath, int* toPath, int pathLen);
void APrintMap();

void PlaceAToC(int mode);

int main() {
	for (int i = 0; i < 12; i++)
		bmap[i] = bomap[i];

	int mapCase = BGetCase();
	int success = 0;
	int totFrom[30], totTo[30], totLen = 0;

	for (int step = 0; step < 3; step++) {
		BSetGoal(mapCase, step);

		int from[15], to[15], pl;
		if (success = BSearch(0, from, to, &pl)) {
			BAddPath(totFrom, totTo, &totLen, from, to, pl);
			BExecutePath(from, to, pl);
		}
	}

	if (success) {
		for (int i = 0; i < 12; i++)
			bmap[i] = bomap[i];

		BPrintTotalPath(totFrom, totTo, totLen);
		printf("=======================Success\n");
	}
	
	for(int i = 0; i < 12; i++){
		printf("%d ", bmap[i]);
		if(i % 4 == 3) printf("\n");
	};

	printf("=======================Complete B Sorting");
	system("pause");

	for (int i = 0; i < 3; i++)
		ARunStep(i);

	printf("=======================Complete A Sorting");
	system("pause");

	for (int i = 0; i < 12; i++)
		afix[i] = 0;

	amap[0] = -7;
	amap[6] = -8;

	ARunStep(3);
	PlaceAToC(0);
	APrintMap();
	BPrintMap();

	printf("=======================Complete Place A1 Puck To B2");
	system("pause");

	ARunStep(4);
	PlaceAToC(1);
	APrintMap();
	BPrintMap();

	printf("=======================Complete Place A2 Puck To B3");

	return 0;

}

/////////////////////////////////////////////////////////////// Solve C Functions

int BGetCase() {
	if (bob[4] == -1 && bob[5] == -1)
		return 0;
	else if (bob[4] == -1 && bob[6] == -1)
		return 1;
	else if (bob[4] == -1 && bob[7] == -1)
		return 2;
	else if (bob[5] == -1 && bob[7] == -1)
		return 3;
	else if (bob[6] == -1 && bob[7] == -1)
		return 4;

	return -1;
}

void BSetGoalPart(int isTop, int isLeft, int cnt) {
	int s = 0, e = 0, p = 0, c = 0;

	if (isTop) {
		if (isLeft) {
			s = 0;
			e = 3;
			p = 1;
		}
		else {
			s = 3;
			e = 0;
			p = -1;
		}
	}
	else {
		if (isLeft) {
			s = 8;
			e = 11;
			p = 1;
		}
		else {
			s = 11;
			e = 8;
			p = -1;
		}
	}

	if (s < e) {
		for (int i = s; i <= e; i += p) {
			if (bogoal[i])
				bgoal[s + c++ * p] = bogoal[i];

			if (c >= cnt)
				return;
		}
	}
	else {
		for (int i = s; i >= e; i += p) {
			if (bogoal[i])
				bgoal[s + c++ * p] = bogoal[i];

			if (c >= cnt)
				return;
		}
	}
}

void BSetGoal(int mapCase, int step) {
	if (step == 0) {
		for (int i = 0; i < 12; i++)
			bgoal[i] = -2;

		switch (mapCase)
		{
			case 0: // -1 -1 0 0
				BSetGoalPart(1, 1, 1);
				bgoal[4] = -1;
				BSetGoalPart(0, 1, 1);
				break;
			case 1: // -1 0 -1 0
				BSetGoalPart(1, 0, 1);
				BSetGoalPart(0, 0, 1);
				//if (bmap[6] != bgoal[3] && bmap[6] != bgoal[11])
				//	bgoal[6] = -1;
				break;
			case 2: // -1 0 0 -1
				BSetGoalPart(1, 0, 1);
				bgoal[7] = -1;
				BSetGoalPart(0, 0, 1);
				break;
			case 3: // 0 -1 0 -1
				BSetGoalPart(1, 1, 1);
				BSetGoalPart(0, 1, 1);
				//if (bmap[5] != bgoal[0] && bmap[5] != bgoal[8])
				//	bgoal[5] = -1;
				break;
			case 4: // 0 0 -1 -1
				BSetGoalPart(1, 0, 1);
				bgoal[7] = -1;
				BSetGoalPart(0, 0, 1);
				break;
		}
	}
	else if (step == 1) {
		int i = 0, c = 0;
		switch (mapCase)
		{
			case 0: // -1 -1 0 0
				BSetGoalPart(1, 1, 2);
				bgoal[5] = -1;
				BSetGoalPart(0, 1, 2);
				break;
			case 1: // -1 0 -1 0
				BSetGoalPart(1, 0, 3);
				bgoal[6] = -1;
				//BSetGoalPart(0, 0, 1);
				break;
			case 2: // -1 0 0 -1
				BSetGoalPart(1, 1, 1);

				bgoal[4] = -1;

				BSetGoalPart(0, 1, 1);
				break;
			case 3: // 0 -1 0 -1
				BSetGoalPart(1, 1, 3);
				bgoal[5] = -1;
				//BSetGoalPart(0, 1, 1);
				break;
			case 4: // 0 0 -1 -1
				BSetGoalPart(1, 0, 2);
				bgoal[6] = -1;
				BSetGoalPart(0, 0, 2);
				break;
		}
	}
	else {
		for (int i = 0; i < 12; i++) {
			if (i >= 4 && i <= 7)
				bgoal[i] = bob[i];
			else
				bgoal[i] = bogoal[i];
		}

	}
}

int BGetCanPlacePos(int p, int* result, int* count) {
	*count = 0;

	int q[12], chk[12] = { 0 };
	int head = 0, tail = 0;

	if (p >= 4 && p <= 7) {
		if (!bmap[blink[p][0]] && !bmap[blink[p][2]]) {
			q[tail++] = blink[p][0];
			chk[blink[p][0]] = 1;

			q[tail++] = blink[p][2];
			chk[blink[p][2]] = 1;
		}
	}
	else {
		q[tail++] = p;
		chk[p] = 1;
	}

	int exCount = tail;

	while (head < tail) {
		int node = q[head++];

		int flag = 0;
		for (int i = 0; i < 4; i++) {
			int next = blink[node][i];
			if (next >= 0 && !chk[next] && !bmap[next]) {
				q[tail++] = next;
				chk[next] = 1;
				if (next < 4 || next > 7)
					flag = 1;
			}
		}

		if (!exCount && (node < 4 || node > 7) /*&& (!flag || bmap[p] == bgoal[node])*/)
			result[(*count)++] = node;

		if (exCount)
			exCount--;

	}

	return *count > 0;
}

int BGetScore() {
	int p = 0;
	for (int i = 0; i < 12; i++) {
		if (bgoal[i] >= -1 && bmap[i] != bgoal[i])
			p++;
	}

	return p;
}

int BSearch(int mode, int* fromPath, int* toPath, int* pathLen) {
	*pathLen = 0;
/*
	for (int i = 0; i < 24; i++) {
		*pathLen = BSearchDfs(i%2, fromPath, toPath, -1, i/2, -1, 0);
		if (*pathLen >= 0)
			return 1;
	}*/

	for (int i = 0; i < 12; i++) {
		*pathLen = BSearchDfs(mode, fromPath, toPath, -1, i, -1, 0);
		if (*pathLen >= 0)
			return 1;
	}
	return 0;
}


int BSearchDfs(int mode, int* fromPath, int* toPath, int lp, int maxDepth, int prevPos, int depth) {
	if (!BGetScore()) {
		return depth;
	}
	else {
		if (depth == maxDepth)
			return -1;

		for (int i = 0; i < 12; i++) {
			if(i == lp)	continue;
			if (i == prevPos || bmap[i] <= 0)
				continue;
			
			int canPos[8] = { 0 }, canPosCnt = 0;
			if(BGetCanPlacePos(i, canPos, &canPosCnt)) {
				for (int l = 0; l < canPosCnt; l++) {
					int next = canPos[l];

					fromPath[depth] = i;
					toPath[depth] = next;
					bmap[next] = bmap[i];
					bmap[i] = bob[i];

					int result = BSearchDfs(mode, fromPath, toPath, next, maxDepth, next, depth + 1);

					bmap[i] = bmap[next];
					bmap[next] = bob[next];

					if (result >= 0)
						return result;
				}
			}
		}
	}

	return -1;
}
//---------------------------------------
/*
int BSearchDfs(int mode, int* fromPath, int* toPath, int lp, int maxDepth, int prevPos, int depth){
	if (!BGetScore()) {
		return depth;
	}
	else {
		if (depth == maxDepth)
			return -1;

		for (int i = 0; i < 12; i++) {
			if(i == lp)	continue;
			if(mode == 0){
				if (i == prevPos || bmap[i] <= 0)
					continue;
			}
			if(mode == 1){
				if (i == prevPos || bmap[i] <= -1)
					continue;
			}
			int canPos[8] = { 0 }, canPosCnt = 0;
			if (BGetCanPlacePos(i, canPos, &canPosCnt)) {
				for (int l = 0; l < canPosCnt; l++) {
					int next = canPos[l];

					fromPath[depth] = i;
					toPath[depth] = next;
					bmap[next] = bmap[i];
					bmap[i] = bob[i];

					int result = BSearchDfs(mode, fromPath, toPath, next, maxDepth, next, depth + 1);

					bmap[i] = bmap[next];
					bmap[next] = bob[next];

					if (result >= 0)
						return result;
				}
			}
		}
	}

	return -1;
}
*/

void BAddPath(int* totalFromPath, int* totalToPath, int* totalPathLen, int* fromPath, int* toPath, int pathLen) {
	for (int i = 0; i < pathLen; i++) {
		totalFromPath[*totalPathLen] = fromPath[i];
		totalToPath[*totalPathLen] = toPath[i];
		(*totalPathLen)++;
	}
}

void BExecutePath(int* fromPath, int* toPath, int pathLen) {
	for (int i = 0; i < pathLen; i++) {
		bmap[toPath[i]] = bmap[fromPath[i]];
		bmap[fromPath[i]] = bob[fromPath[i]];
	}
}

void BPrintMap() {
	printf("B MAP======================================\n");
	printf("%2d %2d %2d %2d\n", bmap[0], bmap[1], bmap[2], bmap[3]);
	printf("%2d %2d %2d %2d\n", bmap[4], bmap[5], bmap[6], bmap[7]);
	printf("%2d %2d %2d %2d\n", bmap[8], bmap[9], bmap[10], bmap[11]);
	printf("===========================================\n");
}

void BPrintTotalPath(int* fromPath, int* toPath, int pathLen) {
	for (int i = 0; i < pathLen; i++) {
		printf("%d -> %d\n", fromPath[i], toPath[i]);

		bmap[toPath[i]] = bmap[fromPath[i]];
		bmap[fromPath[i]] = bob[fromPath[i]];
	}
}

/////////////////////////////////////////////////////////////// Solve A Functions

int ARunStep(int mode) {
	int fromPath[10], toPath[10], pathLen;
	if (ASearch(mode, fromPath, toPath, &pathLen)) {
		AExecutePath(fromPath, toPath, pathLen);
		APrintMap();

		AFixMap(mode);
		printf("%d step success=============================\n", mode);
		//system("pause");

		return 1;
	}
	else
	{
		printf("%d step failed=============================\n", mode);
		//system("pause");

		return 0;
	}
}

int AGetCanPlacePos(int p, int* result, int* count) {
	int q[8], chk[8] = { 0 };
	int head = 0, tail = 0;

	q[tail++] = p;
	chk[p] = 1;

	while (head < tail) {
		int node = q[head++];

		int flag = 0;
		for (int i = 0; i < 4; i++) {
			int next = alink[node][i];
			if (next >= 0 && !chk[next] && !amap[next]) {
				q[tail++] = next;
				chk[next] = 1;
				result[(*count)++] = next;
			}
		}
	}

	return *count > 0;
}

int AGetConditionResult(int mode) {
	int noexit = aexit == 5 ? 3 : 5;

	switch (mode) {
	case 0:
		if ((amap[0] && amap[0] == amap[1])
			|| (amap[6] && amap[6] == amap[7]))
			return 1;
		break;

	case 1:
		if ((amap[0] && amap[0] == amap[1])
			&& (amap[6] && amap[6] == amap[7]))
			return 1;
		break;

	case 2:
		if ((amap[0] && amap[0] == amap[1])
			&& (amap[6] && amap[6] == amap[7])
			&& (amap[noexit] && amap[noexit - 1] == amap[noexit]))
			return 1;
		break;

	case 3:
		if (amap[aexit] == -7)
			return 1;
		break;

	case 4:
		if (amap[aexit] == -8)
			return 2;
		break;
	}

	return 0;
}

int ASearch(int mode, int* fromPath, int* toPath, int* pathLen) {
	*pathLen = 0;

	for (int i = 1; i < 10; i++) {
		*pathLen = ASearchDfs(mode, fromPath, toPath, i, -1, 0);
		if (*pathLen >= 0)
			return 1;
	}

	return 0;
}

int ASearchDfs(int mode, int* fromPath, int* toPath, int maxDepth, int prevPos, int depth) {
	if (AGetConditionResult(mode)) {
		return depth;
	}
	else {
		if (depth == maxDepth)
			return -1;

		for (int i = 0; i < 8; i++) {
			if (i == prevPos || !amap[i] || afix[i])
				continue;

			int canPos[8] = { 0 }, canPosCnt = 0;
			if (AGetCanPlacePos(i, canPos, &canPosCnt)) {
				for (int j = 0; j < canPosCnt; j++) {
					int next = canPos[j];
					fromPath[depth] = i;
					toPath[depth] = next;
					amap[next] = amap[i];
					amap[i] = 0;

					int result = ASearchDfs(mode, fromPath, toPath, maxDepth, next, depth + 1);

					amap[i] = amap[next];
					amap[next] = 0;

					if (result >= 0)
						return result;
				}
			}
		}
	}

	return -1;
}

void AFixMap(int mode) {
	switch (mode)
	{
	case 0:
		if (amap[0] && amap[0] == amap[1])
			afix[0] = afix[1] = 1;
		else if (amap[6] && amap[6] == amap[7])
			afix[6] = afix[7] = 1;
		break;
	case 1:
		afix[0] = afix[1] = afix[6] = afix[7] = 1;
		break;
	}
}

void AExecutePath(int* fromPath, int* toPath, int pathLen) {
	for (int i = 0; i < pathLen; i++) {
		amap[toPath[i]] = amap[fromPath[i]];
		amap[fromPath[i]] = 0;
		printf("%2d -> %2d\n", fromPath[i], toPath[i]);
	}
}

void APrintMap() {
	printf("A MAP======================================\n");
	printf("%2d %2d\n", amap[0], amap[1]);
	printf("%2d %2d\n", amap[2], amap[3]);
	printf("%2d %2d\n", amap[4], amap[5]);
	printf("%2d %2d\n", amap[6], amap[7]);
	printf("===========================================\n");
}

void PlaceAToC(int mode) {
	if (amap[aexit]) {
		for (int i = 0; i < 4; i++) {
			if (!bmap[mode * 8 + i]) {
				bmap[mode * 8 + i] = amap[aexit];
				amap[aexit] = 0;
			}
		}
	}
}
