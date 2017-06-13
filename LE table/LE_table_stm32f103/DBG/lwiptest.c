#include "io.h"
#include "stm32f0xx.h"
#include "bsp.h"
#include "lwip\tcpip.h"
//#include "netif\ppp\ppp.h"
//#include "netif\ppp\pppos.h"
//
//
//#include "lwip/sockets.h"
//#include "lwip/tcpip.h"
//#include "ppp/ppp.h"


#include <stdio.h>


void pppd(void *p);
//static void tcpip_init_done(void *arg);
//static void linkStatusCB(void* ctx, int errCode, void* arg);


void pppd(void *p)
{
   tcpip_init(NULL,NULL);

//       ppp_init(); 
//       ppp = ppp_new(); 
//       ppp_set_auth();
  
//    int connected = 0;
//    volatile int setup = 0;
//    int pd;
//    const char *username = "myuser";
//    const char *password = "mypassword";
//    tcpip_init(tcpip_init_done, &setup);
//    pppInit();
    //pppSetAuth(PPPAUTHTYPE_ANY, username, password);
      //void ppp_set_auth(ppp_pcb *pcb, u8_t authtype, const char *user, const char *passwd)
//ppp_set_auth(NULL, 1, NULL, NULL);
//pppos_input(int pd, u_char* data, int len);
//pppos_input(0, "siski", 5);
}



