#include <includes.h>
#include <soap_includes.h>

char *progpath;

/*int main()
{
	struct soap *soap = soap_new();
	soap->authrealm = "panelsoap";
	return soap_serve(soap);
	//return soap_serve(soap_new());
	//start_service();
}*/

int main(int argc, char **argv) 
{
  progpath = argv[0];
  struct soap soap; 
  soap_init(&soap);
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
