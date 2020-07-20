#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/sdp.h>
#include <bluetooth/sdp_lib.h>
#include <bluetooth/rfcomm.h>
#include <sys/wait.h>
#include <pthread.h>
#include <signal.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <mcp23008.h>


/*
#define IO1 7
#define IO2 0
#define IO3 2
#define IO4 4
#define IO5 3
#define IO6 5

#define IO7 14
#define IO8 11
#define IO9 10
#define IO10 13
#define IO11 6
#define IO12 12
#define IO13 26
#define IO14 22
*/

#define IO1 4
#define IO2 17
#define IO3 27
#define IO4 23
#define IO5 22
#define IO6 24

#define IO7 11
#define IO8 7	
#define IO9 8	
#define IO10 9
#define IO11 25
#define IO12 10
#define IO13 12
#define IO14 6


#define PIN 13

#define RELAY 65
#define RELAYS_ADDRESS	0x20

int count = 65;
int pin, pin12, pin13, object, colorNum, obj_none, cap_none, chooseColor, buttonNum, menuNum = 0;

void* ThreadMain(void* argument);
bdaddr_t bdaddr_any = { 0, 0, 0, 0, 0, 0 };
bdaddr_t bdaddr_local = { 0, 0, 0, 0xff, 0xff, 0xff };

//----------------------------
void start_motor() {
	pinMode(count + 1, OUTPUT);
	digitalWrite(count + 1, 1);
}

void stop_motor() {
	pinMode(count + 1, OUTPUT);
	digitalWrite(count + 1, 0);
}

void sorting() {
	printf("sorting\n");

	start_motor();

	pinMode(IO12, INPUT);
	pinMode(IO13, INPUT);
	pinMode(count + 2, OUTPUT);
	pinMode(count + 3, OUTPUT);
	pullUpDnControl(IO12, PUD_UP);
	pullUpDnControl(IO13, PUD_UP);

	while (1) {
		pin12 = digitalRead(IO12);
		pin13 = digitalRead(IO13);

		if (colorNum == 1) {
			delay(5000);
			break;
		}

		if (colorNum == 2) {
			digitalWrite(count + 2, 1);
			delay(4000);
			digitalWrite(count + 2, 0);
			break;
		}
		if (colorNum == 3) {
			digitalWrite(count + 3, 1);
			delay(4000);
			digitalWrite(count + 3, 0);
			break;
		}

	}

	delay(1000);
	stop_motor();
}

void emergency() {
	pinMode(IO14, INPUT);
	pinMode(count + 1, OUTPUT);
	pinMode(count + 2, OUTPUT);
	pinMode(count + 3, OUTPUT);
	pinMode(count + 4, OUTPUT);
	pinMode(count + 5, OUTPUT);
	pinMode(count + 6, OUTPUT);
	pinMode(count + 7, OUTPUT);
	pullUpDnControl(IO14, PUD_UP);

	int pin14 = digitalRead(IO14);
	if (pin14 == 0) {
		digitalWrite(count + 1, 0);
		digitalWrite(count + 2, 0);
		digitalWrite(count + 3, 0);
		digitalWrite(count + 4, 0);
		digitalWrite(count + 5, 0);
		digitalWrite(count + 6, 0);
		digitalWrite(count + 7, 0);
	}
}

void object_out() {
	printf("object_out\n");
	pinMode(IO1, INPUT);
	pinMode(IO7, INPUT);
	pinMode(count + 1, OUTPUT);
	pinMode(count + 7, OUTPUT);
	pullUpDnControl(IO1, PUD_UP);
	pullUpDnControl(IO7, PUD_UP);

	pin = digitalRead(IO1);
	object = digitalRead(IO7);

	obj_none = 0;

	//printf("object : %d\n",object);
	delay(1000);

	if (pin == 0) {
		digitalWrite(count + 1, 0);
		printf("Object None\n");
		obj_none = 1;
	}

	if (pin == 0) {
		if (object == 0) {
			digitalWrite(count + 7, 1);
			delay(1000);
			//digitalWrite(count+7,0);
		}

	}
}

