## Visão Geral
* A proposta deste trabalho é implantar um sistema de comunicação para controle e recebimento de dados entre um objeto locomotor e uma aplicação mobile. A interface do usuário é dotada de duas funcionalidades principais: joystick virtual para movimentação do automóvel e exibição dos dados que foram coletados, sendo esta, possui a opção de enviar a um servidor hospedado na internet preparado para armazenar as informações daquele dado instante.	O projeto foi batizado de Curiásity, possuindo duas homenagens: ao verbo “curiar” (bastante difundido pelo nordeste), e também ao astromóvel desenvolvido pela NASA: Curiosity.

  <p align="center">
    <img src="https://github.com/cardosorrenan/iot-ufc/blob/master/curiasity/img/schema.png" width="600">
  </p>
  
### Hardware

#### NodeMCU 
* O módulo NodeMCU ESP-12E é uma placa que junta o ESP8266 (chip de arquitetura 32 bits com Wi-Fi integrado), interface USB-Serial e regulador de tensão 3,3V. Ela atua no projeto como uma ponte entre a web e o Arduino. A placa pode ser alimentada via USB ou um carregador genérico de celular (5V).

  <p align="center">
    <img src="https://github.com/cardosorrenan/iot-ufc/blob/master/curiasity/img/node.png" width="200">
  </p>
  
#### Arduino
* A alimentação fornecida ao Arduíno são duas baterias 9V ligadas em paralelo. 
O circuito na protoboard menor tem objetivo obter um ganho de corrente para alimentar o motor.

  <p align="center">
    <img src="https://github.com/cardosorrenan/iot-ufc/blob/master/curiasity/img/arduino.png" width="450">
  </p>

#### Demo

  <p align="center">
    <img src="https://github.com/cardosorrenan/iot-ufc/blob/master/curiasity/img/hardware.jpeg" width="350">
  </p>

### Internet

#### Interface Blynk
* A plataforma fornece uma comunicação rápida e estável entre o hardware conectado e a aplicação móvel Blynk App. O serviço disponibiliza uma biblioteca rica em funções para os diversos componentes e que podem ser requisitadas facilmente através de comandos. O dispositivo só poderá ser monitorado ou controlado, através de um token de autenticação que é inserido no próprio código, ele é gerado no instante da criação do projeto. 

#### Banco de dados (phpMyAdmin)
* O armazenamento foi feito com o auxílio da plataforma de administração de banco de dados phpMyAdmin, é open source e voltado para aplicações hospedadas em servidores online. Foi desenvolvido um banco relacional MySQL, com apenas uma tabela, denominada sensores. Ela possui cinco atributos, são eles: ID (chave primária), proximidade, temperatura, umidade e data, que armazena o dia e horário no momento em que o registro for inserido. O banco de dados foi armazenado na plataforma 000Webhost, que é um serviço gratuito de hospedagem e domínio.
    Os dados são enviados do NodeMCU via URL para um arquivo php, também armazenado no mesmo servidor e está preparado para receber esses dados. Quando as informações chegam de acordo como o arquivo espera, é criada uma comunicação com o banco de dados e os registros são despejados, após o processo, a comunicação é encerrada para receber outra.

#### Página
* Uma vez os dados armazenados em um serviço online e disponível, é de suma importância exibir essas informações. Portanto, uma página foi desenvolvida para a visualização dos registros:

  <p align="center">
    <img src="https://github.com/cardosorrenan/iot-ufc/blob/master/curiasity/img/page.png" width="500">
  </p>
  <p align="center">
    https://curiasity.000webhostapp.com/index.html
  </p>

### Smartphone
  <p align="center">
    <img src="https://github.com/cardosorrenan/iot-ufc/blob/master/curiasity/img/blynk.jpeg" width="350">
  </p>
