#include "stm32f1xx_hal.h"

// --- Definiciones para CLK ---
#define CLK_PIN   GPIO_PIN_0
#define CLK_PORT  GPIOA

// --- Definiciones para DATA ---
#define DATA_PIN  GPIO_PIN_1
#define DATA_PORT GPIOA

// --- Definiciones para EN ---
#define EN_PIN    GPIO_PIN_2
#define EN_PORT   GPIOA

#define FRECUENCY 10000

static uint32_t delay_uS = (1 / (2 * FRECUENCY)) * 10e6;

// Prototipos de funciones
void SystemClock_Config(void);
void GPIO_Init(void);
void Send_19Bits(uint32_t data);
void Delay_uS(uint32_t us);

void GPIO_Init(void) {
    // 1. Habilitar el reloj del puerto A
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // 2. Configuración para el CLK
    GPIO_InitStruct.Pin = CLK_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(CLK_PORT, &GPIO_InitStruct);

    // 2. Configuración para el DATA
    GPIO_InitStruct.Pin = DATA_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(DATA_PORT, &GPIO_InitStruct);

    // 2. Configuración para el EN
    GPIO_InitStruct.Pin = EN_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(EN_PORT, &GPIO_InitStruct);
}
void SystemClock_Config(void) {
}

int main(void) {
    HAL_Init(); // Inicializa la librería HAL
    SystemClock_Config();
    GPIO_Init();
    uint32_t miDato = 0x07D06; // N = 250, A = 6 y SW1 = SW2 = 0
    // 0x7FFFC; todos 1 y dos ceros al final
    // N = 250, A = 6 y SW1 = SW2 = 0 -> miDato = 0x07D06
    // SW1 = SW2 = 1 -> miDato = 0x67D06
    // SW1 = 1 y SW2 = 0 -> miDato = 0x47D06

    while (1) {
        Send_19Bits(miDato);
        HAL_Delay(2000);
    }
}

void Send_19Bits(uint32_t data) {
    // 1. Asegurar estado inicial: CLK bajo, EN bajo, DATA en Idle (alto)
    HAL_GPIO_WritePin(CLK_PORT, CLK_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(EN_PORT, EN_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DATA_PORT, DATA_PIN, GPIO_PIN_SET);
    HAL_Delay(1);

    // 2. Transmision de los 19 bits
    for (int i = 18; i >= 0; i--) {
        // Establecer el bit en DATA antes del flanco de subida
        if ((data >> i) & 0x01) {
            HAL_GPIO_WritePin(DATA_PORT, DATA_PIN, GPIO_PIN_SET);
        } else {
            HAL_GPIO_WritePin(DATA_PORT, DATA_PIN, GPIO_PIN_RESET);
        }

        HAL_Delay(1); // Tiempo de establecimiento

        // Flanco ascendente de CLK (El receptor lee aquí)
        HAL_GPIO_WritePin(CLK_PORT, CLK_PIN, GPIO_PIN_SET);
        HAL_Delay(1);
        HAL_GPIO_WritePin(CLK_PORT, CLK_PIN, GPIO_PIN_RESET);
    }

    // 3. Generar pulso de Enable tras los 19 bits
    HAL_Delay(1);
    // HAL_GPIO_WritePin(CLK_PORT, CLK_PIN, GPIO_PIN_SET);
    HAL_GPIO_WritePin(EN_PORT, EN_PIN, GPIO_PIN_SET);
    HAL_Delay(1);
    // HAL_GPIO_WritePin(CLK_PORT, CLK_PIN, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(EN_PORT, EN_PIN, GPIO_PIN_RESET);

    // 4. Dejar DATA en Idle (1)
    HAL_GPIO_WritePin(DATA_PORT, DATA_PIN, GPIO_PIN_SET);
}

void Delay_uS(uint32_t us) {
    uint32_t count = us * 3;
    while (count--) {
        __NOP();
    }
}
void SysTick_Handler(void) {
    HAL_IncTick();
}
