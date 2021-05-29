#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXR  100
#define MAXC   27

char value_str[MAXR][MAXC][20];
int value[MAXR][MAXC];
int Row;
int Col;

int my_strncmp(char * cs, char * ct, int count)
{
	char res = 0;

	while (count) {
		if ((res = *cs - *ct++) != 0 || !*cs++)
			break;
		count--;
	}

	return res;
}

char * my_strcpy(char * dest,char *src)
{
	char *tmp = dest;

	while ((*dest++ = *src++) != '\0')
		/* nothing */;
	return tmp;
}

int my_atoi( char *str )
{
	int n=0; 
	int s=1;
	int i;

	if(str[0] == '-' )
	{
		s = -1;
		str++;
	}

	for(i=0; str[i]; i++ )
		n = n*10 + str[i] - '0';

	return n*s;
}

char * my_strchr(const char * s, int c)
{
	for(; *s != (char) c; ++s)
		if (*s == '\0')
			return NULL;
	return (char *) s;
}

#define my_isdigit(c) (('0' <= (c) && (c)  <= '9') || (c) == '-' )

void init(int C, int R)
{
	int i=0;
	int j=0;

	Col = C;
	Row = R;

	for(i=0; i<Row; i++)
		for(j=0; j<Col; j++)
			my_strcpy( value_str[i][j], "0" ); 
}

void set(int col, int row, char input[])
{
	int i=0;
	int j=0;
	my_strcpy( value_str[row-1][col-1], input+j); 
}

int is_expr( char *expr )
{
	if( my_strncmp( expr, "ADD", 3 ) == 0 )
		return 1;
	if( my_strncmp( expr, "SUB", 3 ) == 0 )
		return 1;
	if( my_strncmp( expr, "MUL", 3 ) == 0 )
		return 1;
	if( my_strncmp( expr, "DIV", 3 ) == 0 )
		return 1;
	if( my_strncmp( expr, "MAX", 3 ) == 0 )
		return 1;
	if( my_strncmp( expr, "MIN", 3 ) == 0 )
		return 1;
	if( my_strncmp( expr, "SUM", 3 ) == 0 )
		return 1;
	return 0;
}

