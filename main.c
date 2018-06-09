#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define STACK_SIZE 200
/*
	functions for use stack with int value, for char defined operations
*/
int op2int(char operation)
{
	switch(operation)
	{
		case '+': return 0; break;
		case '-': return 1; break;
		case '*': return 2; break;
		case '/': return 3; break;
		case '(': return 4; break;
		case ')': return 5; break;
	}
}

char* int2op(int operation)
{
	switch(operation)
	{
		case 0: return "+"; break;
		case 1: return "-"; break;
		case 2: return "*"; break;
		case 3: return "/"; break;
		case 4: return "("; break;
		case 5: return ")"; break;
	}
}

int getPriority(int op)
{
	switch(op)
	{
		case 0: return 0; break;// +
		case 1: return 0; break;// -
		case 2: return 1; break;// *
		case 3: return 1; break;// /
		default: return -1;break;
	}
}

int variables[26];//variable values
int isChanged[26];//is program used this variable

//stack define
typedef struct { 
	int sp;
	int data[STACK_SIZE];
}stack;

/*stack functions*/
void stack_init(stack* s)
{
	s->sp = 0;
}
void stack_push(stack *s, int val)
{
	if(s->sp != STACK_SIZE)
		s->data[(s->sp)++] = val;
}
int stack_pop(stack *s)
{
	return (s->sp != 0) ? (s->data[--(s->sp)]) : INT_MAX; //2,147,483,647 : you can not use this value in your calculations.
														  //this value(INT_MAX) reserved for indicate stack is empty. 
}
int stack_peak(stack *s)
{
	return (s->sp != 0) ? (s->data[s->sp - 1]) : INT_MAX;	
}
void stack_print(stack *s)
{
	int i =0; ;
	while(i != s->sp)
		printf(" %s ",int2op(s->data[i++]));
}
/*end of stack functions*/


/**
	this function transform infix formed operation to postfix form.
**/
char * infix2postfix(char* operation)
{
	
	operation[strlen(operation)-2] = '\0';
	stack s;
	stack_init(&s);
	char* postfix = (char*)malloc(sizeof(char)*200);
	strcpy(postfix,"");
	char * token = strtok(operation," ");
	while(token)
	{
		
		if ((token[0] >= 'a' && token[0] <= 'z') || (token[0] >= '0' && token[0] <= '9'))
		{
			strcat(postfix, token);
			strcat(postfix, " ");
		}
		else if(token[0] == '(')
		{
			stack_push(&s, op2int(token[0]));
		}
		else if(token[0] == ')')
		{
			while(stack_peak(&s) != op2int('('))
			{
				int ope = stack_pop(&s);
				strcat(postfix, int2op(ope));
				strcat(postfix, " ");
			}
			stack_pop(&s);
		}
		else 
		{
			while(getPriority(stack_peak(&s)) >= getPriority(op2int(token[0])))
			{
				int ope = stack_pop(&s);
				strcat(postfix, int2op(ope));
				strcat(postfix, " ");
			}
			stack_push(&s,op2int(token[0]));
		}
		token = strtok(NULL, " ");
	}
	while(stack_peak(&s) != INT_MAX)
	{
		int ope = stack_pop(&s);
		strcat(postfix, int2op(ope));
		strcat(postfix, " ");
	}
	postfix[strlen(postfix)-1] = '\0';
	return postfix;
}
int calculate(int a, int b, char o)
{
	switch(o)
	{
		case '+': return a+b; break;
		case '-': return a-b; break;
		case '*': return a*b; break;
		case '/': return a/b; break;
	}
}
int evaluate(char* postfix)
{
	stack s;
	stack_init(&s);
	char * token = strtok(postfix," ");
	while(token)
	{
		
		if(token[0] >= 'a' && token[0] <= 'z')
		{
			if(isChanged[token[0]-'a'])
			{
				stack_push(&s, variables[token[0]-'a']);
			}
			else
			{
				printf("%c", token[0]);
				return INT_MAX;
			}
		}
		else if(token[0] >= '0' && token[0] <= '9')
		{
			stack_push(&s, atoi(token));
		}
		else
		{
			stack_push(&s,calculate(stack_pop(&s), stack_pop(&s), token[0]));
		}
		token = strtok(NULL, " ");
	}
	return stack_pop(&s);
}
void readFile(char* fname)
{
	FILE *f = fopen(fname, "r");
	if(!f)
	{
		printf("couldn't open %s file.\n", fname);
		exit(1);
	}
	char buffer[200];
	while(!feof(f))
	{
		strcpy(buffer,"");
		fgets(buffer, 200, f);
		printf("Operation: %s",buffer);
		char* op = infix2postfix(&buffer[4]);
		printf("Postfix: %s\n", op);
	    int result = evaluate(op);
	    free(op);
	    if(result != INT_MAX)
	    {
	    	variables[buffer[0]-'a'] = result;
	    	isChanged[buffer[0]-'a'] = 1;
			printf("%c <- %d\n\n", buffer[0], variables[buffer[0]-'a']);
		}
		else
		{
			printf(" is undeclared.\n\n");
		}

	}
	fclose(f);
}
void print_variables()
{
	printf("\n");
	int i = 0;
	for(;i<26;i++)
	{
		if(isChanged[i])
		{
			printf("%c <- %d \n", 'a'+i, variables[i]);
		}
	}
}
int main(int argc, char **argv)
{	
	readFile("input.txt");
	print_variables();
	return 0;
}
