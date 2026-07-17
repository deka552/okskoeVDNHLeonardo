// ============================================================
//  ARDUINO LEONARDO
//  Пин 5  — кнопка (INPUT_PULLUP, замыкание на GND)
//  Пин 12 — всегда HIGH, по нажатию импульс LOW 100 мс
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

// ---------- импульс LOW на пине 12 ----------
bool pulseActive = false;
unsigned long pulseStartMs = 0;
const unsigned long pulseDurationMs = 100; // 100 мс хватит любому реле

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
  digitalWrite(RELAY_PIN, HIGH); // покой = HIGH
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
      if (btnStable) // замыкание
      {
        pulseActive = true;
        pulseStartMs = millis();
        digitalWrite(RELAY_PIN, LOW); // начало импульса

        ledActive = true;
        ledStartMs = millis();
        digitalWrite(LED_PIN, HIGH);

        Serial.println("BTN → pulse LOW 100ms, LED 3s");
      }
    }
  }
  lastBtnState = raw;

  // ---------- конец импульса → обратно HIGH ----------
  if (pulseActive && millis() - pulseStartMs >= pulseDurationMs)
  {
    pulseActive = false;
    digitalWrite(RELAY_PIN, HIGH);
  }

  // ---------- неблокирующее выключение LED ----------
  if (ledActive && millis() - ledStartMs >= ledDurationMs)
  {
    ledActive = false;
    digitalWrite(LED_PIN, LOW);
  }
}
