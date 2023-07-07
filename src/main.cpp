#include <Arduino.h>
#include <IRremote.h> //INCLUSÃO DE BIBLIOTECA
#include <PushButton.h>

// DEFININDO BOTÕES
#define BT_POWER 12
#define BT_ENTER 19
#define BT_FREEZE 13
#define BT_SOURCE 18
#define LED_INFO 2
#define RETRO_SWITCH 10

#define frequecia 38 // em KHz
int RECV_PIN = 2;    // PINO DIGITAL UTILIZADO PELO FOTORRECEPTOR KY-022

void ircode(decode_results *results);
void encoding(decode_results *results);
void dumpInfo(decode_results *results);
void dumpRaw(decode_results *results);
void dumpCode(decode_results *results);

// DECLARAÇÃO DE TECLAS CLONADAS EPSON
unsigned int bt_power_epson[] = {8800, 4450, 550, 1650, 550, 1650, 550, 550, 600, 550, 550, 550, 600, 500, 600, 550, 550, 1650, 550, 1650, 600, 500, 600, 1600, 600, 550, 550, 1650, 550, 550, 550, 1650, 600, 500, 600, 550, 600, 500, 600, 500, 600, 550, 550, 1650, 600, 500, 600, 500, 600, 1650, 550, 1650, 550, 1650, 600, 1600, 550, 1650, 600, 500, 600, 1650, 550, 1650, 550, 550, 600};
unsigned int bt_enter_epson[] = {8800, 4450, 550, 1650, 550, 1650, 550, 600, 500, 550, 600, 550, 550, 550, 550, 550, 550, 1700, 550, 1650, 550, 550, 550, 1650, 550, 550, 550, 1700, 550, 550, 550, 1650, 550, 550, 550, 1650, 600, 550, 550, 1650, 550, 550, 550, 600, 550, 550, 550, 600, 500, 1650, 550, 600, 550, 1650, 550, 550, 550, 1650, 550, 1650, 550, 1650, 550, 1700, 550, 550, 550};
unsigned int bt_freeze_epson[] = {8800, 4450, 550, 1650, 550, 1650, 550, 600, 500, 600, 550, 550, 550, 600, 500, 600, 550, 1650, 550, 1650, 550, 600, 500, 1650, 550, 600, 550, 1650, 550, 600, 500, 1650, 550, 600, 550, 550, 550, 1650, 550, 600, 500, 600, 550, 1650, 550, 600, 500, 600, 500, 1650, 600, 1650, 550, 550, 550, 1650, 550, 1650, 550, 600, 550, 1650, 550, 1650, 550, 600, 500};
unsigned int bt_source_epson[] = {8800, 4450, 550, 1650, 550, 1650, 550, 600, 550, 550, 550, 600, 500, 600, 550, 550, 550, 1650, 550, 1650, 550, 600, 550, 1650, 550, 550, 550, 1650, 550, 600, 500, 1650, 600, 550, 550, 600, 500, 600, 550, 1650, 550, 1650, 550, 550, 550, 600, 500, 600, 550, 1650, 550, 1650, 550, 1650, 550, 600, 500, 600, 550, 1650, 550, 1650, 550, 1650, 550, 600, 500};

