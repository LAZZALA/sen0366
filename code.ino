/*!
   @file       code.ino
   @brief      Custom driver for DFRobot Laser Distance Sensor (SEN0366)
   @note       Based on the original work by dDenVil (https://github.com/dDenVil/sen0366)
*/

//SENSOR PROTOCOL COMMANDS
const unsigned char singleMeasurement[] = { 0x80, 0x06, 0x02, 0x78 };               // 1
const unsigned char continuousMeasurement[] = { 0x80, 0x06, 0x03, 0x77 };           // 2
const unsigned char shutdownDevice[] = { 0x80, 0x04, 0x02, 0x7A };                  // 3
const unsigned char setAddress[] = { 0xFA, 0x04, 0x01, 0x80, 0x81 };                // 4
const unsigned char distanceModMinus1[] = { 0xFA, 0x04, 0x06, 0x2D, 0x01, 0xCE };   // 5
const unsigned char distanceModPlus1[] = { 0xFA, 0x04, 0x06, 0x2B, 0x01, 0xD0 };    // 6
const unsigned char timeInterval1S[] = { 0xFA, 0x04, 0x05, 0x01, 0xFC };            // 7
const unsigned char setStartPointTop[] = { 0xFA, 0x04, 0x08, 0x01, 0xF9 };          // 8
const unsigned char setStartPointBackEnds[] = { 0xFA, 0x04, 0x08, 0x00, 0xFA };     // 9
const unsigned char setRange5m[] = { 0xFA, 0x04, 0x09, 0x05, 0xF4 };                // 10
const unsigned char setRange10m[] = { 0xFA, 0x04, 0x09, 0x0A, 0xEF };               // 11
const unsigned char setRange30m[] = { 0xFA, 0x04, 0x09, 0x1E, 0xDB };               // 12
const unsigned char setRange50m[] = { 0xFA, 0x04, 0x09, 0x32, 0xC7 };               // 13
const unsigned char setRange80m[] = { 0xFA, 0x04, 0x09, 0x50, 0xA9 };               // 14
const unsigned char setFrequency0[] = { 0xFA, 0x04, 0x0A, 0x00, 0xF8 };             // 15
const unsigned char setFrequency5[] = { 0xFA, 0x04, 0x0A, 0x05, 0xF3 };             // 16
const unsigned char setFrequency10[] = { 0xFA, 0x04, 0x0A, 0x0A, 0xEE };            // 17
const unsigned char setFrequency20[] = { 0xFA, 0x04, 0x0A, 0x14, 0xE4 };            // 18
const unsigned char setResolution1mm[] = { 0xFA, 0x04, 0x0C, 0x01, 0xF5 };          // 19
const unsigned char setResolution0_1mm[] = { 0xFA, 0x04, 0x0C, 0x02, 0xF4 };        // 20
const unsigned char startMeasurementOnPowerOff[] = { 0xFA, 0x04, 0x0D, 0x00, 0xF5 };// 21
const unsigned char startMeasurementOnPowerOn[] = { 0xFA, 0x04, 0x0D, 0x01, 0xF4 }; // 22
const unsigned char singleMeasurementBroadcast[] = { 0xFA, 0x06, 0x06, 0xFA };      // 23
const unsigned char readCache[] = { 0x80, 0x06, 0x07, 0x73 };                       // 24
const unsigned char controlLaserOpen[] = { 0x80, 0x06, 0x05, 0x01, 0x74 };          // 25
const unsigned char controlLaserClose[] = { 0x80, 0x06, 0x05, 0x00, 0x75 };         // 26

int n = 11;                  // Default packet length: 11 bytes (1mm resolution), 12 bytes (0.1mm resolution)
unsigned char data[15] = {0};
byte dataIndex = 0;        

// Clears the hardware serial buffer
void flushSerial1() {
  while (Serial1.available() > 0) {
    Serial1.read();
  }
}

