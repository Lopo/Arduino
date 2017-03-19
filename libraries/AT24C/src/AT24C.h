#ifndef _AT24C_h
#define _AT24C_h

#if defined(ARDUINO) && ARDUINO>=100
#	include "Arduino.h"
#else
extern "C" {
#	include "WProgram.h"
}
#endif


class at24c
{
protected:
	uint8_t _i2c; // 0x50
	uint32_t _size;
	uint16_t _page;
	bool _addr2B; // false
	uint8_t _wDelay; // 5

	void sendAddress(const uint32_t address) const;

public:
	at24c(uint8_t i2c, uint32_t size, uint16_t page, bool addr2B, uint8_t wDelay);
	bool have() const;
	void write(const uint32_t address, const uint8_t b) const;
	size_t write(const uint32_t address, const uint8_t* data, const size_t length) const;
	uint8_t read(const uint32_t address) const;
	size_t read(const uint32_t address, uint8_t* buf, const size_t length) const;

	void clear();
};


class at24c01 // http://www.atmel.com/Images/doc0180.pdf
	: public at24c
{
public:
	at24c01(uint8_t i2c);
};


class at24c02 // http://www.atmel.com/Images/doc0180.pdf
	: public at24c
{
public:
	at24c02(uint8_t i2c);
};


class at24c04 // http://www.atmel.com/Images/doc0180.pdf
	: public at24c
{
public:
	at24c04(uint8_t i2c);
};


class at24c08 // http://www.atmel.com/Images/doc0180.pdf
	: public at24c
{
public:
	at24c08(uint8_t i2c);
};


class at24c16 // http://www.atmel.com/Images/doc0180.pdf
	: public at24c
{
public:
	at24c16(uint8_t i2c);
};


class at24c32 // http://www.atmel.com/images/doc0336.pdf
	: public at24c
{
public:
	at24c32(uint8_t i2c);
};


class at24c64 // http://www.atmel.com/images/doc0336.pdf
	: public at24c
{
public:
	at24c64(uint8_t i2c);
};


class at24c128 // http://www.atmel.com/Images/doc0670.pdf
	: public at24c
{
public:
	at24c128(uint8_t i2c);
};


class at24c256 // http://www.atmel.com/Images/doc0670.pdf
	: public at24c
{
public:
	at24c256(uint8_t i2c);
};


class at24c512 // http://www.atmel.com/images/doc1116.pdf
	: public at24c
{
public:
	at24c512(uint8_t i2c);
};


class at24c1024 // http://www.atmel.com/Images/doc1471.pdf
	: public at24c
{
public:
	at24c1024(uint8_t i2c);
};

#endif
