# MeSure

Visto que estamos inseridos em um mundo globalizado e de intenso tráfego de grandes distâncias, surge assim a dificuldade de garantir a qualidade e a integridade de produtos que demandam condições especiais. Pensando nisso, nós da MeSure, desenvolvemos um dispositivo capaz de monitorar temperatura, dispondo essas informações através de um visor LCD ou através da nossa plataforma on-line.

## Descrição do sistema
O sistema tem início a partir de uma leitura analógica feita em divisor de tensão, responsável por converter uma tensão de 10V para uma tensão máxima de 3,3 V, sendo essa leitura resultando em um valor em bits. Com a tensão em bits, é necessário converte-la novamente para um valor decimal, para assim, utilizando-se de uma fórmula pré-estabelecida converter o valor de tensão em um valor de temperatura correspondente ao sistema externo.
Com o valor de temperatura, o próprio Esp32, realiza a observação dos valores e com a ajuda dos LEDs definidos no sistema físico, sinais que indiquem o status de temperatura do sistema, se estão abaixo ou acima dos limites definidos.
Durante todo o período que se mantém ativo, o sensor realiza a leitura dos valores de temperatura obtidos, deixa-os reservados em memória e para assim a cada minuto enviar uma média aritmética para a plataforma de observação os valores obtidos, enquanto no visor LCD do dispositivo, a temperatura sempre é exibida em tempo real.
Além disso, através do próprio dispositivo é possível a alteração dos parâmetros desejados, como unidade de temperatura e limites de temperatura (Temperatura máxima e temperatura mínima aceitas pelo sistema), atualizando em tempo real, os sinais de falha, caso necessário.

## Diagramas do circuito

### Visualização do projeto


### Circuito
<img src="https://github.com/PedroReis16/MeSure/blob/main/Circuito.png" alt="CircuitoEletrico" style="width: 80%;">
