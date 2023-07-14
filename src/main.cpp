#include <Arduino.h>
#include <IRremote.hpp> //INCLUSÃO DE BIBLIOTECA
#include <PushButton.h>

#define DELAY_POWER 1500
#define DELAY_OUTROS_BOTOES 200

#define IR_SEND_PIN     11

#define RC5_START_BIT_1   0x80
#define RC5_START_BIT_2   0x40

uint16_t sAddress = 0x3000;
uint16_t sCommand_epson = 30;

// DEFININDO BOTÕES
#define BT_POWER 12
#define BT_ENTER 19
#define BT_FREEZE 13
#define BT_SOURCE 18
#define LED_INFO 2
#define RETRO_SWITCH 10

#define frequencia 38 // em KHz


// DECLARAÇÃO DE VARIÁVEIS BENQ
uint16_t address_benq = 0x3000; // Código NEC

uint16_t command_on_benq = 0x4F;
uint16_t command_off_benq = 0x4E;
uint16_t command_source_benq = 0x04;
uint16_t command_ok_benq = 0x15;
uint16_t command_seta_baixo_benq = 0x0C;
uint16_t command_freeze_benq = 0x03;

// DECLARAÇÃO DE VARIÁVEIS EPSON
uint16_t address_epson = 0x00;

uint16_t address_power_and_freeze = 0;
uint16_t address_source = 1;
uint16_t address_enter = 3;

uint16_t command_power_epson = 6;
uint16_t command_source_epson = 60;
uint16_t command_enter_epson = 62;
uint16_t command_freeze_epson = 60;

unsigned int  power_epson[] = {8800,4450, 550,1650, 550,1650, 550,550, 600,550, 550,550, 600,500, 600,550, 550,1650, 550,1650, 600,500, 600,1600, 600,550, 550,1650, 550,550, 550,1650, 600,500, 600,550, 600,500, 600,500, 600,550, 550,1650, 600,500, 600,500, 600,1650, 550,1650, 550,1650, 600,1600, 550,1650, 600,500, 600,1650, 550,1650, 550,550, 600};
unsigned int  enter_epson[] = {8800,4450, 550,1650, 550,1650, 550,600, 500,550, 600,550, 550,550, 550,550, 550,1700, 550,1650, 550,550, 550,1650, 550,550, 550,1700, 550,550, 550,1650, 550,550, 550,1650, 600,550, 550,1650, 550,550, 550,600, 550,550, 550,600, 500,1650, 550,600, 550,1650, 550,550, 550,1650, 550,1650, 550,1650, 550,1700, 550,550, 550};
unsigned int  freeze_epson[67] = {8800,4450, 550,1650, 550,1650, 550,600, 500,600, 550,550, 550,600, 500,600, 550,1650, 550,1650, 550,600, 500,1650, 550,600, 550,1650, 550,600, 500,1650, 550,600, 550,550, 550,1650, 550,600, 500,600, 550,1650, 550,600, 500,600, 500,1650, 600,1650, 550,550, 550,1650, 550,1650, 550,600, 550,1650, 550,1650, 550,600, 500};
unsigned int  source_epson[67] = {8800,4450, 550,1650, 550,1650, 550,600, 550,550, 550,600, 500,600, 550,550, 550,1650, 550,1650, 550,600, 550,1650, 550,550, 550,1650, 550,600, 500,1650, 600,550, 550,600, 500,600, 550,1650, 550,1650, 550,550, 550,600, 500,600, 550,1650, 550,1650, 550,1650, 550,600, 500,600, 550,1650, 550,1650, 550,1650, 550,600, 500};

// DECLARAÇÃO DE VARIÁVEIS DE CONTROLE PARA O BENQ
bool projetor_ligado = false;
bool source_menu = false;
unsigned long tempo_atual_source;
unsigned long tempo_anterior_source;

// DECLARAÇÃO DE VARIÁVEL DE TEMPO DO LED
unsigned long tempo_atual_led;
unsigned long tempo_anterior_led;
int estado_led;

