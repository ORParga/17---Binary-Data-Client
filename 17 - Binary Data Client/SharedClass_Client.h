#pragma once
class SharedClass {
public:
	unsigned char Mark[8] = {0x8e,0xb3,0x98,0xd2,0x58,0x8b,0x47,0x2d};
	//unsigned long Mark = 0x8eb398d2588b472d;
	unsigned long checksum = 0;
	unsigned char data1 = 0;
	unsigned char data2 = 0;
	unsigned long data3 = 0;
	unsigned long data4 = 0;
};