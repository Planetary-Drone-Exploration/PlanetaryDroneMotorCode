extern "C" {
#include "stm32f4xx_hal.h"
#include <stdio.h>
}

WWDG_HandleTypeDef hwwdg;

void WWDG_Init(void) {
    __HAL_RCC_WWDG_CLK_ENABLE();

    hwwdg.Instance = WWDG;
    hwwdg.Init.Prescaler = WWDG_PRESCALER_8;
    hwwdg.Init.Window = 0x40;     // Maximum window
    hwwdg.Init.Counter = 0x7F;    // Start counter at max
    hwwdg.Init.EWIMode = WWDG_EWI_DISABLE; // Disable early warning interrupt

    if (HAL_WWDG_Init(&hwwdg) != HAL_OK) {
        // Initialization Error
        printf("WWDG init failed\r\n");
        while (1);
    }
}


// --- Init Functions ---
void SystemClock_Config(void);
void GPIO_Init(void);
void UART2_Init(void);

// --- Main ---
int main(void) {
    HAL_Init();
    SystemClock_Config();
    GPIO_Init();
    WWDG_Init();

    printf("Starting program...\r\n");

    while (1) 
    {
        // Toggle LED on PD12
        HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_12);
        printf("LED toggled\r\n");
        // HAL_Delay(10);
        for (int i = 0; i < 100; ++i) 
        {
            HAL_Delay(1); // 10 ms * 10 = 100 ms
            HAL_WWDG_Refresh(&hwwdg);
        }
        printf("Zankei ni shosu");
    }
}

// --- GPIO Init (LEDs) ---
void GPIO_Init(void) {
    __HAL_RCC_GPIOD_CLK_ENABLE();

    GPIO_InitTypeDef gpio_led = {0};
    gpio_led.Pin = GPIO_PIN_12;
    gpio_led.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_led.Pull = GPIO_NOPULL;
    gpio_led.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOD, &gpio_led);
}

// --- Clock Config ---
void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 8;
    RCC_OscInitStruct.PLL.PLLN = 336;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 7;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                  RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);
}
