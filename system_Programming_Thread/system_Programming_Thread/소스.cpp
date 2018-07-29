//
// Thread programming homework
// A simple thread pipeline (multiple single producer and consumer version)
// Student Name : ¿Ã¥Î±Ÿ
// Student Number : B311142
//
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>



struct autoPart {
	int partNumber;
	struct autoPart *next;
};



struct autoPartBox {
	int bid;			// autoPartBox id
	int SIZE;			// SIZE Of autoPartBox
	int count;			// the number of autoParts in the Box
	struct autoPart *lastPart;	// Pointer to the last auto part
	struct autoPart *firstPart;	// Pointer to the first auto part
	pthread_mutex_t mutex;
	pthread_cond_t full;
	pthread_cond_t empty;
};



struct stageArg {
	int sid;
	int defectNumber;
};



struct startArg {
	int sid;
	int nPart;
};



#define ENDMARK -1
struct autoPartBox *AutoBox;
pthread_barrier_t barrier;




void sendAutoPart(int id, struct autoPart *ap, struct autoPartBox *apBox) {
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Critical region
	pthread_mutex_lock(&apBox->mutex);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////	

	// Box is full
	while (apBox->count >= apBox->SIZE)
	{
		printf("SEND:: Stage %d thread waiting on autPartBox %d full \n", id, apBox->bid);
		pthread_cond_wait(&apBox->full, &apBox->mutex);
	};

	ap->next = NULL;
	if (apBox->firstPart == NULL)
	{
		apBox->firstPart = ap;
		apBox->lastPart = ap;
	}
	else
	{
		apBox->lastPart->next = ap;
		apBox->lastPart = ap;
	}
	apBox->count++;
	printf("SEND:: Stage %d sending autoPart %d to autoPartBox %d\n", id, apBox->lastPart->partNumber, apBox->bid);

	// Box is not empty anymore
	if (apBox->count > 0)
	{
		printf("SEND:: Stage %d signals autoBoxPart %d NOT empty\n", id, apBox->bid);
		pthread_cond_signal(&apBox->empty);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Critical region Fin
	pthread_mutex_unlock(&apBox->mutex);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
}





struct autoPart *receiveAutoPart(int id, struct autoPartBox *apBox) {
	struct autoPart* autoPtr;
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Critical region
	pthread_mutex_lock(&apBox->mutex);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Box is empty
	while (apBox->count <= 0)
	{
		printf("RECEIVE:: Stage %d waiting on autoPartBox %d empty\n", id, apBox->bid);
		pthread_cond_wait(&apBox->empty, &apBox->mutex);
	};

	autoPtr = apBox->firstPart;
	apBox->firstPart = apBox->firstPart->next;
	if (apBox->firstPart == NULL)
		apBox->lastPart = NULL;
	apBox->count--;
	printf("RECEIVE:: Stage %d receiving autoPart %d from autoPartBox %d\n", id, autoPtr->partNumber, apBox->bid);

	// Box is not full anymore
	if (apBox->count < apBox->SIZE)
	{
		printf("RECEIVE:: Stage %d signals autoPartBox %d NOT full\n", id, apBox->bid);
		pthread_cond_signal(&apBox->full);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Critical region Fin
	pthread_mutex_unlock(&apBox->mutex);
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	return(autoPtr);
}





// Generate autoParts and put the autoParts into the first autoPartBox
void *startThread(void *ag) {
	int i;
	int id = ((struct startArg *)ag)->sid;
	int nPart = ((struct startArg *)ag)->nPart;
	long int startThreadSum = 0;

	for (i = 0; i < nPart; i++)
	{
		struct autoPart* newAutoPart = (struct autoPart*) malloc(sizeof(struct autoPart));
		newAutoPart->partNumber = rand();
		newAutoPart->next = NULL;
		printf("Start Thread Stage %d sending autoPart %d to autoPartBox %d\n", 0, newAutoPart->partNumber, 0);
		sendAutoPart(id, newAutoPart, &(AutoBox[id]));
		startThreadSum += newAutoPart->partNumber;
	}

	struct autoPart* newAutoPart = (struct autoPart*) malloc(sizeof(struct autoPart));
	newAutoPart->partNumber = ENDMARK;
	newAutoPart->next = NULL;
	printf("Start Thread Stage %d sending ENDMARK to autoPartBox %d\n", 0, 0);
	sendAutoPart(id, newAutoPart, &(AutoBox[id]));
	pthread_barrier_wait(&barrier);
	pthread_exit((void*)startThreadSum);
}





// Get autoParts from the last autoPartBox and add all of them
void *endThread(void *ag) {
	int tid = *((int*)ag);
	long int endThreadSum = 0;

	while (1)
	{
		// try to get auto part from id - 1 's auto part box
		struct autoPart* autoPtr = receiveAutoPart(tid, &(AutoBox[tid - 1]));
		if (autoPtr->partNumber != -1)
		{
			printf("End Thread Stage %d receiving autoPart %d from autoPartBox %d\n", tid, autoPtr->partNumber, tid - 1);
			endThreadSum += autoPtr->partNumber;
		}
		else
		{
			printf("End Thread Stage %d receiving ENDMARK from autoPartBox %d\n", tid, tid - 1);
			pthread_barrier_wait(&barrier);
			pthread_exit((void*)endThreadSum);
		}
	}

}





// Check autoParts from the input box and remove faulty parts
// Add all faulty parts number; Put valid autoParts into the output box
// The faulty part number is a multiple of the stage defect number
void *stageThread(void *ptr) {
	struct stageArg * stArg = (struct stageArg *) ptr;
	long int stageThreadSum = 0;

	while (1)
	{
		// try to get auto part from id - 1 's auto part box
		struct autoPart* autoPtr = receiveAutoPart(stArg->sid, &(AutoBox[stArg->sid - 1]));
		if (autoPtr->partNumber != -1)
		{
			printf("Stage %d receiving autoPart %d from autoPartBox %d\n", stArg->sid, autoPtr->partNumber, stArg->sid - 1);
			if (autoPtr->partNumber % stArg->defectNumber == 0)
			{
				// find defect
				printf("Stage %d deleting autoPart %d\n", stArg->sid, autoPtr->partNumber);
				stageThreadSum += autoPtr->partNumber;
				continue;
			}

			// try to send auto part to id's auto part box
			printf("Stage %d sending autoPart %d to autoPartBox %d\n", stArg->sid, autoPtr->partNumber, stArg->sid);
			sendAutoPart(stArg->sid, autoPtr, &(AutoBox[stArg->sid]));
		}
		else
		{
			// receive ENDMARK from id - 1 's auto part box
			printf("Stage %d receiving ENDMARK from autoPartBox %d\n", stArg->sid, stArg->sid - 1);

			// try to send auto part to id's auto part box
			printf("Stage %d sending ENDMARK to autoPartBox %d\n", stArg->sid, stArg->sid);
			sendAutoPart(stArg->sid, autoPtr, &(AutoBox[stArg->sid]));
			pthread_barrier_wait(&barrier);
			pthread_exit((void*)stageThreadSum);
		}
	}

}





int main(int argc, char *argv[]) {

	long int startThreadSum = 0;
	long int endThreadSum = 0;
	long int stageThreadSum = 0;
	void *status;
	int i;
	int nStages;
	int boxSize;
	int nPart;
	int endThread_arg; // Thread number
	struct startArg startThread_arg;
	struct stageArg *stageThread_arg;
	pthread_t startTid;
	pthread_t *stageTid;
	pthread_t endTid;
	srand(100);
	//**************************************************************************





	// get init information from arguments
	if (argc < 5) { printf("Usage : tp nStages boxSize nPart defectNumber[0..*]\n"); exit(-1); }
	nStages = atoi(argv[1]);
	boxSize = atoi(argv[2]);
	nPart = atoi(argv[3]);

	stageTid = (pthread_t*)malloc(sizeof(pthread_t) * nStages);
	AutoBox = (struct autoPartBox*) malloc(sizeof(struct autoPartBox) * (nStages + 1));
	stageThread_arg = (struct stageArg *) malloc(sizeof(struct stageArg) * nStages);

	// set start thread's arguments
	startThread_arg.sid = 0;
	startThread_arg.nPart = nPart;
	endThread_arg = nStages + 1;

	// get defect number for each thread
	for (i = 0; i < nStages; i++)
	{
		stageThread_arg[i].sid = i + 1;
		stageThread_arg[i].defectNumber = atoi(argv[4 + i]);
	}

	// initialize auto box
	for (i = 0; i < nStages + 1; i++)
	{
		AutoBox[i].bid = i;
		AutoBox[i].SIZE = boxSize;
		AutoBox[i].count = 0;
		AutoBox[i].firstPart = NULL;
		AutoBox[i].lastPart = NULL;
		pthread_mutex_init(&AutoBox[i].mutex, NULL);
		pthread_cond_init(&AutoBox[i].full, NULL);
		pthread_cond_init(&AutoBox[i].empty, NULL);
	}




	pthread_barrier_init(&barrier, NULL, nStages + 3); // nStage Thread + main + start + end
	pthread_create(&startTid, NULL, startThread, (void *)&startThread_arg);
	for (i = 0; i < nStages; i++)
	{
		pthread_create(&stageTid[i], NULL, stageThread, (void *)&(stageThread_arg[i]));
	}
	pthread_create(&endTid, NULL, endThread, (void *)&endThread_arg);
	pthread_barrier_wait(&barrier);




	//**************************************************************************
	printf("*** Part Sum Information ***\n");
	pthread_join(startTid, &status);
	printf("startThread sum %ld\n", status);
	startThreadSum = status;

	pthread_join(endTid, &status);
	printf("endThread sum %ld\n", status);
	endThreadSum = status;

	for (i = 0; i < nStages; i++) {
		pthread_join(stageTid[i], &status);
		stageThreadSum += status;
		printf("Stage %d sum %ld\n", i, status);
	}

	assert(startThreadSum == (endThreadSum + stageThreadSum));

	pthread_exit(0);

}
