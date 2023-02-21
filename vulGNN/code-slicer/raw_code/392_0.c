struct timeval * dtls1_get_timeout ( SSL * s , struct timeval * timeleft ) {
 struct timeval timenow ;
 if ( s -> d1 -> next_timeout . tv_sec == 0 && s -> d1 -> next_timeout . tv_usec == 0 ) {
 return NULL ;
 }
 get_current_time ( & timenow ) ;
 if ( s -> d1 -> next_timeout . tv_sec < timenow . tv_sec || ( s -> d1 -> next_timeout . tv_sec == timenow . tv_sec && s -> d1 -> next_timeout . tv_usec <= timenow . tv_usec ) ) {
 memset ( timeleft , 0 , sizeof ( * timeleft ) ) ;
 return timeleft ;
 }
 memcpy ( timeleft , & ( s -> d1 -> next_timeout ) , sizeof ( struct timeval ) ) ;
 timeleft -> tv_sec -= timenow . tv_sec ;
 timeleft -> tv_usec -= timenow . tv_usec ;
 if ( timeleft -> tv_usec < 0 ) {
 timeleft -> tv_sec -- ;
 timeleft -> tv_usec += 1000000 ;
 }
 if ( timeleft -> tv_sec == 0 && timeleft -> tv_usec < 15000 ) {
 memset ( timeleft , 0 , sizeof ( * timeleft ) ) ;
 }
 return timeleft ;
 }