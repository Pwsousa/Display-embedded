# Teclado de Membrana 4x4 - Guia de Uso

## Visão Geral
Este projeto implementa um teclado de membrana 4x4 para navegação no menu do sistema STM32F407. O teclado permite navegar pelas opções do menu e selecionar itens.

## Mapeamento do Teclado

### Layout do Teclado
```
[ 1 ] [ 2 ] [ 3 ] [ A ]
[ 4 ] [ 5 ] [ 6 ] [ B ]
[ 7 ] [ 8 ] [ 9 ] [ C ]
[ * ] [ 0 ] [ # ] [ D ]
```

### Controles de Navegação
- **Tecla 9**: Navegar para cima no menu
- **Tecla 8**: Navegar para baixo no menu
- **Tecla C**: Selecionar/confirmar item do menu

### Conexões GPIO
- **Linhas (Rows)**: PA0 (L1), PA1 (L2), PA2 (L3), PA3 (L4)
- **Colunas (Columns)**: PA4 (C1), PA5 (C2), PA6 (C3), PA7 (C4)

## Funcionalidades Implementadas

### 1. Inicialização
```c
void Keypad_Init(void);
```
- Configura os pinos GPIO para o teclado
- Define linhas como saída e colunas como entrada com pull-up
- Inicializa o estado interno do teclado

### 2. Leitura de Teclas
```c
KeypadKey_t Keypad_Scan(void);
KeypadKey_t Keypad_GetKey(void);
```
- `Keypad_Scan()`: Escaneia o teclado e retorna a tecla pressionada
- `Keypad_GetKey()`: Retorna a tecla atualmente pressionada

### 3. Detecção de Eventos
```c
uint8_t Keypad_IsKeyPressed(KeypadKey_t key);
uint8_t Keypad_IsKeyReleased(KeypadKey_t key);
```
- Detecta quando uma tecla é pressionada ou liberada
- Útil para evitar leituras múltiplas da mesma tecla

### 4. Navegação no Menu
```c
int8_t Keypad_GetNavigationDelta(void);
uint8_t Keypad_IsSelectPressed(void);
```
- `Keypad_GetNavigationDelta()`: Retorna -1 (cima), 1 (baixo) ou 0 (sem navegação)
- `Keypad_IsSelectPressed()`: Retorna 1 quando tecla de seleção é pressionada

## Integração com o Menu

O teclado está integrado ao sistema de menu existente:

1. **Navegação**: Use as teclas 9 e 8 para navegar entre as opções
2. **Seleção**: Use a tecla C para selecionar uma opção
3. **Ações**: Cada opção do menu executa uma ação específica:
   - **Conectar**: Mostra mensagem "Conectando..."
   - **Configurar**: Mostra mensagem "Configuracoes"
   - **Informações**: Mostra mensagem "Informacoes do Sistema"

## Configurações

### Debounce
- Tempo de debounce: 50ms
- Evita leituras múltiplas de teclas

### Frequência de Escaneamento
- Delay entre escaneamentos: 10ms
- Delay no loop principal: 10ms

## Exemplo de Uso

```c
// No loop principal
while (1) {
    // Escanear teclado
    Keypad_Scan();
    
    // Verificar navegação
    int8_t nav_delta = Keypad_GetNavigationDelta();
    if (nav_delta != 0) {
        Menu_HandleInput(nav_delta);
    }
    
    // Verificar seleção
    if (Keypad_IsSelectPressed()) {
        MenuAction action = Menu_HandleInput(0);
        // Processar ação...
    }
    
    HAL_Delay(10);
}
```

## Troubleshooting

### Teclado não responde
1. Verifique as conexões GPIO
2. Confirme que os pinos estão configurados corretamente
3. Verifique se o pull-up está habilitado nas colunas

### Leitura múltipla de teclas
1. Ajuste o tempo de debounce se necessário
2. Verifique se o delay entre escaneamentos está adequado

### Navegação não funciona
1. Confirme que as teclas 2 e 8 estão mapeadas corretamente
2. Verifique se o sistema de menu está recebendo os deltas corretos

