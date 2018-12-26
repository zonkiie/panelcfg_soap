#include <includes.h>
#include <soap_includes.h>
#define debug 0

int run = 1;
int sigcount = 0;

void terminate(int sig)
{
	if(sig == SIGINT || sig == SIGTERM)
	{
		run = 0;
		sigcount++;
		fprintf(stderr, "Terminating Process.\n");
	}
	if(sigcount >= 2) exit(0);
}

void soap_cleanup();

/// @see https://stackoverflow.com/questions/15302348/how-can-i-use-sigint-to-break-out-of-the-wait-for-accept-or-read
void set_signal_handlers()
{
	//signal(SIGINT, terminate);
	//signal(SIGTERM, terminate);
	struct sigaction act;
	act.sa_handler=terminate;
	act.sa_flags = 0;
	sigemptyset( &act.sa_mask );

	sigaction(SIGINT, &act, NULL);
	sigaction(SIGTERM, &act, NULL);

}

int start_cgi()
{
	struct soap *soap = soap_new();
	soap->authrealm = "panelsoap";
	return soap_serve(soap);
	//return soap_serve(soap_new());
	//start_service();
}

int start_mt(int argc, char **argv) 
{
  set_signal_handlers();
  progpath = argv[0];
  progdir = dirname(progpath);
  struct soap soap; 
  soap_init(&soap);
  soap.fget = http_get;
  soap.authrealm = "panelsoap";
  if (argc < 2) // no args: assume this is a CGI application 
  {
    soap_serve(&soap);   // serve request, one thread, CGI style 
    soap_destroy(&soap); // delete allocated class instances 
    soap_end(&soap);     // delete allocated data and temporaries 
  } 
  else 
  {
    struct soap *soap_thr[MAX_THR]; // each thread needs a context 
    THREAD_TYPE tid[MAX_THR];       // array of thread IDs
    int port = atoi(argv[1]);       // first command-line arg is port 
    SOAP_SOCKET m, s; 
    int i; 
    soap.send_timeout = 10;     // 10 seconds max socket delay 
    soap.recv_timeout = 10;     // 10 seconds max socket delay 
    soap.accept_timeout = 3600; // server stops after 1 hour of inactivity 
    soap.max_keep_alive = 100;  // max keep-alive sequence 
    m = soap_bind(&soap, NULL, port, BACKLOG); 
    if (!soap_valid_socket(m)) 
      exit(EXIT_FAILURE);
    fprintf(stderr, "Socket connection successful %d\n", m); 
    for (i = 0; i < MAX_THR; i++) 
      soap_thr[i] = NULL; 
    while (1)
    {
      for (i = 0; i < MAX_THR; i++) 
      {
        s = soap_accept(&soap); 
        if (soap_valid_socket(s)) 
        {
          fprintf(stderr, "Thread %d accepts socket %d connection from IP %ld.%ld.%ld.%ld\n", i, s, (soap.ip>>24)&0xFF, (soap.ip>>16)&0xFF, (soap.ip>>8)&0xFF, soap.ip&0xFF); 
          if (!soap_thr[i]) // first time around 
          {
            soap_thr[i] = soap_copy(&soap); 
            if (!soap_thr[i]) 
              exit(EXIT_FAILURE); // could not allocate 
          } 
          else // recycle threaded soap contexts
          {
            // optionally, we can cancel the current thread when stuck on IO:
            // soap_close_connection(soap_thr[i]); // requires compiling 2.8.71 or greater with -DWITH_SELF_PIPE
            THREAD_JOIN(tid[i]); 
            fprintf(stderr, "Thread %d completed\n", i); 
            soap_destroy(soap_thr[i]);            // delete allocated class instances of thread 
            soap_end(soap_thr[i]);                // delete allocated data and temporaries of thread 
            soap_copy_stream(soap_thr[i], &soap); // pass the connection on to the thread
          } 
          while (THREAD_CREATE(&tid[i], (void*(*)(void*))soap_serve, (void*)soap_thr[i]))
            sleep(1); // failed, try again
        }
        else if (soap.errnum) // accept failed, try again after 1 second
        {
          soap_print_fault(&soap, stderr); 
          sleep(1);
        } 
        else
        {
          fprintf(stderr, "Server timed out\n"); 
          goto end;
        } 
      } 
    } 
end:
    for (i = 0; i < MAX_THR; i++) 
    {
      if (soap_thr[i]) 
      {
        THREAD_JOIN(tid[i]); 
        soap_destroy(soap_thr[i]);
        soap_end(soap_thr[i]);
        soap_free(soap_thr[i]);
      }
    }
  } 
  soap_destroy(&soap);
  soap_end(&soap);
  soap_done(&soap);
  return 0; 
}

