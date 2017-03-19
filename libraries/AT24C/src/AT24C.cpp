/*
 * Arduino library support for the Atmel AT24C family I2C EEPROMs
 *
 * Copyright (c)2017 Pavol Hluchy. All rights reserved
 * License GNU LGPLv3
*/
#include "AT24C.h"
#include <Wire.h>
#include <string.h>

#ifndef min
#	define min(a, b) (((a)<(b))? (a) : (b))
#endif


at24c::at24c(uint8_t i2c, uint32_t size, uint16_t page, bool addr2B=false, uint8_t wDelay=5) :_i2c(i2c), _size(size), _page(page), _addr2B(addr2B), _wDelay(wDelay)
{
	Wire.begin();
}

bool at24c::have() const
{
	Wire.beginTransmission(_i2c);
	return !Wire.endTransmission();
}

void at24c::sendAddress(const uint32_t address) const
{
	if (_addr2B) {
		Wire.write((uint8_t)(address >> 8) & 0xff); // MSB
		Wire.write((uint8_t)address & 0xff); // LSB
		}
	else {
		Wire.write((uint8_t)address);
		}
}

void at24c::write(const uint32_t address, const uint8_t b) const
{
	Wire.beginTransmission(_i2c);
	sendAddress(address);
	Wire.write(b);
	Wire.endTransmission();
	delay(_wDelay);
}

size_t at24c::write(const uint32_t address, const uint8_t* data, const size_t length) const
{
	if (length+address>=_size) { // too long
		return 0;
		}
	uint8_t wLen, bSize=min(_page, 16);

	Wire.beginTransmission(_i2c);
	sendAddress(address);
	wLen=Wire.write(data, min(length, bSize-(address % bSize)));
	Wire.endTransmission();
	delay(_wDelay);
	if (wLen==length) {
		return wLen;
		}

	do {
		Wire.beginTransmission(_i2c);
		sendAddress(address+wLen);
		wLen+=Wire.write(data+wLen, min(bSize, length-wLen));
		Wire.endTransmission();
		delay(_wDelay);
		} while (wLen<length);
	return wLen;
}

uint8_t at24c::read(const uint32_t address) const
{
	if (address>=_size) {
		return 0xff;
		}
	Wire.beginTransmission(_i2c);
	sendAddress(address);
	Wire.endTransmission();
	Wire.requestFrom(_i2c, (uint8_t)1);
	if (Wire.available()) {
		return Wire.read();
		}
	return 0xff;
}

size_t at24c::read(const uint32_t address, uint8_t* buf, const size_t length) const
{
	uint32_t rLen, i, r=0;
	if (address>=_size) {
		return 0xff;
		}
	if (!length) {
		return 0;
		}
	rLen= length+address>_size
		? _size-address
		: length;

	Wire.beginTransmission(_i2c);
	sendAddress(address);
	Wire.endTransmission();
	Wire.requestFrom(_i2c, rLen);
	for (i=0; i<rLen; i++) {
		if (Wire.available()) {
			buf[i]=Wire.read();
			r++;
			}
		else {
			return r;
			}
		}
	return r;
}

void at24c::clear()
{
	uint8_t i;
	uint8_t bSize=min(_page, 16);
	uint32_t bNum=_size/bSize;
	uint8_t block[bSize];

	memset(block, 0xff, bSize);
	for (i=0; i<bNum; i++) {
		write(i*bSize, block, bSize);
		}
}

at24c01::at24c01(const uint8_t i2c) :at24c(i2c, 128, 8, false) { };
at24c02::at24c02(const uint8_t i2c) :at24c(i2c, 256, 8, false) { };
at24c04::at24c04(const uint8_t i2c) :at24c(i2c, 512, 16, false) { };
at24c08::at24c08(const uint8_t i2c) :at24c(i2c, 1024, 16, false) { };
at24c16::at24c16(const uint8_t i2c) :at24c(i2c, 2048, 16, false) { };
at24c32::at24c32(const uint8_t i2c) :at24c(i2c, 4096, 32, true, 10) { };
at24c64::at24c64(const uint8_t i2c) :at24c(i2c, 8192, 32, true, 10) { };
at24c128::at24c128(const uint8_t i2c) :at24c(i2c, 16384, 64, true) { };
at24c256::at24c256(const uint8_t i2c) :at24c(i2c, 32768, 64, true) { };
at24c512::at24c512(const uint8_t i2c) :at24c(i2c, 65536, 128, true) { };
at24c1024::at24c1024(const uint8_t i2c) :at24c(i2c, 131072, 256, true) { };
