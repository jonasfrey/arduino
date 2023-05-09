# update arduino repo
sudo apt update

# install arduino binary
sudo apt install arduino-mk

this will install stuff in
cd /usr/share/arduino

# create a folder
mkdir sketchbook

# create a .ino script

`c
    int n_pin_builtin_led = 13;
void setup(){

    pinMode(n_pin_builtin_led, OUTPUT);
}
void loop(){
    digitalWrite(n_pin_builtin_led, 1);
    delay(100);
    digitalWrite(n_pin_builtin_led, 0);
    delay(1000);
}
`

# create a makefile
```makefile
ARDUINO_DIR = /usr/share/arduino
# fint the device with 'hwinfo --short' or 'lsusb -v'
ARDUINO_PORT = /dev/

```

