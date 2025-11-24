#include "dm_request_queue.hpp"
#include "dm_thingmap.hpp"


DmRequestQueue * DmRequestQueue::Instance = 0;

/*DmRequestQueue::DmRequestQueue() {
}
DmRequestQueue::~DmRequestQueue() {
}*/

DmRequestQueue * DmRequestQueue::GetInstance() {
	if (Instance == 0) {
		Instance = new DmRequestQueue();
	}
	return Instance;
}

bool DmRequestQueue::DMReqQueueInit() {

	RequestResponse init;
	init.Command = "Initial message, ignore this.";
	init.Message_Id = "Initial message, ignore this.";
	init.priority = -1;
	init.Thing_Id = "Initial message, ignore this.";

	DMReqQueueDataTronx.push_back(init);

	/*syslog(LOG_DEBUG,
			"[DM] : Request  queue process init messag queue size: [%ld]",
			DMReqQueueDataTronx.size());*/

	return true;
}

void DmRequestQueue::DMReqQueueheapify(
		std::vector<RequestResponse> &DMReqQueueDataTronx, int i, int n) {
	RequestResponse temp;
	temp = DMReqQueueDataTronx[i];

	int j = 2 * i;
	while (j <= n) {
		if (j < n
				&& DMReqQueueDataTronx[j + 1].priority
						< DMReqQueueDataTronx[j].priority) {
			j = j + 1;
		}

		if (temp.priority <= DMReqQueueDataTronx[j].priority) {
			break;
		} else if (temp.priority > DMReqQueueDataTronx[j].priority) {
			DMReqQueueDataTronx[j / 2] = DMReqQueueDataTronx[j];
			j = 2 * j;
		}
	}

	DMReqQueueDataTronx[j / 2] = temp;
//	DMReqQueueDataTronx.shrink_to_fit();
}

void DmRequestQueue::DMReqQueueheapsort(
		std::vector<RequestResponse> &DMReqQueueDataTronx, int n) {
	int i;
	RequestResponse temp;

	for (i = n; i >= 2; i--) {
		temp = DMReqQueueDataTronx[i];

		if (DMReqQueueDataTronx[1].priority
				!= DMReqQueueDataTronx[i].priority) {
			DMReqQueueDataTronx[i] = DMReqQueueDataTronx[1];

			DMReqQueueDataTronx[1] = temp;

			DMReqQueueheapify(DMReqQueueDataTronx, 1, i - 1);
		}
	}
//	DMReqQueueDataTronx.shrink_to_fit();
}


void DmRequestQueue::DMReqQueuebuild_heap(
		std::vector<RequestResponse> &DMReqQueueDataTronx, int n) {
	for (int i = n / 2; i >= 1; i--) {
		DMReqQueueheapify(DMReqQueueDataTronx, i, n);
	}
//	DMReqQueueDataTronx.shrink_to_fit();
}

void DmRequestQueue::DMReqQueuefilter(
		std::vector<RequestResponse> &DMReqQueueDataTronx,
		std::vector<MessageIndex> &DMReqQueueIndexTronx, std::string id,
		bool &update, int &position) {

	update = true;
	size_t counter;

	for (size_t i = 0; i < DMReqQueueIndexTronx.size(); i++) {
		if (id == DMReqQueueIndexTronx[i].Thing_Id) {
			if (DMReqQueueIndexTronx[i].repeatAllowedStatus == 0
					&& DMReqQueueIndexTronx[i].count == 1) {
				update = false;
				counter = 1;
				while (counter < DMReqQueueDataTronx.size()) {
					if (id == DMReqQueueDataTronx[counter].Thing_Id) {

						syslog(LOG_DEBUG,
								"[DM] : Filtering request for Thing ID: [%s]",
								DMReqQueueDataTronx[counter].Thing_Id.c_str());

						DMReqQueueDataTronx.erase(
								DMReqQueueDataTronx.begin() + counter);
						break;
					}
					counter++;
				}
			}
			position = i;
			break;
		}
	}
/*	syslog(LOG_DEBUG,
			"[DM] : The vector size [%d] and its capacity BEFORE: [%d]=======RES",
			DMReqQueueDataTronx.size(), DMReqQueueDataTronx.capacity());
	DMReqQueueDataTronx.shrink_to_fit();
	syslog(LOG_DEBUG,
			"[DM] : The vector size [%d] and its capacity AFTER: [%d]=======RES",
			DMReqQueueDataTronx.size(), DMReqQueueDataTronx.capacity());
	syslog(LOG_DEBUG,
			"[DM] : The vector size [%d] and its capacity BEFORE: [%d]=======RES",
			DMReqQueueIndexTronx.size(), DMReqQueueIndexTronx.capacity());
	DMReqQueueIndexTronx.shrink_to_fit();
	syslog(LOG_DEBUG,
			"[DM] : The vector size [%d] and its capacity AFTER: [%d]=======RES",
			DMReqQueueIndexTronx.size(), DMReqQueueIndexTronx.capacity());*/
}

bool compare(RequestResponse one, RequestResponse two) {
	return (one.priority < two.priority);
}

/* Function name : DMReqQueueAddMessage
 * Description   : Add the received request into process queue vector
 */