// DECLARAÇÃO DE TECLAS CLONADAS BENQ
unsigned int bt_on_benq[] = {8850, 4500, 500, 600, 550, 600, 500, 600, 550, 550, 550, 600, 500, 650, 500, 600, 500, 600, 500, 600, 550, 600, 500, 650, 500, 550, 550, 1700, 500, 1700, 550, 550, 550, 600, 500, 1700, 550, 1650, 550, 1700, 500, 1700, 500, 600, 550, 600, 500, 1700, 500, 650, 500, 600, 500, 600, 550, 550, 550, 600, 500, 1700, 550, 1650, 550, 600, 500, 1700, 500};
unsigned int bt_off_benq[] = {8850, 4500, 500, 600, 550, 600, 500, 600, 550, 550, 550, 600, 550, 550, 550, 600, 500, 600, 500, 600, 550, 600, 550, 550, 550, 550, 550, 1700, 550, 1650, 550, 550, 550, 600, 550, 550, 550, 1650, 550, 1700, 550, 1650, 550, 550, 550, 600, 550, 1650, 550, 550, 550, 1700, 550, 550, 550, 550, 550, 600, 550, 1650, 550, 1650, 550, 600, 550, 1650, 550};
unsigned int bt_source_benq[] = {8900, 4450, 550, 600, 550, 550, 550, 550, 600, 550, 550, 550, 550, 600, 550, 550, 550, 550, 600, 550, 550, 550, 550, 600, 550, 550, 550, 1650, 550, 1650, 600, 550, 550, 550, 550, 600, 550, 550, 550, 1650, 550, 600, 550, 550, 550, 550, 550, 600, 550, 550, 550, 1650, 600, 1650, 550, 550, 550, 1650, 550, 1700, 550, 1650, 550, 1650, 550, 1650, 550};
unsigned int bt_ok_benq[] = {8900, 4450, 600, 500, 600, 550, 550, 550, 600, 500, 600, 550, 600, 500, 600, 550, 550, 550, 600, 500, 600, 550, 600, 500, 550, 600, 550, 1650, 600, 1600, 600, 500, 600, 550, 550, 1650, 600, 500, 600, 1650, 550, 550, 600, 1600, 600, 550, 550, 550, 600, 500, 600, 550, 600, 1600, 600, 500, 600, 1650, 550, 550, 600, 1600, 600, 1650, 550, 1650, 600};
unsigned int bt_seta_cima_benq[] = {8850, 4500, 550, 600, 500, 600, 550, 550, 550, 600, 500, 600, 550, 600, 500, 600, 550, 550, 550, 600, 500, 600, 550, 550, 550, 600, 500, 1700, 550, 1650, 550, 600, 500, 650, 500, 1650, 550, 1700, 500, 600, 550, 1650, 550, 600, 550, 550, 550, 550, 550, 600, 550, 600, 500, 600, 500, 1700, 500, 650, 500, 1650, 550, 1700, 550, 1650, 550, 1650, 550};
unsigned int bt_seta_baixo_benq[] = {8900, 4450, 550, 600, 500, 600, 550, 550, 550, 600, 550, 550, 550, 600, 500, 600, 500, 600, 550, 600, 500, 600, 550, 550, 550, 600, 500, 1700, 550, 1650, 550, 600, 550, 600, 500, 550, 550, 600, 500, 1700, 550, 1650, 550, 600, 550, 550, 550, 600, 500, 600, 550, 1650, 550, 1650, 550, 600, 500, 650, 500, 1650, 550, 1700, 550, 1650, 500, 1700, 550};
unsigned int bt_seta_direita_benq[] = {8900, 4450, 550, 550, 550, 550, 600, 550, 600, 500, 600, 550, 550, 550, 600, 500, 600, 550, 550, 550, 600, 550, 550, 550, 600, 500, 600, 1650, 550, 1650, 600, 500, 600, 550, 550, 550, 600, 1600, 600, 1650, 550, 1650, 600, 500, 550, 600, 550, 550, 600, 500, 600, 1650, 550, 550, 600, 500, 600, 550, 600, 1600, 600, 1600, 600, 1650, 550, 1650, 600};
unsigned int bt_seta_esquerda_benq[] = {8900, 4450, 550, 550, 550, 600, 500, 600, 550, 600, 550, 600, 500, 550, 550, 600, 550, 550, 550, 600, 500, 600, 550, 550, 550, 600, 550, 1650, 550, 1650, 550, 600, 500, 600, 550, 1650, 550, 600, 550, 1650, 550, 1650, 550, 550, 550, 600, 550, 550, 550, 600, 550, 550, 550, 1650, 550, 600, 550, 550, 550, 1650, 550, 1650, 550, 1700, 550, 1650, 550};
unsigned int bt_freeze_benq[67] = {8900, 4450, 550, 550, 550, 600, 550, 600, 500, 600, 550, 550, 550, 550, 550, 600, 550, 550, 550, 550, 600, 550, 550, 550, 550, 600, 550, 1650, 550, 1650, 550, 600, 550, 550, 550, 1650, 550, 1650, 600, 550, 550, 550, 550, 600, 550, 550, 550, 550, 550, 600, 550, 550, 550, 600, 550, 1650, 550, 1650, 550, 1650, 550, 1700, 550, 1650, 550, 1650, 550};

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

