TCP 기반 원격 장치 제어 시스템

(Raspberry Pi 4 / Ubuntu Linux)

1. 프로젝트 개요

본 프로젝트는 TCP 통신을 이용한 원격 장치 제어 시스템을 구현하는 것을 목표로 한다.
서버는 Raspberry Pi 4에서 동작하며, 클라이언트는 Ubuntu Linux 환경에서 실행된다.
서버는 데몬(Daemon) 프로세스 형태로 동작하고, 각 장치는 동적 라이브러리(.so)로 분리하여 필요 시 라이브러리 파일만 교체하여 기능을 확장할 수 있도록 설계하였다.
또한, 개발 환경과 Raspberry Pi 환경이 다르기 때문에 크로스 컴파일을 지원하여 Ubuntu에서 빌드한 파일을 Raspberry Pi에서도 원활하게 실행할 수 있도록 구성하였다.

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
├── rpi_headers/        # 라즈베리파이 하드웨어 관련 헤더
│   ├── wiringPi.h
│   ├── softTone.h
│   ├── softPwm.h
│   └── pcf8591.h
│
├── include/            # 장치 인터페이스 헤더
│   ├── device.h
│   ├── cds_led.h
│   ├── led_pwm.h
│   ├── music.h
│   └── segment.h
│
├── src/                # 구현 소스
│   ├── main.c          # TCP 서버 & 데몬 프로세스
│   ├── client.c        # 클라이언트 소스
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
├── Makefile            # 빌드 설정
├── main                # 서버 실행 파일
├── client              # 클라이언트 실행 파일
└── README.md           # 프로젝트 설명

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
조도 값에 따라 LED ON/OFF 제어 (저는 225 보다 어두우면 LED ON으로 진행 -> 환경에 따라 다르게 설정하시는 것을 추천)
{조도값은 0(밝기 최대)-255(밝기 최소) 입니다.}

2) PWM LED (libled_pwm.so)

스위치 입력에 따라 LED 밝기 단계 조절 (끄기, 낮은 밝기, 중간 밝기, 최대 밝기)
softPwm 사용

3) Music / Buzzer (libmusic.so)

스위치 입력 시 멜로디 재생 (학교종이 땡땡땡)
재생 중 정지 가능

4) 7-Segment (libsegment.so)

입력 숫자부터 0까지 카운트다운
종료 후 음계 재생 (도레미파솔라시도)

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

11. 실제 시연 방법 //@@@@@@@@ 여기가 핵심 실행 방법입니다.

11-1. S/W

우분투
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
├── lib/
│
├── Makefile
└── README.md


위 구조에서 상위 디렉토리(project)에서 
mkdir rpi_headers // 디랙토리 생성
scp suseok@192.168.0.35:~/rpi_headers/wiringPi.h ~/rpi_headers/wiringPi.h // wiringPi 사용 해더
scp suseok@192.168.0.35:~/rpi_headers/pcf8591.h ~/rpi_headers/pcf8591.h  // IC2(pcf8591) 사용을 위한 해더
scp suseok@192.168.0.35:/usr/include/softPwm.h /home/changjoh/lsnp/linux_project_22/rpi_headers/softPwm.h // PWM 제어 사용에 필요한 해더
scp suseok@192.168.0.35:/usr/include/softTone.h /home/changjoh/lsnp/linux_project_22/rpi_headers/softTone.h // PWM 제어 사용에 필요한 해더

make 명령어 실행 // 우분투에서 .so file과 main 및 clinet 실행 파일이 생성된다. 

그리고 서버와 실제 필요한 라이브러리 파일만 라즈베리에 전송한다. 예를들어 라즈베리 project_rpi 디렉토리에 시작이면 아래와 같다.
scp main suseok@192.168.0.35:/home/suseok/project_rpi /
scp -r lib suseok@192.168.0.35:/home/suseok/project_rpi/

//라즈베리 이동
ssh suseok@192.168.0.35 // 주소는 본인 라즈베리 파이 주소로 수정
cd ~/project
./main 서버 실행

//우분투
project/ 다랙토리의 ./client 실행 후 192.168.0.35 입력 후 접속하여 사용.

