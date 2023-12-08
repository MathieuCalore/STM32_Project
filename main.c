#include "main.h"
#include "cmsis_os.h"

UART_HandleTypeDef huart2;
osThreadId defaultTaskHandle;
osThreadId led_mngtHandle;
osThreadId uart_RxHandle;
osThreadId uart_TxHandle;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
void StartDefaultTask(void const * argument);
void StartTask02(void const * argument);
void uart_rx(void const * argument);
void uart_tx(void const * argument);

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_USART2_UART_Init();

  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  osThreadDef(led_mngt, StartTask02, osPriorityHigh, 0, 128);
  led_mngtHandle = osThreadCreate(osThread(led_mngt), NULL);

  osThreadDef(uart_Rx, uart_rx, osPriorityHigh, 0, 128);
  uart_RxHandle = osThreadCreate(osThread(uart_Rx), NULL);

  osThreadDef(uart_Tx, uart_tx, osPriorityHigh, 0, 128);
  uart_TxHandle = osThreadCreate(osThread(uart_Tx), NULL);

  osKernelStart();

  while (1)
  {
  }
}

/* Définition de la clé de cryptage initiale */
char encryptionKey[] = "SHA-256";

/* Fonction pour modifier la clé de cryptage */
void ChangeEncryptionKey(void) {
    // Logique pour changer la clé de cryptage.
    // Ici, pour l'exemple, nous allons simplement basculer entre "SHA-256" et "MD5".
    if (strcmp(encryptionKey, "SHA-256") == 0) {
        strcpy(encryptionKey, "MD5");
    } else {
        strcpy(encryptionKey, "SHA-256");
    }

    // Affichage de la nouvelle clé sur la console de débogage
    printf("Nouvelle clé de cryptage : %s\r\n", encryptionKey);

    // Affichage de la nouvelle clé sur le terminal UART
    char txData[50];
    sprintf(txData, "Nouvelle clé de cryptage : %s\r\n", encryptionKey);
    HAL_UART_Transmit(&huart2, (uint8_t*)txData, strlen(txData), HAL_MAX_DELAY);
}

/* Fonction pour gérer les appuis sur le bouton */
void HandleButtonPress(void) {
    static uint32_t lastButtonPressTime = 0;

    // Vérifie si le bouton est appuyé
    if (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_RESET) {
        // Anti-rebond simple : vérifie si suffisamment de temps s'est écoulé depuis le dernier appui
        if (HAL_GetTick() - lastButtonPressTime > 200) { // 200 ms anti-rebond
            // Mettre à jour le temps du dernier appui
            lastButtonPressTime = HAL_GetTick();

            // Modifier la clé de cryptage
            ChangeEncryptionKey();
        }
    }
}


void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;

  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

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

static void MX_USART2_UART_Init(void)
{
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);
}

void StartDefaultTask(void const * argument)
{
  for(;;)
  {
    osDelay(1);
  }
}


void StartTask02(void const * argument)
{
  int ledState = 0;
  for(;;)
  {
	  if (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_RESET) {
	          ledState = (ledState + 1) % 3;
	          HAL_Delay(200);
	      }
	      switch (ledState) {
	          case 0:
	              HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);
	              break;
	          case 1:
	              HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
	              break;
	          case 2:
	              HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
	              HAL_Delay(500);
	              break;
	      }
  }
}

void uart_rx(void const * argument)
{
  for(;;)
  {
    char rxData[100];
    HAL_UART_Receive(&huart2, (uint8_t*)rxData, sizeof(rxData) - 1, HAL_MAX_DELAY);
    rxData[sizeof(rxData) - 1] = '\0';
  }
}

void uart_tx(void const * argument)
{
  for(;;)
  {
    if (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_RESET) {
      char txData[] = "Bonjour\r\n";
      HAL_UART_Transmit(&huart2, (uint8_t*)txData, sizeof(txData) - 1, HAL_MAX_DELAY);
    }
  }
}

void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line){}
#endif /* USE_FULL_ASSERT */