IRrecv irrecv(RECV_PIN); // PASSA O PARÂMETRO PARA A FUNÇÃO irrecv
IRsend emissor(11);

decode_results results; // VARIÁVEL QUE ARMAZENA OS RESULTADOS (SINAL IR RECEBIDO)

void setup()
{
  tempo_anterior_led = millis();

  Serial.begin(9600);  // INICIALIZA A SERIAL
  irrecv.enableIRIn(); // INICIALIZA A RECEPÇÃO DE SINAIS IR

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
  if (tempo_atual_led - tempo_anterior_led >= 500)
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

  tempo_atual_source = millis();
  if (tempo_atual_source - tempo_anterior_source >= 20500)
  { // Atraso de 1 segundo (1000 milissegundos)
    source_menu = false;
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
      for (int i = 0; i < 20; i++)
      {
        emissor.sendRaw(bt_power_epson, sizeof(bt_power_epson) / sizeof(bt_power_epson[0]), frequecia);
        Serial.println("Enviando Botão de Power");
      }
      digitalWrite(LED_INFO, HIGH);
      estado_led = 1;
      delay(1500);
      emissor.sendRaw(bt_power_epson, sizeof(bt_power_epson) / sizeof(bt_power_epson[0]), frequecia);
    }

    if (BtEnter.pressed())
    {
      emissor.sendRaw(bt_enter_epson, sizeof(bt_enter_epson) / sizeof(bt_enter_epson[0]), frequecia);
      Serial.println("Enviando Botão de Enter");
      digitalWrite(LED_INFO, HIGH);
      estado_led = 1;
      delay(300);
    }

    if (BtFreeze.pressed())
    {
      emissor.sendRaw(bt_freeze_epson, sizeof(bt_freeze_epson) / sizeof(bt_freeze_epson[0]), frequecia);
      Serial.println("Enviando Botão de Freeze");
      digitalWrite(LED_INFO, HIGH);
      estado_led = 1;
      delay(300);
    }

    if (BtSource.pressed())
    {
      emissor.sendRaw(bt_source_epson, sizeof(bt_source_epson) / sizeof(bt_source_epson[0]), frequecia);
      Serial.println("Enviando Botão de Source");
      digitalWrite(LED_INFO, HIGH);
      estado_led = 1;
      delay(300);
    }
  }
  else
  {
    Serial.println("Projetor Benq...");
    
    if (projetor_ligado)
    {
      if (source_menu)
      {
        if (BtSource.pressed())
        {
          emissor.sendRaw(bt_seta_baixo_benq, sizeof(bt_seta_baixo_benq) / sizeof(bt_seta_baixo_benq[0]), frequecia);
          Serial.println("Enviando Botão de Seta para baixo");
          digitalWrite(LED_INFO, HIGH);
          estado_led = 1;
          delay(100);
          // Zerar o contador do source
          tempo_atual_source = 0;
          tempo_anterior_source = 0;
        }

        if (BtEnter.pressed())
        {
          emissor.sendRaw(bt_ok_benq, sizeof(bt_ok_benq) / sizeof(bt_ok_benq[0]), frequecia);
          Serial.println("Enviando Botão de Ok");
          digitalWrite(LED_INFO, HIGH);
          estado_led = 1;
          delay(300);
          // Desabilitar o menu source no código
          source_menu = false;
        }
      }
      else // Projetor ligado, sem o menu source ativado
      {
        if (BtPower.pressed())
        {
          for (int i = 0; i < 20; i++)
          {
            emissor.sendRaw(bt_off_benq, sizeof(bt_off_benq) / sizeof(bt_off_benq[0]), frequecia);
            Serial.println("Enviando Botão de Off");
          }
          digitalWrite(LED_INFO, HIGH);
          estado_led = 1;
          delay(1500);
          emissor.sendRaw(bt_off_benq, sizeof(bt_off_benq) / sizeof(bt_off_benq[0]), frequecia);
          projetor_ligado = false;
        }

        if (BtEnter.pressed())
        {
          emissor.sendRaw(bt_ok_benq, sizeof(bt_ok_benq) / sizeof(bt_ok_benq[0]), frequecia);
          Serial.println("Enviando Botão de Ok");
          digitalWrite(LED_INFO, HIGH);
          estado_led = 1;
          delay(300);
        }

        if (BtFreeze.pressed())
        {
          emissor.sendRaw(bt_freeze_benq, sizeof(bt_freeze_benq) / sizeof(bt_freeze_benq[0]), frequecia);
          Serial.println("Enviando Botão de Freeze");
          digitalWrite(LED_INFO, HIGH);
          estado_led = 1;
          delay(300);
        }

        if (BtSource.pressed())
        {
          emissor.sendRaw(bt_source_benq, sizeof(bt_source_benq) / sizeof(bt_source_benq[0]), frequecia);
          Serial.println("Enviando Botão de Source");
          digitalWrite(LED_INFO, HIGH);
          estado_led = 1;
          delay(300);
        }
      }
    }
    else
    {
      if (BtPower.pressed()) // Ligando o projetor
      {
        for (int i = 0; i < 20; i++)
        {
          emissor.sendRaw(bt_on_benq, sizeof(bt_on_benq) / sizeof(bt_on_benq[0]), frequecia);
          Serial.println("Enviando Botão de Power");
        }
        digitalWrite(LED_INFO, HIGH);
        estado_led = 1;
        delay(1500);
        projetor_ligado = true;
      }
    }
  }

  /* ROTINA DE RECEPÇÃO DE SINAIS

  //CAPTURA O SINAL IR

  if (irrecv.decode(&results)) {
    Serial.print("\nCódigo HEX: "); //IMPRIME O TEXTO NO MONITOR SERIAL
    Serial.println(results.value, HEX); //IMPRIME NO MONITOR SERIAL O CÓDIGO IR EM FORMATO HEXADECIMAL
    Serial.print("Código DEC: "); //IMPRIME O TEXTO NO MONITOR SERIAL
    Serial.println(results.value); //IMPRIME NO MONITOR SERIAL O CÓDIGO IR EM FORMATO DECIMAL
    Serial.println(""); //QUEBRA DE LINHA NA SERIAL
    ircode   (&results);
    encoding (&results);
    dumpInfo (&results);
    dumpRaw  (&results);
    dumpCode (&results);
    irrecv.resume(); //AGUARDA O RECEBIMENTO DE UM NOVO SINAL IR
  }
  */

  // delay(1000); //INTERVALO DE 100 MILISSEGUNDOS
}

