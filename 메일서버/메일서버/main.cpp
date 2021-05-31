#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>

#define INIT         0
#define SENDMAIL     1
#define GETCOUNT     2
#define DELETEMAIL   3
#define SEARCHMAIL   4

extern void init(int N, int K);
extern void sendMail(char subject[], int uID, int cnt, int rIDs[]);
extern int getCount(int uID);
extern int deleteMail(int uID, char subject[]);
extern int searchMail(int uID, char text[]);

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

#define MAX_WORD 10000
static char Word[MAX_WORD][11];

static int N, W, minR, maxR, SW;

static int mSeed;
static int pseudo_rand(void)
{
	mSeed = mSeed * 214013 + 2531011;
	return (mSeed >> 16) & 0x7FFF;
}

static void make_string(int seed)
{
	mSeed = seed;

	for (int i = 0; i < W; ++i) {
		int length = 5 + pseudo_rand() % 6;
		for (int k = 0; k < length; ++k) {
			Word[i][k] = 'a' + pseudo_rand() % 26;
		}
		Word[i][length] = '\0';
	}
}

static void send_mail(int seed)
{
	char str[200];
	int pos = 0;

	mSeed = seed;

	int wcnt = 1 + pseudo_rand() % SW;
	for (int i = 0; i < wcnt; ++i) {
		int widx = pseudo_rand() % W;
		for (int k = 0; k < 10; ++k) {
			if (Word[widx][k] == '\0') break;
			str[pos++] = Word[widx][k];
		}
		str[pos++] = ' ';
	}
	str[pos - 1] = '\0';

	int uid = pseudo_rand() % N;
	int rcnt = minR + pseudo_rand() % (maxR - minR + 1);
	int rids[50];
	for (int i = 0; i < rcnt; ++i) {
		rids[i] = pseudo_rand() % N;
	}

	sendMail(str, uid, rcnt, rids);
}

static int delete_mail(int uid, int seed)
{
	char str[200];
	int pos = 0;

	mSeed = seed;

	int wcnt = 1 + pseudo_rand() % SW;
	for (int i = 0; i < wcnt; ++i) {
		int widx = pseudo_rand() % W;
		for (int k = 0; k < 10; ++k) {
			if (Word[widx][k] == '\0') break;
			str[pos++] = Word[widx][k];
		}
		str[pos++] = ' ';
	}
	str[pos - 1] = '\0';

	return deleteMail(uid, str);
}

static int run(int answer)
{
	int Q, K, cmd, sample, seed, param1, param2, ret;
	char str[30];

	scanf("%d %d %d %d", &Q, &sample, &N, &K);
	if (sample == 0) {
		scanf("%d %d %d %d %d", &W, &minR, &maxR, &SW, &seed);
		make_string(seed);
	}

	init(N, K);

	for (int i = 1; i < Q; ++i) {
		scanf("%d", &cmd);
		switch (cmd) {
		case SENDMAIL:
			if (sample == 1) {
				int uid, rcnt;
				int rids[10];
				scanf("%s %d %d", str, &uid, &rcnt);
				for (int k = 0; k < rcnt; ++k) scanf("%d", &rids[k]);
				for (int k = 0; k < 30; ++k) {
					if (str[k] == '\0') break;
					if (str[k] == '_') str[k] = ' ';
				}
				sendMail(str, uid, rcnt, rids);
			}
			else {
				scanf("%d", &seed);
				send_mail(seed);
			}
			break;
		case GETCOUNT:
			scanf("%d %d", &param1, &param2);
			ret = getCount(param1);
			if (ret != param2)
				answer = 0;
			break;
		case DELETEMAIL:
			if (sample == 1) {
				int uid;
				scanf("%d %s %d", &param1, str, &param2);
				for (int k = 0; k < 30; ++k) {
					if (str[k] == '\0') break;
					if (str[k] == '_') str[k] = ' ';
				}
				ret = deleteMail(param1, str);
				if (ret != param2)
					answer = 0;
			}
			else {
				scanf("%d %d %d", &param1, &seed, &param2);
				ret = delete_mail(param1, seed);
				if (ret != param2)
					answer = 0;
			}
			break;
		case SEARCHMAIL:
			scanf("%d %s %d", &param1, str, &param2);
			ret = searchMail(param1, str);
			if (ret != param2)
				answer = 0;
			break;
		default:
			break;
		}
	}

	return answer;
}

