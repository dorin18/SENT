#include "SENT.h"

// Constructorul clasei SENT
SENT::SENT(uint8_t pin, uint8_t mode) {
  _pin = pin;
  if (mode == SEND_SENT)
    pinMode(_pin, OUTPUT);
  else
    pinMode(_pin, INPUT);
}

// Inițializează perioada de baza semnal SENT
void SENT::begin(unsigned long tick) {
  this->_tick = tick;
}

// Trimite datele prin protocolul SENT
void SENT::send(uint16_t dataSensor1, uint16_t dataSensor2) {
  unsigned long currentTime = micros();

  uint8_t timenibblelength = 0;

// Calculează CRC
// Poly 101101 -: 23   1111 - 15
  uint8_t data[] = {  
        uint8_t(dataSensor1 & 0x3F),
        uint8_t((dataSensor1>>6) & 0x3F),
        uint8_t(dataSensor2 & 0x3F),
        uint8_t((dataSensor2>>6) & 0x3F)
  };
  uint8_t crcval = calculateCRC(data);

  // Synchronization 56(55+1) ticks
  digitalWrite(_pin, LOW);
  unsigned long oldTime = currentTime;
  while ((currentTime - oldTime) <= this->_tick) {
    currentTime = micros();
  }

  oldTime = currentTime;
  digitalWrite(_pin, HIGH);
  while ((currentTime - oldTime)/this->_tick <= 55) {
    currentTime = micros();
  }

  oldTime = currentTime;
  // Status nibble
  digitalWrite(_pin, LOW);
  while ((currentTime - oldTime)/this->_tick <= 1) {
    currentTime = micros();
  }

  digitalWrite(_pin, HIGH);
  oldTime = micros();
  currentTime = micros();
  timenibblelength = 11 + _status_bit;
  while ((currentTime - oldTime)/this->_tick <  timenibblelength) {
    currentTime = micros();
  }

  oldTime = currentTime;
  // Nibble1 DataSensor1
  digitalWrite(_pin, LOW);
  while ((currentTime - oldTime)/this->_tick <= 1) {
    currentTime = micros();
  }

  oldTime = currentTime;
  digitalWrite(_pin, HIGH);
  timenibblelength = 11 + (dataSensor1 & 0x0F);
  while ((currentTime - oldTime)/this->_tick <= timenibblelength) {
    currentTime = micros();
  }

  oldTime = currentTime;
  // Nibble2 DataSensor1
  digitalWrite(_pin, LOW);
  while ((currentTime - oldTime)/this->_tick <= 1) {
    currentTime = micros();
  }

  oldTime = currentTime;
  digitalWrite(_pin, HIGH);
  timenibblelength = 11 + ((dataSensor1 >> 4) & 0x0F);
  while ((currentTime - oldTime)/this->_tick <= timenibblelength) {
    currentTime = micros();
  }

  oldTime = currentTime;
  // Nibble3 DataSensor1
  digitalWrite(_pin, LOW);
  while ((currentTime - oldTime)/this->_tick <= 1) {
    currentTime = micros();
  }

  oldTime = currentTime;
  digitalWrite(_pin, HIGH);
  timenibblelength = 11 + ((dataSensor1 >> 8) & 0x0F);
  while ((currentTime - oldTime)/this->_tick <= timenibblelength) {
    currentTime = micros();
  }

  oldTime = currentTime;
  // Nibble1 DataSensor2
  digitalWrite(_pin, LOW);
  while ((currentTime - oldTime)/this->_tick <= 1) {
    currentTime = micros();
  }

  oldTime = currentTime;
  digitalWrite(_pin, HIGH);
  timenibblelength = (11 + (dataSensor2 & 0x0F));
  while ((currentTime - oldTime)/this->_tick <= timenibblelength) {
    currentTime = micros();
  }

  oldTime = currentTime;
  // Nibble2 DataSensor2
  digitalWrite(_pin, LOW);
  while ((currentTime - oldTime)/ this->_tick <= 1) {
    currentTime = micros();
  }

  oldTime = currentTime;
  digitalWrite(_pin, HIGH);
  timenibblelength = (11 + ((dataSensor2 >> 4) & 0x0F));
  while ((currentTime - oldTime)/ this->_tick <= timenibblelength) {
    currentTime = micros();
  }

  oldTime = currentTime;
  // Nibble3 DataSensor2
  digitalWrite(_pin, LOW);
  while ((currentTime - oldTime)/ this->_tick <= 1) {
    currentTime = micros();
  }

  oldTime = currentTime;
  digitalWrite(_pin, HIGH);
  timenibblelength = (11 + ((dataSensor2 >> 8) & 0x0F));
  while ((currentTime - oldTime)/ this->_tick <= timenibblelength) {
    currentTime = micros();
  }

  oldTime = currentTime;
  digitalWrite(_pin, LOW);
  while ((currentTime - oldTime)/ this->_tick) {
    currentTime = micros();
  }

  oldTime = currentTime;
  digitalWrite(_pin, HIGH);

// Trimiterea CRC-ului
  timenibblelength = 11 + (crcval & 0x3F); // ??
  digitalWrite(_pin, LOW);
  while ((currentTime - oldTime)/ this->_tick <= 1) {
    currentTime = micros();
  }

  oldTime = currentTime;
  digitalWrite(_pin, HIGH);
  while ((currentTime - oldTime)/ this->_tick <= timenibblelength) {
    currentTime = micros();
  }

  /*** oldTime = currentTime;
  // restul până la 270
  digitalWrite(_pin, LOW);
  while ((currentTime - oldTime)/this->_tick <= 1) {
    currentTime = micros();
  }

  digitalWrite(_pin, HIGH);
  while ((currentTime - oldTime) <= this->_tick * 270) {
    currentTime = micros();
  } ***/

  // pause
  oldTime = currentTime;
  digitalWrite(_pin, LOW);
  while ((currentTime - oldTime)/this->_tick <= 1) {
    currentTime = micros();
  }

  oldTime = currentTime;
  digitalWrite(_pin, HIGH);
  while ((currentTime - oldTime)/this->_tick <= 76) {
    currentTime = micros();
  }
  digitalWrite(_pin, LOW);
}