라즈베리 환경에서
cat /home/suseok/daemon.log 입력한 명령어 확인 가능

11-2. H/W
//내가 물리적으로 연결한 위치
@7-segment

GPIO pin 기준

a GPIO 23 (BCM13)
b GPIO 22 (BCM6)
c GPIO 27 (BCM16)
d GPIO 28 (BCM20)
e GPIO 29 (BCM21)
f GPIO 24 (BCM19)
g GPIO 25 (BCM26)
dp GPIO 26 (BCM12) // 이건 사용 안했습니다.

@I2C(PCF8591)

SDA BCM2 - wPi 8 (BCM2)
SCL BCM3 - wPi 9 (BCM3)
GDN  0v

@부저
부저 - GPIO25 (BCM6)
GND 0v

@스위치

SW1 - GPIO 5 (BCM 24)
GND 0v

SW2 GPIO 4 (BCM 23)
GND 0v

suseok@raspberrypi:~/project $ gpio readall
 +-----+-----+---------+------+---+---Pi 4B--+---+------+---------+-----+-----+
 | BCM | wPi |   Name  | Mode | V | Physical | V | Mode | Name    | wPi | BCM |
 +-----+-----+---------+------+---+----++----+---+------+---------+-----+-----+
 |     |     |    3.3v |      |   |  1 || 2  |   |      | 5v      |     |     |
 |   2 |   8 |   SDA.1 | ALT0 | 1 |  3 || 4  |   |      | 5v      |     |     |
 |   3 |   9 |   SCL.1 | ALT0 | 1 |  5 || 6  |   |      | 0v      |     |     |
 |   4 |   7 | GPIO. 7 |   IN | 1 |  7 || 8  | 1 | IN   | TxD     | 15  | 14  |
 |     |     |      0v |      |   |  9 || 10 | 0 | OUT  | RxD     | 16  | 15  |
 |  17 |   0 | GPIO. 0 |   IN | 1 | 11 || 12 | 0 | OUT  | GPIO. 1 | 1   | 18  |
 |  27 |   2 | GPIO. 2 |   IN | 0 | 13 || 14 |   |      | 0v      |     |     |
 |  22 |   3 | GPIO. 3 |   IN | 0 | 15 || 16 | 0 | IN   | GPIO. 4 | 4   | 23  |
 |     |     |    3.3v |      |   | 17 || 18 | 1 | IN   | GPIO. 5 | 5   | 24  |
 |  10 |  12 |    MOSI |   IN | 0 | 19 || 20 |   |      | 0v      |     |     |
 |   9 |  13 |    MISO |  OUT | 0 | 21 || 22 | 0 | OUT  | GPIO. 6 | 6   | 25  |
 |  11 |  14 |    SCLK |   IN | 0 | 23 || 24 | 1 | IN   | CE0     | 10  | 8   |
 |     |     |      0v |      |   | 25 || 26 | 1 | IN   | CE1     | 11  | 7   |
 |   0 |  30 |   SDA.0 |   IN | 1 | 27 || 28 | 1 | IN   | SCL.0   | 31  | 1   |
 |   5 |  21 | GPIO.21 |  OUT | 0 | 29 || 30 |   |      | 0v      |     |     |
 |   6 |  22 | GPIO.22 |   IN | 1 | 31 || 32 | 1 | OUT  | GPIO.26 | 26  | 12  |
 |  13 |  23 | GPIO.23 |   IN | 1 | 33 || 34 |   |      | 0v      |     |     |
 |  19 |  24 | GPIO.24 |   IN | 1 | 35 || 36 | 1 | IN   | GPIO.27 | 27  | 16  |
 |  26 |  25 | GPIO.25 |   IN | 1 | 37 || 38 | 0 | IN   | GPIO.28 | 28  | 20  |
 |     |     |      0v |      |   | 39 || 40 | 1 | IN   | GPIO.29 | 29  | 21  |
 +-----+-----+---------+------+---+----++----+---+------+---------+-----+-----+
 | BCM | wPi |   Name  | Mode | V | Physical | V | Mode | Name    | wPi | BCM |
 +-----+-----+---------+------+---+---Pi 4B--+---+------+---------+-----+-----+