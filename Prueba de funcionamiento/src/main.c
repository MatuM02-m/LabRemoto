/* * ARCHIVO: src/main.c (PLACA ESCLAVA - LED EXTERNO PB0)
 */
#include "stm32f1xx_hal.h"

SPI_HandleTypeDef hspi1;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_SPI1_Init();

  uint8_t rx_buffer = 0;

  while (1)
  {
    /* Esperar dato del Maestro */
    if (HAL_SPI_Receive(&hspi1, &rx_buffer, 1, HAL_MAX_DELAY) == HAL_OK)
    {
      if (rx_buffer == 0x01) {
        /* ENCIENDE EL LED EXTERNO (Lógica Positiva) */
        /* PB0 se pone en 3.3V (SET) */
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
      } 
      else {
        /* APAGA EL LED EXTERNO */
        /* PB0 se pone en GND (RESET) */
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
      }
    }
  }
}

static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* 1. Habilitar Relojes */
  __HAL_RCC_GPIOA_CLK_ENABLE(); // Para SPI (PA4, PA5, PA7)
  __HAL_RCC_GPIOB_CLK_ENABLE(); // <--- ¡NUEVO! Para el LED en PB0
  __HAL_RCC_SPI1_CLK_ENABLE();  // Para el periférico SPI

  /* 2. Configurar LED EXTERNO (PB0) */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET); // Iniciar Apagado
  
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // Push-Pull
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct); // <--- OJO: GPIOB

  /* 3. Configurar Pines SPI (Entradas: SCK, MOSI, NSS) */
  GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_7 | GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* 4. Configurar Pin MISO (Salida AF) */
  GPIO_InitStruct.Pin = GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

static void MX_SPI1_Init(void)
{
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_SLAVE;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_HARD_INPUT; // Hardware NSS (PA4)
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;

  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    while(1);
  }
}

void SystemClock_Config(void) {}

void SysTick_Handler(void)
{
  HAL_IncTick();
}