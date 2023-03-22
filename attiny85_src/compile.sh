avr-gcc example.c -o out.a -Os -DF_CPU=8000000 -mmcu=attiny85
avr-objcopy -j .text -j .data -O binary out.a out.bin
