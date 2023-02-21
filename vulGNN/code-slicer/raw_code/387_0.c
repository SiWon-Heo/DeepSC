int dtls1_is_timer_expired ( SSL * s ) {
 struct timeval timeleft ;
 if ( dtls1_get_timeout ( s , & timeleft ) == NULL ) {
 return 0 ;
 }
 if ( timeleft . tv_sec > 0 || timeleft . tv_usec > 0 ) {
 return 0 ;
 }
 return 1 ;
 }