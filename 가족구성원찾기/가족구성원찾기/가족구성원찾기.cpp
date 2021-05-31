#define uint unsigned int
#define rint register int
#define MU 200
#define MH 500
 
struct NODE {
    int sex, parent, couple;
} NODEs[MU];
int NodeCount;
 
int Map[MU][MU], DisMap[MU][MU];
 
uint NameCode[MH];
int NameID[MH];
 
#define getCode(code,str)   for (rint i=0; str[i]; i++) code = (code << 5) + str[i] - 96;
 
void init(char name[], int sex) {
    NodeCount = 0;
    rint i, j;
    for (i = 0; i < MH; i++) {
        NameCode[i] = 0;
    }
    for (i = 0; i < MU; i++) {
        for (j = 0; j < MU; j++)
            Map[i][j] = -1;
    }
    for (i = 0; i < MU; i++) {
        for (j = 0; j < MU; j++)
            DisMap[i][j] = 0;
    }
 
    NODEs[NodeCount] = { sex, -1, -1 };
 
    register uint code = 0; 
    getCode(code, name)
    NameCode[code % MH] = code;
    NameID[code % MH] = NodeCount++;
}
 
void update(int idN, int idE, int relation) {
    rint i;
    for (i = 0; i < idE; i++) {
        Map[idN][i] = Map[idE][i] + relation;
        DisMap[idN][Map[idN][i]]++;
        DisMap[i][Map[idN][i]]++;
    }
    Map[idN][idE] = relation;
    DisMap[idN][relation]++;
    DisMap[idE][relation]++;
    for (i = idE + 1; i < idN; i++) {
        Map[idN][i] = Map[i][idE] + relation;
        DisMap[idN][Map[idN][i]]++;
        DisMap[i][Map[idN][i]]++;
    }
}
 
bool addMember(char name[], int sex, int relationship, char existingName[]) {
    register uint codeE = 0;
    getCode(codeE, existingName)
    rint h = codeE % MH;
    while (NameCode[h] != codeE)
        h = (h + 1) % MH;
    int idE = NameID[h];    
 
    NODE *nodeE = &NODEs[idE];
 
    if (relationship == 0) {
        if ((nodeE->sex == sex) || (nodeE->couple != -1))
            return 0;       
 
        NODEs[NodeCount] = { sex, -1, idE };
        nodeE->couple = NodeCount;
        update(NodeCount, idE, 0);
    }
    else if (relationship == 1) {
        int idP = nodeE->parent;
        if (idP == -1) {
            NODEs[NodeCount] = { sex, -1, -1 };
            nodeE->parent = NodeCount;
            update(NodeCount, idE, 1);
        }
        else {
            NODE *nodeP = &NODEs[idP];
            if ((nodeP->sex == sex) || (nodeP->couple != -1))
                return 0;
 
            NODEs[NodeCount] = { sex, -1, idP };
            nodeP->couple = NodeCount;
            update(NodeCount, idP, 0);
        }
    }
    else {
        NODEs[NodeCount] = { sex, idE, -1 };
        update(NodeCount, idE, 1);
    }
 
    register uint code = 0;
    getCode(code, name)
    h = code % MH;
    while (NameCode[h])
        h = (h + 1) % MH;
    NameCode[h] = code;
    NameID[h] = NodeCount++;
 
    return 1;
}
 
int  getDistance(char nameA[], char nameB[]) {
    register uint code = 0;
    getCode(code, nameA)    
 
    rint h = code % MH;
    while (NameCode[h] != code)
        h = (h + 1) % MH;
    int idA = NameID[h];
 
    code = 0;
    getCode(code, nameB)
 
    h = code % MH;
    while (NameCode[h] != code)
        h = (h + 1) % MH;
    int idB = NameID[h];
 
    return (idA > idB) ? Map[idA][idB] : Map[idB][idA];
}
 
int  countMember(char name[], int dist) {
    register uint code = 0; 
    getCode(code, name)
     
    rint h = code % MH;
    while (NameCode[h] != code)
        h = (h + 1) % MH;
 
    return DisMap[NameID[h]][dist];
}