bool SENT::available() {
  /// Măsoară durata semnalului LOW
  //unsigned long lowDuration = this->_tick; //pulseIn(_pin, LOW, this->_tick*270); // Timeout de 10,000 de microsecunde

  // Măsoară durata semnalului HIGH
  unsigned long highDuration = pulseIn(_pin, HIGH, 3000000);

  // Verifică dacă durata semnalului HIGH este suficient de lungă
  /// bool isHighAvailable = ((uint16_t)(highDuration/this->_tick) == 56);
  bool isHighAvailable = ((uint16_t)(highDuration/this->_tick) == 55); // 55 + _tick = 56
  // if(isHighAvailable) Serial.println("Sinc ok"); else Serial.println("Sinc not yet");

  /// Opțional, verifică dacă durata LOW este într-un anumit interval acceptabil (dacă este necesar)
  ///bool isLowValid = (uint16_t)(lowDuration/this->_tick) >= 1; // MIN_LOW_DURATION && lowDuration <= MAX_LOW_DURATION);

  // Returnează true doar dacă ambele condiții sunt îndeplinite (sau doar cea HIGH, dacă LOW nu este relevant)
  return isHighAvailable;
  ///return (unsigned long)(highDuration  /*this->_tick*/);

}

uint8_t SENT::correctError(uint8_t data) {
  // Simulează corectarea erorilor - întoarce pur și simplu datele primite pentru acest exemplu
  return data;
}