// DECLARANDO OBJETOS DOS BOTÕES
PushButton BtPower(BT_POWER);
PushButton BtEnter(BT_ENTER);
PushButton BtFreeze(BT_FREEZE);
PushButton BtSource(BT_SOURCE);


void setup()
{
  IrSender.begin(IR_SEND_PIN); // Start with IR_SEND_PIN as send pin and enable feedback LED at default feedback LED pin

  IrSender.enableIROut(36); // Call it with 38 kHz just to initialize the values printed below

  tempo_anterior_led = millis();

  Serial.begin(9600);  // INICIALIZA A SERIAL

  pinMode(RETRO_SWITCH, INPUT);
  pinMode(LED_INFO, OUTPUT);
  digitalWrite(LED_INFO, HIGH);
  estado_led = 1;

  pinMode(3, OUTPUT);
  digitalWrite(3, HIGH);

  Serial.println("Configuração finalizada.");
}

void loop()
{
  // Altenar o estado do led
  tempo_atual_led = millis();
  if (tempo_atual_led - tempo_anterior_led >= 200)
  { // Atraso de 1 segundo (1000 milissegundos)
    if (estado_led)
    {
      digitalWrite(LED_INFO, LOW);
      estado_led = 0;
    }
    else
    {
      digitalWrite(LED_INFO, HIGH);
      estado_led = 1;
    }

    tempo_anterior_led = tempo_atual_led; // Atualiza o tempo anterior para o próximo atraso
  }

  // TESTANDO ESTADO DOS BOTÕES
  BtPower.button_loop();
  BtEnter.button_loop();
  BtFreeze.button_loop();
  BtSource.button_loop();

  if (digitalRead(RETRO_SWITCH))
  {
    if (BtPower.pressed())
    {
      // IrSender.sendRC5(( (address_epson & 0x1F) | RC5_START_BIT_1 | RC5_START_BIT_2)  , (0x3F & sCommand_epson), 0, 0);
      // IrSender.sendRC5(( (address_power_and_freeze & 0x1F) | RC5_START_BIT_1 | RC5_START_BIT_2)  , (0x3F & command_power_epson), 0, 0);
      IrSender.sendRaw(power_epson, sizeof(power_epson)/sizeof(power_epson[0]), frequencia);
      Serial.println("Enviando Botão de Power");
      digitalWrite(LED_INFO, HIGH);
      estado_led = 1;
      delay(DELAY_POWER);
      IrSender.sendRaw(power_epson, sizeof(power_epson)/sizeof(power_epson[0]), frequencia);
      // IrSender.sendRC5(( (address_epson & 0x1F) | RC5_START_BIT_1 | RC5_START_BIT_2)  , (0x3F & sCommand_epson), 0, 0);
      // IrSender.sendRC5(( (address_power_and_freeze & 0x1F) | RC5_START_BIT_1 | RC5_START_BIT_2)  , (0x3F & command_power_epson), 0, 0);
    }

    if (BtEnter.pressed())
    {
      // IrSender.sendRC5(( (address_epson & 0x1F) | RC5_START_BIT_1 | RC5_START_BIT_2)  , (0x3F & sCommand_epson), 0, 0);
      // IrSender.sendRC5(( (address_enter & 0x1F) | RC5_START_BIT_1 | RC5_START_BIT_2)  , (0x3F & command_enter_epson), 0, 0);
      IrSender.sendRaw(enter_epson, sizeof(enter_epson)/sizeof(enter_epson[0]), frequencia);
      Serial.println("Enviando Botão de Enter");
      digitalWrite(LED_INFO, HIGH);
      estado_led = 1;
      delay(DELAY_POWER);
    }

    if (BtFreeze.pressed())
    {
      // IrSender.sendRC5(( (address_epson & 0x1F) | RC5_START_BIT_1 | RC5_START_BIT_2)  , (0x3F & sCommand_epson), 0, 0);
      // IrSender.sendRC5(( (address_power_and_freeze & 0x1F) | RC5_START_BIT_1 | RC5_START_BIT_2)  , (0x3F & command_freeze_epson), 0, 0);
      IrSender.sendRaw(freeze_epson, sizeof(freeze_epson)/sizeof(freeze_epson[0]), frequencia);
      Serial.println("Enviando Botão de Freeze");
      digitalWrite(LED_INFO, HIGH);
      estado_led = 1;
      delay(DELAY_POWER);
    }

    if (BtSource.pressed())
    {
      // IrSender.sendRC5(( (address_epson & 0x1F) | RC5_START_BIT_1 | RC5_START_BIT_2)  , (0x3F & sCommand_epson), 0, 0);
      // IrSender.sendRC5(( (address_source & 0x1F) | RC5_START_BIT_1 | RC5_START_BIT_2)  , (0x3F & command_source_epson), 0, 0);
      IrSender.sendRaw(source_epson, sizeof(source_epson)/sizeof(source_epson[0]), frequencia);
      Serial.println("Enviando Botão de Source");
      digitalWrite(LED_INFO, HIGH);
      estado_led = 1;
      delay(DELAY_POWER);
    }
  }
  else
  {

    tempo_atual_source = millis();
    if (tempo_atual_source - tempo_anterior_source >= 20500)
    { // Atraso de 1 segundo (1000 milissegundos)
      source_menu = false;
      tempo_anterior_source = tempo_atual_source; // Atualiza o tempo anterior para o próximo atraso
    }

    // Serial.println("Projetor Benq...");

    if (projetor_ligado)
    {
      // Serial.println("Projetor ligado");

      if (source_menu)
      {
        Serial.println("Menu ativado");

        if (BtSource.pressed())
        {
          IrSender.sendNEC(address_benq, command_seta_baixo_benq, 1);
          Serial.println("Enviando Botão de Seta para baixo");
          digitalWrite(LED_INFO, HIGH);
          estado_led = 1;
          delay(DELAY_OUTROS_BOTOES);
          // Zerar o contador do source
          tempo_anterior_source = tempo_atual_source;
          
        }

        if (BtEnter.pressed())
        {
          IrSender.sendNEC(address_benq, command_ok_benq, 0);
          Serial.println("Enviando Botão de Ok dentro do menu source");
          digitalWrite(LED_INFO, HIGH);
          estado_led = 1;
          delay(DELAY_OUTROS_BOTOES);
          // Desabilitar o menu source no código
          source_menu = false;
        }
      }
      else // Projetor ligado, sem o menu source ativado
      {
        if (BtPower.pressed())
        {
          IrSender.sendNEC(address_benq, command_off_benq, 1);
          Serial.println("Enviando Botão de Off");
          digitalWrite(LED_INFO, HIGH);
          estado_led = 1;
          delay(DELAY_POWER);
          IrSender.sendNEC(address_benq, command_off_benq, 1);
          projetor_ligado = false;
        }

        if (BtEnter.pressed())
        {
          IrSender.sendNEC(address_benq, command_ok_benq, 0);
          Serial.println("Enviando Botão de Ok");
          digitalWrite(LED_INFO, HIGH);
          estado_led = 1;
          delay(DELAY_POWER);
        }

        if (BtFreeze.pressed())
        {
          IrSender.sendNEC(address_benq, command_freeze_benq, 0);
          Serial.println("Enviando Botão de Freeze");
          digitalWrite(LED_INFO, HIGH);
          estado_led = 1;
          delay(DELAY_POWER);
        }

        if (BtSource.pressed())
        {
          IrSender.sendNEC(address_benq, command_source_benq, 0);
          Serial.println("Enviando Botão de Source");
          digitalWrite(LED_INFO, HIGH);
          estado_led = 1;
          delay(DELAY_POWER);
          source_menu = true;
        }
      }
    }
    else
    {
      Serial.println("Projetor desligado");

      if (BtPower.pressed()) // Ligando o projetor
      {
        IrSender.sendNEC(address_benq, command_on_benq, 0);
        Serial.println("Enviando Botão de on");
        digitalWrite(LED_INFO, HIGH);
        estado_led = 1;
        delay(DELAY_POWER);
        projetor_ligado = true;
      }
    }
  }
}

