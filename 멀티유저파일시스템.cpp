//#define   DEBUG
 
#define uint    unsigned int
#define byte    unsigned char
 
#define R       register
#define ruint   register unsigned int
#define rint    register int
#define rshort  register short
#define rbyte   register unsigned char
 
#define HASH    1009
#define NULL    0
 
#define MAX_N   10000
#define MAX_U   20
#define MAX_G   10
#define MAX_NL  8
#define MAX_PL  410
 
#define PMS_USER    0
#define PMS_GROUP   1
#define PMS_EVERY   2
 
uint hUser[MAX_U];
uint hGroup[MAX_U];
byte userSize;
 
struct FILE {
    uint hFile;
    uint hExt;
    FILE* next;
} filePool[MAX_N];
short fileSize;
#define newFile()   &(filePool[fileSize++])
 
struct DIR {
#ifdef DEBUG
    char szName[MAX_NL];
#endif
    uint hDir;
    byte uId;
    uint gId;
    byte permission;
    FILE* file;
    DIR* child;
    DIR* next;
} dirPool[MAX_N];
short dirSize;
DIR root;
#define newDir()    &(dirPool[dirSize++])
 
uint getHash(const char name[MAX_NL]) {
    ruint hash = 0;
    for (rint i = 0; name[i]; i++)
        hash = (hash << 5) + name[i] - 'a' + 1;
    return hash;
}
 
byte getUser(const char name[MAX_NL]) {
    ruint hash = getHash(name);
    for (rbyte i = 0; i < userSize; i++)
        if (hUser[i] == hash) return i;
    return MAX_U + 1; //Error
}
 
DIR* getDir(const char path[MAX_PL]) {
    R DIR* cur = &root;
    ruint hash = 0;
    for (rint i = 1; path[i]; i++) {
        if (path[i] == '/') {
            for (cur = cur->child; cur; cur = cur->next)
                if (cur->hDir == hash) break;
            hash = 0;
        } else {
            hash = (hash << 5) + path[i] - 'a' + 1;
        }
    }
    return cur;
}
 
void mstrcpy(char dst[], const char src[]) {
    while (*src)
        *dst++ = *src++;
    *dst = '\0';
}
 
void init() {
    userSize = 0;
    fileSize = 0;
    dirSize = 0;
 
    ruint hName = getHash("admin");
    hUser[userSize] = hGroup[userSize++] = hName; //admin user & group
 
#ifdef DEBUG
    root.szName[0] = '\0';
#endif
    root.hDir = 0;
    root.uId = 0;
    root.gId = hName;
    root.permission = PMS_EVERY;
    root.file = NULL;
    root.child = NULL;
    root.next = NULL;
}
 
void createUser(char userName[], char groupName[]) {
    hUser[userSize] = getHash(userName);
    hGroup[userSize++] = getHash(groupName);
}
 
bool isAccessable(byte uId, DIR* cur) {
    //dir의 permission에 따라 new dir 생성이 가능여부 판단
    if (cur->permission == PMS_USER) {
        //생성한 사용자만이 접근 가능
        if (uId != cur->uId)
            return false;
    } else if (cur->permission == PMS_GROUP) {
        //생성한 사용자의 동일 그룹의 사용자만이 접근 가능
        if (hGroup[uId] != cur->gId)
            return false;
    }
    return true;
}
 
int createDirectory(char userName[], char path[], char directoryName[], int permission) {
    rbyte uId = getUser(userName);
#ifdef DEBUG
    if (uId > MAX_U)
        return false;
#endif
 
    R DIR* cur = getDir(path);
#ifdef DEBUG
    if (cur == NULL)
        return false;
#endif
 
    if (isAccessable(uId, cur) == false)
        return 0;
 
    //new dir 생성
#ifdef DEBUG
    R DIR* dir = newDir();
    mstrcpy(dir->szName, directoryName);
    dir->hDir = getHash(directoryName);
    dir->uId = uId;
    dir->gId = hGroup[uId];
    dir->permission = (byte)permission;
    dir->file = NULL;
    dir->child = NULL;
    dir->next = NULL;
#else
    dirPool[dirSize] = { getHash(directoryName), uId, hGroup[uId], (byte)permission, NULL, NULL, NULL };
    R DIR* dir = newDir();
#endif
    dir->next = cur->child;
    cur->child = dir;
    return 1;
}
 
int createFile(char userName[], char path[], char fileName[], char fileExt[]) {
    rbyte uId = getUser(userName);
#ifdef DEBUG
    if (uId > MAX_U)
        return false;
#endif
 
    R DIR* cur = getDir(path);
#ifdef DEBUG
    if (cur == NULL)
        return false;
#endif
 
    if (isAccessable(uId, cur) == false)
        return 0;
 
    filePool[fileSize] = { getHash(fileName), getHash(fileExt), NULL };
    R FILE* file = newFile();
    file->next = cur->file;
    cur->file = file;
    return 1;
}
 
int count(DIR* cur, char pattern[MAX_PL], byte uId, uint gId) {
    ruint hash = 0; rint cnt = 0, i = 1;
    for (; pattern[i] != '/' && pattern[i] != '.'; i++)
        hash = (hash << 5) + pattern[i] - 'a' + 1;
    if (pattern[i] == '/') {
        for (R DIR* dir = cur->child; dir; dir = dir->next) {
            if (pattern[i - 1] != '*' && dir->hDir != hash)
                continue;
            if (dir->permission == PMS_USER && dir->uId != uId)
                continue;
            if (dir->permission == PMS_GROUP && dir->gId != gId)
                continue;
            cnt += count(dir, pattern + i, uId, gId);
        }
    } else {
        ruint ext = 0; rint j = 0;
        for (j = i + 1; pattern[j] != '\0'; j++)
            ext = (ext << 5) + pattern[j] - 'a' + 1;
        for (R FILE* file = cur->file; file; file = file->next) {
            if (pattern[j - 1] != '*' && file->hExt != ext)
                continue;
            if (pattern[i - 1] != '*' && file->hFile != hash)
                continue;
            cnt++;
        }
    }
    return cnt;
}
 
int find(char userName[], char pattern[]) {
    rbyte uId = getUser(userName);
#ifdef DEBUG
    if (uId > MAX_U)
        return 0;
#endif
    int n = count(&root, pattern, uId, hGroup[uId]);
    return n;
}
