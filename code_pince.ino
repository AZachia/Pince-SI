#include <Servo.h>
#include <Wire.h>
#include <rgb_lcd.h>

#define FORCEMAX 400     // Bloque la fermeture de la pince a 400 sur le capteur de force

rgb_lcd monRgb;          //écran lcd
Servo servo_pin_A1;      // Servomoteur de la pince sur la broche A1
int potentiometre = A0;  // potentiommetre sur la broche A0
int force = A2;          // Capteur de force sur la broche A2
int maxAt;                // Valeur a partir de laquel un objet est attrpé
bool ForceOn = false;    // Si la force du capteur de force est supertieur au max

void setup() {
    servo_pin_A1.attach(A1);
    //   servo_pin_A1.write(40);
    pinMode(potentiometre, INPUT);
    maxAt = analogRead(potentiometre);
    pinMode(force, INPUT);
    Serial.begin(9600);
    monRgb.branch();
}

void loop() {
    int current = analogRead(potentiometre); // Valeure actuelle du potentiometre

    if (analogRead(force) > FORCEMAX) { // si le capteur de force est appuyé plus que le maximum autorisé pour l'ouverture
        if (!ForceOn) {
            maxAt = current;
            ForceOn = true;
        }

        if (current > maxAt) {
          maxAt = current - 1;
        }

    } else {
        ForceOn = false;
    }

    if (!ForceOn or current > maxAt) {                       // Bloque la fermeture de la pince a 400 sur le capteur de force
        servo_pin_A1.write(map(current, 0, 1023, 90, 180)); // valeur du potentiometre: entre 0 et 1023 | angle du servo: de 40° a 140°
    }

    //affichage de la force sur l'écran lcd
    if (!ForceOn) {
        monRgb.placerCurseurEn(0, 0);
        monRgb.ecrire("Force ok:");
        monRgb.ecrire(analogRead(force));
    } else {
        monRgb.placerCurseurEn(0, 0);
        monRgb.ecrire("Force > 400:");
        monRgb.ecrire(analogRead(force));
    }
}
