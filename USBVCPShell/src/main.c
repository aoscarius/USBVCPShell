/**
 * @file:	main.c
 * @author: Castello Oscar, Iorio Raffaele
 * @date:	12 June 2016
 *
 * @description: This is a simple shell project for the STM32F4-Discovery
 *               board, that use the functionality of the USB-CDC class for
 *               implement a simple UART Terminal and FreeRTOS for the multitasking
 *               functionality.
 *
 * @license: This work is licensed under the Creative Commons
 *           Attribution-NonCommercial-ShareAlike 4.0 International License.
 *           To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-sa/4.0/.
 */

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_gpio.h"
#include "lis3dsh.h"

#include "init_function.h"
#include "shell.h"

#include "cmsis_os.h"

#define LED_GREEN	GPIO_PIN_12	// LED Verde (Left)
#define LED_ORANGE 	GPIO_PIN_13	// LED Arancio (Up)
#define LED_RED		GPIO_PIN_14	// LED Rosso (Right)
#define LED_BLUE 	GPIO_PIN_15	// LED Blu (Down)
#define LEDS 		(LED_GREEN | LED_ORANGE | LED_RED | LED_BLUE)

// Shell task prototype and task structure definitions
void taskSHELL(void const *args);
osThreadDef(SHELL, taskSHELL, osPriorityNormal, 1, SHELL_STACK_SIZE);
void taskLEDON(void const *args);
osThreadDef(LEDON, taskLEDON, osPriorityNormal, 1, configMINIMAL_STACK_SIZE);
void taskLEDONargs(void const *args);
osThreadDef(LEDONargs, taskLEDONargs, osPriorityNormal, 1, configMINIMAL_STACK_SIZE);
void taskLEDOFF(void const *args);
osThreadDef(LEDOFF, taskLEDOFF, osPriorityNormal, 1, configMINIMAL_STACK_SIZE);
void taskLEDOFFargs(void const *args);
osThreadDef(LEDOFFargs, taskLEDOFFargs, osPriorityNormal, 1, configMINIMAL_STACK_SIZE);
void taskINITGPIO(void const *args);
osThreadDef(INITGPIO, taskINITGPIO, osPriorityNormal, 1, configMINIMAL_STACK_SIZE);
void taskMEMST(void const *args);
osThreadDef(MEMST, taskMEMST, osPriorityNormal, 1, configMINIMAL_STACK_SIZE);

// Shell command prototype definitions
void cmdHelp(int cmd, int argn, char *argv[]);
void cmdList(int cmd, int argn, char *argv[]);
void cmdEcho(int cmd, int argn, char *argv[]);
void cmdTaskLaunch(int cmd, int argn, char *argv[]);
void cmdCleanup(int cmd, int argn, char *argv[]);

/* Shell command list exported structure
 * Note: Without this definition the shell will not found.
 */
tShell_t tShellCommand[] = {
	{"help", 		cmdHelp,  			0,		"Print the current list"},
	{"list", 		cmdList,  			0,		"Show current task list"},
	{"echo", 		cmdEcho,  			0,		"Echo all character passed"},
	{"cleanup", 	cmdCleanup,  		0,		"Active the cleanup process"},
	{"ledsON", 		cmdTaskLaunch,  	0,		"Switch ON all leds"},
	{"ledsOFF", 	cmdTaskLaunch,		1,		"Switch OFF all leds"},
	{"initGPIO", 	cmdTaskLaunch,		2,		"Initlialize leds GPIO"},
	{"ledON",		cmdTaskLaunch,		3,		"[N] swhitch ON leds list N"},
	{"ledOFF",		cmdTaskLaunch,		4,		"[N] swhitch OFF leds list N"},
	{"activeMEMS",	cmdTaskLaunch,		5,		"Active the MEMS driver"},
	{"\0", 			NULL, 				0,		"\0"}
};

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
	/* STM32F4xx HAL library initialization:
	- Configure the Flash prefetch, instruction and Data caches
	- Configure the Systick to generate an interrupt each 1 msec
	- Set NVIC Group Priority to 4
	- Global MSP (MCU Support Package) initialization
	*/
	HAL_Init();
	//GPIOLED_Init();

	// Setting of the correct clock configuration
	SystemClock_Config();
	// Setting and enable USB CDC Interface
	USBConfig();

	// This delay stay for a pause between the plug-in and the run of a terminal
	HAL_Delay(2000);
	printf("Welcome to " BLDRED "SE2Group RTOS Shell" TXTRST " command.\n\r"); fflush(stdout);

	osThreadCreate(osThread(SHELL), NULL);
	osKernelStart();

	// This print is execute if the kernel scheduler stop
	printf("System shutdown!\n\r");
	/* Infinite loop */
	while (1){
	}
}

