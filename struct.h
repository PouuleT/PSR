#ifndef STRUCTURE
#define STRUCTURE

typedef struct node{
  int nb_emited;
	int nb_traited;
	int nb_recieved;
	int nb_destroyed;
	int nb_waiting;
	int avg_waiting;
}node;

typedef struct packet{
  node source;
  node destination;
  double departure;
  double last_time;
  int size;
}packet;

typedef struct log{
  int nb_packets;
  int nb_flux;
	int nb_emited;
	int nb_traited;
	int nb_recieved;
	int nb_destroyed;
}log;

#endif