void DmRequestQueue::DMReqQueueAddMessage(RequestResponse ReqMessageDataOBJ) {

	/*syslog(LOG_INFO,
			"[DM] : Request  queue process start adding message into queue start");*/

	pthread_mutex_lock(&ReqAddQueueRcvMutx);

	/*syslog(LOG_INFO,
			"[DM] : Request  queue process start adding message from queue lock rspaddqueuercvmutx");*/

	bool updateStatus = false;
	int position = 0;
	bool ItemFlag = false;

	/*syslog(LOG_DEBUG,
			"[DM] : Request  queue process before adding message queue size: [%ld]",
			DMReqQueueDataTronx.size());*/

	for (size_t i = 0; i < DMReqQueueIndexTronx.size(); i++) {
		if (ReqMessageDataOBJ.Thing_Id == DMReqQueueIndexTronx[i].Thing_Id) {
			ItemFlag = true;
		}
	}

	if (ItemFlag == false) {
		MessageIndex temp;
		temp.Thing_Id = ReqMessageDataOBJ.Thing_Id;
		temp.repeatAllowedStatus = ManyRequest;
		temp.count = 0;
		DMReqQueueIndexTronx.push_back(temp);
	}

	DMReqQueuefilter(DMReqQueueDataTronx, DMReqQueueIndexTronx,
			ReqMessageDataOBJ.Thing_Id, updateStatus, position);

	/*syslog(LOG_DEBUG,
			"[DM] : Request  queue process adding into message queue message_id: [%s] notif_id: [%s] thing_id: [%s]",
			ReqMessageDataOBJ.Message_Id.c_str(),
			ReqMessageDataOBJ.Notif_Id.c_str(),
			ReqMessageDataOBJ.Thing_Id.c_str());*/

	DMReqQueueDataTronx.push_back(ReqMessageDataOBJ);
	syslog (LOG_DEBUG,"[DM] Dm_requestqueue_capacity [%zu]", DMReqQueueDataTronx.capacity());
	syslog (LOG_DEBUG,"[DM] Dm_Requestqueue_size [%zu]", DMReqQueueDataTronx.size());
/*	syslog(LOG_DEBUG,
			"[DM] : Request  queue process after adding message queue size: [%d]",
			DMReqQueueDataTronx.size());*/

//	int size = (DMReqQueueDataTronx.size() - 1);
//	DMReqQueuebuild_heap(DMReqQueueDataTronx, size);
//	DMReqQueueheapsort(DMReqQueueDataTronx, size);

//	std::sort(DMReqQueueDataTronx.begin() + 1, DMReqQueueDataTronx.end(),
//			compare);

	std::stable_sort(DMReqQueueDataTronx.begin() + 1, DMReqQueueDataTronx.end(),
				compare);


	if (updateStatus) {
		DMReqQueueIndexTronx[position].count++;
	}
	
	
	/* RspQueueSizeflag is to indicate whether to process the queue or wait for a message.
	 * 0 = Indicates empty queue
	 * 1 = Indicates proccess the queue and reset to 0 after processing
	*/
	if (RspQueueSizeflag == 0) {
		pthread_cond_signal(&ReqAddRcvCond);
		RspQueueSizeflag = 1;
	}

	pthread_mutex_unlock(&ReqAddQueueRcvMutx);

	/*syslog(LOG_INFO,
			"[DM] : Request  queue process start receiving message from queue unlock rspaddqueuercvmutx");

	syslog(LOG_INFO,
			"[DM] : Request  queue process start adding message into queue end");*/
}

void DmRequestQueue::DMReqQueueRemoveMessage() {

	for (size_t i = 0; i < DMReqQueueIndexTronx.size(); i++) {
		if (DMReqQueueIndexTronx[i].Thing_Id
				== DMReqQueueDataTronx[1].Thing_Id) {

			syslog(LOG_DEBUG,
					"[DM] : Remove from Queue request for Thing ID: [%s]",
					DMReqQueueDataTronx[1].Thing_Id.c_str());

			DMReqQueueIndexTronx[i].count--;
			break;
		}
	}
	DMReqQueueDataTronx.erase(DMReqQueueDataTronx.begin() + 1);
}

RequestResponse DmRequestQueue::DMReqQueueReceiveMessage() {

	RequestResponse ReqMessageDataOBJ;

	if (DMReqQueueDataTronx.size() == 1) {

		/*syslog(LOG_INFO,
				"[DM] : Request  queue process receiving queue is empty waiting to add message");*/
		RspQueueSizeflag = 0;
		pthread_cond_wait(&ReqAddRcvCond, &ReqMutex3Cond);

	}

	pthread_mutex_lock(&ReqAddQueueRcvMutx);

	/*syslog(LOG_INFO,
			"[DM] : Request  queue process start receiving message from queue lock rsp add queue rcv mutx");

	syslog(LOG_DEBUG,
			"[DM] : Request  queue process before receiving message queue size: [%ld]",
			DMReqQueueDataTronx.size());*/

	ReqMessageDataOBJ = DMReqQueueDataTronx[1];

	syslog(LOG_DEBUG,
			"[DM] : Request  queue process receiving from message queue message_id: [%s] thing_id: [%s]",
			ReqMessageDataOBJ.Message_Id.c_str(),
			ReqMessageDataOBJ.Thing_Id.c_str());

	DMReqQueueRemoveMessage();

	/*syslog(LOG_DEBUG,
			"[DM] : Request  queue process after receiving message queue size: [%ld]",
			DMReqQueueDataTronx.size());*/

	pthread_mutex_unlock(&ReqAddQueueRcvMutx);

	/*syslog(LOG_INFO,
			"[DM] : Request  queue process start receiving message from queue unlock rsp add queue rcv mutx");*/

	return ReqMessageDataOBJ;

}
