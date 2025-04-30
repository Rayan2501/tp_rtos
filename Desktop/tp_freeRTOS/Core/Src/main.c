#include "main.h"
#include "cmsis_os.h"
#include <stdio.h>
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h" // À placer après FreeRTOS

UART_HandleTypeDef huart1;

/* ---- Configuration tâches ---- */
#define STACK_SIZE       256
#define TASK1_PRIORITY   1
#define TASK2_PRIORITY   2
#define TASK1_DELAY      1
#define TASK2_DELAY      2

/* ---- Handles ---- */
TaskHandle_t task1Handle;
TaskHandle_t task2Handle;
SemaphoreHandle_t printMutex;

/* ---- Prototypes ---- */
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
void task_bug(void *pvParameters);

/* ---- printf redirection ---- */
int __io_putchar(int ch) {
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
  return ch;
}

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_USART1_UART_Init();

  printf("System started: test reentrance & mutex\r\n");

  /* Création du mutex */
  printMutex = xSemaphoreCreateMutex();
  configASSERT(printMutex != NULL);

  /* Création des tâches */
  BaseType_t ret;
  ret = xTaskCreate(task_bug, "Tache 1", STACK_SIZE, (void *)TASK1_DELAY, TASK1_PRIORITY, &task1Handle);
  configASSERT(ret == pdPASS);

  ret = xTaskCreate(task_bug, "Tache 2", STACK_SIZE, (void *)TASK2_DELAY, TASK2_PRIORITY, &task2Handle);
  configASSERT(ret == pdPASS);

  vTaskStartScheduler();

  while (1);
}

void task_bug(void *pvParameters)
{
  int delay = (int) pvParameters;

  for (;;)
  {
    if (xSemaphoreTake(printMutex, portMAX_DELAY) == pdTRUE)
    {
      printf("Je suis %s et je m'endors pour %d ms\r\n", pcTaskGetName(NULL), delay * 1000);
      xSemaphoreGive(printMutex);
    }

    vTaskDelay(pdMS_TO_TICKS(delay * 1000));
  }
}

/* ---- Fonctions nécessaires à STM32CubeMX ---- */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 4;

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    Error_Handler();

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                              | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
    Error_Handler();
}

static void MX_GPIO_Init(void)
{
  __HAL_RCC_GPIOI_CLK_ENABLE();

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);
}

static void MX_USART1_UART_Init(void)
{
  __HAL_RCC_USART1_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* PA9 = TX, PA10 = RX */
  GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_10;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;

  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
}

void Error_Handler(void)
{
  __disable_irq();
  while (1) {}
}
