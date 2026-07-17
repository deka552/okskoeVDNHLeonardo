// ============================================================
//  ARDUINO LEONARDO
//  Пин 5  — кнопка (INPUT_PULLUP, замыкание на GND)
//  Пин 12 — реле SSR, HIGH 35 сек по нажатию
//  Пин 13 — встроенный LED, горит 3 сек по нажатию
// ============================================================
#include <Arduino.h>

#define BTN_PIN   5
#define RELAY_PIN 12
#define LED_PIN   13

// ---------- debounce ----------
bool lastBtnState = false;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;
bool btnStable = false;

// ---------- реле (пин 12) — 35 сек ----------
bool relayActive = false;
unsigned long relayStartMs = 0;
const unsigned long relayDurationMs = 35000;

// ---------- LED (пин 13) — 3 сек ----------
bool ledActive = false;
unsigned long ledStartMs = 0;
const unsigned long ledDurationMs = 3000;

void setup()
{
  Serial.begin(115200);

  pinMode(BTN_PIN, INPUT_PULLUP);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
  digitalWrite(LED_PIN, LOW);

  Serial.println("Leonardo ready");
}

void loop()
{
  bool raw = (digitalRead(BTN_PIN) == LOW);

  if (raw != lastBtnState)
  {
    lastDebounceTime = millis();
  }

  if (millis() - lastDebounceTime > debounceDelay)
  {
    if (raw != btnStable)
    {
      btnStable = raw;
      if (btnStable) // замыкание = срабатывание
      {
        relayActive = true;
        relayStartMs = millis();
        digitalWrite(RELAY_PIN, HIGH);

        ledActive = true;
        ledStartMs = millis();
        digitalWrite(LED_PIN, HIGH);

        Serial.println("BTN → RELAY 35s, LED 3s");
      }
    }
  }
  lastBtnState = raw;

  // ---------- неблокирующее выключение реле ----------
  if (relayActive && millis() - relayStartMs >= relayDurationMs)
  {
    relayActive = false;
    digitalWrite(RELAY_PIN, LOW);
    Serial.println("RELAY off");
  }

  // ---------- неблокирующее выключение LED ----------
  if (ledActive && millis() - ledStartMs >= ledDurationMs)
  {
    ledActive = false;
    digitalWrite(LED_PIN, LOW);
  }
}
