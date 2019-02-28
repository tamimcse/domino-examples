// rcp.c
//Capacity of the line card in megabytes
#define C 64000 
#define T 50 //Control Interval in ms
#define A 50000 //T*1000

//State variables
int avg_rtt = 200; // in ms
int feedback_rate = 200; //in MB
int bytes_received = 0; //in Bytes
int incoming_rate = 0; //in MB

//Packet headers and meta-data
struct Packet {
  int size_bytes;
  int rtt;
  int queue;
  int feedback_thput;
  int tick;
};

void func(struct Packet pkt) {
  //Calculate running average of RTT
  avg_rtt = (avg_rtt * 49 + pkt.rtt)/50;

  //Write the throughput in packet header
  if (pkt.feedback_thput > feedback_rate)
    pkt.feedback_thput = feedback_rate;  
  
  //Control interval has expired, so
  // calculate the feeback throughput
  // and reset the state variables
  if (pkt.tick % T == 0) {
    incoming_rate = C - bytes_received/A;
    bytes_received = 0;
    feedback_rate += (C + (((C - \
      ((pkt.queue/avg_rtt)/2) - \
      incoming_rate) * T)/avg_rtt))/C;
  }
  else {
    bytes_received += pkt.size_bytes; 
  }
}
