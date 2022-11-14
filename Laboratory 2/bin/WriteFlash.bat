@echo Writing flash.bin to AT91SAM7S256
@echo off
openocd-ftd2xx -f /sam7s256_flash.cfg -s "C:/Program Files/openOCD" 2> null