#define MAX_ADDR 4
#define MAX_DOMAIN 20
typedef char string[30];

typedef struct table_row
{
    string domain;
    string address[MAX_ADDR];
}entry;


int is_IpTaken(entry table[MAX_DOMAIN], char * address){

	for(int i=0;i<MAX_DOMAIN;++i){

		for (int j = 0; j < MAX_ADDR; ++j)
		{
			if(table[i].address[j][0] && strcmp(table[i].address[j],address)==0){
				printf("IP address already taken\n");
				return 1;
			}
		}
	}
	return 0;
}

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
int createEntry(entry table[MAX_DOMAIN], char *domain, char *address)
{

    int index = -1;
    int flag = 0;

    if(is_IpTaken(table,address) == 1 || is_IpInvalid(address) == 1)
    {
    	return 2;
    }

    for (int i = 0; i < MAX_DOMAIN; i++)
    {
        if (strcmp(table[i].domain, domain) == 0)
        {
            for (int j = 0; j < MAX_ADDR; j++)
                if (!table[i].address[j][0])
                {
                    strcpy(table[i].address[j], address);
                    flag = 1;
                    break;
                }
            break;
        }
        if (!table[i].domain[0] && index == -1)
            index = i;
    }

    if (!flag)
    {
        strcpy(table[index].domain, domain);
        strcpy(table[index].address[0], address);
        flag = 1;
    }

    return flag;
}

entry *getAddress(entry *table, char *const domain)
{
    static entry result;
    bzero(&result, sizeof(entry));
    strcpy(result.domain, domain);

    for (int i = 0; i < MAX_DOMAIN; i++)
    {

        if (strcmp(table[i].domain, domain) == 0)
        {
            for (int j = 0; j < MAX_ADDR; j++)
            {
                strcpy(result.address[j], table[i].address[j]);
            }
            break;
        }
    }
    return (&result);
}
