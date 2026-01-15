#include "stm32f1xx_hal.h"

// Función para inicializar el PIN del LED
void GPIO_Init(void) {
    // 1. Estructura de configuración
    GPIO_InitTypeDef LedGreen = {0};
    GPIO_InitTypeDef LedRed = {0};
    GPIO_InitTypeDef ButtonD = {0};
    GPIO_InitTypeDef ButtonU = {0};
    GPIO_InitTypeDef LedBlue = {0};
    GPIO_InitTypeDef LedBP = {0};

    // 2. Activar el reloj de los Puerto A, B y C
    // IMPORTANTE: Sin esto, el puerto no funciona y el programa se cuelga
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();

    // 3. Configurar los pines
    // Pin A0 como salida (LED verde)
    LedGreen.Pin = GPIO_PIN_0;
    LedGreen.Mode = GPIO_MODE_OUTPUT_PP;
    LedGreen.Pull = GPIO_NOPULL;
    LedGreen.Speed = GPIO_SPEED_FREQ_LOW;

    // Pin C15 como salida (LED rojo)
    LedRed.Pin = GPIO_PIN_15;
    LedRed.Mode = GPIO_MODE_OUTPUT_PP;
    LedRed.Pull = GPIO_NOPULL;
    LedRed.Speed = GPIO_SPEED_FREQ_LOW;

    // Pin B12 como entrada con resistencia de pull-down (Apretado = 1, Suelto = 0)
    ButtonD.Pin = GPIO_PIN_12;
    ButtonD.Mode = GPIO_MODE_INPUT;
    ButtonD.Pull = GPIO_PULLDOWN;
    ButtonD.Speed = GPIO_SPEED_FREQ_LOW;

    // LED de la placa (es activo en bajo)
    LedBP.Pin = GPIO_PIN_13;
    LedBP.Mode = GPIO_MODE_OUTPUT_PP;
    LedBP.Pull = GPIO_NOPULL;
    LedBP.Speed = GPIO_SPEED_FREQ_LOW;

    // Pin B9 como entrada con resistencia de pull-up (Apretado = 0, Suelto = 1)
    ButtonU.Pin = GPIO_PIN_9;
    ButtonU.Mode = GPIO_MODE_INPUT;
    ButtonU.Pull = GPIO_PULLUP;
    ButtonU.Speed = GPIO_SPEED_FREQ_LOW;

    // Pin A9 como salida (LED azul)
    LedBlue.Pin = GPIO_PIN_9;
    LedBlue.Mode = GPIO_MODE_OUTPUT_PP;
    LedBlue.Pull = GPIO_NOPULL;
    LedBlue.Speed = GPIO_SPEED_FREQ_LOW;

    // 4. Aplicar la configuración
    HAL_GPIO_Init(GPIOA, &LedGreen);
    HAL_GPIO_Init(GPIOB, &ButtonD);
    HAL_GPIO_Init(GPIOC, &LedBP);
    HAL_GPIO_Init(GPIOC, &LedRed);
    HAL_GPIO_Init(GPIOB, &ButtonU);
    HAL_GPIO_Init(GPIOA, &LedBlue);
}

int main(void) {
    // 1. Inicializar la librería HAL (Configura SysTick para HAL_Delay, etc.)
    HAL_Init();

    // 2. Inicializar nuestros periféricos (GPIO)
    GPIO_Init();

    // 3. Bucle infinito (El "loop" de Arduino)
    while (1) {
        // Encender LED verde
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, GPIO_PIN_SET);

        // Alternar estado del LED rojo
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_15);

        // Leer estado del botón conectado a B12 y controlar el LED de la placa
        if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_12) == GPIO_PIN_SET){
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
        } else {
            HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
        }

        // Leer estado del botón conectado a B9 y controlar el LED azul
        if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9)){
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_SET);
        } else {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_9, GPIO_PIN_RESET);  
        }

        // Esperar 500 milisegundos
        HAL_Delay(50);
    }
}

// Necesario para que SysTick funcione correctamente en STM32Cube
void SysTick_Handler(void) {
    HAL_IncTick();
}