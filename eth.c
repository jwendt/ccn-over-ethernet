#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <linux/if_arp.h>

//#define ETH_FRAME_LEN 1518

int main(){

	int s; /*socketdescriptor*/
	int j;

	s = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if (s == -1){
		printf("error encountered (you probalby need to run as root)\n");
	}

/*******************************************************/

	/*target address*/
	struct sockaddr_ll socket_address;

	/*buffer for ethernet frame*/
	void* buffer = (void*)malloc(ETH_FRAME_LEN);
 
	/*pointer to ethenet header*/
	unsigned char* etherhead = buffer;

	/*userdata in ethernet frame*/
	unsigned char* data = buffer + 14;

	/*another pointer to ethernet header*/
	struct ethhdr *eh = (struct ethhdr *)etherhead;
 
	int send_result = 0;

	/*our MAC address*/
	unsigned char src_mac[6] = {0x00, 0x01, 0x02, 0xFA, 0x70, 0xAA};

	/*other host MAC address*/
	//unsigned char dest_mac[6] = {0x00, 0x04, 0x75, 0xC8, 0x28, 0xE5};
	unsigned char dest_mac[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

	/*RAW communication*/
	socket_address.sll_ifindex  = 2;

	/*set the frame header*/
	memcpy((void*)buffer, (void*)dest_mac, ETH_ALEN);
	memcpy((void*)(buffer+ETH_ALEN), (void*)src_mac, ETH_ALEN);
	eh->h_proto = 0xFFFF;// <-- wireshark doesn't know this protocol, so let's call it the CCN protocol
	//eh->h_proto = 0x00;
	/*fill the frame with some data*/
	for (j = 0; j < 1500; j++) {
		data[j] = (unsigned char)((int) (255.0*rand()/(RAND_MAX+1.0)));
	}

	/*send the packet*/
	send_result = sendto(s, buffer, ETH_FRAME_LEN, 0, (struct sockaddr*)&socket_address, sizeof(socket_address));
	if (send_result == -1) {
		printf("error!\n");
	}



/****************************************************/

	return 0;

}