#include <time.h>
int main()
{
	setbuf(stdout, NULL);
	freopen("sample_input.txt", "r", stdin);

	int T, Mark;
	scanf("%d %d", &T, &Mark);

	clock_t start, end;
	start = clock();
	for (int tc = 1; tc <= T; tc++) {
		printf("#%d %d\n", tc, run(Mark));
	}
	end = clock();
	printf("%f\n", end - start);
	return 0;
}
//-----------------------------------------------------------------------

#if 0
/// jjjj ´Ô ÄÚµå ÂüÁ¶
#define R   register
#define MAXM    10001
#define MAXU    1000
typedef unsigned long long ull;

typedef struct MAIL {
	int t_cnt;
	ull text[10];
} MAIL;
MAIL MailPool[MAXM];
int Mailcnt, Capa;
inline MAIL* getMailPool(int a) { MailPool[a] = {}; return &MailPool[a]; }

typedef struct USER {
	int m_cnt, head, tail;
	int mail[1000];
} USER;
USER UserPool[MAXU];

void init(int N, int _K)
{
	Capa = _K;
	Mailcnt = 1;
	for (R int i = 0; i < N; i++)
		UserPool[i] = {};
}

// sendMail("test email abcd", 0, 3, {0, 1, 2})
void sendMail(char subject[], int uID, int cnt, int rIDs[])
{
	R MAIL* mp = getMailPool(Mailcnt++);

	while (*subject) {
		if (*subject == ' ')
			mp->t_cnt++;
		else {
			mp->text[mp->t_cnt] = (mp->text[mp->t_cnt] << 5) + (*subject - 'a' + 1);
		}
		subject++;
	}
	mp->t_cnt++;

	for (R int i = 0; i < cnt; i++) {
		R USER* up = &UserPool[rIDs[i]];

		if (up->m_cnt == Capa) {
			while (!up->mail[up->head])
				up->head++;  // find head position
			up->mail[up->head++] = 0; // delete last one
			up->m_cnt--;
		}
		up->m_cnt++;
		up->mail[up->tail++] = Mailcnt - 1;
	}
}

int getCount(int uID)
{
	return UserPool[uID].m_cnt;
}

// deleteMail(2, "test email abcd")
int deleteMail(int uID, char subject[])
{
	R int scnt = 0;
	R ull t[10] = {};

	while (*subject) {
		if (*subject == ' ')
			scnt++;
		else
			t[scnt] = (t[scnt] << 5) + (*subject - 'a' + 1);
		*subject++;
	}
	scnt++;

	R int delcnt = 0;
	R USER* user = &UserPool[uID];

	for (R int i = user->head; i < user->tail; i++) {
		if (!user->mail[i])
			continue;
		if (MailPool[user->mail[i]].t_cnt != scnt)
			continue;

		R int delflag = 1;
		for (R int j = 0; j < MailPool[user->mail[i]].t_cnt; j++) {
			if (MailPool[user->mail[i]].text[j] != t[j]) {
				delflag = 0;
				break;
			}
		}
		if (delflag) {
			user->mail[i] = 0;
			user->m_cnt--;
			delcnt++;
		}
	}
	return delcnt;
}

// searchMail(2, "test")
int searchMail(int uID, char text[])
{
	R int cnt = 0;
	R ull t = { 0 };

	while (*text)
		t = (t << 5) + (*text++ - 'a' + 1);

	R USER* user = &UserPool[uID];
	for (R int i = user->head; i < user->tail; i++) {
		if (!user->mail[i])
			continue;

		for (R int j = 0; j < MailPool[user->mail[i]].t_cnt; j++) {
			if (MailPool[user->mail[i]].text[j] == t) {
				cnt++;
				break;
			}
		}
	}
	return cnt;
}
#endif

//-----------------------------------------------------------------------

#if 1
#undef  offsetof
#define offsetof(TYPE, MEMBER)	((unsigned long)&((TYPE *)0)->MEMBER)

#define container_of(ptr, type, member)       \
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

struct list_head {
	struct list_head* next, * prev;
};

