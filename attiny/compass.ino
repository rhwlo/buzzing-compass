#include <TinyWireM.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>

#define BUZZER_Y                    1
#define BUZZER_Z                    4
#define BASE_MAGNETIC_STRENGTH      30000 // ms / (radian * µT)
// ^^ I need to re-assess this one, based off of the ~25 µT measured here.
#define PI                          3.1416
#define GRANULARITY                 10 // milliseconds

Adafruit_LSM303_Mag_Concise mag = Adafruit_LSM303_Mag_Concise(12345);
vector3_t mag_vector;

void pulse_with_intensity(const vector3_t &vec) {
    uint8_t i, period;

    float abs_mag = sqrt(pow(vec.x, 2) + pow(vec.y, 2) + pow(vec.z, 2));

    // We want the full magnetic field strength to determine the period
    period = BASE_MAGNETIC_STRENGTH / (1 + abs_mag);

    // But when we're judging direction, we want the absolute magnitude only of the yz plane
    abs_mag = sqrt(pow(vec.y, 2) + pow(vec.z, 2));

    analogWrite(BUZZER_Y, floor(120 * (1 + vec.y / abs_mag)));
    analogWrite(BUZZER_Z, floor(120 * (1 + vec.z / abs_mag)));
    delay(period);
    analogWrite(BUZZER_Y, 0);
    analogWrite(BUZZER_Z, 0);
    delay(period * 2);
}

void setup() {
    pinMode(BUZZER_Y, OUTPUT);
    pinMode(BUZZER_Z, OUTPUT);

    mag.enableAutoRange(true);
    while (!mag.begin());
}

void loop() {
    // to avoid durations that are too long, we keep running mag.getVector until it's true.
    while (!mag.getVector(&mag_vector));
    pulse_with_intensity(mag_vector);
}
