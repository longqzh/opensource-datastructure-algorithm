
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif


#include <stdio.h>

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
#define MALE   0
#define FEMALE 1

#define INIT         0
#define ADDMEMBER    1
#define GETDISTANCE  2
#define COUNTMEMBER  3

#define COUPLE  0
#define PARENT  1
#define CHILD   2

#define NAME_LEN_MAX 19

////////////////////////////////////////////////////////////////////////////////

extern void init(char initialMemberName[], int initialMemberSex);
extern bool addMember(char newMemberName[], int newMemberSex, int relationship, char existingMemberName[]);
extern int  getDistance(char nameA[], char nameB[]);
extern int  countMember(char name[], int dist);

////////////////////////////////////////////////////////////////////////////////

static int score = 0;

static void cmd_init()
{
    char initialMemberName[NAME_LEN_MAX + 1];
    int initialMemberSex;

    scanf("%s %d", initialMemberName, &initialMemberSex);

    init(initialMemberName, initialMemberSex);
}

static void cmd_addMember()
{
    char newMemberName[NAME_LEN_MAX + 1];
    int newMemberSex;
    int relationship;
    char existingMemberName[NAME_LEN_MAX + 1];

    scanf("%s %d %d %s", newMemberName, &newMemberSex, &relationship, existingMemberName);

    bool userAns = addMember(newMemberName, newMemberSex, relationship, existingMemberName);

    int ans;
    scanf("%d", &ans);
    bool ansbool = ans == 1 ? true : false;

    if (ansbool != userAns)
    {
        score = 0;
    }
}

static void cmd_getDistance()
{
    char nameA[NAME_LEN_MAX + 1];
    char nameB[NAME_LEN_MAX + 1];

    scanf("%s %s", nameA, nameB);

    int userAns = getDistance(nameA, nameB);

    int ans;
    scanf("%d", &ans);

    if (ans != userAns)
    {
        score = 0;
    }
}

static void cmd_countMember()
{
    char name[NAME_LEN_MAX + 1];
    int dist;

    scanf("%s %d", name, &dist);

    int userAns = countMember(name, dist);

    int ans;
    scanf("%d", &ans);

    if (ans != userAns)
    {
        score = 0;
    }
}

