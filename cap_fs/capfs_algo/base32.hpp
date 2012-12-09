#ifndef MINORFS_CAPFS_BASE32_HPP
#define MINORFS_CAPFS_BASE32_HPP
#include <string>

template <int Len>
std::string b32encode(unsigned char *binary);

template <>
std::string b32encode<5>(unsigned char *binary) {
   char target[9];
   target[0]=(binary[0] >> 3) & 0x01f;
   target[1]=((binary[0] << 2) & 0x01c) | ((binary[1] >> 6) & 0x007);
   target[2]=(binary[1] >> 1) & 0x01f;
   target[3]=((binary[1] << 4) & 0x010) | ((binary[2] >>4) & 0x00f);
   target[4]=((binary[2] << 1 ) & 0x01e) | ((binary[3] >> 7) & 0x001);
   target[5]=(binary[3] >> 2) & 0x01f;
   target[6]=((binary[3] << 3) & 0x018) | ((binary[4] >> 5) & 0x007);
   target[7]=binary[4] & 0x01f;
   for (size_t index=0;index<8;index++) {
      target[index] += (target[index]<26) ? 'A' : '2' - 26;
   }
   target[8]=0;
   return target;
}

template <>
std::string b32encode<30>(unsigned char *binary) {
   return b32encode<5>(binary) +
          b32encode<5>(binary+5) +
          b32encode<5>(binary+10) +
          b32encode<5>(binary+15) +
          b32encode<5>(binary+20) +
          b32encode<5>(binary+25);
}

template <>
std::string b32encode<32>(unsigned char *binary) {
    unsigned char padded[5];
    padded[0]=binary[30];
    padded[1]=binary[31];
    padded[2]=padded[3]=padded[4]=0;
    return b32encode<30>(binary) + b32encode<5>(padded).substr(0,4);
}

template <int Len>
void b32decode(std::string input,unsigned char *binary);

template <>
void b32decode<8>(std::string input,unsigned char *binary) {
   unsigned char numbers[8];
   for (size_t index=0;index<8;index++) {
      char c=input.c_str()[index];
      numbers[index]=0;
      if ((c > '@') and ( c < '[')) {
         numbers[index] = c - 'A';
      } else if ((c > '/') and ( c < ':')) {
         numbers[index] = c + 26 - '2';
      }
   }
   binary[0] = ((numbers[0] << 3) & 0x0f8) | ((numbers[1] >> 2) & 0x007);
   binary[1] = ((numbers[1] << 6) & 0x0d0)  | ((numbers[2] << 1) & 0x03e) | ((numbers[3] >> 4) & 0x001);
   binary[2] = ((numbers[3] << 4) & 0x0f0) | ((numbers[4] >> 1 ) & 0x00f);
   binary[3] = ((numbers[4] << 7) & 0x080)  | ((numbers[5] << 2) & 0x07c) | ((numbers[6] >> 3 ) & 0x003);
   binary[4] = ((numbers[6] << 5) & 0x0e0) | ((numbers[7]) & 0x01f);
}

template <>
void b32decode<48>(std::string input,unsigned char *binary) {
  b32decode<8>(input.substr(0,8),binary);
  b32decode<8>(input.substr(8,8),binary+5);
  b32decode<8>(input.substr(16,8),binary+10);
  b32decode<8>(input.substr(24,8),binary+15);
  b32decode<8>(input.substr(32,8),binary+20);
  b32decode<8>(input.substr(40,8),binary+25);
}

template <>
void b32decode<52>(std::string input,unsigned char *binary) {
  b32decode<48>(input.substr(0,48),binary);
  unsigned char work[5];
  b32decode<8>(input.substr(48,4)+"AAAA",work);
  binary[30]=work[0];
  binary[31]=work[1];
}
#endif
