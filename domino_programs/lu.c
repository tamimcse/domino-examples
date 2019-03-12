#define LEVEL16_SIZE 65536
#define LEVEL24_SIZE 65536 //actually LEVEL24_SIZE * 4
#define CHUNK_SIZE 256
#define DEF_NEXT_HOP 1

int N16[LEVEL16_SIZE] = {0}; 
int C16[LEVEL16_SIZE] = {0};
int N24[LEVEL24_SIZE] = {0};
int CK24_bitmap[LEVEL24_SIZE] = {0};
//int CK24_offset[LEVEL24_SIZE] = {0};

struct Packet {
  int dport;
  int daddr;
  int idx16;
  int idx24;
  int ck24_idx;
  int ck24_off;
  int part_idx;
  int part_off;
  int bitmap;
};

int tmp = 0;

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
  //CK24_bitmap[pkt.part_idx] &
  if ( (1 << pkt.part_off)) {
    pkt.dport = N24[pkt.idx24];
  }
}
