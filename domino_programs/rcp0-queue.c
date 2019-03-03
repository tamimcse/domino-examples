//Capacity of the line card in megabytes
#define C 64000 
#define T 50 //Control Interval in ms
#define A 50000 //1000*T

/*********State Variables****************/
// Running average of RTT in ms
int RTT = 200; 
int R = 200; //RCP feedback rate in MB/s
int B = 0; //Number of Bytes received
int S = 0; //Spare capacity in MB/s
 
/****Packet headers and meta-data********/
struct Packet {
  int size_bytes;
  int rtt; //Parsed from RCP header
  int queue;
  int Rp; //RCP feedback rate
  int tick;
};

/*********Packet Transaction****************/
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
    S = C - B/A;
    B = 0;
    R *= 1+((S-((pkt.queue/RTT)/2))*T/RTT)/C;
  }
  else {
    B += pkt.size_bytes; 
  }
}
