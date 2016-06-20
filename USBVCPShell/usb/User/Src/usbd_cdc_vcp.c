/**
  ******************************************************************************
  * @file    usbd_cdc_vcp.c
  * @author  MCD Application Team
  * @version V2.2.0
  * @date    13-June-2014
  * @brief   Generic media access Layer.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include <usbd_cdc_vcp.h>

extern USBD_HandleTypeDef hUSBDDevice;

static struct
{
    uint8_t Buffer[CDC_DATA_HS_OUT_PACKET_SIZE];
    int Position, Size;
    char ReadDone;
} s_RxBuffer;

char g_VCPInitialized;

static int8_t VCP_Init    (void);
static int8_t VCP_DeInit  (void);
static int8_t VCP_Control (uint8_t cmd, uint8_t* pbuf, uint16_t len);
static int8_t VCP_Receive (uint8_t* pbuf, uint32_t *len);

USBD_CDC_ItfTypeDef USBD_CDC_VCP_fops =
{
	VCP_Init,
	VCP_DeInit,
	VCP_Control,
	VCP_Receive
};

USBD_CDC_LineCodingTypeDef linecoding =
{
	115200, /* baud rate*/
	0x00,   /* stop bits-1*/
	0x00,   /* parity - none*/
	0x08    /* nb. of bits 8*/
};

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  VCP_Init
  *         Initializes the CDC media low layer
  * @param  None
  * @retval Result of the opeartion: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t VCP_Init(void)
{
    USBD_CDC_SetRxBuffer(&hUSBDDevice, s_RxBuffer.Buffer);
    g_VCPInitialized = 1;
    return (0);
}

/**
  * @brief  VCP_DeInit
  *         DeInitializes the CDC media low layer
  * @param  None
  * @retval Result of the opeartion: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t VCP_DeInit(void)
{
	return USBD_OK;
}


/**
  * @brief  VCP_Control
  *         Manage the CDC class requests
  * @param  cmd: Command code
  * @param  pbuf: Buffer containing command data (request parameters)
  * @param  len: Number of data to be sent (in bytes)
  * @retval Result of the opeartion: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t VCP_Control  (uint8_t cmd, uint8_t* pbuf, uint16_t len)
{ 
	switch (cmd){
		case CDC_SEND_ENCAPSULATED_COMMAND:
			break;

		case CDC_GET_ENCAPSULATED_RESPONSE:
			break;

		case CDC_SET_COMM_FEATURE:
			break;

		case CDC_GET_COMM_FEATURE:
			break;

		case CDC_CLEAR_COMM_FEATURE:
			break;

		case CDC_SET_LINE_CODING:
			linecoding.bitrate    = (uint32_t)(pbuf[0] | (pbuf[1] << 8) |\
									(pbuf[2] << 16) | (pbuf[3] << 24));
			linecoding.format     = pbuf[4];
			linecoding.paritytype = pbuf[5];
			linecoding.datatype   = pbuf[6];

			break;

		case CDC_GET_LINE_CODING:
			pbuf[0] = (uint8_t)(linecoding.bitrate);
			pbuf[1] = (uint8_t)(linecoding.bitrate >> 8);
			pbuf[2] = (uint8_t)(linecoding.bitrate >> 16);
			pbuf[3] = (uint8_t)(linecoding.bitrate >> 24);
			pbuf[4] = linecoding.format;
			pbuf[5] = linecoding.paritytype;
			pbuf[6] = linecoding.datatype;

			break;

		case CDC_SET_CONTROL_LINE_STATE:
			break;

		case CDC_SEND_BREAK:
			break;

		default:
			break;
	}

	return USBD_OK;
}

/**
  * @brief  VCP_Receive
  *         Data received over USB OUT endpoint are sent over CDC interface 
  *         through this function.
  *           
  *         @note
  *         This function will block any OUT packet reception on USB endpoint 
  *         untill exiting this function. If you exit this function before transfer
  *         is complete on CDC interface (ie. using DMA controller) it will result 
  *         in receiving more data while previous ones are still not sent.
  *                 
  * @param  pbuf: Buffer of data to be received
  * @param  len: Number of data received (in bytes)
  * @retval Result of the opeartion: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t VCP_Receive (uint8_t* pbuf, uint32_t *len)
{
    s_RxBuffer.Position = 0;
    s_RxBuffer.Size = *len;
    s_RxBuffer.ReadDone = 1;
    return (0);
}

/**
  * @brief  VCP_Read
  * 		Request a len block of data from host through the OUT Endpoint
  * 		and store it into the passed buffer.
  *
  * @param  pbuf: Buffer of data to be received
  * @param  len: Number of data to be received (in bytes)
  * @retval The number of received byte avaiable
  */
int VCP_Read(uint8_t *pbuf, uint16_t len)
{
    if (!s_RxBuffer.ReadDone)
        return 0;

    int remaining = s_RxBuffer.Size - s_RxBuffer.Position;
    int todo = MIN(remaining, len);
    if (todo <= 0)
        return 0;

    memcpy(pbuf, s_RxBuffer.Buffer + s_RxBuffer.Position, todo);
    s_RxBuffer.Position += todo;
    if (s_RxBuffer.Position >= s_RxBuffer.Size)
    {
        s_RxBuffer.ReadDone = 0;
        USBD_CDC_ReceivePacket(&hUSBDDevice);
    }

    return todo;
}

/**
  * @brief  VCP_Write
  * 		Transmit a len block of data to host through the IN Endpoint.
  *
  * @param  pbuf: Buffer of data to be transmit
  * @param  len: Number of to data be transmit (in bytes)
  * @retval The number of transmitted byte
  */
int VCP_Write(uint8_t *pbuf, uint16_t len)
{
	if (len > CDC_DATA_HS_OUT_PACKET_SIZE)
	    {
	        int offset;
	        for (offset = 0; offset < len; offset++)
	        {
	            int todo = MIN(CDC_DATA_HS_OUT_PACKET_SIZE,
	            		len - offset);
	            int done = VCP_Write(((char *)pbuf) + offset, todo);
	            if (done != todo)
	                return offset + done;
	        }

	        return len;
	    }

	    USBD_CDC_HandleTypeDef *pCDC =
	            (USBD_CDC_HandleTypeDef *)hUSBDDevice.pClassData;
	    while(pCDC->TxState) { } //Wait for previous transfer

	    USBD_CDC_SetTxBuffer(&hUSBDDevice, (uint8_t *)pbuf, len);
	    if (USBD_CDC_TransmitPacket(&hUSBDDevice) != USBD_OK)
	        return 0;

	    while(pCDC->TxState) { } //Wait until transfer is done
	    return len;
}