void one_color() {
	printf("one_color\n");
	pinMode(count + 6, OUTPUT);
	pinMode(count + 7, OUTPUT);
	int io9 = digitalRead(IO9);
	int io10 = digitalRead(IO10);

	//printf("io9 : %d\n",io9);
	//printf("io10 : %d\n",io10);

	colorNum = 0;

	if (io9 == 1) {
		if (io10 == 1) {
			printf("color is Black\n");
			colorNum = 1;
			if (chooseColor == colorNum) {
				printf("This Object is Your Choice Color! \n");
				digitalWrite(count + 6, 0);
				digitalWrite(count + 7, 0);
				start_motor();
				delay(2000);
				stop_motor();
			}
			else {
				printf("This Object is Not Your Choice Color! \n");
				stop_motor();
				digitalWrite(count + 6, 1);
				delay(1000);
				digitalWrite(count + 6, 0);
				digitalWrite(count + 7, 0);
			}
		}
		else if (io10 == 0) {
			printf("color is Red\n");
			colorNum = 2;
			if (chooseColor == colorNum) {
				printf("This Object is Your Choice Color! \n");
				digitalWrite(count + 6, 0);
				digitalWrite(count + 7, 0);
				start_motor();
				delay(2000);
				stop_motor();
			}
			else {
				printf("This Object is Not Your Choice Color! \n");
				stop_motor();
				digitalWrite(count + 6, 1);
				delay(1000);
				digitalWrite(count + 6, 0);
				digitalWrite(count + 7, 0);
			}
		}
	}
	else if (io9 == 0) {
		if (io10 == 1) {
			printf("color is Red\n");
			colorNum = 2;
			if (chooseColor == colorNum) {
				printf("This Object is Your Choice Color! \n");
				digitalWrite(count + 6, 0);
				digitalWrite(count + 7, 0);
				start_motor();
				delay(2000);
				stop_motor();
			}
			else {
				printf("This Object is Not Your Choice Color! \n");
				stop_motor();
				digitalWrite(count + 6, 1);
				delay(1000);
				digitalWrite(count + 6, 0);
				digitalWrite(count + 7, 0);
			}
		}
		else if (io10 == 0) {
			printf("color is Silver\n");
			colorNum = 3;
			if (chooseColor == colorNum) {
				printf("This Object is Your Choice Color! \n");
				digitalWrite(count + 6, 0);
				digitalWrite(count + 7, 0);
				start_motor();
				delay(2000);
				stop_motor();
			}
			else {
				printf("This Object is Not Your Choice Color! \n");
				stop_motor();
				digitalWrite(count + 6, 1);
				delay(1000);
				digitalWrite(count + 6, 0);
				digitalWrite(count + 7, 0);
			}
		}
	}
}

int discretion_color() {
	printf("discretion_color\n");
	pinMode(IO9, INPUT);
	pullUpDnControl(IO9, PUD_UP);
	pinMode(IO10, INPUT);
	pullUpDnControl(IO10, PUD_UP);

	pinMode(count + 7, OUTPUT);
	pinMode(count + 6, OUTPUT);

	int io9 = digitalRead(IO9);
	int io10 = digitalRead(IO10);

	//printf("io9 : %d\n",io9);
	//printf("io10 : %d\n",io10);

	colorNum = 0;

	if (io9 == 1) {
		if (io10 == 1) {
			printf("color is Black\n");
			colorNum = 1;
		}
		else if (io10 == 0) {
			printf("color is Red\n");
			colorNum = 2;
		}
	}
	else if (io9 == 0) {
		if (io10 == 1) {
			printf("color is Red\n");
			colorNum = 2;
		}
		else if (io10 == 0) {
			printf("color is Silver\n");
			colorNum = 3;
		}
	}

	if (chooseColor == colorNum) {
		printf("This Object is Your Choice Color! \n");
		digitalWrite(count + 6, 0);
		digitalWrite(count + 7, 0);
		start_motor();
		delay(2000);
		stop_motor();
	}
	else {
		printf("This Object is Not Your Choice Color! \n");
		stop_motor();
		digitalWrite(count + 6, 1);
		delay(1000);
		digitalWrite(count + 6, 0);
		digitalWrite(count + 7, 0);
	}


	return colorNum;
}	// colorNum 1 = Black  2 = Red  3 = Silver

