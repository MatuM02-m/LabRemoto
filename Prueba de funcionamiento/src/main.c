#include "stm32f1xx_hal.h"

// Función para inicializar el PIN del LED
void GPIO_Init(void) {
    // 1. Estructura de configuración
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // 2. Activar el reloj del Puerto C (Donde está el LED PC13)
    // IMPORTANTE: Sin esto, el puerto no funciona y el programa se cuelga
    __HAL_RCC_GPIOC_CLK_ENABLE();

    // 3. Configurar el Pin PC13
    GPIO_InitStruct.Pin = GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // Push-Pull
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

    // 4. Aplicar la configuración
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

int main(void) {
    // 1. Inicializar la librería HAL (Configura SysTick para HAL_Delay, etc.)
    HAL_Init();

    // 2. Inicializar nuestros periféricos (GPIO)
    GPIO_Init();

    // 3. Bucle infinito (El "loop" de Arduino)
    while (1) {
        // Togglear (invertir) el estado del pin PC13
        HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);

        // Esperar 500 milisegundos
        HAL_Delay(500);
    }
}

// Necesario para que SysTick funcione correctamente en STM32Cube
void SysTick_Handler(void) {
    HAL_IncTick();
}