void ircode(decode_results *results)
{
  // Panasonic has an Address
  if (results->decode_type == PANASONIC)
  {
    Serial.print(results->address, HEX);
    Serial.print(":");
  }

  // Print Code
  Serial.print(results->value, HEX);
}

//+=============================================================================
// Display encoding type
//
void encoding(decode_results *results)
{
  switch (results->decode_type)
  {
  default:
  case UNKNOWN:
    Serial.print("UNKNOWN");
    break;
  case NEC:
    Serial.print("NEC");
    break;
  case SONY:
    Serial.print("SONY");
    break;
  case RC5:
    Serial.print("RC5");
    break;
  case RC6:
    Serial.print("RC6");
    break;
  case DISH:
    Serial.print("DISH");
    break;
  case SHARP:
    Serial.print("SHARP");
    break;
  case JVC:
    Serial.print("JVC");
    break;
  case SANYO:
    Serial.print("SANYO");
    break;
  case MITSUBISHI:
    Serial.print("MITSUBISHI");
    break;
  case SAMSUNG:
    Serial.print("SAMSUNG");
    break;
  case LG:
    Serial.print("LG");
    break;
  case WHYNTER:
    Serial.print("WHYNTER");
    break;
  case AIWA_RC_T501:
    Serial.print("AIWA_RC_T501");
    break;
  case PANASONIC:
    Serial.print("PANASONIC");
    break;
  case DENON:
    Serial.print("Denon");
    break;
  }
}

