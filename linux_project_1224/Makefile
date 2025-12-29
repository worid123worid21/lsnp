CC = gcc
CFLAGS = -Wall -O2 -fPIC -Iinclude
LDFLAGS = -ldl -lpthread -lwiringPi   # <- softPwm/softTone 제거

LIB_DIR = lib
SRC_DIR = src

LIBS = libcds_led.so libled_pwm.so libmusic.so libsegment.so

TARGET = main

all: $(LIBS) $(TARGET)

libcds_led.so:
	$(CC) $(CFLAGS) -shared -o $(LIB_DIR)/$@ $(SRC_DIR)/cds_led.c $(LDFLAGS)

libled_pwm.so:
	$(CC) $(CFLAGS) -shared -o $(LIB_DIR)/$@ $(SRC_DIR)/led_pwm.c $(LDFLAGS)

libmusic.so:
	$(CC) $(CFLAGS) -shared -o $(LIB_DIR)/$@ $(SRC_DIR)/music.c $(LDFLAGS)

libsegment.so:
	$(CC) $(CFLAGS) -shared -o $(LIB_DIR)/$@ $(SRC_DIR)/segment.c $(LDFLAGS)

$(TARGET):
	$(CC) $(CFLAGS) -o $@ $(SRC_DIR)/main.c $(LDFLAGS)

clean:
	rm -f $(LIB_DIR)/*.so $(TARGET)
