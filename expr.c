#include <math.h>
#include <stdio.h>
#include <string.h>

#define MAXLEN 100

char buf[MAXLEN];
int pos;

char top()
{
	return buf[pos];
}

char next()
{
	return buf[pos++];
}

double literal();
double atom();
double addexpr();
double mulexpr();

double literal()
{
	double res;
	int n;
	
	sscanf(&buf[pos], "%lf%n", &res, &n);
	pos += n;
	
	return res;
}

double atom()
{
	double res = NAN;
	
	if(isdigit(top())) {
		res = literal();
	} else if(top() == '(') {
		next();
		res = addexpr();
		next();
	} else if(top() == '-') {
		next();
		res = -atom();
	}
	
	return res;
}

double mulexpr()
{
	double res = atom();
	
	while(top() == '*' || top() == '/') {
		if(next() == '*') {
			res *= atom();
		} else {
			res /= atom();
		}
	}
	
	return res;
}

double addexpr()
{
	double res = mulexpr();
	
	while(top() == '+' || top() == '-') {
		if(next() == '+') {
			res += mulexpr();
		} else {
			res -= mulexpr();
		}
	}
	
	return res;
}

double evaluate(char *expr)
{
	int i = 0;
	char ch;
	
	memset(&buf[0], 0, MAXLEN);
	pos = 0;
	
	while((ch = *expr++) != '\0') {
		if(!isspace(ch)) {
			buf[i++] = ch;
		}
	}
	
	return addexpr();
}