void cap_out() {
	printf("cap_out\n");
	pinMode(IO8, INPUT);
	pinMode(IO11, INPUT);
	pinMode(count + 5, OUTPUT);
	pinMode(count + 4, OUTPUT);
	pullUpDnControl(IO8, PUD_UP);
	pullUpDnControl(IO11, PUD_UP);

	int io8 = digitalRead(IO8);
	int io11 = digitalRead(IO11);

	cap_none = 0;

	if (io8 == 1) {
		printf("Cap None\n");
		cap_none = 1;
	}
	else if (io8 == 0) {
		if (io11 == 1) {
			printf("Object isn't Position\n");
		}
		else if (io11 == 0) {
			stop_motor();
			printf("Object is Position\n");
			digitalWrite(count + 5, 1);
			delay(1000);
			digitalWrite(count + 5, 0);
			//delay(1000);
			digitalWrite(count + 4, 1);
			start_motor();
			delay(1500);
			digitalWrite(count + 4, 0);
			stop_motor();
		}
	}

}

void button_flow() {

	buttonNum = 0;
	for (int i = 0; i < 5; i++) {
		emergency();
		printf("button_flow\n");
		switch (buttonNum) {
		case 1:
			object_out();
			break;
		case 2:
			discretion_color();
			break;
		case 3:
			cap_out();
			break;
		case 4:
			sorting();
			break;
		default:
			break;
		}
		delay(1000);
	}

}

void auto_flow() {

	for (int i = 0; i < 5; i++) {
		object_out();
		//if(obj_none == 1) break;
		one_color();
		//discretion_color();
		cap_out();
		//if(cap_none == 1) break;
		sorting();
		emergency();
	}
}
//----------------------------

int _str2uuid(const char* uuid_str, uuid_t* uuid) {
	/* This is from the pybluez stack */

	uint32_t uuid_int[4];
	char* endptr;

	if (strlen(uuid_str) == 36) {
		char buf[9] = { 0 };

		if (uuid_str[8] != '-' && uuid_str[13] != '-' &&
			uuid_str[18] != '-' && uuid_str[23] != '-') {
			return 0;
		}
		// first 8-bytes
		strncpy(buf, uuid_str, 8);
		uuid_int[0] = htonl(strtoul(buf, &endptr, 16));
		if (endptr != buf + 8) return 0;
		// second 8-bytes
		strncpy(buf, uuid_str + 9, 4);
		strncpy(buf + 4, uuid_str + 14, 4);
		uuid_int[1] = htonl(strtoul(buf, &endptr, 16));
		if (endptr != buf + 8) return 0;

		// third 8-bytes
		strncpy(buf, uuid_str + 19, 4);
		strncpy(buf + 4, uuid_str + 24, 4);
		uuid_int[2] = htonl(strtoul(buf, &endptr, 16));
		if (endptr != buf + 8) return 0;

		// fourth 8-bytes
		strncpy(buf, uuid_str + 28, 8);
		uuid_int[3] = htonl(strtoul(buf, &endptr, 16));
		if (endptr != buf + 8) return 0;

		if (uuid != NULL) sdp_uuid128_create(uuid, uuid_int);
	}
	else if (strlen(uuid_str) == 8) {
		// 32-bit reserved UUID
		uint32_t i = strtoul(uuid_str, &endptr, 16);
		if (endptr != uuid_str + 8) return 0;
		if (uuid != NULL) sdp_uuid32_create(uuid, i);
	}
	else if (strlen(uuid_str) == 4) {
		// 16-bit reserved UUID
		int i = strtol(uuid_str, &endptr, 16);
		if (endptr != uuid_str + 4) return 0;
		if (uuid != NULL) sdp_uuid16_create(uuid, i);
	}
	else {
		return 0;
	}

	return 1;

}



