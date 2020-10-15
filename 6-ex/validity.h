typedef char string[30];

typedef struct arp_packet{

	int arpOperation;
	string sourceMAC;
	string sourceIP;
	string destIP;
	string destMAC;
	char data[100];

}packet;

int is_IpInvalid(char * address){
	string temp;
	strcpy(temp,address);

    char *split;
    int val;
    split = strtok(temp, ".");
    while (split)
    {
        val = atoi(split);
        if (val < 0 || val > 255){
            printf("Invalid IP address\n");
        return 1;
        }
        split = strtok(NULL, ".");
    }
    return 0;
}

int is_MACInvalid(char * address){
	string temp;
	strcpy(temp, address);

	int n = strlen(address);
	if(n != 17)
		return 1;

	for(int i =0; i<17; ++i){
		if((temp[i] >= '0' && temp[i] <= '9' )||(temp[i] >= 'A' && temp[i] <= 'F'));
		else if (temp[i] == '-' && (i+1)%3==0);
		else return 1;
	}

	return 0;
}

packet createARP(){

	packet new_pack;

	printf("Enter the details of received packet \n\n");
	printf("Destination IP \t: ");
	scanf("%s",new_pack.destIP);
	if (is_IpInvalid(new_pack.destIP))
		return createARP();

	printf("Source IP \t: ");
	scanf("%s",new_pack.sourceIP);
	if (is_IpInvalid(new_pack.sourceIP))
		return createARP();

	printf("Source MAC \t: ");
	scanf("%s",new_pack.sourceMAC);
	if(is_MACInvalid(new_pack.sourceMAC))
		return createARP();
	
	printf("Data \t\t: ");
	scanf("%s",new_pack.data);

	new_pack.arpOperation = 1;
	strcpy(new_pack.destMAC, "00-00-00-00-00-00");

	return new_pack;

}

void display(packet pack){
	printf("\n\t%d | %s | %s | %s | %s\n",pack.arpOperation, pack.sourceMAC, pack.sourceIP, pack.destIP, pack.destMAC);

}