//+=============================================================================
// Dump out the decode_results structure.
//
void dumpInfo(decode_results *results)
{
  // Check if the buffer overflowed
  if (results->overflow)
  {
    Serial.println("IR code too long. Edit IRremoteInt.h and increase RAWBUF");
    return;
  }

  // Show Encoding standard
  Serial.print("Encoding  : ");
  encoding(results);
  Serial.println("");

  // Show Code & length
  Serial.print("Code      : ");
  ircode(results);
  Serial.print(" (");
  Serial.print(results->bits, DEC);
  Serial.println(" bits)");
}

//+=============================================================================
// Dump out the decode_results structure.
//
void dumpRaw(decode_results *results)
{
  // Print Raw data
  Serial.print("Timing[");
  Serial.print(results->rawlen - 1, DEC);
  Serial.println("]: ");

  for (int i = 1; i < results->rawlen; i++)
  {
    unsigned long x = results->rawbuf[i] * USECPERTICK;
    if (!(i & 1))
    { // even
      Serial.print("-");
      if (x < 1000)
        Serial.print(" ");
      if (x < 100)
        Serial.print(" ");
      Serial.print(x, DEC);
    }
    else
    { // odd
      Serial.print("     ");
      Serial.print("+");
      if (x < 1000)
        Serial.print(" ");
      if (x < 100)
        Serial.print(" ");
      Serial.print(x, DEC);
      if (i < results->rawlen - 1)
        Serial.print(", "); //',' not needed for last one
    }
    if (!(i % 8))
      Serial.println("");
  }
  Serial.println(""); // Newline
}

//+=============================================================================
// Dump out the decode_results structure.
//
void dumpCode(decode_results *results)
{
  // Start declaration
  Serial.print("unsigned int  ");         // variable type
  Serial.print("rawData[");               // array name
  Serial.print(results->rawlen - 1, DEC); // array size
  Serial.print("] = {");                  // Start declaration

  // Dump data
  for (int i = 1; i < results->rawlen; i++)
  {
    Serial.print(results->rawbuf[i] * USECPERTICK, DEC);
    if (i < results->rawlen - 1)
      Serial.print(","); // ',' not needed on last one
    if (!(i & 1))
      Serial.print(" ");
  }

  // End declaration
  Serial.print("};"); //

  // Comment
  Serial.print("  // ");
  encoding(results);
  Serial.print(" ");
  ircode(results);

  // Newline
  Serial.println("");

  // Now dump "known" codes
  if (results->decode_type != UNKNOWN)
  {

    // Some protocols have an address
    if (results->decode_type == PANASONIC)
    {
      Serial.print("unsigned int  addr = 0x");
      Serial.print(results->address, HEX);
      Serial.println(";");
    }

    // All protocols have data
    Serial.print("unsigned int  data = 0x");
    Serial.print(results->value, HEX);
    Serial.println(";");
  }
}
