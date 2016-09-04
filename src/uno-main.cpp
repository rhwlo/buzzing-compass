#include <Wire.h>
#include <Adafruit_LSM303_U.h>

#define BASE_MAGNETIC_STRENGTH      15000 // ms / (radian * µT)
#define GRANULARITY                 10 // milliseconds
#define PIN 6
#define BUZZER_Y 3
#define BUZZER_Z 5

Adafruit_LSM303_Mag_Concise mag = Adafruit_LSM303_Mag_Concise(12345);

vector3_t magVector;

void setup() {
    Serial.begin(9600);
    mag.begin();
    pinMode(BUZZER_Y, OUTPUT);
    pinMode(BUZZER_Z, OUTPUT);
}

void pulse_with_intensity(vector3_t * vec) {
    uint8_t period;
    float abs_mag = sqrt(pow(vec->x, 2) + pow(vec->y, 2) + pow(vec->z, 2));

    // We want the full magnetic field strength to determine the period
    period = BASE_MAGNETIC_STRENGTH / (1 + abs_mag);

    // But when we're judging direction, we want the absolute magnitude only of the yz plane
    abs_mag = sqrt(pow(vec->y, 2) + pow(vec->z, 2));
    analogWrite(BUZZER_Y, floor(240 * vec->y / abs_mag));
    analogWrite(BUZZER_Z, floor(240 * vec->z / abs_mag));
    delay(period);
    analogWrite(BUZZER_Y, 0);
    analogWrite(BUZZER_Z, 0);
    delay(period * 2);
}

float vecAbsolute(vector3_t * vec) {
    return sqrt(pow(vec->x, 2) + pow(vec->y, 2) + pow(vec->z, 2));
}

void normalizeVec(vector3_t * vec) {
    float absVal = vecAbsolute(vec);
    if (absVal != 0) {
        vec->x /= absVal;
        vec->y /= absVal;
        vec->z /= absVal;
    }
}

void loop(void)
{
    float magAbs, heading;
    mag.getVector(&magVector);

    magAbs = vecAbsolute(&magVector);

    if (magAbs == 0) {
        return;
    }

    heading = atan2(magVector.y, magVector.z);
    while (heading < 0)
        heading += PI * 2;

    Serial.print("magnetic field strength: "); Serial.print(magAbs); Serial.println(" uT");
    Serial.print("magnetic vector: ["); Serial.print(magVector.x); Serial.print(" "); Serial.print(magVector.y); Serial.print(" "); Serial.print(magVector.z); Serial.println("]");
    Serial.print("heading: "); Serial.print(heading * 180 / PI); Serial.println(" degrees");
    Serial.println("\n");

    pulse_with_intensity(&magVector);
}
