# Vitamin Modules

Assorted modules for VCV Rack.

## Vitamin B (Effect Bypass)

An 8-channel effect bypass. When channel is enabled, connects input to send and
return to output. When disabled, connects input directly to output and zeroes
send.

![Vitamin B Screenshot](https://raw.githubusercontent.com/xaviershay/vcv-modules/master/images/vitamin-b.png)

## Vitamin P (Pan)

Pan a polyphonic mono input using constant power law to maintain overall
volume.

`CV` is also polyphonic. It's voltages are summed with the knob value, so for
full range of control either:

* set the knob to left position with a unipolar input 0V to 10V.
* set the knob to middle position with a bipolar input -5V to 5V.

![Vitamin P Screenshot](https://raw.githubusercontent.com/xaviershay/vcv-modules/master/images/vitamin-p.png)

# Development

    export RACK_DIR=../path/to/rack
    ruby create_module.rb Bypass # Update CPP from SVG. Better than helper.py.
    make && make install

Font is lakmus.
