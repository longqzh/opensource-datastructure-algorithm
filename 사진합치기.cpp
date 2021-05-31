#define rint register int
 
#define MPictureSize 1000
#define MPieceSize 100
#define MPiece 1500
#define MNode 300000
#define MHash 65535
 
extern bool setPicture(int id, int x, int y);
 
int PieceCount, PieceSize, PictureSize, Diff;
int Used[MPiece];
struct _RefHash {
    int right, down, left, up;
} RefHash[MPiece];
 
struct NODE {
    int id, pos;
    NODE * n;
} NODEs[MNode];
int NodeCount;
 
NODE *SumY[MHash], *SumX[MHash];
 
int Merged;
 
void dfs(int id, int x, int y) {
    Used[id] = 1;
    if (++Merged == PieceCount)
        return;
 
    rint ny, nx, _id;
    register NODE *node;
 
    // right
    if (x < Diff) {
        node = SumY[RefHash[id].right];
        while (node) {
            _id = node->id;
            if (Used[_id] == 0) {
                nx = x + PieceSize - 2 - node->pos;
                if ((nx <= Diff) && setPicture(_id, nx, y)) {
                    dfs(_id, nx, y);
                    break;
                }
            }
            node = node->n;
        }
    }
 
    if (Merged == PieceCount) return;
 
    // down
    if (y < Diff) {
        node = SumX[RefHash[id].down];
        while (node) {
            _id = node->id;
            if (Used[_id] == 0) {
                ny = y + PieceSize - 2 - node->pos;
                if ((ny <= Diff) && setPicture(_id, x, ny)) {
                    dfs(_id, x, ny);
                    break;
                }
            }
            node = node->n;
        }
    }
 
    if (Merged == PieceCount) return;
 
    // left
    if (x > 0) {
        node = SumY[RefHash[id].left];
        while (node) {
            _id = node->id;
            if (Used[_id] == 0) {
                nx = x + 1 - node->pos;
                if ((nx >= 0) && setPicture(_id, nx, y)) {
                    dfs(_id, nx, y);
                    break;
                }
            }
            node = node->n;
        }
    }
 
    if (Merged == PieceCount) return;
 
    // up
    if (y > 0) {
        node = SumX[RefHash[id].up];
        while (node) {
            _id = node->id;
            if (Used[_id] == 0) {
                ny = y + 1 - node->pos;
                if ((ny >= 0) && setPicture(_id, x, ny)) {
                    dfs(_id, x, ny);
                    break;
                }
            }
            node = node->n;
        }
    }
}
 
void mergePictures(int n, int m, int k, char pictures[MPiece][MPieceSize][MPieceSize]) {
    rint i, j;
 
    Merged = 0;
    PieceCount = n;
    PieceSize = m;
    PictureSize = k;
    Diff = PictureSize - PieceSize;
    for (i = 0; i < PieceCount; i++) {
        Used[i] = 0;
    }
 
    NodeCount = 0;
    for (i = 0; i < MHash; i++) {
        SumY[i] = 0;
    }
    for (i = 0; i < MHash; i++) {
        SumX[i] = 0;
    }
 
    register char (*pic)[MPieceSize];
    int diff = PieceSize - 2;
    pic = pictures[0];
    RefHash[0].right = pic[0][diff] | pic[1][diff] << 4 | pic[2][diff] << 8 | pic[3][diff] << 12;
    RefHash[0].down = pic[diff][0] | pic[diff][1] << 4 | pic[diff][2] << 8 | pic[diff][3] << 12;
         
    for (i = 1; i < PieceCount; i++) {
        pic = pictures[i];
        RefHash[i].right = pic[0][diff] | pic[1][diff] << 4 | pic[2][diff] << 8 | pic[3][diff] << 12;
        RefHash[i].down = pic[diff][0] | pic[diff][1] << 4 | pic[diff][2] << 8 | pic[diff][3] << 12;
        RefHash[i].left = pic[0][1] | pic[1][1] << 4 | pic[2][1] << 8 | pic[3][1] << 12;
        RefHash[i].up = pic[1][0] | pic[1][1] << 4 | pic[1][2] << 8 | pic[1][3] << 12;
    }
 
    rint sum;
    register NODE * node;
    for (i = 1; i < PieceCount; i++) {
        pic = pictures[i];
        for (j = 0; j < PieceSize; j++) {
            sum = pic[0][j] | pic[1][j] << 4 | pic[2][j] << 8 | pic[3][j] << 12;
            node = &NODEs[NodeCount++];
            node->id = i;
            node->pos = j;
            node->n = SumY[sum];
            SumY[sum] = node;
 
            sum = pic[j][0] | pic[j][1] << 4 | pic[j][2] << 8 | pic[j][3] << 12;
            node = &NODEs[NodeCount++];
            node->id = i;
            node->pos = j;
            node->n = SumX[sum];
            SumX[sum] = node;
        }
    }
 
    setPicture(0, 0, 0);
    dfs(0, 0, 0);
}
