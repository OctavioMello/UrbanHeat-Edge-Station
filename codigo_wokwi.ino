// ============================================================
//  UrbanHeat v2.0 - Estação Edge de Monitoramento Ambiental
//  Global Solution FIAP - Edge Computing
//  Plataforma: Arduino Uno / Wokwi Simulator
// ============================================================

#include <DHT.h>
#include <LiquidCrystal_I2C.h>

// ----------------------------------------------------------
//  SEÇÃO 1 — Pinos e tipo de sensor
// ----------------------------------------------------------
#define PINO_DHT          2
#define PINO_LED_VERDE    8
#define PINO_LED_AMARELO  9
#define PINO_LED_VERMELHO 10
#define PINO_BUZZER       11
#define TIPO_DHT          DHT22

// ----------------------------------------------------------
//  SEÇÃO 2 — Parâmetros de classificação
// ----------------------------------------------------------
#define LIMITE_MEDIA          30.0
#define LIMITE_ALTA           35.0
#define LIMIAR_UMIDADE_ALTA   70
#define PESO_UMIDADE          3.0

// ----------------------------------------------------------
//  SEÇÃO 3 — Temporização
// ----------------------------------------------------------
#define INTERVALO_LEITURA       2000
#define MAX_ERROS_CONSECUTIVOS  3
#define DURACAO_BIPE_MEDIA      150
#define DURACAO_BIPE_ALTA       250
#define INTERVALO_BIPE_MEDIA    1200
#define INTERVALO_BIPE_ALTA     600
#define FREQ_BIPE_MEDIA         800
#define FREQ_BIPE_ALTA          1200

// ----------------------------------------------------------
//  SEÇÃO 4 — Instâncias
// ----------------------------------------------------------
DHT dht(PINO_DHT, TIPO_DHT);
LiquidCrystal_I2C lcd(0x27, 16, 2);

// ----------------------------------------------------------
//  SEÇÃO 5 — Prioridades
// ----------------------------------------------------------
enum Prioridade { BAIXA, MEDIA, ALTA };

// ----------------------------------------------------------
//  SEÇÃO 6 — Variáveis globais
// ----------------------------------------------------------
float temperatura = 0.0;
float umidade = 0.0;
float indiceRisco = 0.0;
Prioridade prioridadeAtual = BAIXA;
uint8_t errosConsecutivos = 0;

unsigned long ultimaLeitura = 0;

// ----------------------------------------------------------
//  Protótipos
// ----------------------------------------------------------
float calcularIndiceRisco(float temp, float umid);
Prioridade classificarRisco(float indice);
void aplicarAtuadores(Prioridade prio);
void gerenciarBuzzer(Prioridade prio);
void atualizarLCD(float temp, float umid, float indice, Prioridade prio);
void exibirErroLCD(uint8_t contagem);
void desligarTudo();
void logSerial(float temp, float umid, float indice, Prioridade prio);
const char* nomePrioridade(Prioridade prio);

// ============================================================
//  SETUP
// ============================================================
void setup() {
  Serial.begin(9600);

  pinMode(PINO_LED_VERDE, OUTPUT);
  pinMode(PINO_LED_AMARELO, OUTPUT);
  pinMode(PINO_LED_VERMELHO, OUTPUT);
  pinMode(PINO_BUZZER, OUTPUT);

  desligarTudo();

  dht.begin();

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print(" UrbanHeat v2.0 ");
  lcd.setCursor(0, 1);
  lcd.print("Edge Station  OK");
  delay(2500);
  lcd.clear();

  Serial.println(F("============================================="));
  Serial.println(F(" UrbanHeat v2.0 - Edge Computing Station "));
  Serial.println(F(" FIAP Global Solution "));
  Serial.println(F("============================================="));
}

// ============================================================
//  LOOP
// ============================================================
void loop() {
  unsigned long agora = millis();

  if (agora - ultimaLeitura >= INTERVALO_LEITURA) {
    ultimaLeitura = agora;

    float tempLida = dht.readTemperature();
    float umidLida = dht.readHumidity();

    if (isnan(tempLida) || isnan(umidLida)) {
      errosConsecutivos++;

      exibirErroLCD(errosConsecutivos);

      Serial.print(F("[ERRO] Leitura invalida. Tentativa: "));
      Serial.println(errosConsecutivos);

      if (errosConsecutivos >= MAX_ERROS_CONSECUTIVOS) {
        desligarTudo();
        digitalWrite(PINO_LED_VERMELHO, HIGH);
      }

      return;
    }

    errosConsecutivos = 0;

    temperatura = tempLida;
    umidade = umidLida;

    indiceRisco = calcularIndiceRisco(temperatura, umidade);
    prioridadeAtual = classificarRisco(indiceRisco);

    aplicarAtuadores(prioridadeAtual);
    atualizarLCD(temperatura, umidade, indiceRisco, prioridadeAtual);
    logSerial(temperatura, umidade, indiceRisco, prioridadeAtual);
  }

  gerenciarBuzzer(prioridadeAtual);
}