sdp_session_t* register_service(uint8_t rfcomm_channel) {

	/* A 128-bit number used to identify this service. The words are ordered from most to least
	* significant, but within each word, the octets are ordered from least to most significant.
	* For example, the UUID represneted by this array is 00001101-0000-1000-8000-00805F9B34FB. (The
	* hyphenation is a convention specified by the Service Discovery Protocol of the Bluetooth Core
	* Specification, but is not particularly important for this program.)
	*
	* This UUID is the Bluetooth Base UUID and is commonly used for simple Bluetooth applications.
	* Regardless of the UUID used, it must match the one that the Armatus Android app is searching
	* for.
	*/
	const char* service_name = "Armatus Bluetooth server";
	const char* svc_dsc = "A HERMIT server that interfaces with the Armatus Android app";
	const char* service_prov = "Armatus";

	uuid_t root_uuid, l2cap_uuid, rfcomm_uuid, svc_uuid,
		svc_class_uuid;
	sdp_list_t* l2cap_list = 0,
		* rfcomm_list = 0,
		* root_list = 0,
		* proto_list = 0,
		* access_proto_list = 0,
		* svc_class_list = 0,
		* profile_list = 0;
	sdp_data_t* channel = 0;
	sdp_profile_desc_t profile;
	sdp_record_t record = { 0 };
	sdp_session_t* session = 0;

	// set the general service ID
	//sdp_uuid128_create(&svc_uuid, &svc_uuid_int);
	_str2uuid("00001101-0000-1000-8000-00805F9B34FB", &svc_uuid);
	sdp_set_service_id(&record, svc_uuid);

	char str[256] = "";
	sdp_uuid2strn(&svc_uuid, str, 256);
	printf("Registering UUID %s\n", str);

	// set the service class
	sdp_uuid16_create(&svc_class_uuid, SERIAL_PORT_SVCLASS_ID);
	svc_class_list = sdp_list_append(0, &svc_class_uuid);
	sdp_set_service_classes(&record, svc_class_list);

	// set the Bluetooth profile information
	sdp_uuid16_create(&profile.uuid, SERIAL_PORT_PROFILE_ID);
	profile.version = 0x0100;
	profile_list = sdp_list_append(0, &profile);
	sdp_set_profile_descs(&record, profile_list);

	// make the service record publicly browsable
	sdp_uuid16_create(&root_uuid, PUBLIC_BROWSE_GROUP);
	root_list = sdp_list_append(0, &root_uuid);
	sdp_set_browse_groups(&record, root_list);

	// set l2cap information
	sdp_uuid16_create(&l2cap_uuid, L2CAP_UUID);
	l2cap_list = sdp_list_append(0, &l2cap_uuid);
	proto_list = sdp_list_append(0, l2cap_list);

	// register the RFCOMM channel for RFCOMM sockets
	sdp_uuid16_create(&rfcomm_uuid, RFCOMM_UUID);
	channel = sdp_data_alloc(SDP_UINT8, &rfcomm_channel);
	rfcomm_list = sdp_list_append(0, &rfcomm_uuid);
	sdp_list_append(rfcomm_list, channel);
	sdp_list_append(proto_list, rfcomm_list);

	access_proto_list = sdp_list_append(0, proto_list);
	sdp_set_access_protos(&record, access_proto_list);

	// set the name, provider, and description
	sdp_set_info_attr(&record, service_name, service_prov, svc_dsc);

	// connect to the local SDP server, register the service record,
	// and disconnect
	session = sdp_connect(&bdaddr_any, &bdaddr_local, SDP_RETRY_IF_BUSY);
	sdp_record_register(session, &record, 0);

	// cleanup
	sdp_data_free(channel);
	sdp_list_free(l2cap_list, 0);
	sdp_list_free(rfcomm_list, 0);
	sdp_list_free(root_list, 0);
	sdp_list_free(access_proto_list, 0);
	sdp_list_free(svc_class_list, 0);
	sdp_list_free(profile_list, 0);

	return session;
}



