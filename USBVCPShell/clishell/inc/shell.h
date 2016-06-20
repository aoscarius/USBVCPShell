/**
 * @file:	shell.h
 * @author: Castello Oscar, Iorio Raffaele
 * @date:	12 June 2016
 *
 * @description: Header file with prototype of the shell function
 *               and declaration of the exported type and configuration macro.
 *
 * @license: This work is licensed under the Creative Commons
 *           Attribution-NonCommercial-ShareAlike 4.0 International License.
 *           To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-sa/4.0/.
 */

#ifndef SHELL_H_
#define SHELL_H_

#include "stdint.h"
#include "termcolor.h"

#include "usbd_cdc_vcp.h"

// Change this macro with your correct Read and Write chars functions
#define SHGETCHS(buf, len) VCP_Read(buf, len)
#define SHPUTCHS(buf, len) VCP_Write(buf, len)

// Stack size for the shell task (remeber that it call all other function)
#define SHELL_STACK_SIZE	512

#define MAX_PS1_SIZE	32
#define MAX_SHELL_SIZE	126
extern char PS1[MAX_PS1_SIZE];

#define MAX_NAME_LEN	32
#define MAX_DESC_LEN	32
#define MAX_ARGN		32

// Typedef struct of shell command scanner line
typedef struct {
	char sName[MAX_NAME_LEN];
	void (*sFunc)(int cmd, int argn, char *argv[]);
	int sCmd;
	char sDesc[MAX_DESC_LEN];
} tShell_t;

extern tShell_t tShellCommand[];

ssize_t scanLine(char *buffer);
int shellParser(char* shellstr);

#endif /* INC_SHELL_H_ */
