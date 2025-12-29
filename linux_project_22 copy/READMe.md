TCP 기반 원격 장치 제어 시스템

(Raspberry Pi 4 / Ubuntu Linux)

1. 프로젝트 개요

본 프로젝트는 TCP 통신을 이용한 원격 장치 제어 시스템을 구현하는 것을 목표로 한다.
서버는 Raspberry Pi 4에서 동작하며, 클라이언트는 Ubuntu Linux 환경에서 실행된다.
서버는 데몬(Daemon) 프로세스 형태로 동작하고, 각 장치는 동적 라이브러리(.so) 로 분리하여 필요 시 라이브러리 파일만 교체하여 기능을 확장할 수 있도록 설계하였다.

2. 개발 환경

OS
Server: Raspberry Pi OS (Raspberry Pi 4)
Client: Ubuntu Linux
Language: C
Build Tool: Make
Library
POSIX Thread (pthread)
Dynamic Loading (dlopen, dlsym)
wiringPi (GPIO, softPwm, softTone)
I2C (PCF8591)

3. 사용 장치

LED (220Ω 저항 사용)
CDS 센서 (PCF8591 I2C)
부저 (FQ-035)
7-Segment (s-5101asr)
PWM LED

스위치 입력

4. 전체 시스템 구조
Client (Ubuntu)
   |
   |  TCP Command
   v
Server (Raspberry Pi, Daemon)
   |
   |-- dlopen()
   |-- pthread
   |
   +-- libcds_led.so     (조도센서 기반 LED 제어)
   +-- libled_pwm.so     (PWM LED 제어)
   +-- libmusic.so       (부저 음악 재생)
   +-- libsegment.so     (7-Segment 표시)

5. 디렉터리 구조
project/
├── include/            # 장치 인터페이스 헤더
│   ├── device.h
│   ├── cds_led.h
│   ├── led_pwm.h
│   ├── music.h
│   └── segment.h
│
├── src/                # 구현 소스
│   ├── main.c          # TCP 서버 & 데몬 프로세스
│   ├── cds_led.c
│   ├── led_pwm.c
│   ├── music.c
│   └── segment.c
│
├── lib/                # 동적 라이브러리(.so)
│   ├── libcds_led.so
│   ├── libled_pwm.so
│   ├── libmusic.so
│   └── libsegment.so
│
├── Makefile
└── README.md

6. 구현 내용
6.1 TCP 서버

TCP 포트 5000 사용
클라이언트 명령 수신 후 장치 제어
SIGINT(Ctrl+C) 신호에 대해서만 정상 종료 처리
서버 종료 시 실행 중인 모든 장치 스레드 정리

6.2 데몬 프로세스

서버는 fork(), setsid()를 이용해 데몬 프로세스로 동작
터미널과 독립적으로 백그라운드 실행 가능

6.3 동적 라이브러리 구조

각 장치는 독립적인 .so 라이브러리로 구성
서버는 dlopen() / dlsym()을 이용해 런타임에 로드
장치 기능 업그레이드 시 서버 재컴파일 없이 .so 파일만 교체 가능

6.4 멀티 스레드 장치 제어

각 장치는 별도의 스레드에서 동작
running 플래그를 이용해 안전하게 시작/종료 제어
pthread_join()을 통해 스레드 종료 대기

6.5 장치별 기능
1) CDS + LED (libcds_led.so)

CDS 센서 값을 I2C(PCF8591)로 읽음
조도 값에 따라 LED ON/OFF 제어

2) PWM LED (libled_pwm.so)

스위치 입력에 따라 LED 밝기 단계 조절
softPwm 사용

3) Music / Buzzer (libmusic.so)

스위치 입력 시 멜로디 재생
재생 중 정지 가능

4) 7-Segment (libsegment.so)

입력 숫자부터 0까지 카운트다운
종료 후 음계 재생

7. 명령 프로토콜 (Client → Server)
명령	동작
cds	CDS LED 시작
cs	CDS LED 정지
led	PWM LED 시작
ls	PWM LED 정지
mus	음악 재생 시작
ms	음악 정지
sN	7-Segment N부터 카운트다운
ss	7-Segment 정지
EXIT	서버 종료

8. 빌드 및 실행 방법

빌드
make

실행
export LD_LIBRARY_PATH=./lib
./main

9. 특징 및 장점

동적 라이브러리 기반 확장성
멀티 스레드 기반 동시 장치 제어
데몬 프로세스를 이용한 안정적인 서버 운영
장치 제어 코드와 서버 로직의 완전 분리

10. 결론

본 프로젝트는 TCP 통신, 데몬 프로세스, 멀티 스레드, 동적 라이브러리를 통합하여
리눅스 기반 임베디드 시스템에서의 확장 가능하고 안정적인 원격 제어 구조를 구현하였다.