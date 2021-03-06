/*
 * Exercise 4-5. Add access to library functions like sin, exp, and pow. See
 * <math.h> in Appendix B, Section 4.
 */

#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "sol.h"

#define MAXOP 100

void rpn(void);
int getop(char []);
void push(double);
double pop(void);
int getch(void);
void ungetch(int c);
double atof2(char s[]);
void add(void);
void multiply(void);
void modulus(void);
void subtract(void);
void divide(void);
void peek(void);
void duplicate(void);
void clear(void);
void swap(void);
void sine(void);
void power(void);
void exponential(void);

void rpn(void)
{
   int type;
   double op2;
   double op3;
   char s[MAXOP];

   while ((type = getop(s)) != EOF) {
      switch(type) {
         case NUMBER:
            push(atof2(s));
            break;
         case '+':
            add(); break;
         case '*':
            multiply(); break;
         case '-':
            subtract(); break;
         case '/':
            divide(); break;
         case '%':
            modulus(); break;
         case PEEK:
            peek(); break;
         case DUPLICATE:
            duplicate(); break;
         case CLEAR:
            clear(); break;
         case SWAP:
            swap(); break;
         case SIN:
            sine(); break;
         case POW:
            power(); break;
         case EXP:
            exponential(); break;
         case '\n':
            printf("\t%.8g\n", pop());
            break;
         default:
            printf("error: unknown command %s\n", s);
            break;
      }
   }
}

#define MAXVAL 100
int sp = 0;
double val[MAXVAL];

void push(double f)
{
   if (sp < MAXVAL)
      val[sp++] = f;
   else
      printf("error: stack full, can't push %g\n", f);
}

double pop(void)
{
   if (sp > 0)
      return val[--sp];
   else {
      printf("error: stack empty\n");
      return 0.0;
   }
}

void add(void)
{
   push(pop() + pop());
}

void multiply(void)
{
   push(pop() * pop());
}

void subtract(void)
{
   double op2;
   op2 = pop();
   push(pop() - op2);
}

void divide(void)
{
   double op2 = pop();
   if (op2 != 0.0)
      push(pop() / op2);
   else
      printf("error: zero divisor\n");
}

void modulus(void)
{
   double op2;
   op2 = pop();
   if (op2 != 0.0)
      push((int) pop() % (int) op2);
   else
      printf("error: zero divisor\n");
}

void clear(void)
{
   sp = 0;
}

void peek(void)
{
   if (sp > 0)
      printf("\t%.8g\n", val[sp-1]);
   else {
      printf("error: stack empty\n");
   }
}

void duplicate(void)
{
   double op2;
   op2 = pop();
   push(op2);
   push(op2);
}

void swap(void)
{
   double op2, op3;
   op2 = pop();
   op3 = pop();
   push(op2);
   push(op3);
}

void sine(void)
{
   push(sin(pop()));
}

void power(void)
{
   double op2 = pop();
   push(pow(pop(), op2));
}

void exponential(void)
{
   push(exp(pop()));
}

int getop(char s[])
{
   int i, c;

   while ((s[0] = c = getch()) == ' ' || c == '\t')
      ;
   s[1] = '\0';

   i = 0;
   if (!isdigit(c) && c != '.' && c != '-') {
      if (!islower(c))
         return c;

      while (islower(s[++i] = c = getch()))
         ;
      s[i] = '\0';
      if (c != EOF)
         ungetch(c);
      if (strlen(s) == 1) {
         return c;
      }
      else if (strcmp(s, "peek") == 0)
         return PEEK;
      else if (strcmp(s, "duplicate") == 0)
         return DUPLICATE;
      else if (strcmp(s, "clear") == 0)
         return CLEAR;
      else if (strcmp(s, "swap") == 0)
         return SWAP;
      else if (strcmp(s, "sin") == 0)
         return SIN;
      else if (strcmp(s, "exp") == 0)
         return EXP;
      else if (strcmp(s, "pow") == 0)
         return POW;
   }

   i = 0;
   if (c == '-')
      s[++i] = c = getch();
      if (!isdigit(c)) {
         ungetch(c);
         return c;
      }
   if (isdigit(c))
      while (isdigit(s[++i] = c = getch()))
         ;
   if (c == '.')
      while (isdigit(s[++i] = c = getch()))
         ;
   s[i] = '\0';
   if (c != EOF)
      ungetch(c);
   return NUMBER;
}

#define BUFSIZE 100

char buf[BUFSIZE];
int bufp = 0;

int getch(void)
{
   return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c)
{
   if (bufp >= BUFSIZE)
      printf("ungetch: too many characters\n");
   else
      buf[bufp++] = c;
}

double atof2(char s[])
{
   double val, power, result;
   int i, sign, exp, expSign;
   for (i = 0; isspace(s[i]); i++)
      ;
   sign = (s[i] == '-') ? -1 : 1;
   if (s[i] == '+' || s[i] == '-')
      i++;
   for (val = 0.0; isdigit(s[i]); i++)
      val = 10.0 * val + (s[i] - '0');
   if (s[i] == '.')
      i++;
   for (power = 1.0; isdigit(s[i]); i++) {
      val = 10.0 * val + (s[i] - '0');
      power *= 10.0;
   }
   
   if (s[i] == 'e' || s[i] == 'E')
      i++;
   expSign = (s[i] == '-') ? -1 : 1;
   if (s[i] == '+' || s[i] == '-')
      i++;
   for (exp = 0; isdigit(s[i]); i++) {
      exp = 10 * exp + (s[i] - '0');
   }

   result = sign * val / power;

   if (exp)
      result *= pow(10, expSign * exp);

   return result;
}

int getline2(char s[], int lim)
{
   int c, i; for (i = 0; i < lim - 1 && (c=getchar()) != EOF && c != '\n'; i++) {
      s[i] = c;
   }
   if (c == '\n') {
      s[i] = c;
      ++i;
   }
   s[i] = '\0';
   return i;
}

void copy(char to[], char from[])
{
   int i = 0;
   while ((to[i] = from[i]) != '\0') {
      ++i;
   }
}
