#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#define BUFLEN 1024
typedef struct {
    char user_name[BUFLEN];
    char password[BUFLEN];
    int status;
} elementtype;
typedef struct node node;
typedef struct node {
	elementtype element;
	node *next;
} node;

typedef struct {
  node *root;
  node *cur;
  node *prev;
  node *tail;
} singleList;
singleList list;
//Declare function of linked list
void createSingleList(singleList *list);
node *makeNewNode(elementtype e);
node *insertEnd(singleList *list, elementtype e);
node *deleteHead(singleList *list);
node *deleteAtPosition(singleList *list, int position);
void printSingleList(singleList list);
//Declare function
int strcicmp(char const *a, char const *b);
void readDataFromFile(singleList *list);
int searchAccount(singleList list, char username[BUFLEN]);
void alterDataOfFile(singleList list);
void blockAccount(singleList list,char username[BUFLEN]);
int checkBlocked(singleList list, char username[BUFLEN]);//if blocked, return 1
int checkCorrectPassword(singleList list, char username[BUFLEN], char pass[BUFLEN]);
int changePass(singleList accountList, char username[BUFLEN], char pass[BUFLEN], char new_pass[BUFLEN]);
int strcicmp(char const *a, char const *b);
int split_number_and_string(char *input, char *number,char *string);
//Implement function of linked list
void createSingleList(singleList *list)
{
    (*list).root = (*list).prev = (*list).cur = (*list).tail  = NULL;
}
node *makeNewNode(elementtype e)
{
    node *new = (node *)malloc(sizeof(node));
    new->element = e;
    new->next = NULL;
    return new;
}
node *insertEnd(singleList *list, elementtype e)
{
	node *new = makeNewNode(e);
	if((*list).root == NULL)
	{
        (*list).root = (*list).tail = new;
	}
	else
	{
		(*list).tail->next = new;
		(*list).tail = new;
	}
	return (*list).tail;
}
node *deleteHead(singleList * list)
{
    if((*list).root != NULL)
    {
        node *new = (*list).root;
		(*list).root = (*list).root->next;
		free(new);
	}
    return (*list).root;
}
node *deleteAtPosition(singleList *list, int position){
    if((*list).root == NULL)
    {
        return (*list).root;
    }
    (*list).cur = (*list).prev = (*list).root;
	//Delete head node
	if (position == 0){
        return deleteHead(list);
	}
	(*list).cur = (*list).prev = (*list).root;
	//store previous of to be deleted node
    for (int i = 0; (*list).cur != NULL && i < position - 1; i++)
    {
        (*list).prev = (*list).cur;
        (*list).cur = (*list).cur->next;
    }
    if ((*list).cur == NULL || (*list).cur->next == NULL)// If position is more than number of nodes
    {
        return (*list).cur;
    }
    if ((*list).cur->next->next == NULL){
        (*list).prev =( *list).cur;
        (*list).cur = (*list).cur->next;
        node *new = (*list).cur;
		(*list).cur = (*list).cur->next;
		free(new);
        (*list).tail = (*list).prev;
        (*list).tail->next = NULL;
        return (*list).tail;
    }
    //delete node at pos (next of pos-1)
    (*list).prev = (*list).cur;
    node *next = (*list).cur->next->next;
    free((*list).cur->next);
    (*list).prev->next = next;
    return (*list).cur;
}
void printSingleList(singleList list){
    list.cur = list.root;
	while(list.cur != NULL)
    {
        printf("%s %s %d\n", list.cur->element.user_name, list.cur->element.password, list.cur->element.status);
        list.cur = list.cur->next;
    }
    return;
}
//Implement function
void readDataFromFile(singleList *list){
    elementtype element;
    FILE *fp;
    fp = fopen("nguoidung.txt","r");
    char chr = getc(fp);
    int count_lines = 0;
    while (chr != EOF)
    {
        //Count whenever new line is encountered
        if (chr == '\n')
        {
            count_lines = count_lines + 1;
        }
        //take next character from file.
        chr = getc(fp);
    }
    fseek(fp, 0, SEEK_SET);
    for(int i = 0; i < count_lines; i++){
      fscanf(fp, "%s %s %d", element.user_name, element.password, &element.status);
      insertEnd(list, element);
    }
    fclose(fp);
}
int searchAccount(singleList list, char username[512]){
    list.cur = list.root;
    // if existed , return 1
    // else return 0
    while (list.cur != NULL)
    {
        if (strcicmp(list.cur->element.user_name, username) == 0)
            return 1;
        list.cur = list.cur->next;
    }
    return 0;
}
void alterDataOfFile(singleList list){
	FILE *fp;
    fp = fopen("nguoidung.txt","w");
    if (fp == NULL){
        printf("No file input\n");
        return;
    }
    list.cur = list.root;
	while(list.cur != NULL)
    {
        fputs(list.cur->element.user_name, fp);
        fputs(" ", fp);
        fputs(list.cur->element.password, fp);
        fputs(" ", fp);
        fprintf(fp, "%d", list.cur->element.status);
        fputs("\n", fp);
        list.cur = list.cur->next;
    }
    fclose(fp);
    return;
}
void blockAccount(singleList list,char username[512]){
	list.cur = list.root;
	while(list.cur != NULL){
		if (strcicmp(list.cur->element.user_name, username) == 0){
			list.cur->element.status = 0;
			return;
		}
		list.cur = list.cur->next;
	}
}
int checkBlocked(singleList list, char username[512]){
    list.cur = list.root;
	while(list.cur != NULL){
		if (strcicmp(list.cur->element.user_name, username) == 0){
			if(list.cur->element.status == 0){
				return 1;
			}
			else{
				return 0;
			}
		}
		list.cur = list.cur->next;
	}
}
int checkCorrectPassword(singleList list, char username[512], char pass[512]){
    list.cur = list.root;
	while(list.cur != NULL){
		if ((strcicmp(list.cur->element.user_name, username) == 0) && (strcicmp(list.cur->element.password, pass) == 0)){
			return 1;
		}
		list.cur = list.cur->next;
	}
	return 0;
}
int changePass(singleList accountList, char username[BUFLEN], char pass[BUFLEN], char new_pass[BUFLEN]){
	accountList.cur = accountList.root;
	while(accountList.cur != NULL){
		if ((strcicmp(accountList.cur->element.user_name, username) == 0) && (strcicmp(accountList.cur->element.password, pass) == 0)){
            strcpy(accountList.cur->element.password, new_pass);
            return 1;
		}
		accountList.cur = accountList.cur->next;
	}
	return 0;
}
int strcicmp(char const *a, char const *b){
    for (;; a++, b++) {
        int d = tolower((unsigned char)*a) - tolower((unsigned char)*b);
        if (d != 0 || !*a)
            return d;
	}
}
int split_number_and_string(char *input, char *number,char *string){
	// if number, copy to number
	// if character, copy to string
	int x = 0, y = 0;

	for(int i = 0; i < strlen(input); i++){
		if (input[i] == '\0') break;
		if(input[i] >= '0' && input[i] <= '9'){
    	    number[x] = input[i];
    	   	x++;
   		}
   		else if ((input[i] >= 'a' && input[i] <= 'z')||(input[i] >= 'A' && input[i] <= 'Z')){
   			string[y] = input[i];
   			y++;
   		}
   		else{
   			return 0;
   		}
   	}
   	number[x] = '\0'; 
	string[y] = '\0'; 
	return 1;
}
int main(int argc, char *argv[]){
	// catch wrong input
	if(argc != 2){
		printf("Please input port number\n");
		return 0;
	}
	//Receive port number
	int port = atoi(argv[1]);
	char username[BUFLEN], pass[BUFLEN], new_pass[BUFLEN], buf[BUFLEN], number[BUFLEN], string[BUFLEN];
	FILE *fp;
    fp = fopen("nguoidung.txt","r");
	createSingleList(&list);
	readDataFromFile(&list);
	//Create Server Socket
	int sockfd, ret;
	struct sockaddr_in serverAddr;

	int newSocket;
	struct sockaddr_in newAddr;

	socklen_t addr_size;

	pid_t childpid;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Server Socket is created.\n");
	
	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = INADDR_ANY;  //ip server

	ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-]Error in binding.\n");
		exit(1);
	}
	printf("[+]Bind to port %d\n", port);

	if(listen(sockfd, 10) == 0){
		printf("[+]Listening....\n");
	}else{
		printf("[-]Error in binding.\n");
	}

	while(1){
		newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
		if(newSocket < 0){
			exit(1);
		}
		printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));

		if((childpid = fork()) == 0){
			close(sockfd);

			while(1){
				memset(username,'\0', BUFLEN);
				memset(pass, '\0', BUFLEN);
				memset(number,'\0', BUFLEN);
				memset(string,'\0', BUFLEN);
				memset(new_pass,'\0', BUFLEN);
				//read data from socket 
				recv(newSocket, username, BUFLEN, 0);
				if(strcmp(username, "\n") == 0){
					printf("Disconnected from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
					break;
				}else{
					printf("Received username: %s\n", username);
					if (searchAccount(list, username)){//check if account exist ?
						//write data via socket
						memset(buf, '\0', BUFLEN);
						strcpy(buf, "Insert password\n");
						//buf[strlen(buf) - 1] = '\0';
						send(newSocket, buf, strlen(buf), 0);
						bzero(buf, sizeof(buf));//buf can receive message from another client
						int wrong_pass_count = 0;
						while (1){
							//read data from socket 
							recv(newSocket, pass, BUFLEN, 0);
							printf("Received pass: %s\n",pass);
							if(checkCorrectPassword(list, username, pass)){//check if pass is correct? 
								if(checkBlocked(list, username) != 1){//check if account is blocked?
									//write data via socket
									memset(buf, '\0', BUFLEN);
									strcpy(buf, "OK\nEnter new password or 'bye' to sign out\n");
									send(newSocket, buf, strlen(buf), 0);
									bzero(buf, sizeof(buf));
									//read data from socket 
									recv(newSocket, new_pass, BUFLEN, 0);
									printf("Received new pass: %s\n", new_pass);
									if(strcmp(new_pass, "bye") == 0){
										//write data via socket
										memset(buf, '\0', BUFLEN);
										strcpy(buf, "Goodbye ");
										strcat(buf, username);
										send(newSocket, buf, strlen(buf), 0);
										bzero(buf, sizeof(buf));
										break;
									}
									if(split_number_and_string(new_pass, number, string)){
										//write data via socket
										memset(buf, '\0', BUFLEN);
										strcpy(buf, number);
										strcat(buf, string);
										send(newSocket, buf, strlen(buf), 0);
										bzero(buf, sizeof(buf));
										changePass(list, username, pass, new_pass);
										alterDataOfFile(list);
									}
									else{//wrong format
										//write data via socket
										memset(buf, '\0', BUFLEN);
										strcpy(buf, "Error");
										send(newSocket, buf, strlen(buf), 0);
										bzero(buf, sizeof(buf));
									}
									break;
								}
								else{
									//write data via socket
									memset(buf, '\0', BUFLEN);
									strcpy(buf, "Account not ready\n");
									//buf[strlen(buf) - 1] = '\0';
									send(newSocket, buf, strlen(buf), 0);
									bzero(buf, sizeof(buf));
									break;
								}
							}
							else{
								if (wrong_pass_count == 3){
									break;
								}
								//write data via socket
								memset(buf, '\0', BUFLEN);
								strcpy(buf, "Not OK\n");
								send(newSocket, buf, strlen(buf), 0);
								bzero(buf, sizeof(buf));
								wrong_pass_count++;
							}
						}
						if (wrong_pass_count == 3){
							//write data via socket
							memset(buf, '\0', BUFLEN);
							strcpy(buf, "Account is blocked\n");
							//buf[strlen(buf) - 1] = '\0';
							send(newSocket, buf, strlen(buf), 0);
							bzero(buf, sizeof(buf));
							blockAccount(list, username);
							alterDataOfFile(list);
							continue;
						}
					}
					else{
						memset(buf, '\0', BUFLEN);
						strcpy(buf, "Wrong account\n");
						send(newSocket, buf, strlen(buf), 0);
						bzero(buf, sizeof(buf));
						continue;
					}
				}
			}
		}
	}
	close(newSocket);
	return 0;
}