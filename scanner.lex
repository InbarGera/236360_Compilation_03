%{

#include "attributes.h"
#include "parser.tab.hpp"

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
\( 		            			return LPAREN;
\) 		            			return RPAREN;
\{ 		            			return LBRACE;
\} 		            			return RBRACE;
\[ 			            		return LBRACK;
\] 			            		return RBRACK;
= 				                return ASSIGN;
(==|!=)			        		return EQ_OP;
(<|>|<=|>=)		 	        	return RELOP;
[+-]					        return PLUS_MINUS;
[\/\*]				        	return MUL_DIV;
[a-zA-Z][a-zA-Z0-9]* 	        	{yylval = new parsedData(yytext, IS_ID);  return ID;}
(0|[1-9][0-9]*) 			        {yylval = new parsedData(yytext, IS_INT); return NUM;}
\"([^\n\r\"\\]|\\[rnt"\\])+\" 		{yylval = new parsedData(yytext, IS_STR); return STRING;}
\/\/[^\r\n]*[\r|\n|\r\n]?			    {}
[ \n|\r|\t]				{}

%%
