
// IPC using Message Queues

/*
    A message queue is a linked list of messages stored within the kernel and identified by a message queue identifier.
    A
    New messages are added to the end of a queue by msgsnd(). Every message has a positive long integer type field, a non-negative length, and the actual data bytes (corresponding to the length), all of which are specified to msgsnd() when the message is added to a queue. Messages are fetched from a queue by msgrcv(). We don’t have to fetch the messages in a first-in, first-out order. Instead, we can fetch messages based on their type field.

    All processes can exchange information through access to a common system message queue. The sending process places a message (via some (OS) message-passing module) onto a queue which can be read by another process. Each message is given an identification or type so that processes can select the appropriate message. Process must share a common key in order to gain access to the queue in the first place.

    ftok():
        It is used to generate a unique key.

     int msgget(key_t key, int msgflg)

        returns the message queue identifier for a newly created message queue or returns the identifiers for a queue which exists with the same key value.

    int msgsnd(int msgid, const void *msgp, size_t msgsz, int msgflg)

        Write into message queue means Data is placed on to a message queue.
    msgrcv():
        Read from the message queue means messages are retrieved from a queue.
    msgctl():
         It performs various operations on a queue. Generally it is use to destroy message queue.

Now, let us check the syntax and certain information on the above calls.

    int msgget(key_t key, int msgflg)

        key -
        recognizes the message queue. The key can be either an arbitrary value or one that can be derived from the library function ftok().

        shmflg -
        specifies the required message queue flag/s such as IPC_CREAT (creating message queue if not exists) or IPC_EXCL (Used with IPC_CREAT to create the message queue and the call fails, if the message queue already exists). Need to pass the permissions as well.

    int msgsnd(int msgid, const void *msgp, size_t msgsz, int msgflg)


        msgid - recognizes the message queue i.e., message queue identifier.
        msgp - pointer to the message, sent to the caller, defined in the structure
        msgsz, is the size of message (the message should end with a null character)
        The fourth argument, msgflg, indicates certain flags such as IPC_NOWAIT (returns immediately when no message is found in queue or MSG_NOERROR (truncates message text, if more than msgsz bytes)

            struct msgbuf {
                long mtype;
                char mtext[1];
            };

    If msgtype is 0 − Reads the first received message in the queue

        If msgtype is +ve − Reads the first message in the queue of type msgtype (if msgtype is 10, then reads only the first message of type 10 even though other types may be in the queue at the beginning)

        If msgtype is –ve − Reads the first message of lowest type less than or equal to the absolute value of message type (say, if msgtype is -5, then it reads first message of type less than 5 i.e., message type from 1 to 5)

        The fifth argument, msgflg, indicates certain flags such as IPC_NOWAIT (returns immediately when no message is found in the queue or MSG_NOERROR (truncates the message text if more than msgsz bytes)


    int msgrcv(int msgid, const void *msgp, size_t msgsz, long msgtype, int msgflg)

        This system call retrieves the message from the message queue (System V).

        The first argument, msgid, recognizes the message queue i.e., the message queue identifier. The identifier value is received upon the success of msgget()

        The second argument, msgp, is the pointer of the message received from the caller. It is defined in the structure of the following form −

        The third argument, msgsz, is the size of the message received (message should end with a null character)

        The fouth argument, msgtype, indicates the type of message −



    int msgctl(int msgid, int cmd, struct msqid_ds *buf)

        This system call performs control operations of the message queue (System V)

        The first argument, msgid, recognizes the message queue i.e., the message queue identifier. The identifier value is received upon the success of msgget()

        The second argument, cmd, is the command to perform the required control operation on the message queue. Valid values for cmd are −

            IPC_STAT − Copies information of the current values of each member of struct msqid_ds to the passed structure pointed by buf. This command requires read permission on the message queue.

            IPC_SET − Sets the user ID, group ID of the owner, permissions etc pointed to by structure buf.

            IPC_RMID − Removes the message queue immediately.

            IPC_INFO − Returns information about the message queue limits and parameters in the structure pointed by buf, which is of type struct msginfo

            MSG_INFO − Returns an msginfo structure containing information about the consumed system resources by the message queue.

            The third argument, buf, is a pointer to the message queue structure named struct msqid_ds. The values of this structure would be used for either set or get as per cmd.

    Let us see the description before looking at the program −

    Step 1 − Create two processes, one is for sending into message queue (msgq_send.c) and
    another is for retrieving from the message queue (msgq_recv.c)

    Step 2 − Creating the key, using ftok() function. For this, initially file msgq.txt is created to get a unique key.

    Step 3 − The sending process performs the following.
    Reads the string input from the user Removes the new line, if it exists
    Sends into message queue Repeats the process until the end of input (CTRL + D)
    Once the end of input is received, sends the message "end" to signify the end of the process

    Step 4 − In the receiving process, performs the following.
    Reads the message from the queue
    Displays the output
    If the received message is "end", finishes the process and exits


*/
// Process - 1 Write message into message queue
// C Program for Message Queue (Writer Process)

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#define MAX 10

// structure for message queue
struct mesg_buffer
{
    long mesg_type;
    char mesg_text[100];
} message;

int write_message_queue()
{
    // ftok to generate unique key
    key_t key = ftok("progfile", 65);

    // msgget creates a message queue
    // and returns identifier
    int msgid = msgget(key, 0666 | IPC_CREAT);
    message.mesg_type = 1;

    printf("Write Data : ");
    fgets(message.mesg_text, MAX, stdin);

    // msgsnd to send message
    msgsnd(msgid, &message, sizeof(message), 0);

    // display the message
    printf("Data send is : %s \n", message.mesg_text);

    return 0;
}

// Process - 2 Read message From message queue
// C Program for Message Queue (Reader Process)
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>

int read_message_queue()
{
    // ftok to generate unique key
    key_t key = ftok("progfile", 65);

    // msgget creates a message queue
    // and returns identifier
    int msgid = msgget(key, 0666 | IPC_CREAT);

    // msgrcv to receive message
    msgrcv(msgid, &message, sizeof(message), 1, 0);

    // display the message
    printf("Data Received is : %s \n",
           message.mesg_text);

    // to destroy the message queue
    msgctl(msgid, IPC_RMID, NULL);

    return 0;
}

int main()
{
    write_message_queue();
    read_message_queue();

    return 0;
}