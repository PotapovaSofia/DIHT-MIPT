#include <stdio.h>
#include <stdlib.h>
#include <string.h>



 int main()
 {
     int stack[1000];
     // sp = индекс €чейки, куда будет push-итьс€ очередное число
     int sp =0;      // (sp-1) = индекс €чейки, €вл€ющейс€ вершиной стека

     char c;
     c = getchar();


	 do {

         int x;
         switch (c)
		 {
             case  ' ':
             case '\n':
                 break;
             case '=':
                 printf("%d\n", stack[sp - 1]);  sp--;
                 break;
             case '+':
                stack[sp-2] = stack[sp-2] + stack[sp-1];  sp--;
                break;
             case '-':
                stack[sp-2] = stack[sp-2] - stack[sp-1];  sp--;
                break;
             case '*':
                stack[sp-2] = stack[sp-1] * stack[sp-2];  sp--;
                break;
             case '/':
               stack[sp-2] = stack[sp-2] / stack[sp-1];   sp--;
                break;

             default:
                     stack[sp] = c-'0'; sp++;
         }
         char c = getchar();
     } while ( с != '\0');
     return 0;
 }
