#include <wiringPi.h>
#include <pcf8591.h>
#include <stdio.h>
#include <stdlib.h>

#define LED 1        // GPIO18 -> wPi 1
#define CDS_CH 0     // PCF8591 A0 채널

#define I2C_ADDR 0x48 // PCF8591 기본 I2C 주소
#define THRESHOLD 200 // LED ON/OFF 기준값 (0~255)

// PCF8591 초기화
void setupPCF8591() {
    if (pcf8591Setup(I2C_ADDR, I2C_ADDR) == -1) {
        printf("PCF8591 초기화 실패!\n");
        exit(1);
    }
}

int main(void) {
    wiringPiSetup();           // wPi 번호 기준 초기화
    pinMode(LED, OUTPUT);      // LED 출력 설정

    setupPCF8591();            // PCF8591 초기화

    int cdsValue;

    while (1) {
        cdsValue = analogRead(I2C_ADDR + CDS_CH); // A0 값 읽기 (0~255)
        printf("조도값: %d\n", cdsValue);

        if (cdsValue < THRESHOLD) {
            digitalWrite(LED, LOW);  // 어두움 → LED ON
        } else {
            digitalWrite(LED, HIGH);   // 밝음 → LED OFF
        }

        delay(500); // 0.5초마다 갱신
    }

    return 0;
}