void setup() {
  Serial.begin(115200);
  Serial1.begin(9600);

  delay(100);

  // Sensor initialization and default setup
  flushSerial1();
  Serial1.write(setResolution1mm, sizeof(setResolution1mm));
  delay(60);
  flushSerial1();

  Serial1.write(continuousMeasurement, sizeof(continuousMeasurement));
  Serial.println("System Ready. Enter a command code (1-26)...");
}

void loop() {

  // ===================================================================
  //  PC SERIAL MONITOR INPUT HANDLING 
  // ===================================================================
  if (Serial.available() > 0) {
    int input = Serial.parseInt();

    if (input > 0 && input <= 26) {

      // Clear hardware serial buffer and reset index before sending a new command
      flushSerial1();
      dataIndex = 0;

      switch (input) {
        case 1:  Serial1.write(singleMeasurement, sizeof(singleMeasurement)); Serial.println("1 - singleMeasurement"); break;
        case 2:  Serial1.write(continuousMeasurement, sizeof(continuousMeasurement)); Serial.println("2 - continuousMeasurement"); break;
        case 3:  Serial1.write(shutdownDevice, sizeof(shutdownDevice)); Serial.println("3 - shutdownDevice"); break;
        case 4:  Serial1.write(setAddress, sizeof(setAddress)); Serial.println("4 - setAddress"); break;
        case 5:  Serial1.write(distanceModMinus1, sizeof(distanceModMinus1)); Serial.println("5 - distanceModMinus1"); break;
        case 6:  Serial1.write(distanceModPlus1, sizeof(distanceModPlus1)); Serial.println("6 - distanceModPlus1"); break;
        case 7:  Serial1.write(timeInterval1S, sizeof(timeInterval1S)); Serial.println("7 - timeInterval1S"); break;
        case 8:  Serial1.write(setStartPointTop, sizeof(setStartPointTop)); Serial.println("8 - setStartPointTop"); break;
        case 9:  Serial1.write(setStartPointBackEnds, sizeof(setStartPointBackEnds)); Serial.println("9 - setStartPointBackEnds"); break;
        case 10: Serial1.write(setRange5m, sizeof(setRange5m)); Serial.println("10 - setRange5m"); break;
        case 11: Serial1.write(setRange10m, sizeof(setRange10m)); Serial.println("11 - setRange10m"); break;
        case 12: Serial1.write(setRange30m, sizeof(setRange30m)); Serial.println("12 - setRange30m"); break;
        case 13: Serial1.write(setRange50m, sizeof(setRange50m)); Serial.println("13 - setRange50m"); break;
        case 14: Serial1.write(setRange80m, sizeof(setRange80m)); Serial.println("14 - setRange80m"); break;
        case 15: Serial1.write(setFrequency0, sizeof(setFrequency0)); Serial.println("15 - setFrequency0"); break;
        case 16: Serial1.write(setFrequency5, sizeof(setFrequency5)); Serial.println("16 - setFrequency5"); break;
        case 17: Serial1.write(setFrequency10, sizeof(setFrequency10)); Serial.println("17 - setFrequency10"); break;
        case 18: Serial1.write(setFrequency20, sizeof(setFrequency20)); Serial.println("18 - setFrequency20"); break;
        case 19:
          n = 11;
          Serial1.write(setResolution1mm, sizeof(setResolution1mm));
          Serial.println("19 - setResolution1mm");
          break;
        case 20:
          n = 12;
          Serial1.write(setResolution0_1mm, sizeof(setResolution0_1mm));
          Serial.println("20 - setResolution0_1mm");
          break;
        case 21: Serial1.write(startMeasurementOnPowerOff, sizeof(startMeasurementOnPowerOff)); Serial.println("21 - startMeasurementOnPowerOff"); break;
        case 22: Serial1.write(startMeasurementOnPowerOn, sizeof(startMeasurementOnPowerOn)); Serial.println("22 - startMeasurementOnPowerOn"); break;
        case 23: Serial1.write(singleMeasurementBroadcast, sizeof(singleMeasurementBroadcast)); Serial.println("23 - singleMeasurementBroadcast"); break;
        case 24: Serial1.write(readCache, sizeof(readCache)); Serial.println("24 - readCache"); break;
        case 25: Serial1.write(controlLaserOpen, sizeof(controlLaserOpen)); Serial.println("25 - controlLaserOpen"); break;
        case 26: Serial1.write(controlLaserClose, sizeof(controlLaserClose)); Serial.println("26 - controlLaserClose"); break;
      }

      // ACK CHECK (Skipped for distance measurement commands, which are handled in Part 2)
      if (input != 1 && input != 2 && input != 23 && input != 24) {

        // Non-blocking timeout: wait for ACK up to 100ms
        unsigned long startTime = millis();
        int bytesRead = 0;

        Serial.print("Command Response (HEX): ");

        while (millis() - startTime < 100) {
          if (Serial1.available() > 0) {
            unsigned char incomingByte = Serial1.read();
            bytesRead++;

            Serial.print(incomingByte, HEX);
            Serial.print(" ");
          }
        }
        Serial.println();

        if (bytesRead > 0) {
          Serial.println(">> STATUS: Response received from sensor.");
        } else {
          Serial.println(">> DIAGNOSTIC ERROR: Sensor did not respond within 100ms.");
        }

        // Clean up leftover bytes and reset index
        flushSerial1();
        dataIndex = 0;
      }
    }
  }

  // ===================================================================
  //  DATA PACKET READING AND DECODING
  // ===================================================================
  while (Serial1.available() > 0) {
    unsigned char currentByte = Serial1.read();

    // SMART ALIGNMENT SYSTEM:
    // If the expected header sequence is not detected, discard the byte and reset index.
    // Base Address = 0x80, Response Command = 0x06, Sub-Command = 0x82 or 0x83.
    if (dataIndex == 0 && currentByte != 0x80) continue;
    if (dataIndex == 1 && currentByte != 0x06) {
      dataIndex = 0;
      continue;
    }
    if (dataIndex == 2 && currentByte != 0x82 && currentByte != 0x83) {
      dataIndex = 0;
      continue;
    }

    // Save valid header and subsequent bytes into the buffer
    data[dataIndex] = currentByte;
    dataIndex++;

    // Process frame once the full packet size is reached
    if (dataIndex >= n) {

      // Calculate Two's Complement Checksum
      unsigned char Check = 0;
      for (int i = 0; i < (n - 1); i++) {
        Check += data[i];
      }
      Check = ~Check + 1;

      // Validate packet checksum
      if (data[n - 1] == Check) {

        // Check for Out of Range sensor exception
        if (data[3] == 'E' && data[4] == 'R' && data[5] == 'R') {
          Serial.println("Error: Out of Range measurement");
        } else {
          long distance = 0;

          if (n == 12) {  // 0.1mm Resolution (12 bytes total)
            distance = (data[3] - '0') * 100000L
                     + (data[4] - '0') * 10000L
                     + (data[5] - '0') * 1000L
                     + (data[7] - '0') * 100L
                     + (data[8] - '0') * 10L
                     + (data[9] - '0') * 1L;
            int tenths = (data[10] - '0');

            Serial.print(distance);
            Serial.print(",");
            Serial.println(tenths);
          } else {  // 1mm Resolution (11 bytes total)
            distance = (data[3] - '0') * 100000L
                     + (data[4] - '0') * 10000L
                     + (data[5] - '0') * 1000L
                     + (data[7] - '0') * 100L
                     + (data[8] - '0') * 10L
                     + (data[9] - '0') * 1L;

            Serial.println(distance);
          }
        }
      } else {
        Serial.println("Invalid Data! (Checksum Error, packet discarded)");
      }

      // Packet processing complete, reset index for the next frame
      dataIndex = 0;
    }
  }
}
