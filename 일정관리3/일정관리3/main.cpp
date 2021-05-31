#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>

typedef enum {
    INIT,
    ADD,
    DELETE,
    CHANGE,
    GETCOUNT
} COMMAND;

extern void init();
extern void addEvent(int uid, char ename[], int groupid);
extern int deleteEvent(int uid, char ename[]);
extern int changeEvent(int uid, char ename[], char cname[]);
extern int getCount(int uid);

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

static int mSeed;
static int pseudo_rand(void)
{
    mSeed = mSeed * 431345 + 2531999;
    return mSeed & 0x7FFFFFFF;
}

static void make_string(char str[], int seed)
{
    mSeed = seed;
    int length = 5 + pseudo_rand() % 10;
    for (int i = 0; i < length; ++i) {
        str[i] = 'a' + pseudo_rand() % 26;
    }
    str[length] = 0;
}

static int run()
{
    int answer = 100;

    int cmd, p1, p2, p3, p4;
    char str1[15], str2[15];

    int N, ret, flag;
    scanf("%d %d %d", &cmd, &N, &flag);

    init();

    for (int i = 1; i < N; ++i) {
        scanf("%d", &cmd);
        switch (cmd) {
        case ADD:
            if (flag == 1) {
                scanf("%d %s %d", &p1, str1, &p3);
            }
            else {
                scanf("%d %d %d", &p1, &p2, &p3);
                make_string(str1, p2);
            }
            addEvent(p1, str1, p3);
            break;

        case DELETE:
            if (flag == 1) {
                scanf("%d %s %d", &p1, str1, &p3);
            }
            else {
                scanf("%d %d %d", &p1, &p2, &p3);
                make_string(str1, p2);
            }
            ret = deleteEvent(p1, str1);
            if (ret != p3)
                answer = 0;
            break;

        case CHANGE:
            if (flag == 1) {
                scanf("%d %s %s %d", &p1, str1, str2, &p4);
            }
            else {
                scanf("%d %d %d %d", &p1, &p2, &p3, &p4);
                make_string(str1, p2);
                make_string(str2, p3);
            }
            ret = changeEvent(p1, str1, str2);
            if (ret != p4)
                answer = 0;
            break;

        case GETCOUNT:
            scanf("%d %d", &p1, &p2);
            ret = getCount(p1);
            if (ret != p2)
                answer = 0;
            break;

        default:
            break;
        }
    }

    return answer;
}

int main()
{
    setbuf(stdout, NULL);
    freopen("sample_input.txt", "r", stdin);

    int T;
    scanf("%d", &T);

    for (int tc = 1; tc <= T; tc++) {
        printf("#%d %d\n", tc, run());
    }

    return 0;
}

#if 1
#undef  offsetof
#define offsetof(TYPE, MEMBER)	((unsigned int)&((TYPE *)0)->MEMBER)

#define container_of(ptr, type, member) 			\
		((type *)( (char *)ptr - offsetof(type,member) ))

#define list_entry(ptr, type, member) \
	container_of(ptr, type, member)

#define list_first_entry(ptr, type, member) \
	list_entry((ptr)->next, type, member)

#define list_for_each(pos, head) \
	for (pos = (head)->next; pos != (head); pos = pos->next)

#define list_for_each_entry(pos, head, member)				\
	for (pos = list_first_entry(head, typeof(*pos), member);	\
			&pos->member != (head);					\
			pos = list_next_entry(pos, member))

#define list_for_each_safe(pos, n, head) \
	for (pos = (head)->next, n = pos->next; pos != (head); \
		pos = n, n = pos->next)

typedef struct node 
{
	struct node* next, * prev;
} NODE;

void __list_add(NODE* temp,
	NODE* prev,
	NODE* next)
{
	next->prev = temp;
	temp->next = next;
	temp->prev = prev;
	prev->next = temp;
}

void list_add(NODE* temp, NODE* head)
{
	__list_add(temp, head, head->next);
}

void list_add_tail(NODE* temp, NODE* head)
{
	__list_add(temp, head->prev, head);
}

void __list_del(NODE* prev, NODE* next)
{
	next->prev = prev;
	prev->next = next;
}

void list_del(NODE* entry)
{
	__list_del(entry->prev, entry->next);
}

int list_empty(const NODE* head)
{
	return head->next == head;
}

void INIT_LIST_HEAD(NODE* list)
{
	list->next = list;
	list->prev = list;
}
//-------------------------------------------------------------------------------

#define MAX_USER (1000)
#define MAX_GROUP (100)
#define MAX_BUF (100 * 1000 * 2)
#define MAX_HASH (1024)

typedef unsigned long long u64;