void *process_queue(void*); 
void enqueue(SOAP_SOCKET); 
SOAP_SOCKET dequeue(); 
static SOAP_SOCKET queue[MAX_QUEUE]; // The global request queue of sockets 
static int head = 0, tail = 0;
static MUTEX_TYPE queue_lock;    // mutex for queue ops critical sections
static COND_TYPE queue_notempty; // condition variable when queue is empty
static COND_TYPE queue_notfull;  // condition variable when queue is full
struct soap soap;
struct soap *soap_thr[MAX_THR];
THREAD_TYPE tid[MAX_THR];
/// @see https://www.genivia.com/doc/guide/html/index.html#mt
int start_mt_queue(int argc, char **argv) 
{
	set_signal_handlers();
	atexit(soap_cleanup);
	progpath = argv[0];
	progdir = dirname(progpath);
	//struct soap soap; 
	soap_init(&soap);
	soap.fget = http_get;
	soap.authrealm = "panelsoap";
	if (argc < 2) // no args: assume this is a CGI application 
	{
		soap_serve(&soap);   // serve request, one thread, CGI style 
		soap_destroy(&soap); // delete allocated class instances 
		soap_end(&soap);     // delete allocated data and temporaries 
	} 
	else 
	{
		//struct soap *soap_thr[MAX_THR]; // each thread needs a context 
		//THREAD_TYPE tid[MAX_THR]; 
		int port = atoi(argv[1]); // first command-line arg is port 
		SOAP_SOCKET m, s; 
		int i; 
		m = soap_bind(&soap, NULL, port, BACKLOG); 
		if (!soap_valid_socket(m))
		{
			fprintf(stderr, "Error: %s\n", strerror(errno));
			exit(EXIT_FAILURE); 
		}
		if(debug) fprintf(stderr, "Socket connection successful %d\n", m); 
		MUTEX_SETUP(queue_lock); 
		COND_SETUP(queue_notempty); 
		COND_SETUP(queue_notfull); 
		for (i = 0; i < MAX_THR; i++) 
		{
			soap_thr[i] = soap_copy(&soap); 
			if(debug) fprintf(stderr, "Starting thread %d\n", i); 
			while (THREAD_CREATE(&tid[i], (void*(*)(void*))process_queue, (void*)soap_thr[i]))
				sleep(1); // failed, try again
		} 
		while (run)
		{
			s = soap_accept(&soap); 
			if (soap_valid_socket(s)) 
			{
				if(debug) fprintf(stderr, "Thread %d accepts socket %d connection from IP %ld.%ld.%ld.%ld\n", i, s, (soap.ip>>24)&0xFF, (soap.ip>>16)&0xFF, (soap.ip>>8)&0xFF, soap.ip&0xFF); 
				enqueue(s);
			}
			else if (soap.errnum) // accept failed, try again after 1 second
			{
				soap_print_fault(&soap, stderr); 
				sleep(1);
			} 
			else
			{
				if(debug) fprintf(stderr, "Server timed out\n"); 
				break; 
			} 
		}
		/*fprintf(stderr, "Terminating work.\n");
		for (i = 0; i < MAX_THR; i++) 
			enqueue(SOAP_INVALID_SOCKET);
		for (i = 0; i < MAX_THR; i++) 
		{
			if(debug) fprintf(stderr, "Waiting for thread %d to terminate... ", i); 
			THREAD_JOIN(tid[i]); 
			if(debug) fprintf(stderr, "terminated\n"); 
			soap_free(soap_thr[i]); 
		} 
		COND_CLEANUP(queue_notfull); 
		COND_CLEANUP(queue_notempty); 
		MUTEX_CLEANUP(queue_lock); */
	} 
	/*soap_destroy(&soap);
	soap_end(&soap);
	soap_done(&soap); */
	return 0; 
} 
void *process_queue(void *soap) 
{
	struct soap *tsoap = (struct soap*)soap; 
	while (run)
	{
		tsoap->socket = dequeue(); 
		if (!soap_valid_socket(tsoap->socket)) 
			break; 
		soap_serve(tsoap); 
		soap_destroy(tsoap); 
		soap_end(tsoap); 
		if(debug) fprintf(stderr, "served\n"); 
	} 
	return NULL; 
} 
/* add job (socket with pending request) to queue */
void enqueue(SOAP_SOCKET s)
{
	int next;
	MUTEX_LOCK(queue_lock);
	next = (tail + 1) % MAX_QUEUE;
	if (next == head)
		COND_WAIT(queue_notfull, queue_lock);
	queue[tail] = s;
	tail = next;
	COND_SIGNAL(queue_notempty);
	MUTEX_UNLOCK(queue_lock);
}
/* remove job (socket with request) from queue */
SOAP_SOCKET dequeue()
{
	SOAP_SOCKET s;
	MUTEX_LOCK(queue_lock);
	if (head == tail)
		COND_WAIT(queue_notempty, queue_lock);
	s = queue[head];
	head = (head + 1) % MAX_QUEUE;
	COND_SIGNAL(queue_notfull);
	MUTEX_UNLOCK(queue_lock);
	return s;
}

void soap_cleanup()
{
	int i;
	fprintf(stderr, "Terminating work.\n");
	for (i = 0; i < MAX_THR; i++) 
		enqueue(SOAP_INVALID_SOCKET);
	for (i = 0; i < MAX_THR; i++) 
	{
		if(debug) fprintf(stderr, "Waiting for thread %d to terminate... ", i); 
		THREAD_JOIN(tid[i]); 
		if(debug) fprintf(stderr, "terminated\n"); 
		soap_free(soap_thr[i]); 
	} 
	COND_CLEANUP(queue_notfull); 
	COND_CLEANUP(queue_notempty); 
	MUTEX_CLEANUP(queue_lock);
	soap_destroy(&soap);
	soap_end(&soap);
	soap_done(&soap);
}

int main(int argc, char **argv)
{
	return start_mt_queue(argc, argv);
}

