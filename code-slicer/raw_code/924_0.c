char * ec_strtok ( char * s , const char * delim , char * * ptrptr ) {
 # ifdef HAVE_STRTOK_R return strtok_r ( s , delim , ptrptr ) ;
 # else # warning unsafe strtok # undef strtok return strtok ( s , delim ) ;
 # endif }