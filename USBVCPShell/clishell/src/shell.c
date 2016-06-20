/**
 * @file:	shell.c
 * @author: Castello Oscar, Iorio Raffaele
 * @date:	12 June 2016
 *
 * @description: Implementation of the shell reader and the shell parser
 *               for the USBVCPShell project.
 *
 * @license: This work is licensed under the Creative Commons
 *           Attribution-NonCommercial-ShareAlike 4.0 International License.
 *           To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-sa/4.0/.
 */

#include "shell.h"

// Declare this populated array structure in your main
extern tShell_t tShellCommand[];

// Set this variable for change the shell PROMPT STRING
char PS1[MAX_PS1_SIZE] = TXTGRN "demo@vcpshell" TXTRST;

/**
 * @brief	This function read bytes on OUT endpoint until it not find a
 * 			\n or \r and it discard any escape character.
 * 			To work it require that SHGETCHS and SHPUTCHS macros are defined
 * 			with the correct corresponding funcion.
 *
 * @param	Buffer where copy the read chars
 * @retval	Return the number of read chars
 */
ssize_t scanLine(char *buffer)
{
	uint8_t chr = 0;
	uint32_t bufPtr = 0;

	while ((chr != '\n') && (chr != '\r') && (bufPtr < MAX_SHELL_SIZE)){
		if (SHGETCHS(&chr, 1) != 0){
			switch(chr){
				case '\e':
					SHGETCHS(&chr, 2);
					break;
				case '\r':
				case '\n':
					break;
				case '\b':
					if (bufPtr > 0){
						bufPtr--;
						SHPUTCHS("\b \b",3);
					}
					break;
				//case ' ':
				//	buffer[bufPtr++] = '\0';
				//	SHPUTCHS(&chr ,1);
				//	break;
				default:
					buffer[bufPtr++] = chr;
					SHPUTCHS(&chr ,1);
					break;
			}
		}
	}

	SHPUTCHS("\n\r" ,2);
	buffer[bufPtr] = '\0';

	return bufPtr;
}

/**
 * @brief	A small parser for the scanned command line. A simple tokenizer
 * 			split the command and the argument from the passed shell line
 * 			and after a simple scanner search the corresponding key in the
 * 			tShellCommand array and run the corresponding function.
 * 			Note: if tShellCommand is not defined this function don't work.
 *
 * @param	Buffer where read the shell line
 * @retval	Return the boolean result of operation
 */
int shellParser(char* shellstr)
{
	int i=0;
	char *strPtr = shellstr;

	int argn = 0;
	char *argv[MAX_ARGN];

	if (strcmp(strPtr,"\0")==0)
		return 0;

	// Tokenizer
	while(*strPtr!='\0'){
		argv[argn++]=strPtr;
		while((*strPtr!=' ') && (*strPtr!='\0')) strPtr++;
		*strPtr++='\0';
	}

	// Scanner
	while(strcmp(tShellCommand[i].sName,"\0")!=0){
		if(strcmp(tShellCommand[i].sName,argv[0])==0){
			tShellCommand[i].sFunc(tShellCommand[i].sCmd, argn, argv);
			return 1;
		}
		i++;
	}

	printf("Command not found. Type help for list of available command.\n\r");
	return 0;
}


