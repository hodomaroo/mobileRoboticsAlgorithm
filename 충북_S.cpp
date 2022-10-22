#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

int link[12][4] = {
	 3,-1,-1, 1, //  0
	 4, 0,-1, 2, //  1
	 5, 1,-1,-1, //  2
	 
	 6,-1, 0, 4, //  3
	 7, 3, 1, 5, //  4
	 8, 4, 2,-1, //  5
	 
	 9,-1, 3, 7, //  6
	10, 6, 4, 8, //  7 
	11, 7, 5,-1, //  8
	
	-1,-1, 6,10, //  9
	-1, 9, 7,11, // 10
	-1,10, 8,-1  // 11
};

int goal[12] = {
	 1, 2, 2,
	 3,-1, 0,
	 2, 1, 2,
	 3, 3, 1
};

int map[12] = {
	 0, 0, 2,
	 3,-1, 3,
	 1, 1, 0,
	 1, 2, 2
};

int step[2][4] = {
	0,1,3,2, // 1, 4
	3,2,0,1  // 7, 10
};

int fixMap[12] = { 0 };
int robotPos = 0;

int RunStep(int mode);

int GetObstaclePos();
int GetPathTo(int* path, int* pathLen, int from, int to);
void GetPathToDfs(int* path, int* pathLen, int* st, int stLen, int from, int to);

int GetCanGoPlacePos(int* canGoPos, int* prevCanGoPos, int* count, int p);
int GetCanGoPuckPos(int* canGoPos, int* count, int p);

int IsMatch(int* rgbCount, int index);
int GetConditionResult(int mode);

void FixPos(int index);
void FixMap(int mode);

int Search(int mode, int* fromPath, int* toPath, int* pathLen, int robotPos);
int SearchDfs(int mode, int* fromPath, int* toPath, int maxDepth, int robotPos, int depth);

void ExecutePath(int* robotPos, int* fromPath, int* toPath, int pathLen);

void PrintMap();
void PrintGoal();

int main() {

	// �κ� ������ġ ����.
	robotPos = 0;

	int success[4] = { 0 };
	
	int obstaclePos = GetObstaclePos();
	FixPos(obstaclePos);
	
	PrintMap();
	PrintGoal();

	for(int mode = 0; mode < 4; mode++) {
		int q[3] = {1,0,2};
		for(int j = 0 ; j < 3 ; j++){
			int focus = step[obstaclePos/6][mode]*3 + q[j];
			RunStep(focus);
			
			PrintMap();
			PrintGoal();
		}
	}

	return 0;
}

int RunStep(int mode) {
	int fromPath[20], toPath[20], pathLen;
	if (Search(mode, fromPath, toPath, &pathLen, robotPos)) { // �� �˻� 
		ExecutePath(&robotPos, fromPath, toPath, pathLen); // �� ã������ ���� 
		FixMap(mode); // �� ���� 
		printf("%d step success=============================\n", mode);
		system("pause");

		return 1;
	}
	else {
		printf("%d step failed=============================\n", mode);
		system("pause");

		return 0;
	}
}

int GetObstaclePos() { // ��ֹ� ��ġ �ν� 
	for (int i = 0; i < 12; i++) {
		if (map[i] == -1) // map[i]�� -1�� i�� ��ֹ� ��ġ 
			return i;
	}

	return -1;
}

int GetPathTo(int* path, int* pathLen, int from, int to) {
	*pathLen = 999;
	int st[12] = { from };

	GetPathToDfs(path, pathLen, st, 1, from, to);

	return *pathLen != 999;
}

void GetPathToDfs(int* path, int* pathLen, int *st, int stLen, int from, int to) {
	if (stLen > 12)
		return;

	if (from == to) {
		if (stLen < *pathLen) {
			for (int i = 0; i < stLen; i++)
				path[i] = st[i];

			*pathLen = stLen;
		}
	}
	else {
		for (int i = 0; i < 4; i++) {
			int next = link[from][i];
			if (next >= 0 && (!map[next] || next == to)) {
				int b = map[next];
				map[next] = -2;
				st[stLen] = next;
				GetPathToDfs(path, pathLen, st, stLen + 1, next, to);
				map[next] = b;
			}
		}
	}
}

int GetCanGoPlacePos(int* canGoPos, int* prevCanGoPos, int* count, int p) { // �� �� �ִ� ������ �� ��ǥ���� �̵� ������ ���� �˻� 
	*count = 0;

	int q[12], prevQ[12], chk[12] = { 0 };
	int head = 0, tail = 0;

	q[tail++] = p;
	chk[p] = 1;

	while (head < tail) {
		int prevNode = prevQ[head];
		int node = q[head++];

		int flag = 0;
		for (int i = 0; i < 4; i++) {
			int next = link[node][i];
			if (next >= 0 && !chk[next] && !map[next])
			{
				prevQ[tail] = node;
				q[tail++] = next;

				chk[next] = 1;
				flag = 1;
			}
		}

		if ((!flag || map[p] == goal[node]) && node != p) {
			prevCanGoPos[*count] = prevNode;
			canGoPos[(*count)++] = node;
		}
	}

	return *count > 0;
}

int GetCanGoPuckPos(int* canGoPos, int* count, int p) { // ���� ���� ���� �� �ִ� �� ã�� 
	*count = 0;

	int q[12], chk[12] = { 0 };
	int head = 0, tail = 0;

	q[tail++] = p;
	chk[p] = 1;

	while (head < tail) {
		int node = q[head++];

		for (int i = 0; i < 4; i++) {
			int next = link[node][i];
			if (next >= 0 && !chk[next])
			{
				chk[next] = 1;
				if (!map[next])
					q[tail++] = next;
				else if (map[next] > 0)
					canGoPos[(*count)++] = next;
			}
		}
	}


	return *count > 0;
}

