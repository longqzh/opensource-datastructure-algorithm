/*
 
 * @file: [H2008] �������
 
 * @brief: ���� ���
 
 * @copyright: All rights reserved (c) 2020 Samsung Electronics, Inc.
 
 */
 
#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3
 
extern bool swap(int dir);
 
int dy[4] = { -1, 0, 1, 0 };
int dx[4] = { 0, 1, 0, -1 };
 
/**
* board: ���� ����
* pattern: ���� ����
* Y0: ���� ��ĭ ��ġ (Y��ǥ)
* X0: ���� ��ĭ ��ġ (X��ǥ)
*/
int board[5][5], pattern[3][3], Y0, X0;
 
 
void mySwap(int dir) {
    board[Y0][X0] = board[Y0 + dy[dir]][X0 + dx[dir]];
    board[Y0 + dy[dir]][X0 + dx[dir]] = 0;
 
    Y0 += dy[dir]; X0 += dx[dir];
    swap(dir);
}
/**
* fixed: ���� ���� ��ġ�� 1 ������ -1
* Visited: 
* marked: 
* 
*/
int fixed[5][5], Visited[5][5];
bool marked[5][5];
 
struct Point 
{ 
    int y, x; 
};
Point queue[555]; int front, end;
int Movement[555], moveCnt;
 
void trace(int ys, int xs, int ye, int xe, int mark) {
    int ws = moveCnt;
    while (!(ys == ye && xs == xe)) {
        marked[ye][xe] = mark;
        int dir = Visited[ye][xe];
        Movement[moveCnt++] = dir;
        ye -= dy[dir]; xe -= dx[dir];
    }
     
    // �� �Ʒ��� �׳� reverse ���ִ� ����. �ٸ��� ó���ص� �ɵ�..
    int we = moveCnt;
    for (int i = 0; i < (we - ws) / 2; i++) {
        int t = Movement[ws + i];
        Movement[ws + i] = Movement[we - i - 1];
        Movement[we - i - 1] = t;
    }
}
 
void move(int ys, int xs, int ye, int xe) {
    front = end = 0;
    queue[0].y = ys;
    queue[0].x = xs;
     
    //[ys][xs]�� ���� ��� ��ġ���� �̵� ������ Visited[][] �迭�� ����.
    while (front <= end) {
        Point p = queue[front++];
        for (int i = 0; i < 4; i++) {
            int ny = p.y + dy[i];
            int nx = p.x + dx[i];
             
            // �� ������ �������� üũ
            if (!(0 <= ny && ny < 5 && 0 <= nx && nx < 5)) continue;
             
            // �̹� ����ó���� ��ġ(fixed)�ų� �湮�� ��ġ���� üũ
            if (fixed[ny][nx] || Visited[ny][nx] != -1) continue;
 
            ++end;
            queue[end].y = ny;
            queue[end].x = nx;
 
            Visited[ny][nx] = i;
            if (ny == ye && nx == xe)break;
        }
    }
    trace(ys, xs, ye, xe, 0);
}
 
void calc(int y, int x, int c) {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++)
            Visited[i][j] = -1;
    }
 
    //////////////////////////////////////////////////////////////////////////////////////////
    // ���� ���ϴ� ��ġ�� ��ĭ�� �̵� ��Ŵ
    Visited[Y0][X0] = 0;
    moveCnt = 0;
    move(Y0, X0, y, x); // �̵� ��� ã��
    for (int i = 0; i < moveCnt; i++) 
        mySwap(Movement[i]);
    //////////////////////////////////////////////////////////////////////////////////////////
 
 
    //////////////////////////////////////////////////////////////////////////////////////////
    // ���� ����� target ��ĭ ã��
    int Yt = -1, Xt = -1;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            Visited[i][j] = -1;
            marked[i][j] = 0;
        }
    }
    Visited[Y0][X0] = 0;
 
    front = end = 0;
    queue[0].y = Y0;
    queue[0].x = X0;
     
    while (front <= end && Yt == -1 && Xt == -1) {
        Point p = queue[front++];
        int y = p.y, x = p.x;
        for (int i = 0; i < 4; i++) {
            int ny = y + dy[i];
            int nx = x + dx[i];
            if (!(0 <= ny && ny < 5 && 0 <= nx && nx < 5)) continue;
            if (fixed[ny][nx] || Visited[ny][nx] >= 0) continue;
             
            ++end;
            queue[end].y = ny;
            queue[end].x = nx;
            Visited[ny][nx] = i;
             
            if (board[ny][nx] == c) {
                Yt = ny, Xt = nx;
                break;
            }
        }
    }
    //////////////////////////////////////////////////////////////////////////////////////////
    moveCnt = 0;
    trace(Y0, X0, Yt, Xt, 1);
 
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) Visited[i][j] = marked[i][j] ? 9 : -1;
    }
    Visited[Yt][Xt] = 0;
    move(Yt, Xt, Y0, X0);
 
    for (int i = 0; ; i = (i + 1) % moveCnt) {
        mySwap(Movement[i]);
        if (board[y][x] == c) return;
    }
}
 
void solve(int orgBoard[5][5], int orgPattern[3][3], int callCntLimit) {
    // board �ʱ�ȭ, 
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            marked[i][j] = fixed[i][j] = 0;
            board[i][j] = orgBoard[i][j];
 
            // �� ĭ ��ġ�� �����.
            if (board[i][j] == 0) Y0 = i, X0 = j;
        }
    }
    //pattern �ʱ�ȭ
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            pattern[i][j] = orgPattern[i][j];
        }
    }
 
    // pattern�� ����(1,1) ���� ������� �ϼ��س���
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            // (i+1, i+1) ��ġ�� ��������� �����.
            calc(i + 1, j + 1, pattern[i][j]);
 
            // fix �� ��ġ, 5x5 �ʿ� ���� �ϼ��� ��ġ�� fixed ǥ����
            // ���� bfs Ž������ �� ��ġ�� �湮���� �ʵ��� ó���Ѵ�.
            fixed[i + 1][j + 1] = 1;
        }
    }
}
