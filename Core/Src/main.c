/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdbool.h>
#include "usbd_hid.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef struct
{
  uint8_t MODIFIER;
  uint8_t RESERVED;
  uint8_t KEYCODE1;
  uint8_t KEYCODE2;
  uint8_t KEYCODE3;
  uint8_t KEYCODE4;
  uint8_t KEYCODE5;
  uint8_t KEYCODE6;
} USB_HID_KeyboardTypedef_t;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim6;

/* USER CODE BEGIN PV */
USB_HID_KeyboardTypedef_t usbHIDKbdStruct = {0, 0, 0, 0, 0, 0, 0, 0};
bool key_CTRL_pressed = false, key_C_pressed = false, key_V_pressed = false;

extern USBD_HandleTypeDef hUsbDeviceFS;
extern bool time10msElapsed;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM6_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void task_keypad_polling(void)
{
  static uint8_t key_CTRL_pressed_count = 0, key_C_pressed_count = 0, key_V_pressed_count = 0;
  if (HAL_GPIO_ReadPin(KEY_CTRL_GPIO_Port, KEY_CTRL_Pin) == GPIO_PIN_RESET)
  {
    key_CTRL_pressed_count++;
    if (key_CTRL_pressed_count == 3)
    {
      key_CTRL_pressed_count = 0;
      key_CTRL_pressed = true;
    }
  }
  if (HAL_GPIO_ReadPin(KEY_C_GPIO_Port, KEY_C_Pin) == GPIO_PIN_RESET)
  {
    key_C_pressed_count++;
    if (key_C_pressed_count == 3)
    {
      key_C_pressed_count = 0;
      key_C_pressed = true;
    }
  }
  if (HAL_GPIO_ReadPin(KEY_V_GPIO_Port, KEY_V_Pin) == GPIO_PIN_RESET)
  {
    key_V_pressed_count++;
    if (key_V_pressed_count == 3)
    {
      key_V_pressed_count = 0;
      key_V_pressed = true;
    }
  }
}

void task_act_usb_hid_kbd()
{
  if (key_C_pressed)
  {
    key_C_pressed = false;
    if (key_CTRL_pressed)
    {
      key_CTRL_pressed = false;
      usbHIDKbdStruct.MODIFIER = 0x01; // press 'ctrl'
      usbHIDKbdStruct.KEYCODE1 = 0x06; // press 'c'
      USBD_HID_SendReport(&hUsbDeviceFS, (uint8_t *) &usbHIDKbdStruct, sizeof(usbHIDKbdStruct));
      HAL_Delay(50);
      usbHIDKbdStruct.MODIFIER = 0x00; // release key
      usbHIDKbdStruct.KEYCODE1 = 0x00; // release key
      USBD_HID_SendReport(&hUsbDeviceFS, (uint8_t *) &usbHIDKbdStruct, sizeof(usbHIDKbdStruct));
      HAL_Delay(50);
    }
    else
    {
      usbHIDKbdStruct.KEYCODE1 = 0x06; // press 'c'
      USBD_HID_SendReport(&hUsbDeviceFS, (uint8_t *) &usbHIDKbdStruct, sizeof(usbHIDKbdStruct));
      HAL_Delay(50);
      usbHIDKbdStruct.KEYCODE1 = 0x00; // release key
      USBD_HID_SendReport(&hUsbDeviceFS, (uint8_t *) &usbHIDKbdStruct, sizeof(usbHIDKbdStruct));
      HAL_Delay(50);
    }
  }
  if (key_V_pressed)
  {
    key_V_pressed = false;
    if (key_CTRL_pressed)
    {
      key_CTRL_pressed = false;
      usbHIDKbdStruct.MODIFIER = 0x01; // press 'ctrl'
      usbHIDKbdStruct.KEYCODE1 = 0x19; // press 'v'
      USBD_HID_SendReport(&hUsbDeviceFS, (uint8_t *) &usbHIDKbdStruct, sizeof(usbHIDKbdStruct));
      HAL_Delay(50);
      usbHIDKbdStruct.MODIFIER = 0x00; // release key
      usbHIDKbdStruct.KEYCODE1 = 0x00; // release key
      USBD_HID_SendReport(&hUsbDeviceFS, (uint8_t *) &usbHIDKbdStruct, sizeof(usbHIDKbdStruct));
      HAL_Delay(50);
    }
    else
    {
      usbHIDKbdStruct.KEYCODE1 = 0x19; // press 'v'
      USBD_HID_SendReport(&hUsbDeviceFS, (uint8_t *) &usbHIDKbdStruct, sizeof(usbHIDKbdStruct));
      HAL_Delay(50);
      usbHIDKbdStruct.KEYCODE1 = 0x00; // release key
      USBD_HID_SendReport(&hUsbDeviceFS, (uint8_t *) &usbHIDKbdStruct, sizeof(usbHIDKbdStruct));
      HAL_Delay(50);
    }
  }
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USB_DEVICE_Init();
  MX_TIM6_Init();
  /* USER CODE BEGIN 2 */
  if (HAL_TIM_Base_Start_IT(&htim6) != HAL_OK)
  {
	  /* Starting Error */
	  Error_Handler();
  }
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	if (time10msElapsed)
	{
		time10msElapsed = false;
		task_keypad_polling();
	}
	task_act_usb_hid_kbd();
	HAL_Delay(50);
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 72;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 3;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM6_Init(void)
{

  /* USER CODE BEGIN TIM6_Init 0 */

  /* USER CODE END TIM6_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM6_Init 1 */

  /* USER CODE END TIM6_Init 1 */
  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 3;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 63999;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM6_Init 2 */

  /* USER CODE END TIM6_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pins : KEY_CTRL_Pin KEY_C_Pin KEY_V_Pin */
  GPIO_InitStruct.Pin = KEY_CTRL_Pin|KEY_C_Pin|KEY_V_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