void __list_add(struct list_head* temp,
	struct list_head* prev,
	struct list_head* next)
{
	next->prev = temp;
	temp->next = next;
	temp->prev = prev;
	prev->next = temp;
}

void list_add(struct list_head* temp, struct list_head* head)
{
	__list_add(temp, head, head->next);
}

void list_add_tail(struct list_head* temp, struct list_head* head)
{
	__list_add(temp, head->prev, head);
}

void __list_del(struct list_head* prev, struct list_head* next)
{
	next->prev = prev;
	prev->next = next;
}

void list_del(struct list_head* entry)
{
	__list_del(entry->prev, entry->next);
}

int list_empty(const struct list_head* head)
{
	return head->next == head;
}

void INIT_LIST_HEAD(struct list_head* list)
{
	list->next = list;
	list->prev = list;
}




//-----------------------------------------------------------------------

#define MAXM 10000
#define MAXU 1000

typedef unsigned long long u64;


typedef struct
{
	int word_cnt;
	u64 word[10];
} MAIL;

typedef struct
{
	MAIL* p;
	struct list_head list;
} MAIL_P;

typedef struct
{
	int count;
	struct list_head mail_list;
	MAIL_P  mail_p[1000];
	int mail_pool_count;
} USER;

USER user_pool[MAXU];
MAIL mail_pool[MAXM];
int mail_pool_count;

int mail_max_count;

void init(int N, int K)
{
	int i;

	mail_max_count = K;
	mail_pool_count = 0;

	for (i = 0; i < N; i++)
	{
		//user_pool[i].count = 0;
		//user_pool[i].mail_pool_count = 0;
		user_pool[i] = {};
		INIT_LIST_HEAD(&user_pool[i].mail_list);
	}
}

#define STR_HASH(result,ch) ((result) = ((result) << 5) + ((ch) - 'a' + 1))

void sendMail(char subject[], int uID, int cnt, int rIDs[])
{
	int i;
	MAIL* mp;

	mp = &mail_pool[mail_pool_count++];
	*mp = {};
	//mp->word_cnt = 0;
	//mp->word[mp->word_cnt] = 0;
	while (*subject) {
		if (*subject == ' ')
		{
			mp->word_cnt++;
		}
		else {
			STR_HASH(mp->word[mp->word_cnt], *subject);
		}
		subject++;
	}
	mp->word_cnt++;

	for (i = 0; i < cnt; i++)
	{
		USER *up = &user_pool[rIDs[i]];
		MAIL_P* p = &up->mail_p[up->mail_pool_count++];
		p->p = mp;
		if (up->count == mail_max_count)
		{
			list_del(up->mail_list.next);
			up->count--;
		}
		list_add_tail(&p->list, &up->mail_list);
		up->count++;
	}
}

int getCount(int uID)
{
	return user_pool[uID].count;
}

int deleteMail(int uID, char subject[])
{
	struct list_head* temp, *n;
	MAIL_P* p;
	int remove_count = 0;
	USER* up = &user_pool[uID];

	int scnt = 0;
	u64 word[10] = {0,};
	int i;

	while (*subject) {
		if (*subject == ' ')
			scnt++;
		else
			STR_HASH(word[scnt], *subject);
		*subject++;
	}
	scnt++;

	list_for_each_safe(temp, n, &up->mail_list)
	{
		p = list_entry(temp, MAIL_P, list);
		for (i = 0; i < scnt; i++)
		{
			if (word[i] != p->p->word[i])
				break;
		}
		if( i == scnt )
		{
			list_del(temp);
			up->count--;
			remove_count++;
		}
	}
	return remove_count;
}

int searchMail(int uID, char text[])
{
	struct list_head* temp;
	MAIL_P* p;
	USER* up = &user_pool[uID];

	int search_count = 0;
	u64 word = { 0 };
	int i;

	while (*text)
		STR_HASH(word, *text++);

	list_for_each(temp, &up->mail_list)
	{
		p = list_entry(temp, MAIL_P, list);

		for (i = 0; i < p->p->word_cnt; i++)
		{
			if (p->p->word[i] == word)
			{
				search_count++;
				break;
			}
		}
	}

	return search_count;
}
#endif

