//Capacity of the line card
#define C 1000
//Control Interval
#define T 50
//1000/T
#define A 20 

int avg_rtt = 200;
int feedback_rate = 200;
int bytes_received = 0;
int incoming_rate = 0;
int queue = 0;
int tmp_queue = 0;


struct Packet {
  int size_bytes;
  int rtt;
  int queue;
  int feedback_thput;
  int time;
  int tick;
  int dport;
  int id; // array index
};

void func(struct Packet pkt) {
  avg_rtt = (avg_rtt * 49 + pkt.rtt)/50;

  if (pkt.feedback_thput > feedback_rate)
    pkt.feedback_thput = feedback_rate;  
  
  if (pkt.tick % T == 0) {
    incoming_rate = C - bytes_received * A;
    bytes_received = 0;
    queue = tmp_queue;
    tmp_queue = 1000;
    feedback_rate += (C + (((C - ((queue/avg_rtt)/2) - incoming_rate) * T)/avg_rtt))/C;
  }
  else {
    bytes_received += pkt.size_bytes; 
  }

  if (tmp_queue > pkt.queue)
    tmp_queue = pkt.queue;
}