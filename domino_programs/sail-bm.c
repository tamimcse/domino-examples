#include "popcnt.h"

#define LEVEL16_SIZE 65536
#define LEVEL24_SIZE 65536 //actually LEVEL24_SIZE * 4
#define LEVEL32_SIZE 65536
#define CHUNK_SIZE 256
#define DEF_NEXT_HOP 1

int N16[LEVEL16_SIZE] = {0}; 
int C16[LEVEL16_SIZE] = {0};
int N24[LEVEL24_SIZE] = {0};
int CK24_bitmap[LEVEL24_SIZE] = {0};
int CK24_offset[LEVEL24_SIZE] = {0};
int C24[LEVEL24_SIZE] = {0};
int N32[LEVEL32_SIZE] = {0};

struct Packet {
  int dport;
  int daddr;
  int idx16;
  int idx24;
  int idx32;
  int ck24_idx;
  int ck24_off;
  int part_idx;
  int part_off;
  int bitmap;
  int tmp;
  int tmp1;
};

void func(struct Packet pkt) {
  pkt.dport = DEF_NEXT_HOP;

  pkt.idx16 = pkt.daddr >> 16;
  if (N16[pkt.idx16] != 0) {
    pkt.dport = N16[pkt.idx16]; 
  } 

  if (C16[pkt.idx16] != 0) {
    pkt.ck24_idx = C16[pkt.idx16] - 1;
    pkt.ck24_off = (pkt.daddr & 65280) >> 8;
    pkt.idx24 = pkt.ck24_idx * CHUNK_SIZE + pkt.ck24_off;
  }

  if (N24[pkt.idx24] != 0) {
    pkt.dport = N24[pkt.idx24]; 
  }
  
  pkt.part_idx = pkt.ck24_off / 64;
  pkt.part_idx += pkt.idx24 * 4;
  pkt.part_off = pkt.ck24_off % 64;

  //Hack!! Currently stateful atoms cannot be used for bitwise operator. So made it a packet meta-data. Need to change codelet(int state_1, int state_2, int pkt_1, int pkt_2, int pkt_...to accept bit[32] instead
  pkt.tmp = CK24_bitmap[pkt.part_idx];
  CK24_bitmap[pkt.part_idx] *= 1;

  if ( pkt.tmp & (1 << pkt.part_off)) {
    pkt.tmp1  = popcnt((((1 << pkt.part_off) - 1) & pkt.tmp));
    pkt.idx24 = CK24_offset[pkt.part_idx] + pkt.tmp1;
    pkt.idx32 = (C24[pkt.idx24] - 1) * CHUNK_SIZE + (pkt.daddr & 255);
  }

  if (N32[pkt.idx32] != 0) {
    pkt.dport = N32[pkt.idx32]; 
  }
}
