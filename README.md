# mruby-esp32-app-mirb
mirb (Embeddable Interactive Ruby Shell) for ESP32

![screen](images/screen.png)

## Build with ESP-IDF

This project requires [ESP-IDF](https://github.com/espressif/esp-idf). Please setup it beforehand.

1) Clone the code

```
$ git clone --recursive https://github.com/pandax381/mruby-esp32-app-mirb.git
$ cd mruby-esp32-app-mirb
```

2) Setup serial-port

```
$ make menuconfig
```

  `Serial flasher config` > `Default serial port` > `/dev/cu.xxx`
  
3) Build project

```
$ make
```

## Flash & Run

```
$ make flash monitor
```

Startup mirb, REPL (Read–eval–print loop)
