# Cliente STM32F407VET6 – Interface de Menu  

## Visão Geral  
Este projeto implementa a **interface de usuário embarcada** para o cliente IoT do sistema **Sabores Conectados**, utilizando o microcontrolador **STM32F407VET6** e display TFT.  

O menu gráfico permite ao usuário:  
- Navegar entre opções do sistema  
- Realizar conexão com a rede  
- Configurar parâmetros  
- Consultar informações do dispositivo  

## Funcionalidades  
- ✅ Exibição de cabeçalho com título  
- ✅ Tela de boas-vindas personalizada  
- ✅ Navegação por itens do menu (↑ / ↓)  
- ✅ Seleção de opções do menu (ENTER / CONFIRMAR)  
- ✅ Layout responsivo ao tamanho da tela configurada  
- ✅ Destaque visual para item selecionado  

## Tecnologias  
- **STM32F407VET6 (ARM Cortex-M4)**  
- **STM32CubeIDE**  
- **STM32 HAL Drivers**  
- **LCD TFT + Touch (ILI9341 ou similar)**  
- **FreeRTOS** (opcional)  
- **UART/SPI/I2C** (para comunicação externa)  

## Estrutura do Projeto  

```
stm32-client/
├── Core/
│ ├── Inc/ # Arquivos de cabeçalho
│ │ ├── menu.h # Declarações do menu
│ │ └── lcd.h # Driver do display
│ ├── Src/
│ │ ├── menu.c # Implementação do menu
│ │ └── lcd.c # Implementação do driver LCD
│ └── main.c # Ponto de entrada do firmware
├── Drivers/ # HAL + bibliotecas auxiliares
├── README.md # Este arquivo
└── .project / .cproject # Configuração STM32CubeIDE
```

## Como Executar  

### Pré-requisitos  
- **STM32CubeIDE v1.15+**  
- **Pacote STM32CubeF4** instalado  
- **Placa STM32F407VET6**  
- **Display TFT (ILI9341/SSD1289)** conectado corretamente  

### Compilação e Upload  
1. Abra o projeto no **STM32CubeIDE**  
2. Compile (**Build Project**)  
3. Conecte a placa via **ST-LINK**  
4. Faça upload (**Run** ou **Debug**)  

Ao iniciar, a tela exibirá a mensagem de **Bem-vindo ao Sabores Conectados** e em seguida o **menu principal**.  

## Estrutura do Menu  

### Itens disponíveis  
1. **Conectar** → inicia o processo de conexão com servidor  
2. **Configurar** → abre tela de configuração de rede/parâmetros  
3. **Informações** → exibe dados do dispositivo  