typedef struct
{
	int gid;
	NODE master_list_head;
} GROUP;

typedef struct
{
	int uid;
    u64 c1;
    u64 c2;
    int is_master;
	NODE member_list_head;
	NODE list;
    NODE hash_list;
} EVENT;

NODE  hash_table[MAX_HASH];
GROUP group_pool[MAX_GROUP];
EVENT event_pool[MAX_BUF];

int event_pool_count;
int num_event_of_user[MAX_USER];

u64 hash_str(const char* str, u64* c1, u64* c2)
{
    *c1 = *c2 = 0;

    for ( int i = 0; i < str[i]; ++i) {
        if (i < 10)
            *c1 += (*c1 << 5) + str[i] - 'a' + 1;
        else
            *c2 += (*c1 << 5) + str[i] - 'a' + 1;
    }
    return (*c1 + *c2) % MAX_HASH;
}


void init()
{
	int i;

	event_pool_count = 0;

	for (i = 0; i < MAX_GROUP; i++)
	{
		INIT_LIST_HEAD(&group_pool[i].master_list_head);
	}

    for (i = 0; i < MAX_HASH; i++)
    {
        INIT_LIST_HEAD(&hash_table[i]);
    }

	for (i = 0; i < MAX_USER; i++)
	{
		num_event_of_user[i] = 0;
	}
}

void addEvent(int uid, char ename[], int groupid)
{
	NODE* temp1;
	NODE* temp2;

	EVENT* event=0, * master=0;

	int flag = 0;
    u64 key;


	event = &event_pool[event_pool_count++];
	event->uid = uid;
    key = hash_str(ename, &event->c1, &event->c2);
	INIT_LIST_HEAD(&event->member_list_head);

    list_add_tail(&event->hash_list, &hash_table[key]);

	list_for_each(temp1, &group_pool[groupid].master_list_head)
	{
		master = list_entry(temp1, EVENT, list);
		if ( event->c1==master->c1 && event->c2==master->c2)
		{
			flag = 1;
			break;
		}
	}

	if (flag == 1)
	{
		list_add_tail(&event->list, &master->member_list_head);
        event->is_master = 0;
	}
	else
	{
		list_add_tail(&event->list, &group_pool[groupid].master_list_head);
        event->is_master = 1;
	}
	num_event_of_user[uid]++;
}

int getCount(int uid)
{
	return num_event_of_user[uid];
}

int deleteEvent(int uid, char ename[])
{
	int count = 0;
	NODE* temp1;
	NODE* temp2;
	NODE* n1;
	NODE* n2;
	EVENT* member;
    u64 c1, c2, key;
    EVENT* event;

    key = hash_str(ename, &c1, &c2);

    list_for_each(temp1, &hash_table[key])
    {
        event = list_entry(temp1, EVENT, hash_list);
        if (event->uid == uid && event->c1 == c1 && event->c2 == c2)
        {
            if (event->is_master)
            {
                list_for_each_safe(temp2, n2, &event->member_list_head)
                {
                    count++;
                    member = list_entry(temp2, EVENT, list);
                    list_del(temp2);
                    list_del(&member->hash_list);
                    num_event_of_user[member->uid]--;
                }
            }
            count++;
            list_del(temp1);
            list_del(&event->hash_list);
            num_event_of_user[event->uid]--;
            break;
        }
    }

	return count;
}

int changeEvent(int uid, char ename[], char cname[])
{
    int count = 0;
    NODE* temp1;
    NODE* temp2;
    NODE* n1;
    NODE* n2;
    EVENT* member;
    u64 e_c1, e_c2, c_c1, c_c2, e_key, c_key;
    EVENT* event;

    e_key = hash_str(ename, &e_c1, &e_c2);
    c_key = hash_str(cname, &c_c1, &c_c2);

    list_for_each(temp1, &hash_table[e_key])
    {
        event = list_entry(temp1, EVENT, hash_list);
        if (event->uid == uid && event->c1 == e_c1 && event->c2 == e_c2)
        {
            if (event->is_master)
            {
                list_for_each_safe(temp2, n2, &event->member_list_head)
                {
                    count++;
                    member = list_entry(temp2, EVENT, list);
                    member->c1 = c_c1;
                    member->c2 = c_c2;
                    list_del(&member->hash_list);
                    list_add_tail(&member->hash_list, &hash_table[c_key]);
                }
            }
            count++;
            event->c1 = c_c1;
            event->c2 = c_c2;
            list_del(&event->hash_list);
            list_add_tail(&event->hash_list, &hash_table[c_key]);
            break;
        }
    }

    return count;
}
#endif

#if 0
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

#endif