// ============================================================
//  Índice de risco
// ============================================================
float calcularIndiceRisco(float temp, float umid) {
  if (umid >= LIMIAR_UMIDADE_ALTA) {
    return temp + PESO_UMIDADE;
  }
  return temp;
}

// ============================================================
//  Classificação
// ============================================================
Prioridade classificarRisco(float indice) {
  if (indice < LIMITE_MEDIA) return BAIXA;
  if (indice < LIMITE_ALTA) return MEDIA;
  return ALTA;
}

// ============================================================
//  LEDs
// ============================================================
void aplicarAtuadores(Prioridade prio) {
  desligarTudo();

  switch (prio) {
    case BAIXA:
      digitalWrite(PINO_LED_VERDE, HIGH);
      break;

    case MEDIA:
      digitalWrite(PINO_LED_AMARELO, HIGH);
      break;

    case ALTA:
      digitalWrite(PINO_LED_VERMELHO, HIGH);
      break;
  }
}

// ============================================================
//  Buzzer
// ============================================================
void gerenciarBuzzer(Prioridade prio) {
  static unsigned long ultimoBipe = 0;
  static bool buzzerAtivo = false;

  if (prio == BAIXA) {
    if (buzzerAtivo) {
      noTone(PINO_BUZZER);
      buzzerAtivo = false;
    }
    return;
  }

  unsigned long agora = millis();

  unsigned int freqBipe =
      (prio == ALTA) ? FREQ_BIPE_ALTA : FREQ_BIPE_MEDIA;

  unsigned int durBipe =
      (prio == ALTA) ? DURACAO_BIPE_ALTA : DURACAO_BIPE_MEDIA;

  unsigned int intBipe =
      (prio == ALTA) ? INTERVALO_BIPE_ALTA : INTERVALO_BIPE_MEDIA;

  if (!buzzerAtivo && (agora - ultimoBipe >= intBipe)) {
    tone(PINO_BUZZER, freqBipe);
    buzzerAtivo = true;
    ultimoBipe = agora;
  }
  else if (buzzerAtivo && (agora - ultimoBipe >= durBipe)) {
    noTone(PINO_BUZZER);
    buzzerAtivo = false;
    ultimoBipe = agora;
  }
}

// ============================================================
//  LCD
// ============================================================
void atualizarLCD(float temp, float umid, float indice, Prioridade prio) {

  lcd.setCursor(0, 0);

  lcd.print("T:");
  if (temp < 10.0) lcd.print(" ");
  lcd.print(temp, 1);

  lcd.print((char)223);
  lcd.print("C");

  lcd.print("  U:");

  if (umid < 10.0) lcd.print(" ");
  lcd.print((int)umid);

  lcd.print("% ");

  lcd.setCursor(0, 1);
  lcd.print("PRIO:");

  switch (prio) {

    case BAIXA:
      lcd.print(" BAIXA     ");
      break;

    case MEDIA:
      if (umid >= LIMIAR_UMIDADE_ALTA)
        lcd.print(" MEDIA+U   ");
      else
        lcd.print(" MEDIA     ");
      break;

    case ALTA:
      if (umid >= LIMIAR_UMIDADE_ALTA)
        lcd.print(" ALTA+U [!]");
      else
        lcd.print(" ALTA   [!]");
      break;
  }
}

// ============================================================
//  Erro LCD
// ============================================================
void exibirErroLCD(uint8_t contagem) {
  lcd.setCursor(0, 0);
  lcd.print("ERRO SENSOR DHT!");

  lcd.setCursor(0, 1);
  lcd.print("Tentativa: ");
  lcd.print(contagem);
  lcd.print("     ");
}

// ============================================================
//  Reset dos atuadores
// ============================================================
void desligarTudo() {
  digitalWrite(PINO_LED_VERDE, LOW);
  digitalWrite(PINO_LED_AMARELO, LOW);
  digitalWrite(PINO_LED_VERMELHO, LOW);
  noTone(PINO_BUZZER);
}

// ============================================================
//  Nome da prioridade
// ============================================================
const char* nomePrioridade(Prioridade prio) {
  switch (prio) {
    case BAIXA: return "BAIXA";
    case MEDIA: return "MEDIA";
    case ALTA: return "ALTA";
    default: return "---";
  }
}

// ============================================================
//  Monitor Serial
// ============================================================
void logSerial(float temp, float umid, float indice, Prioridade prio) {

  Serial.print(F("[UrbanHeat] T:"));
  Serial.print(temp, 1);

  Serial.print(F("C | U:"));
  Serial.print(umid, 1);

  Serial.print(F("% | Idx:"));
  Serial.print(indice, 1);

  Serial.print(F("C | Prio:"));
  Serial.print(nomePrioridade(prio));

  if (umid >= LIMIAR_UMIDADE_ALTA) {
    Serial.print(F(" [penalidade umidade ativa]"));
  }

  Serial.println();
}
