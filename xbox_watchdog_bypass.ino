/*
xbox_watchdog_bypass: Bypass XBOX SMC watchdog
Copyright (C) 2025  Tasos Sahanidis

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, see <https://www.gnu.org/licenses/>.
*/

#include <Wire.h>

#define SMC_ADDR 0x10
#define LRESET 4

static void smc_write(const uint8_t reg, const uint8_t data) {
  Wire.beginTransmission(SMC_ADDR);
  Wire.write(reg);
  Wire.write(data);
  Wire.endTransmission();
}

static uint8_t smc_read(const uint8_t reg) {
  digitalWrite(LED_BUILTIN, HIGH);
  Wire.beginTransmission(SMC_ADDR);
  Wire.write(reg);
  Wire.endTransmission();
  Wire.requestFrom(SMC_ADDR, 1U);
  while(!Wire.available());
  digitalWrite(LED_BUILTIN, LOW);
  return Wire.read();
}

void setup() {
  Wire.begin();
  // Awful hack to disable the built in 5v pull ups
  digitalWrite(SDA, 0);
  digitalWrite(SCL, 0);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(LRESET, INPUT);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop() {
  // Don't bother with interrupts for something like this
  while(!digitalRead(LRESET));
  
  delay(25);

  // Reset cursor to 0
  smc_write(0x01, 0);

  delay(2);

  smc_read(0x01); // ???

  delay(27);

  smc_write(0x13, 0x0F);

  delay(2);

  smc_write(0x12, 0xF0);

  delay(10);
  
  uint8_t ver[4];
  for(uint8_t i = 0; i < sizeof(ver); i++) {
    ver[i] = smc_read(0x1c + i);
  }

  uint8_t tmp[4] = { 
    0x33, 
    0xed,
    (ver[0] << 2) ^ (ver[1] + 0x39) ^ (ver[2] >> 2) ^ (ver[3] + 0x63),
    (ver[0] + 0x0b) ^ (ver[1] >> 2) ^ (ver[2] + 0x1b)
  };
  
  for(int i = 0; i < 4; i++) {
    tmp[0] += tmp[1] ^ tmp[2];
    tmp[1] += tmp[0] ^ tmp[3];
  }
  
  smc_write(0x20, tmp[0]);
  smc_write(0x21, tmp[1]);
  
  while(digitalRead(LRESET));
  delay(500);
}
