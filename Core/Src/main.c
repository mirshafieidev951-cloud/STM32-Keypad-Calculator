/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include "i2c-lcd.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c2;

int number = 0;
int result = 0;
char result_str[64] = "";
char phrase[64] = "";


/* USER CODE BEGIN PV */
int read_key(){
	int n = -1;
	  for (int r = 1; r < 5; r++){
		  int row1, row2, row3, row4;
		  if(r == 1) {row1 = 0; row2 = 1; row3 = 1; row4 = 1;}
		  if(r == 2) {row1 = 1; row2 = 0; row3 = 1; row4 = 1;}
		  if(r == 3) {row1 = 1; row2 = 1; row3 = 0; row4 = 1;}
		  if(r == 4) {row1 = 1; row2 = 1; row3 = 1; row4 = 0;}

		  HAL_GPIO_WritePin(GPIOB, R1_Pin, row1);
		  HAL_GPIO_WritePin(GPIOB, R2_Pin, row2);
		  HAL_GPIO_WritePin(GPIOB, R3_Pin, row3);
		  HAL_GPIO_WritePin(GPIOB, R4_Pin, row4);
		  if (!HAL_GPIO_ReadPin(GPIOA, C1_Pin)) {
			  n = 1 + 4*(r-1);
		  }
		  if (!HAL_GPIO_ReadPin(GPIOA, C2_Pin)) {
			  n = 2 + 4*(r-1);
		  }
		  if (!HAL_GPIO_ReadPin(GPIOA, C3_Pin)) {
			  n = 3 + 4*(r-1);
		  }
		  if (!HAL_GPIO_ReadPin(GPIOA, C4_Pin)) {
			  n = 4 + 4*(r-1);
		  }
	  }
	  return n;
	}


  float get_next_number(char **expression) {
    char num_buffer[16];
    int i = 0;
    
    while (isdigit(**expression) || **expression == '.') {
        num_buffer[i++] = **expression;
        (*expression)++; 
    }
    num_buffer[i] = '\0';
    return (float)atof(num_buffer);
}
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C2_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_I2C2_Init();
  /* USER CODE BEGIN 2 */
    lcd_init ();
    lcd_put_cur(0, 0);
    lcd_send_string ("0");
  /* USER CODE END 2 */



  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  int release = 0;
  int n = 0;
  int previous = 0;
  while (1)
  {
    /* USER CODE END WHILE */
	  HAL_Delay(10);
    /* USER CODE BEGIN 3 */
	  n = read_key();
	  if (n != -1) {
		release = 0;
		previous = n;
	}
	  else if (previous != -1 && release == 0) {
		release = 1;
		number = previous;
		previous = -1;

    int len = strlen(phrase);
    /////////////////
      if ((len < 63) && (number != 15)) { 
        if (number == 1)
        {
          phrase[len] = '1';
        }
        else if (number == 2)
        {
          phrase[len] = '2';
        }
        else if (number == 3)
        {
          phrase[len] = '3';
        }
        else if (number == 4)
        {
          phrase[len] = '/';
        }
        else if (number == 5)
        {
          phrase[len] = '4';
        }
        else if (number == 6)
        {
          phrase[len] = '5';
        }
        else if (number == 7)
        {
          phrase[len] = '6';
        }
        else if (number == 8)
        {
          phrase[len] = '*';
        }
        else if (number == 9)
        {
          phrase[len] = '7';
        }
        else if (number == 10)
        {
          phrase[len] = '8';
        }
        else if (number == 11)
        {
          phrase[len] = '9';
        }
        else if (number == 12)
        {
          phrase[len] = '-';
        }
        else if (number == 13)
        {
          phrase[len] = '^';
        }
        else if (number == 14)
        {
          phrase[len] = '0';
        }
        else if (number == 15)
        {
          phrase[len] = '=';
        }
        else if (number == 16)
        {
          phrase[len] = '+';
        }
        lcd_put_cur(0, 0);
        lcd_send_string (phrase);
      }
      else {
        char *p = phrase;
        char current_op = '+';

        result = get_next_number(&p);
        
        while (*p != '\0') {
          current_op = *p;
          p++;
          float next_num = get_next_number(&p);

          switch (current_op) {
                case '+':
                    result = result + next_num;
                    break;
                case '-':
                    result = result - next_num;
                    break;
                case '*':
                    result = result * next_num;
                    break;
                case '/':
                    if (next_num != 0) {
                        result = result / next_num;
                    } else {
                        lcd_clear();
                        lcd_put_cur(1, 0);
                        lcd_send_string("Error: Div by 0");
                        HAL_Delay(2000);
                        result = 0;
                    }
                    break;
          }
        } 


        ///////////////////////
        memset(phrase, 0, 64);
        lcd_clear();
        lcd_put_cur(1, 0);
        sprintf(result_str, "%d", result);
        lcd_send_string(result_str);
        
        /////////////////////////////


      }

	}


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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.ClockSpeed = 100000;
  hi2c2.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, R1_Pin|R2_Pin|R3_Pin|R4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : C1_Pin C2_Pin C3_Pin C4_Pin */
  GPIO_InitStruct.Pin = C1_Pin|C2_Pin|C3_Pin|C4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : R1_Pin R2_Pin R3_Pin R4_Pin */
  GPIO_InitStruct.Pin = R1_Pin|R2_Pin|R3_Pin|R4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
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
#ifdef USE_FULL_ASSERT
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