char input[1024] = { 0 };
char* read_server(int client) {
	// read data from the client
	int bytes_read;
	bytes_read = read(client, input, sizeof(input));
	if (bytes_read > 0) {
		printf("received [%s]\n", input);
		return input;
	}
	else {
		return NULL;
	}
}

void write_server(int client, char* message) {
	// send data to the client
	char messageArr[1024] = { 0 };
	int bytes_sent;
	strcpy(messageArr, message);

	bytes_sent = write(client, messageArr, strlen(messageArr));
	if (bytes_sent > 0) {
		printf("sent [%s] %d\n", messageArr, bytes_sent);
	}
}

int main()
{
	wiringPiSetup();
	mcp23008Setup(RELAY, RELAYS_ADDRESS);

	pthread_t thread_id;

	signal(SIGPIPE, SIG_IGN);


	int port = 3, result, sock, client, bytes_read, bytes_sent;
	struct sockaddr_rc loc_addr = { 0 }, rem_addr = { 0 };
	char buffer[1024] = { 0 };
	socklen_t opt = sizeof(rem_addr);

	// local bluetooth adapter
	loc_addr.rc_family = AF_BLUETOOTH;
	loc_addr.rc_bdaddr = bdaddr_any;
	loc_addr.rc_channel = (uint8_t)port;

	// register service
	sdp_session_t* session = register_service(port);
	// allocate socket
	sock = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
	printf("socket() returned %d\n", sock);

	// bind socket to port 3 of the first available
	result = bind(sock, (struct sockaddr*) & loc_addr, sizeof(loc_addr));
	printf("bind() on channel %d returned %d\n", port, result);

	// put socket into listening mode
	result = listen(sock, 1);
	printf("listen() returned %d\n", result);

	//sdpRegisterL2cap(port);


	while (1)
	{
		// accept one connection
		printf("calling accept()\n");
		client = accept(sock, (struct sockaddr*) & rem_addr, &opt);
		printf("accept() returned %d\n", client);

		ba2str(&rem_addr.rc_bdaddr, buffer);
		fprintf(stderr, "accepted connection from %s\n", buffer);
		memset(buffer, 0, sizeof(buffer));

		pthread_create(&thread_id, NULL, ThreadMain, (void*)client);
	}

}


void* ThreadMain(void* argument)
{
	char buf[1024];

	pthread_detach(pthread_self());
	int client = (int)argument;


	while (1)
	{
		char* recv_message = read_server(client);
		if (recv_message == NULL) {
			printf("client disconnected\n");
			break;
		}

		printf("%s\n", recv_message);

		switch (*recv_message) {
		case '1':
			menuNum = 1;
			break;
		case '2':
			menuNum = 2;
			break;
		case '3':
			object_out();
			break;
		case '4':
			discretion_color();
			break;
		case '5':
			cap_out();
			break;
		case '6':
			sorting();
			break;
		case '7':
			if (menuNum == 1)	chooseColor = 1;
			else if (menuNum == 2)	auto_flow();
			break;
		case '8':
			if (menuNum == 1)	chooseColor = 2;
			else if (menuNum == 2)	auto_flow();
			break;
		case '9':
			if (menuNum == 1)	chooseColor = 3;
			else if (menuNum == 2)	auto_flow();
			break;
		}

		write_server(client, recv_message);
	}

	printf("disconnected\n");
	close(client);

	return 0;
}