void calc( char *expr, int row, int col )
{
	int r1, c1, r2, c2;
	int i;
	char *start;
	char *end;
	char temp[20];

	start = expr+4;
	c1 = *start - 'A';
	start++;
	end = my_strchr(start, ',' );
	*end = 0;
	r1 = my_atoi( start ) - 1;

	start = end+1;
	c2 = *start - 'A';
	start++;
	end = my_strchr(start, ')' );
	*end = 0;
	r2 = my_atoi( start ) - 1;

	printf("calc(), expr=%s  %d, %d, %d, %d\n", expr, r1, c1, r2, c2);


	if( my_strncmp( expr, "ADD", 3) == 0 )
	{
		if( is_expr(value_str[r1][c1]) )
			calc(value_str[r1][c1], r1, c1);

		if( is_expr(value_str[r2][c2]) )
			calc(value_str[r2][c2], r2, c2);
		value[row][col] = value[r1][c1] + value[r2][c2];		
	}
	else if( my_strncmp( expr, "SUB", 3) == 0 )
	{
		if( is_expr(value_str[r1][c1]) )
			calc(value_str[r1][c1], r1, c1);

		if( is_expr(value_str[r2][c2]) )
			calc(value_str[r2][c2], r2, c2);
		value[row][col] = value[r1][c1] - value[r2][c2];		
	}
	else if( my_strncmp( expr, "MUL", 3) == 0 )
	{
		if( is_expr(value_str[r1][c1]) )
			calc(value_str[r1][c1], r1, c1);

		if( is_expr(value_str[r2][c2]) )
			calc(value_str[r2][c2], r2, c2);
		value[row][col] = value[r1][c1] * value[r2][c2];		
	}
	else if( my_strncmp( expr, "DIV", 3) == 0 )
	{
		if( is_expr(value_str[r1][c1]) )
			calc(value_str[r1][c1], r1, c1);

		if( is_expr(value_str[r2][c2]) )
			calc(value_str[r2][c2], r2, c2);
		value[row][col] = value[r1][c1] / value[r2][c2];		
	}
	else if( my_strncmp( expr, "MAX", 3) == 0 )
	{
		int max = -10000;
		if( r1 == r2 )
		{
			for(i=c1; i<=c2; i++ )
			{
				if( is_expr(value_str[r1][i]) )
					calc(value_str[r1][i], r1, i);

				if( value[r1][i] > max )
					max = value[r1][i];
			}
		}
		else if( c1 == c2 )
		{
			for(i=r1; i<=r2; i++ )
			{
				if( is_expr(value_str[i][c1]) )
					calc(value_str[i][c1], i, c1);

				if( value[i][c1] > max )
					max = value[i][c1];
			}
		}
		value[row][col] = max;
	}
	else if( my_strncmp( expr, "MIN", 3) == 0 )
	{
		int min = 10000;
		if( r1 == r2 )
		{
			for(i=c1; i<=c2; i++ )
			{
				if( is_expr(value_str[r1][i]) )
					calc(value_str[r1][i], r1, i);

				if( value[r1][i] < min )
					min = value[r1][i];
			}
		}
		else if( c1 == c2 )
		{
			for(i=r1; i<=r2; i++ )
			{
				if( is_expr(value_str[i][c1]) )
					calc(value_str[i][c1], i, c1);

				if( value[i][c1] < min )
					min = value[i][c1];
			}
		}
		value[row][col] = min;
	}
	else if( my_strncmp( expr, "SUM", 3) == 0 )
	{
		int sum = 0;
		if( r1 == r2 )
		{
			for(i=c1; i<=c2; i++ )
			{
				if( is_expr(value_str[r1][i]) )
					calc(value_str[r1][i], r1, i);

				sum += value[r1][i];
			}
		}
		else if( c1 == c2 )
		{
			for(i=r1; i<=r2; i++ )
			{
				if( is_expr(value_str[i][c1]) )
					calc(value_str[i][c1], i, c1);

				sum += value[i][c1];
			}
		}
		value[row][col] = sum;
	}
	sprintf(value_str[row][col], "%d", value[row][col]);
}

void update(int value[MAXR][MAXC])
{
	int i=0; 
	int j=0; 
	for(i=0; i<Row; i++)
	{
		for(j=0; j<Col; j++)
		{ 
			if( my_isdigit(value_str[i][j][0]) )
			{
				value[i][j] = my_atoi(value_str[i][j]);
			}
		}
	}

	for(i=0; i<Row; i++)
	{
		for(j=0; j<Col; j++)
		{ 
			if( is_expr(value_str[i][j]) )
			{
				calc( value_str[i][j], i, j );
			}
		}
	}
}

void display(void)
{
	int i=0; 
	int j=0; 
	for(i=0; i<Row; i++)
	{
		for(j=0; j<Col; j++)
		{
			printf("%12s", value_str[i][j] ); 
		}
		printf("\n");
	}
}
void display_value(void)
{
	int i=0; 
	int j=0; 
	for(i=0; i<Row; i++)
	{
		for(j=0; j<Col; j++)
		{
			printf("%12d", value[i][j] ); 
		}
		printf("\n");
	}
}

int main()
{
	init(5, 5);
	set(3, 2, "5" );
	set(5, 2, "13" );
	set(1, 5, "2" );
	set(2, 1, "6" );
	set(1, 1, "-9" );
	set(3, 4, "9" );
	set(2, 2, "3" );
	set(2, 4, "7" );
	set(5, 3, "2" );
	set(3, 1, "5" );

	update(value);
	//set(4, 5, "ADD(B1,C1)" );

	set(3, 4, "-9" );
	set(1, 2, "SUM(A5,E5)" );
	set(4, 2, "8" );
	set(4, 5, "MUL(E2,E3)" );
	set(2, 2, "SUB(E4,B4)" );
	set(1, 4, "-9" );
	set(1, 5, "ADD(C2,A4)" );

	update(value);
	display();
	display_value();

	return 0;
}
