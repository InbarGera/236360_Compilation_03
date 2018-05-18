%{
/*      MOVED TO H FILE
#include <cstring>
#include <source.tab.hpp>

 */
#include "attributes.h"


/* MOVED TO H FILE
int string_to_num(char* input);
char* remove_double_quotes(char* input);
*/
%}

%option yylineno
%option noyywrap

%%

void 			            	return VOID;
int 			            	return INT;
byte 			            	return BYTE;
b  				            	return B;
bool  			               	return BOOL;
and  			            	return AND;
or 				                return OR;
not 			            	return NOT;
true 			    	       	return TRUE;
false  			          	    return FALSE;
return 				            return RETURN;
if 				              	return IF;
else			      	        return ELSE;
while 		        			return WHILE;
break		        			return BREAK;
; 	            				return SC;
, 		            			return COMMA;
( 		            			return LPAREN;
) 		            			return RPAREN;
{ 		            			return LBRACE;
} 		            			return RBRACE;
[ 			            		return LBRACK;
] 			            		return RBRACK;
= 				                return ASSIGN;
(==|!=)			        		return EQ_OP;
(<|>|<=|>=)		 	        	return RELOP;
(+|-)					        return PLUS_MINUS;
(/|*)				        	return MUL_DIV;
[a-zA-Z][a-zA-Z0-9]* 	        	{yylval = retVal(yytext, IS_ID);	return ID;}
(0|[1-9][0-9]*) 			        {yylval = retVal(yytext, IS_INT); return NUM;}
"([^\n\r\"\\]|\\[rnt"\\])+" 		{yylval.string = retVal(yytext, IS_STR); return STRING;}
//[^\r\n]*[\r|\n|\r\n]?			;
( \n|\r|\t)				;

%%
/*  MOVED TO THE H FILE
int string_to_num(char* input){
    int sum =0;
    while(input[0] != '\0'){
        sum = sum*10 + (input++[0] - '0');
    }
    return sum;
}

char* remove_double_quotes(char* input){
    char* res = malloc(strlen(input));
    strcpy(res,input+1);
    res[strlen(res) - 1] = '\0';
    return res;
}
 */

