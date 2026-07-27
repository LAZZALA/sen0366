# DFRobot SEN0366 Laser Rangefinder - Arduino Driver

Arduino driver and test suite for the DFRobot SEN0366 Laser Distance Sensor.

This repository contains a refactored firmware developed during a Bachelor's Thesis project to fix communication instability and frame dropping issues present in existing open-source codebases.

## Technical Notes and Sensor Limitations

Experimental testing and metrological characterization highlighted several differences between the manufacturer datasheet and actual module behavior:

1. **Board Compatibility (Arduino UNO R4 vs R3):** Using `SoftwareSerial` on 8-bit boards like Arduino UNO R3 (ATmega328P) causes heavy CPU overhead, timing lag, and firmware freezes. The implementation was moved to an Arduino UNO R4 Minima to leverage its hardware UART port (`Serial1` on pins 0/1) for asynchronous data handling.
2. **Maximum Sampling Rate (4 Hz):** Despite being advertised as a 20 Hz module, empirical measurements show the sensor caps its output update rate at 4 Hz (250 ms period). Sending frequency configuration commands does not override this internal hardware bottleneck.
3. **Resolution (1 mm vs 0.1 mm):** Though the sensor accepts a 0.1 mm mode (12-byte payload), the extra decimal digit is dominated by intrinsic optical noise . Operating at 1 mm resolution (11-byte payload) provides cleaner data and easier buffer processing.
4. **Undocumented Internal Behavior:** The sensor periodically shows stochastic measurement spikes and brief delays. These appear to stem from internal gain adjustments or unrecorded auto-calibration loops.

> If you have further insights or have decoded more of the sensor's low-level register behaviors, feel free to submit a Pull Request.

## Wiring

| Sensor Pin | Arduino Pin | Function |
| :--- | :--- | :--- |
| VCC | 5V / External Power | Power supply |
| GND | GND | Ground |
| TX | RX1 (Pin 0 on UNO R4) | Sensor Transmit -> Arduino Receive |
| RX | TX1 (Pin 1 on UNO R4) | Arduino Transmit -> Sensor Receive |

## Usage

1. Upload `code.ino` to the board.
2. Open the Serial Monitor at **115200 baud**.
3. Input an integer command (1–26) to execute operations:
   * `1`: Single measurement[cite: 1]
   * `2`: Continuous measurement[cite: 1]
   * `19`: Set 1 mm resolution (recommended)[cite: 1]
   * `20`: Set 0.1 mm resolution[cite: 1]
   * `25`: Turn laser ON[cite: 1]
   * `26`: Turn laser OFF[cite: 1]

## Credits

Forked and refactored from [dDenVil/sen0366](https://github.com/dDenVil). 
Validated as part of a Bachelor's Degree Thesis in Biomedical Engineering at the University of Genoa.

## License

This project is open-source and shared under the same terms as the original upstream repository.