void SENT::read(uint16_t *dataSensor1, uint16_t *dataSensor2, uint16_t *sstatus) {

  // status bit
  unsigned long highDuration = pulseIn(_pin, HIGH, this->_tick*270);
  //*sstatus = (uint16_t)(highDuration) ;
  *sstatus = (uint16_t)((highDuration /this->_tick)-11) * (uint8_t)(highDuration/this->_tick <= 27) ;

  // Senzor 1 nibble 1 (low)
  highDuration = pulseIn(_pin, HIGH, this->_tick*270);
  *dataSensor1 =((uint8_t)(highDuration /this->_tick)-11) * (uint8_t)(highDuration/this->_tick <= 27) ;

  // Senzor 1 nibble 2 
  highDuration = pulseIn(_pin, HIGH, this->_tick*270);
  *dataSensor1 = *dataSensor1 | ((((uint8_t)(highDuration /this->_tick-11)<< 4) * (uint8_t)(highDuration/this->_tick <= 27))) ;

  // Senzor 1 nibble 3 (high)
  highDuration = pulseIn(_pin, HIGH, this->_tick*270);
  *dataSensor1 = *dataSensor1 | ((((uint8_t)(highDuration /this->_tick-11)<< 8) * (uint8_t)(highDuration/this->_tick <= 27)));

  // Senzor 2 nibble 1 (low)
  highDuration = pulseIn(_pin, HIGH, this->_tick*270);
  *dataSensor2 = (uint8_t)(highDuration /this->_tick-11) * (uint8_t)(highDuration/this->_tick <= 27) ;

  // Senzor 2 nibble 2 
  highDuration = pulseIn(_pin, HIGH, this->_tick*270);
  *dataSensor2 = *dataSensor2 | ((uint8_t)(highDuration /this->_tick-11) * (uint8_t)(highDuration/_tick <= 27)) << 4 ;

  // Senzor 2 nibble 3 (high)
  highDuration = pulseIn(_pin, HIGH, this->_tick*270);
  *dataSensor2 = *dataSensor2 | ((uint8_t)(highDuration /this->_tick-11) * (uint8_t)(highDuration/this->_tick <= 27)) << 8;

  // CRC
  highDuration = pulseIn(_pin, HIGH, this->_tick*270);
  uint8_t crc = (uint8_t)(highDuration /this->_tick-11); //???? * (uint8_t)(highDuration/this->_tick <= 27) ;

  bool flg = true;
  bool flgout = true;
  unsigned long currentTime, oldTime;
  uint8_t calc_crc = 0;
  int lastState = LOW;
  while(flg) {
    int currentState = digitalRead(this->_pin);

    if (lastState == LOW && currentState == HIGH && flgout) { // runs once
      // Rising edge detected, do something here  } else if (lastState == HIGH && currentState == LOW) {
      // Falling edge detected, do something here
      oldTime = micros();
      flgout = false;
      // Verificare CRC
      uint8_t data[] = {  
        uint8_t((*dataSensor1) & 0x3F),
        uint8_t(((*dataSensor1) >>6) & 0x3F),
        uint8_t((*dataSensor2) & 0x3F),
        uint8_t(((*dataSensor2) >>6) & 0x3F)
      };
      calc_crc = calculateCRC(data);

    }else if (lastState == HIGH && currentState == LOW)  {
      currentTime = micros();
      flg = false;
    }

    lastState = currentState;
  }
  highDuration = currentTime - oldTime;


  // Afisare CRC trimis si primit
  Serial.print("CRC trimis: ");
  Serial.print(crc, HEX);
  Serial.print(" , CRC calculat: ");
  Serial.println(calc_crc, HEX);

  // Pause
  // highDuration = pulseIn(_pin, HIGH, this->_tick*270);
  uint16_t pause = (uint8_t)(highDuration /this->_tick-11) * (uint8_t)(highDuration/this->_tick <= 270) ;

}

uint8_t SENT::calculateCRC(uint8_t *data) {
  static const uint8_t crc6_table[64] = {
    0, 25, 50, 43, 61, 36, 15, 22, 35, 58, 17, 8, 30, 7, 44, 53,
    31, 6, 45, 52, 34, 59, 16, 9, 60, 37, 14, 23, 1, 24, 51, 42,
    62, 39, 12, 21, 3, 26, 49, 40, 29, 4, 47, 54, 32, 57, 18, 11,
    33, 56, 19, 10, 28, 5, 46, 55, 2, 27, 48, 41, 63, 38, 13, 20
  };

  uint8_t checksum = 0x15; // Seed pentru CRC
  for (int i = 0; i < 4; i++) {
    checksum = uint8_t(data[i]) ^ uint8_t(crc6_table[checksum + 1]);
  }
  checksum = uint8_t(0) ^ uint8_t(crc6_table[checksum + 1]);
  return checksum;
}