int main()
{
    setbuf(stdout, NULL);

    freopen("sample_input.txt", "r", stdin);

    int T, scoreIdx;
    scanf("%d %d", &T, &scoreIdx);

    for (int TC = 1; TC <= T; TC++)
    {
        score = scoreIdx;

        int cmdL;
        scanf("%d", &cmdL);

        for (int cmdIdx = 0; cmdIdx < cmdL; ++cmdIdx)
        {
            int c_num;
            scanf("%d", &c_num);

            switch (c_num)
            {
            case INIT:
                cmd_init();
                break;

            case ADDMEMBER:
                cmd_addMember();
                break;

            case GETDISTANCE:
                cmd_getDistance();
                break;

            case COUNTMEMBER:
                cmd_countMember();
                break;

            default:
                break;
            }
        }

        printf("#%d %d\n", TC, score == scoreIdx ? scoreIdx : 0);
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

char* my_strcpy(char* dest, char* src)
{
	char* tmp = dest;

	while ((*dest++ = *src++) != '\0')
		/* nothing */;
	return tmp;
}

int my_strcmp(const char* cs, const char* ct)
{
	unsigned char c1, c2;

	while (1) {
		c1 = *cs++;
		c2 = *ct++;
		if (c1 != c2)
			return c1 < c2 ? -1 : 1;
		if (!c1)
			break;
	}
	return 0;
}
//-----------------------------------------------------------------------
typedef  unsigned long long u64;
#define MAX_POOL 10000
#define  INF  1000000
#define  SIZE 200 

typedef struct member
{
	int id;
	char name[20];
	u64 name_id;
	int  sex;
	struct member* mother;
	struct member* father;
	struct member* wife;
	struct member* husband;
	NODE child;
	NODE list;
	NODE name_hash_list;
} MEMBER;

#undef CHILD 

typedef struct
{
	MEMBER* p;
	NODE list;
} CHILD;

MEMBER  member_pool[MAX_POOL];
CHILD   child_pool[MAX_POOL];
int member_pool_count;
int child_pool_count;

NODE name_hash_heads[SIZE];
NODE member_list;
int member_count;

int map[SIZE][SIZE] = { 0, };
int cacheDistance;

MEMBER* allocMember()
{
	MEMBER* p;
	p = &member_pool[member_pool_count++];
	return p;
}

u64 strToNum(const char* str)
{
	u64 res = 0LL;
	int i;

	i = 0;
	for( i=0; i<10 && *str; i++ ) 
	{
		res = (res << 5) + (*str - 'a' + 1);
		str++;
	}

	return res;
}

void initMember(MEMBER* p, char name[], int sex)
{
	u64 id;

	p->id = member_count++;
	p->name_id = strToNum(name);
	my_strcpy(p->name, name);
	p->sex = sex;
	p->mother = 0;
	p->father = 0;
	p->wife = 0;
	p->husband = 0;
	INIT_LIST_HEAD(&p->child);
	
	list_add_tail(&p->list, &member_list);
	list_add_tail(&p->name_hash_list, &name_hash_heads[p->name_id % SIZE]);
}

void init(char name[], int sex)
{
	int i,j;
	MEMBER* p;

	member_pool_count = 0;
	child_pool_count = 0;
	member_count = 0;
	cacheDistance = 0;

	for (i = 0; i < SIZE; i++)
		INIT_LIST_HEAD(&name_hash_heads[i]);

	INIT_LIST_HEAD(&member_list);

	p = allocMember();
	initMember(p, name, sex);

	for(i=0; i< SIZE; i++ )
		for (j = 0; j < SIZE; j++)
			map[i][j] = 0;
}

MEMBER* findMemberByName(char name[])
{
	NODE* temp;
	MEMBER* p;
	u64 id;

	id = strToNum(name);
	list_for_each(temp, &name_hash_heads[id % SIZE])
	{
		p = list_entry(temp, MEMBER, name_hash_list);
		if (p->name_id == id)
		{
			if( my_strcmp( p->name, name) == 0 )
				return p;
		}
	}
	return 0;
}


bool addMember(char newMemberName[], int newMemberSex, int relationship, char existingMemberName[])
{
	MEMBER* p;
	MEMBER* newMember;
	NODE* temp;

	if (findMemberByName(newMemberName))
	{
		return 0;
	}

	p = findMemberByName(existingMemberName);

	if (p == 0)
	{
		return 0;
	}

	if (relationship == 0)
	{
		CHILD* child, * c;
		if (p->sex == newMemberSex)
		{
			return 0;
		}
		if (newMemberSex == 0)
		{
			if (p->husband != 0)
			{
				return 0;
			}
			newMember = allocMember();
			initMember(newMember, newMemberName, newMemberSex);
			newMember->wife = p;
			p->husband = newMember;
			list_for_each(temp, &p->child)
			{
				child = &child_pool[child_pool_count++];
				c = list_entry(temp, CHILD, list);
				c->p->father = newMember;
				child->p = c->p;

				list_add_tail(&child->list, &newMember->child);
			}
		}
		else if (newMemberSex == 1)
		{
			if (p->wife != 0)
			{
				return 0;
			}
			newMember = allocMember();
			initMember(newMember, newMemberName, newMemberSex);
			newMember->husband = p;
			p->wife = newMember;
			list_for_each(temp, &p->child)
			{
				child = &child_pool[child_pool_count++];
				c = list_entry(temp, CHILD, list);
				c->p->mother = newMember;
				child->p = c->p;

				list_add_tail(&child->list, &newMember->child);
			}
		}
	}
	else if (relationship == 1)
	{
		CHILD* child, * c;
		if (newMemberSex == 0)
		{
			if (p->father != 0)
			{
				return 0;
			}
			newMember = allocMember();
			initMember(newMember, newMemberName, newMemberSex);
			p->father = newMember;

			if (p->mother == 0)
			{
				child = &child_pool[child_pool_count++];
				child->p = p;
				list_add_tail(&child->list, &newMember->child);
			}
			else if (p->mother != 0)
			{
				p->mother->husband = newMember;
				p->father->wife = p->mother;

				list_for_each(temp, &p->mother->child)
				{
					c = list_entry(temp, CHILD, list);

					child = &child_pool[child_pool_count++];
					child->p = c->p;

					c->p->father = newMember;
					list_add_tail(&child->list, &newMember->child);
				}
			}
		}
		else if (newMemberSex == 1)
		{
			if (p->mother != 0)
			{
				return 0;
			}
			newMember = allocMember();
			initMember(newMember, newMemberName, newMemberSex);
			p->mother = newMember;

			if (p->father == 0)
			{
				child = &child_pool[child_pool_count++];
				child->p = p;
				list_add_tail(&child->list, &newMember->child);
			}
			else if (p->father != 0)
			{
				p->father->wife = newMember;
				p->mother->husband = p->father;

				list_for_each(temp, &p->father->child)
				{
					c = list_entry(temp, CHILD, list);

					child = &child_pool[child_pool_count++];
					child->p = c->p;

					c->p->mother = newMember;
					list_add_tail(&child->list, &newMember->child);
				}
			}
		}
	}
	else if (relationship == 2)
	{
		CHILD* child;

		if (p->sex == 0)
		{
			newMember = allocMember();
			initMember(newMember, newMemberName, newMemberSex);

			newMember->father = p;

			child = &child_pool[child_pool_count++];
			child->p = newMember;

			list_add_tail(&child->list, &p->child);

			if (p->wife != 0)
			{
				newMember->mother = p->wife;

				child = &child_pool[child_pool_count++];
				child->p = newMember;

				list_add_tail(&child->list, &p->wife->child);
			}
		}
		else if (p->sex == 1)
		{
			newMember = allocMember();
			initMember(newMember, newMemberName, newMemberSex);

			newMember->mother = p;

			child = &child_pool[child_pool_count++];
			child->p = newMember;

			list_add_tail(&child->list, &p->child);

			if (p->husband != 0)
			{
				newMember->father = p->husband;

				child = &child_pool[child_pool_count++];
				child->p = newMember;

				list_add_tail(&child->list, &p->husband->child);
			}
		}
	}

	cacheDistance = 0;
	return 1;
}


void floyd_warshall()
{
	int i, j, k;
	for (i = 0; i < member_count; i++)
	{
		for (j = 0; j < member_count; j++)
		{
			if (i == j)
				continue;
			for (k = 0; k < member_count; k++)
			{
				if (j != k && map[j][i] + map[i][k] < map[j][k])
					map[j][k] = map[j][i] + map[i][k];
			}
		}
	}
}

void calcDistance()
{
	NODE* temp, * c_temp;
	CHILD* c;
	MEMBER* p;
	int id;
	int i;
	int j;

	for (i = 0; i < member_count; i++)
		for (j = 0; j < member_count; j++)
			map[i][j] = INF;


	list_for_each(temp, &member_list)
	{
		p = list_entry(temp, MEMBER, list);
		map[p->id][p->id] = 0;
		map[p->id][p->id] = 0;

		if (p->sex == 0 && p->wife)
			map[p->id][p->wife->id] = 0;

		if (p->sex == 1 && p->husband)
			map[p->id][p->husband->id] = 0;

		if (p->father)
			map[p->id][p->father->id] = 1;

		if (p->mother)
			map[p->id][p->mother->id] = 1;

		list_for_each(c_temp, &p->child)
		{
			c = list_entry(c_temp, CHILD, list);
			map[p->id][c->p->id] = 1;
		}
	}
	floyd_warshall();

	cacheDistance = 1;
}

int getDistance(char start[], char end[])
{
	int ret;
	int si, ei;
	if (cacheDistance == 0)
		calcDistance();

	si = findMemberByName(start)->id;
	ei = findMemberByName(end)->id;
	ret = map[si][ei];
	return ret;
}

int countMember(char name[], int n)
{
	int count = 0;
	int i, si;
	if (cacheDistance == 0)
		calcDistance();

	si = findMemberByName(name)->id;
	for (i = 0; i < member_count; i++)
	{
		if (si != i && map[si][i] == n)
			count++;
	}
	return count;
}

#endif

