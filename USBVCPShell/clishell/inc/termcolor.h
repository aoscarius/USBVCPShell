/**
 * @file:	termcolor.h
 * @author: Castello Oscar, Iorio Raffaele
 * @date:	12 June 2016
 *
 * @description: This is header contain the declaration of the macro for the POSIX terminal
 *               escape code, for color printf.
 *
 * @license: This work is licensed under the Creative Commons
 *           Attribution-NonCommercial-ShareAlike 4.0 International License.
 *           To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-sa/4.0/.
 */

#ifndef TERMCOLOR_H_
#define TERMCOLOR_H_

// Terminal Color
// Regular
#define TXTBLK "\e[0;30m" // Nero
#define TXTRED "\e[0;31m" // Rosso
#define TXTGRN "\e[0;32m" // Verde
#define TXTYLW "\e[0;33m" // Giallo
#define TXTBLU "\e[0;34m" // Blu
#define TXTPUR "\e[0;35m" // Viola
#define TXTCYN "\e[0;36m" // Ciano
#define TXTWHT "\e[0;37m" // Bianco

// Bold
#define BLDBLK "\e[1;30m" // Nero
#define BLDRED "\e[1;31m" // Rosso
#define BLDGRN "\e[1;32m" // Verde
#define BLDYLW "\e[1;33m" // Giallo
#define BLDBLU "\e[1;34m" // Blu
#define BLDPUR "\e[1;35m" // Viola
#define BLDCYN "\e[1;36m" // Ciano
#define BLDWHT "\e[1;37m" // Bianco

// Underline
#define UNDBLK "\e[4;30m" // Nero
#define UNDRED "\e[4;31m" // Rosso
#define UNDGRN "\e[4;32m" // Verde
#define UNDYLW "\e[4;33m" // Giallo
#define UNDBLU "\e[4;34m" // Blu
#define UNDPUR "\e[4;35m" // Viola
#define UNDCYN "\e[4;36m" // Ciano
#define UNDWHT "\e[4;37m" // Bianco

// Background
#define BAKBLK "\e[40m" // Nero
#define BAKRED "\e[41m" // Rosso
#define BAKGRN "\e[42m" // Verde
#define BAKYLW "\e[43m" // Giallo
#define BAKBLU "\e[44m" // Blu
#define BAKPUR "\e[45m" // Viola
#define BAKCYN "\e[46m" // Ciano
#define BAKWHT "\e[47m" // Bianco

// High Intensty
#define ITXTBLK "\e[0;90m" // Nero
#define ITXTRED "\e[0;91m" // Rosso
#define ITXTGRN "\e[0;92m" // Verde
#define ITXTYLW "\e[0;93m" // Giallo
#define ITXTBLU "\e[0;94m" // Blu
#define ITXTPUR "\e[0;95m" // Viola
#define ITXTCYN "\e[0;96m" // Ciano
#define ITXTWHT "\e[0;97m" // Bianco

// Bold High Intensty
#define IBLDBLK "\e[1;90m" // Nero
#define IBLDRED "\e[1;91m" // Rosso
#define IBLDGRN "\e[1;92m" // Verde
#define IBLDYLW "\e[1;93m" // Giallo
#define IBLDBLU "\e[1;94m" // Blu
#define IBLDPUR "\e[1;95m" // Viola
#define IBLDCYN "\e[1;96m" // Ciano
#define IBLDWHT "\e[1;97m" // Bianco

// Underline High Intensty
#define IUNDBLK "\e[4;90m" // Nero
#define IUNDRED "\e[4;91m" // Rosso
#define IUNDGRN "\e[4;92m" // Verde
#define IUNDYLW "\e[4;93m" // Giallo
#define IUNDBLU "\e[4;94m" // Blu
#define IUNDPUR "\e[4;95m" // Viola
#define IUNDCYN "\e[4;96m" // Ciano
#define IUNDWHT "\e[4;97m" // Bianco

// High Intensty backgrounds
#define IBAKBLK "\e[100m" // Nero
#define IBAKRED "\e[101m" // Rosso
#define IBAKGRN "\e[102m" // Verde
#define IBAKYLW "\e[103m" // Giallo
#define IBAKBLU "\e[104m" // Blu
#define IBAKPUR "\e[105m" // Viola
#define IBAKCYN "\e[106m" // Ciano
#define IBAKWHT "\e[107m" // Bianco

// Reset
#define TXTRST "\e[0m" // Text

#endif /* TERMCOLOR_H_ */
