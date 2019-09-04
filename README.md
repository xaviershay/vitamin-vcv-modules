# Xavier's VCV Modules

Assorted modules for VCV Rack.

## Bypass

A 10-channel effect bypass. When channel is enabled, connects input to send and
return to output. When disabled, connects input directly to output and zeroes
send.

![Bypass Screenshot](https://raw.githubusercontent.com/xaviershay/vcv-modules/master/images/bypass.png)

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
    make && make install

Font is http://www.latofonts.com/ or lakmus