int IsMatch(int* rgbCount, int index) { // index ��ġ�� ���� ���������� Ȯ�� �� ���������� ���� ���� ���� 
	if (map[index] == goal[index]) {
		if (map[index] >= 0 && goal[index] > 0)
			rgbCount[map[index]]--;

		return 1;
	}	

	if (!map[index] && !rgbCount[goal[index]])
		return 1;

	return 0;
}

int GetConditionResult(int mode) { // �켱 ���� ������� ���������� Ȯ�� 
	int rgbCount[4] = { 0 };
	for (int p = 0; p < 12; p++) {
		// �������� ���� ��ġ�� ���� ������
		// �������� �������� ���� ������ ���� ���� �ִ� ��� 
		if (((!fixMap[p]) && map[p] >= 0) || (fixMap[p] && map[p] >= 0 && (!goal[p]))) 
			rgbCount[map[p]]++;
	}
	if(IsMatch(rgbCount, mode))	return 1;
/*
	switch (mode)
	{
		case 0:
			if (IsMatch(rgbCount, 0) && IsMatch(rgbCount, 1) && IsMatch(rgbCount, 2))
				return 1;
			break;
		case 1:
			if (IsMatch(rgbCount, 4) && IsMatch(rgbCount, 3) && IsMatch(rgbCount, 5))
				return 1;
			break;
		case 2:
			if (IsMatch(rgbCount, 7) && IsMatch(rgbCount, 6) && IsMatch(rgbCount, 8))
				return 1;
			break;
		case 3:
			if (IsMatch(rgbCount, 10) && IsMatch(rgbCount, 9) && IsMatch(rgbCount, 11))
				return 1;
			break;
	}*/

	return 0;
}

void FixPos(int index) { // ���� ������ ���� 
	if (map[index] == goal[index] || !goal[index]) // �� �������� ��� ������ �� �ֵ��� 
		fixMap[index] = 1;
}

void FixMap(int mode) { // �켱 ������� �� ���� ���� 
	FixPos(mode);
/*	switch (mode)
	{
		case 0:
			FixPos(0);
			FixPos(1);
			FixPos(2);
			break;
		case 1:
			FixPos(3);
			FixPos(4);
			FixPos(5);
			break;
		case 2:
			FixPos(6);
			FixPos(7);
			FixPos(8);
			break;
		case 3:
			FixPos(9);
			FixPos(10);
			FixPos(11);
			break;
	}*/
}

int Search(int mode, int* fromPath, int* toPath, int* pathLen, int robotPos) {
	*pathLen = 0;

	for (int i = 1; i < 20; i++) {
		*pathLen = SearchDfs(mode, fromPath, toPath, i, robotPos, 0);
		if (*pathLen >= 0)
			return 1;
	}

	return 0;
}

int SearchDfs(int mode, int* fromPath, int* toPath, int maxDepth, int robotPos, int depth) {
	if (depth == maxDepth)
		return -1;

	if (GetConditionResult(mode)) {
		return depth;
	}
	else {
		int canGoPuckPos[12] = { 0 }, canGoPuckPosCnt = 0;
		if (GetCanGoPuckPos(canGoPuckPos, &canGoPuckPosCnt, robotPos)) {
			for (int i = 0; i < canGoPuckPosCnt; i++) {
				int p = canGoPuckPos[i];

				if (fixMap[p] && goal[p])
					continue;

				int canGoPos[12] = { 0 }, prevCanGoPos[12] = { 0 }, canGoPosCnt = 0;
				if (GetCanGoPlacePos(canGoPos, prevCanGoPos, &canGoPosCnt, p)) {
					for (int j = 0; j < canGoPosCnt; j++) {
						int next = canGoPos[j];
						int prevNext = prevCanGoPos[j];
						fromPath[depth] = p;
						toPath[depth] = next;
						map[next] = map[p];
						map[p] = 0;

						int result = SearchDfs(mode, fromPath, toPath, maxDepth, prevNext, depth + 1);

						map[p] = map[next];
						map[next] = 0;

						if (result >= 0)
							return result;
					}
				}
			}
		}
	}
	return -1;
}

void ExecutePath(int* robotPos, int* fromPath, int* toPath, int pathLen) {
	int robotPath[12], robotPathLen;
	for (int i = 0; i < pathLen; i++) {
		if (!GetPathTo(robotPath, &robotPathLen, *robotPos, fromPath[i]))
			return;

		// robot ��ġ ����
		*robotPos = fromPath[i];

		map[toPath[i]] = map[fromPath[i]];
		map[fromPath[i]] = 0;

		printf("%2d -> %2d\n", fromPath[i], toPath[i]);

		if (!GetPathTo(robotPath, &robotPathLen, fromPath[i], toPath[i]))
			return;

		// robot ��ġ ����
		*robotPos = robotPath[robotPathLen - 2];
	}
}

void PrintMap() {
	printf("===============================================\n");
	printf("MAP-ROBOT: %d\n", robotPos);
	printf("%2d %2d %2d %2d\n",map[0],map[3],map[6],map[9]);
	printf("%2d %2d %2d %2d\n",map[1],map[4],map[7],map[10]);
	printf("%2d %2d %2d %2d\n",map[2],map[5],map[8],map[11]);
	printf("===============================================\n");
}

void PrintGoal() {
	printf("===============================================\n");
	printf("GOAL-ROBOT: %d\n", robotPos);
	printf("%2d %2d %2d %2d\n",goal[0],goal[3],goal[6],goal[9]);
	printf("%2d %2d %2d %2d\n",goal[1],goal[4],goal[7],goal[10]);
	printf("%2d %2d %2d %2d\n",goal[2],goal[5],goal[8],goal[11]);
	printf("===============================================\n");
}
