# 서버: ARM64 크로스 컴파일
CC_ARM = aarch64-linux-gnu-gcc
RPI_HEADERS = /home/changjoh/lsnp/linux_project_22/rpi_headers
CFLAGS_ARM = -Wall -O2 -fPIC -Iinclude -I$(RPI_HEADERS)
LDFLAGS_ARM = -ldl -lpthread -lwiringPi

# 클라이언트: Ubuntu 기본 gcc
CC_UBU = gcc
CFLAGS_UBU = -Wall -O2 -Iinclude
LDFLAGS_UBU = -ldl -lpthread

SRC_DIR = src
LIB_DIR = lib

LIBS = libcds_led.so libled_pwm.so libmusic.so libsegment.so
TARGET = main
CLIENT = client

all: $(LIBS) $(TARGET) $(CLIENT)

# 서버용 라이브러리 (ARM64)
libcds_led.so:
	$(CC_ARM) $(CFLAGS_ARM) -shared -o $(LIB_DIR)/$@ $(SRC_DIR)/cds_led.c $(LDFLAGS_ARM)

libled_pwm.so:
	$(CC_ARM) $(CFLAGS_ARM) -shared -o $(LIB_DIR)/$@ $(SRC_DIR)/led_pwm.c $(LDFLAGS_ARM)

libmusic.so:
	$(CC_ARM) $(CFLAGS_ARM) -shared -o $(LIB_DIR)/$@ $(SRC_DIR)/music.c $(LDFLAGS_ARM)

libsegment.so:
	$(CC_ARM) $(CFLAGS_ARM) -shared -o $(LIB_DIR)/$@ $(SRC_DIR)/segment.c $(LDFLAGS_ARM)

# 서버(main)
$(TARGET): $(LIBS)
	$(CC_ARM) $(CFLAGS_ARM) -o $@ $(SRC_DIR)/main.c $(LDFLAGS_ARM)

# 클라이언트(client)
$(CLIENT):
	$(CC_UBU) $(CFLAGS_UBU) -o $@ $(SRC_DIR)/client.c $(LDFLAGS_UBU)

clean:
	rm -f $(LIB_DIR)/*.so $(TARGET) $(CLIENT)