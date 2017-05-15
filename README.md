# mruby-esp32-app-mirb

mirb (Embeddable Interactive Ruby Shell) for ESP32.

![screen](images/screen.png)

*This project is made with reference to [carsonmcdonald/mruby-esp32](https://github.com/carsonmcdonald/mruby-esp32), many thanks.*

## Build with ESP-IDF

0) This project requires [ESP-IDF](https://github.com/espressif/esp-idf). Please setup it beforehand.

1) Clone the code

	```
	$ git clone --recursive https://github.com/mruby-esp32/mruby-esp32-app-mirb.git
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

## Configuration

If you want to add mrbgems please edit `components/mruby/build_config.rb`.

```ruby
MRuby::CrossBuild.new('esp32') do |conf|

  # ...(snip)...

  comf.gem :github => 'xxx/xxx'

end
```