/* ------- Tasks definitions ------- */

/**
  * @brief  Main task, the shell task. It's the responsable of the command
  * 		parsing and the run of any other task and command. It use the clishell
  * 		functions
  * @param  Undefined
  * @retval None
  */
void taskSHELL(void const *args){
	char str_line[MAX_SHELL_SIZE];
	memset(str_line,0,MAX_SHELL_SIZE);

	while (1)
	{
		printf("%s $> ", PS1); fflush(stdout);
		scanLine(str_line);
		shellParser(str_line);
	}

	vTaskDelete(osThreadGetId());
}

/**
  * @brief  An example task that set all LED of the board to ON. Require the
  *         activation of the taskINITGPIO first.
  * @param  Undefined
  * @retval None
  */
void taskLEDON(void const *args){
	HAL_GPIO_WritePin(GPIOD, LEDS, GPIO_PIN_SET);

	vTaskDelete(osThreadGetId());
}

/**
  * @brief  A sample task that set che LED passed with args to ON on the board.
  *         Require the activation of the taskINITGPIO first.
  * @param  LED to set ON
  * @retval None
  */
void taskLEDONargs(void const *args){
	uint32_t led;
	switch((int)args){
		case 0:
			led=LED_GREEN;
			break;
		case 1:
			led=LED_ORANGE;
			break;
		case 2:
			led=LED_RED;
			break;
		case 3:
			led=LED_BLUE;
			break;
		default:break;
	}

	HAL_GPIO_WritePin(GPIOD, led, GPIO_PIN_SET);

	vTaskDelete(osThreadGetId());
}

/**
  * @brief  Specular task of taskLEDON
  * @param  Undefined
  * @retval None
  */
void taskLEDOFF(void const *args){
	HAL_GPIO_WritePin(GPIOD, LEDS, GPIO_PIN_RESET);

	vTaskDelete(osThreadGetId());
}

/**
  * @brief  Specular task of taskLEDONargs
  * @param  LED to reset OFF
  * @retval None
  */
void taskLEDOFFargs(void const *args){
	uint32_t led;
	switch((int)args){
		case 0:
			led=LED_GREEN;
			break;
		case 1:
			led=LED_ORANGE;
			break;
		case 2:
			led=LED_RED;
			break;
		case 3:
			led=LED_BLUE;
			break;
		default:break;
	}

	HAL_GPIO_WritePin(GPIOD, led, GPIO_PIN_RESET);

	vTaskDelete(osThreadGetId());
}

/**
  * @brief  A sample task to initialize the LED GPIO on the board.
  * @param  Undefined
  * @retval None
  */
void taskINITGPIO(void const *args){
	GPIO_InitTypeDef GPIO_InitStruct;

	/* Enable the GPIO_LED Clock */
	__GPIOD_CLK_ENABLE();

	/* Configure the GPIO_LED pins */
	GPIO_InitStruct.Pin = LEDS;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FAST;

	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	HAL_GPIO_WritePin(GPIOD, LEDS, GPIO_PIN_RESET);

	vTaskDelete(osThreadGetId());
}

/**
  * @brief  A sample task to demostrate the activation of a driver at runtime,
  *         with the initialization process and the periodic execution of it.
  *         This sample, initialize the Gyro MEMS on the board and after, 
  *			every 10 seconds it print one sample from this device.
  * @param  Undefined
  * @retval None
  */
