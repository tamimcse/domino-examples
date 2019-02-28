// rcp.c
//Capacity of the line card in megabytes
#define C 64000 
#define T 50 //Control Interval in ms
#define A 50000 //T*1000

// Running average of RTT in ms
int RTT = 200; 
//RCP feedback rate in MB/s
int R = 200; 
//Number of Bytes received during the 
//control interval
int B = 0;
//Incoming rate in MB/s 
int Y = 0; 

//Packet headers and meta-data
struct Packet {
  int size_bytes;
  int rtt;
  int queue;
  int Rp;
  int tick;
};

void func(struct Packet pkt) {
  //Calculate running average of RTT
  RTT = (RTT * 49 + pkt.rtt)/50;

  //Write the throughput in packet header
  if (pkt.Rp > R)
    pkt.Rp = R;  
  
  //Control interval has expired, so
  // calculate the feeback throughput
  // and reset the state variables
  if (pkt.tick % T == 0) {
    Y = C - B/A;
    B = 0;
    R *= 1+(((C-Y-((pkt.queue/RTT)/2))*T)/RTT)/C;
  }
  else {
    B += pkt.size_bytes; 
  }
}
