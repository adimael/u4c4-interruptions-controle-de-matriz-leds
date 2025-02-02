/*
 * Por: Adimael Santos da Silva
*/
#include <stdio.h> 
#include "pico/stdlib.h" 
#include "hardware/pio.h" 
#include "hardware/timer.h" 
#include "hardware/clocks.h" 
#include "ws2812.pio.h" 

// Configurações dos pinos
#define LED_RGB_G 11
#define LED_RGB_B 12
#define LED_RGB_R 13
#define BOTAO_A 5
#define BOTAO_B 6
#define WS2812_PIN 7 
#define NUM_LEDS 25 
#define IS_RGBW false 

// Variáveis globais
volatile int numero_atual = 0; // Número atual exibido
volatile uint32_t ultimo_tempo_a = 0;
volatile uint32_t ultimo_tempo_b = 0;

// Buffer para números na matriz
bool numeros[10][NUM_LEDS] = { 
    // Números 0 a 9 (matriz com valores já existentes)
    // Número 0
    { 
        0, 1, 1, 1, 0,
        0, 1, 0, 1, 0,
        0, 1, 0, 1, 0,
        0, 1, 0, 1, 0,
        0, 1, 1, 1, 0
    },
    // Número 1
    { 
        0, 1, 1, 1, 0,
        0, 0, 1, 0, 0,
        0, 0, 1, 0, 0,
        0, 1, 1, 0, 0,
        0, 0, 1, 0, 0
    },
    // Número 2
    { 
        0, 1, 1, 1, 0,
        0, 1, 0, 0, 0,
        0, 1, 1, 1, 0,
        0, 0, 0, 1, 0,
        0, 1, 1, 1, 0
    },
    // Número 3
    { 
        0, 1, 1, 1, 0,
        0, 0, 0, 1, 0,
        0, 1, 1, 1, 0,
        0, 0, 0, 1, 0,
        0, 1, 1, 1, 0
    },
    // Número 4
    { 
        0, 1, 0, 0, 0,
        0, 0, 0, 1, 0,
        0, 1, 1, 1, 0,
        0, 1, 0, 1, 0,
        0, 1, 0, 1, 0
    },
    // Número 5
    { 
        0, 1, 1, 1, 0,
        0, 0, 0, 1, 0,
        0, 1, 1, 1, 0,
        0, 1, 0, 0, 0,
        0, 1, 1, 1, 0
    },
    // Número 6
    { 
        0, 1, 1, 1, 0,
        0, 1, 0, 1, 0,
        0, 1, 1, 1, 0,
        0, 1, 0, 0, 0,
        0, 1, 1, 1, 0
    },
    // Número 7
    { 
        0, 1, 0, 0, 0,
        0, 0, 0, 1, 0,
        0, 1, 0, 0, 0,
        0, 1, 0, 1, 0,
        0, 1, 1, 1, 0
    },
    // Número 8
    { 
        0, 1, 1, 1, 0,
        0, 1, 0, 1, 0,
        0, 1, 1, 1, 0,
        0, 1, 0, 1, 0,
        0, 1, 1, 1, 0
    },
    // Número 9
    { 
        0, 1, 1, 1, 0,
        0, 0, 0, 1, 0,
        0, 1, 1, 1, 0,
        0, 1, 0, 1, 0,
        0, 1, 1, 1, 0
    } 
};

// Funções auxiliares
static inline uint32_t cor_u32(uint8_t r, uint8_t g, uint8_t b) {
    return ((uint32_t)(r) << 8) | ((uint32_t)(g) << 16) | (uint32_t)(b); 
}

static inline void enviar_pixel(uint32_t pixel_grb) {
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u); 
}

// Função para ajustar brilho
uint32_t ajustar_brilho(uint8_t r, uint8_t g, uint8_t b, float fator) {
    return cor_u32(
        (uint8_t)(r * fator > 255 ? 255 : r * fator), 
        (uint8_t)(g * fator > 255 ? 255 : g * fator), 
        (uint8_t)(b * fator > 255 ? 255 : b * fator)
    );
}

// Função para exibir número
void exibir_numero(int numero) {
    uint32_t cor = ajustar_brilho(0, 128, 128, 0.1); // azul-claro
    for (int i = 0; i < NUM_LEDS; i++) {
        enviar_pixel(numeros[numero][i] ? cor : 0);
    }
}

// Função genérica para configurar pinos de botões
void configurar_botao(uint gpio, void (*callback)(uint, uint32_t)) {
    gpio_init(gpio);
    gpio_set_dir(gpio, GPIO_IN);
    gpio_pull_up(gpio);
    gpio_set_irq_enabled_with_callback(gpio, GPIO_IRQ_EDGE_FALL, true, callback);
}

void handler_irq_gpio(uint gpio, uint32_t eventos) {
    uint32_t tempo_atual = to_us_since_boot(get_absolute_time());
    volatile uint32_t *ultimo_tempo = (gpio == BOTAO_A) ? (volatile uint32_t *)&ultimo_tempo_a : (volatile uint32_t *)&ultimo_tempo_b;

    if (tempo_atual - *ultimo_tempo > 400000) {
        *ultimo_tempo = tempo_atual;
        numero_atual = (gpio == BOTAO_A) ? (numero_atual + 1) % 10 : (numero_atual - 1 + 10) % 10;
        exibir_numero(numero_atual);
    }
}

void piscar_led_rgb() {
    gpio_put(LED_RGB_R, 1); // Vermelho
    gpio_put(LED_RGB_G, 0); // Verde
    gpio_put(LED_RGB_B, 0); // Azul
    sleep_ms(100);
    gpio_put(LED_RGB_R, 0);
    gpio_put(LED_RGB_G, 0);
    gpio_put(LED_RGB_B, 0); // Desliga o LED RGB
    sleep_ms(100);
}

int main() {
    stdio_init_all();

    // Inicialização dos LEDs RGB
    gpio_init(LED_RGB_R);
    gpio_set_dir(LED_RGB_R, GPIO_OUT);
    gpio_init(LED_RGB_G);
    gpio_set_dir(LED_RGB_G, GPIO_OUT);
    gpio_init(LED_RGB_B);
    gpio_set_dir(LED_RGB_B, GPIO_OUT);
    gpio_put(LED_RGB_R, 0); // Desliga o componente vermelho
    gpio_put(LED_RGB_G, 0); // Desliga o componente verde
    gpio_put(LED_RGB_B, 1); // Liga o componente azul

    // Inicialização dos botões
    configurar_botao(BOTAO_A, handler_irq_gpio);
    configurar_botao(BOTAO_B, handler_irq_gpio);

    // Inicialização da matriz WS2812
    PIO pio = pio0; 
    int sm = 0; 
    uint offset = pio_add_program(pio, &ws2812_program); 
    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW); 

    // Loop principal
    while (true) {
        piscar_led_rgb();
    }

    return 0;
}
