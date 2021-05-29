#define MAX_USER (1000)
#define MAX_GROUP (100)
#define MAX_BUF (100 * 1000 * 2)
#define MAX_HASH (1737)
 
typedef unsigned long long ull;
 
struct NODE {
    ull code1, code2;
    int num_user;
    int uid[6];
    NODE* next;
} buf[MAX_BUF], * tb[MAX_GROUP][MAX_HASH];
 
int bcnt;
int num_event_of_user[MAX_USER];
 
ull str2ull(const char* str, ull* c1, ull* c2)
{
    *c1 = *c2 = 0;
 
    for (register int i = 0; i < str[i]; ++i) {
        if (i < 10)
            *c1 += (*c1 << 5) + str[i] - 'a' + 1;
        else
            *c2 += (*c1 << 5) + str[i] - 'a' + 1;
    }
    return (*c1 + *c2) % MAX_HASH;
}
 
void init()
{
    bcnt = 0;
    for (register int i = 0; i < MAX_GROUP; ++i)
        for (register int j = 0; j < MAX_HASH; ++j)
            tb[i][j] = 0;
 
    for (register int i = 0; i < MAX_USER; ++i)
        num_event_of_user[i] = 0;
}
 
void addEvent(int uid, char ename[], int groupid)
{
    NODE* p;
    ull c1, c2;
    ull key = str2ull(ename, &c1, &c2);
 
    for (p = tb[groupid][key]; p; p = p->next) {
        if ((p->code1 == c1) && (p->code2 == c2))
        {
            p->uid[p->num_user] = uid;
            p->num_user++;
            break;
        }
    }
 
    if (!p) {
        p = &buf[bcnt++];
        p->code1 = c1;
        p->code2 = c2;
        p->uid[0] = uid;
        p->num_user = 1;
        p->next = tb[groupid][key];
        tb[groupid][key] = p;
    }
 
    num_event_of_user[uid]++;
}
 
int deleteEvent(int uid, char ename[])
{
    register int i;
    int ret = 0;
     
    NODE* p;
    ull c1, c2;
    ull key = str2ull(ename, &c1, &c2);
     
    for (register int gid = 0; gid < MAX_GROUP; ++gid) {
        for (p = tb[gid][key]; p; p = p->next) {
            if ((p->code1 == c1) && (p->code2 == c2)) {
                for (i = 0; (i < p->num_user) && (p->uid[i] != uid); ++i);
                if (i == p->num_user)
                    continue;
 
                // master
                if (p->uid[0] == uid) {
                    for (i = 0; i < p->num_user; ++i) {
                        num_event_of_user[p->uid[i]]--;
                        ret++;
                    }
                    p->num_user = 0;
                }
                // normal
                else {
                    for (register int j = i; j < p->num_user; ++j)
                        p->uid[j] = p->uid[j + 1];
                    p->num_user--;
                    num_event_of_user[uid]--;
                    ret++;
                }
                return ret;
            }
        }
    }
    return ret;
}
 
int changeEvent(int uid, char ename[], char cname[])
{
    register int i;
    int ret = 0;
     
    NODE* p;
    ull c1, c2;
    ull key = str2ull(ename, &c1, &c2);
     
    for (register int gid = 0; gid < MAX_GROUP; ++gid) {
        for (p = tb[gid][key]; p; p = p->next) {
            if ((p->code1 == c1) && (p->code2 == c2)) {              
                for (i = 0; (i < p->num_user) && (p->uid[i] != uid); ++i);
                 
                if (i == p->num_user)
                    continue;
 
                key = str2ull(cname, &c1, &c2);
                NODE* q = &buf[bcnt++];
                q->code1 = c1;
                q->code2 = c2;
                q->num_user = 0;
                q->next = tb[gid][key];
                tb[gid][key] = q;
 
                // master
                if (p->uid[0] == uid) {
                    for (i = 0; i < p->num_user; ++i) {
                        q->uid[i] = p->uid[i];
                        q->num_user++;
                        ret++;
                    }
                    p->num_user = 0;
                }
                // normal
                else {
                    for (register int j = i; j < p->num_user; ++j)
                        p->uid[j] = p->uid[j + 1];
                    p->num_user--;
                    q->uid[0] = uid;
                    q->num_user++;
                    ++ret;
                }
                return ret;
            }
        }
    }
    return ret;
}
 
int getCount(int uid)
{
    return num_event_of_user[uid];
}
