# ProjectBasedLearning
Project Based Learning, para o íntimos, PBL

# Repositório atualmente
MeSure.ASP 
- Projeto ASP.NET(model-view-controller), configurado na versão .NET 8.0, além de já estar configurado com o HTTPS e docker file, caso necessário

# MeSure.ESP
Atualmente, dentro do projeto do esp, já é possível realizar a medição da temperatura (Utilizando o DHT11). Além disso, o projeto já conta com a navegação entre páginas no sistema, permitindo que o usuário visualize a temperatura atual dentro do sistema, defina a unidade de medida da temperatura, valores máximos e minimos de temperatura que são permitidos para cada situação e o tempo de envio das informações para dentro do servidor.
Porém, todas essas informações ainda estão sendo salvas apenas na memória interna do ESP, não estão sendo repassadas para o servidor. Sendo preciso também:
 - Finalizar a lógica medidas conforme o timeout send
 - Sincronizar as informações locais com as informações do servidor
 - Finalizar a lógica para calculo de temperatura a partir da leitura do servidor
 - Acertar as conversões de temperatura, consequentemente, acertar a sinalização dos limites
 - Talvez, fazer uma opção para alternar entre medição por sensor e medição por resistencia (Facilitar os testes sem o gerador de potencia)
