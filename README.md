# U4C4 - Controle de Matriz de LEDs com Botões

Este projeto envolve a criação de um sistema de controle de LEDs e exibição de números em uma matriz utilizando a placa **BitDogLab**. O código desenvolvido foi feito em C utilizando a biblioteca **Pico SDK** para interagir com os pinos GPIO e controlar LEDs RGB e uma matriz de LEDs WS2812. O projeto também inclui botões para incrementar e decrementar números exibidos na matriz.

## Vídeo da Prática

Segue o link do vídeo dos resultados obtidos durante a simulação:

[Assista ao vídeo demonstrativo. Clique aqui!](https://youtube.com/shorts/FsvcxyC7PWg?feature=share)

## Enunciado do Projeto

O objetivo é criar um sistema que combine hardware e software para:
1. Demonstrar o uso de interrupções e tratamento de bouncing em botões.
2. Manipular LEDs comuns e endereçáveis (WS2812).
3. Fixar o uso de resistores de pull-up internos em botões.

### Componentes Utilizados
- **Matriz 5x5 de LEDs WS2812**: Conectada à GPIO 7.
- **LED RGB**: Conectado às GPIOs 11, 12 e 13.
- **Botão A**: Conectado à GPIO 5.
- **Botão B**: Conectado à GPIO 6.

---

## Organização do Código

### Arquivos Incluídos
- **`main.c`**: Código principal com implementação de funcionalidades.
- **`ws2812.pio`**: Código PIO para controlar os LEDs WS2812.
- **`diagram.json`** e **`wokwi.toml`**: Arquivos que configuram a simulação no Wokwi, facilitando o teste do projeto sem hardware físico.

---

## 🚀 Como Executar o Projeto

### Pré-requisitos
- **Placa BitDogLab** configurada e conectada.
- Ambiente de desenvolvimento configurado com **SDK do RP2040**.

   ou

- Use a extensão do Wokwi no VSCode [https://marketplace.visualstudio.com/items?itemName=Wokwi.wokwi-vscode](https://marketplace.visualstudio.com/items?itemName=Wokwi.wokwi-vscode)

### Passos
1. **Clone o repositório**:
   ```bash
   git clone https://github.com/adimael/u4c4-interruptions-controle-de-matriz-leds.git
   ````
2. Entre na pasta do projeto:
   ````
   cd u4c4-interruptions-controle-de-matriz-leds
   ````

   ## Instruções para Configuração e Compilação do Projeto

   ### Passo 1: Baixar o Ninja
   1. Acesse a página oficial de releases do Ninja:  
   [Ninja Releases no GitHub](https://github.com/ninja-build/ninja/releases).
   2. Baixe o arquivo binário mais recente para Windows (geralmente um `.zip`).
   3. Extraia o conteúdo do arquivo `.zip` (haverá um arquivo `ninja.exe`).

   ---

   ### Passo 2: Adicionar o Ninja ao Path do sistema
   Para que o `ninja` seja reconhecido como um comando em qualquer terminal, siga estas etapas:

   1. Clique com o botão direito no botão **Iniciar** e selecione **Configurações**.
   2. Vá em **Sistema** → **Sobre** → **Configurações avançadas do sistema** (no lado direito).
   3. Na aba **Avançado**, clique em **Variáveis de Ambiente**.
   4. Na seção **Variáveis do Sistema**, localize a variável `Path` e clique em **Editar**.
   5. Clique em **Novo** e adicione o caminho completo para o diretório onde você extraiu o `ninja.exe`. Por exemplo:
      ````
      C:\Users\SeuUsuario\Downloads\ninja-win
      ````
   6. Clique em **OK** em todas as janelas.

   ---

   ### Passo 3: Verificar se o Ninja está funcionando
   1. Abra um terminal (PowerShell ou Prompt de Comando).
   2. Digite o comando abaixo para verificar a instalação:
      ```bash
      ninja --version
      ````

   ### Passo 4: Configurar e Compilar o Projeto

   1. Volte ao diretório do projeto e abra um terminal.
   2. Crie ou limpe a pasta `build`:
      ```bash
      rmdir /s /q build
      mkdir build
      cd build
      
   3. Configure o projeto com o CMake:
      ```bash
      cmake -G Ninja ..
      ````
   4. Compile o projeto:

      ```bash
         ninja
      ````

   5. Após isso, o arquivo main.elf será gerado na pasta build.