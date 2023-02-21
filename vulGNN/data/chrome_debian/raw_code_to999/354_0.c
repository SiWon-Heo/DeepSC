int dtls1_get_queue_priority ( unsigned short seq , int is_ccs ) {
 return seq * 2 - is_ccs ;
 }