void taskMEMST(void const *args){
	int16_t accXYZBuff[3];
	uint8_t temp;

	uint16_t ctrl = 0x0000;
	LIS3DSH_InitTypeDef l1s3dsh_InitStruct;

	/* Set configuration of LIS3DSH MEMS Accelerometer **********************/
	l1s3dsh_InitStruct.Output_DataRate = LIS3DSH_DATARATE_100;
	l1s3dsh_InitStruct.Axes_Enable = LIS3DSH_XYZ_ENABLE;
	l1s3dsh_InitStruct.SPI_Wire = LIS3DSH_SERIALINTERFACE_4WIRE;
	l1s3dsh_InitStruct.Self_Test = LIS3DSH_SELFTEST_NORMAL;
	l1s3dsh_InitStruct.Full_Scale = LIS3DSH_FULLSCALE_6;
	l1s3dsh_InitStruct.Filter_BW = LIS3DSH_FILTER_BW_800;

	/* Configure MEMS: power mode(ODR) and axes enable */
	ctrl = (uint16_t) (l1s3dsh_InitStruct.Output_DataRate | \
				   l1s3dsh_InitStruct.Axes_Enable);

	/* Configure MEMS: full scale and self test */
	ctrl |= (uint16_t) ((l1s3dsh_InitStruct.SPI_Wire    | \
					 l1s3dsh_InitStruct.Self_Test   | \
					 l1s3dsh_InitStruct.Full_Scale  | \
					 l1s3dsh_InitStruct.Filter_BW) << 8);

	/* Configure the accelerometer main parameters */
	LIS3DSH_Init(ctrl);

	while (1){
		//osDelay(10000); // This suspend the task for the 10000 seconds 
		HAL_Delay(10000);
		LIS3DSH_ReadACC(accXYZBuff);
		ACCELERO_IO_Read(&temp,LIS3DSH_OUT_T_ADDR,1);
		printf("\r\nMEMS: [X]:%d [Y]:%d [Z]:%d [T]:%d°C\n\r", accXYZBuff[0], accXYZBuff[1], accXYZBuff[2], (int8_t)(25+temp)); ; fflush(stdout);
	}

	vTaskDelete(osThreadGetId());
}

/* ------- Commands definitions ------- */
/**
  * @brief	This function create the task at runtime and insert it in the
  * 		active task list of the scheduler. This command is scheduled with the
  * 		shell task that call it.
  * @param  cmd is used for define which task run
  * @param	argn number of argument passed from the shell
  * @param	argv list of argument passed from the shell
  * @retval
  */
void cmdTaskLaunch(int cmd, int argn, char *argv[]){
	int i;
	switch(cmd){
		case 0:
			osThreadCreate(osThread(LEDON), NULL);
			break;
		case 1:
			osThreadCreate(osThread(LEDOFF), NULL);
			break;
		case 2:
			osThreadCreate(osThread(INITGPIO), NULL);
			break;
		case 3:
			for (i=1; i<argn; i++){
				osThreadCreate(osThread(LEDONargs), (void*)atoi(argv[i]));
			}
			break;
		case 4:
			for (i=1; i<argn; i++){
				osThreadCreate(osThread(LEDOFFargs), (void*)atoi(argv[i]));
			}
			break;
		case 5:
			osThreadCreate(osThread(MEMST), NULL);
			break;
		default:
			break;
	}
}

/**
  * @brief  This function print a list of active and non active task present in 
  *         the list of the FreeRTOS scheduler. It depend from vTaskList() function
  *         of the FreeRTOS.
  * @param  Undefined
  * @retval None
  */
void cmdList(int cmd, int argn, char *argv[]){
	char str[1024];
	printf("Name\t\tState\tPrio\tStack\tNumID\n\r");
	printf("----\t\t-----\t----\t-----\t-----\n\r");
	vTaskList(str);
	printf("%s\n\r",str);
}

/**
  * @brief  Simple function to print the list of all avaiable command in the shell.
  * @param  Undefined
  * @retval None
  */
void cmdHelp(int cmd, int argn, char *argv[]){
	int i=0;
	while(strcmp(tShellCommand[i].sName,"\0")!=0){
		printf("%s - %s\n\r",tShellCommand[i].sName,tShellCommand[i].sDesc);
		i++;
	}
}

/**
  * @brief  Dummy echo command. It's for joke.
  * @param  Undefined
  * @retval None
  */
void cmdEcho(int cmd, int argn, char *argv[]){
	int i;
	for (i=1; i<argn; i++){
		printf("%s ",argv[i]);
	}
	printf("\r\n");
}

/**
  * @brief  This function set the priority of the IDLE task at same level of the shell
  *         task for almost 1 second and after reset it to low priority. This operation
  *         permit to the IDLE task of run and cleanup of the deleted task list and cleanup 
  *         of unecessary resource.
  * @param  Undefined
  * @retval None
  */
void cmdCleanup(int cmd, int argn, char *argv[]){
	osThreadSetPriority((osThreadId)xTaskGetIdleTaskHandle(),osPriorityNormal);
	HAL_Delay(1000);
	osThreadSetPriority((osThreadId)xTaskGetIdleTaskHandle(),osPriorityIdle);
}

