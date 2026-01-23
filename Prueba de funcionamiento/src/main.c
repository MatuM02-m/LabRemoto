/* * ARCHIVO: src/main.c (PLACA MAESTRA)
 */
#include "stm32f1xx_hal.h"

/* --- Variables Globales --- */
SPI_HandleTypeDef hspi1;

/* --- Prototipos de Funciones --- */
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);

/* --- MAIN --- */
int main(void)
{
  /* 1. Inicialización de bajo nivel */
  HAL_Init();
  SystemClock_Config();

  /* 2. Inicialización de Periféricos */
  MX_GPIO_Init();
  MX_SPI1_Init();

  /* Datos a enviar */
  uint8_t data_on = 0x01;
  uint8_t data_off = 0x00;

  while (1)
  {
    /* ENCENDER LED REMOTO */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET); // CS LOW (Seleccionar)
    HAL_SPI_Transmit(&hspi1, &data_on, 1, 100);           // Enviar
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);   // CS HIGH (Soltar)
    
    HAL_Delay(1000); // Esperar 1s

    /* APAGAR LED REMOTO */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET); // CS LOW
    HAL_SPI_Transmit(&hspi1, &data_off, 1, 100);          // Enviar
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);   // CS HIGH
    
    HAL_Delay(1000);
  }
}

/* --- Configuración SPI1 (MAESTRO) --- */
static void MX_SPI1_Init(void)
{
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  
  // NSS SOFT: Nosotros controlamos el pin PA4 manualmente en el while(1)
  hspi1.Init.NSS = SPI_NSS_SOFT; 
  
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256; // Muy lento (seguro)
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;

  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    while(1); // Error
  }
}

/* --- Configuración GPIO (MAESTRO) --- */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Habilitar Relojes */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_SPI1_CLK_ENABLE();

  /* Configurar PA4 como Salida (Chip Select) */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET); // Default HIGH
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* Configurar PA5 (SCK) y PA7 (MOSI) como Alterna PushPull */
  GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* Configurar PA6 (MISO) como Entrada */
  GPIO_InitStruct.Pin = GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/* --- Funciones del Sistema --- */
void SystemClock_Config(void)
{
  // Configuración por defecto (HSI 8MHz).
  // Si necesitas 72MHz, aquí iría el código del RCC.
}

void SysTick_Handler(void)
{
  HAL_IncTick(); // Necesario para que funcione HAL_Delay
}