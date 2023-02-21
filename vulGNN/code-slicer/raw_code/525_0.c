int mime_parse_time ( const char * & buf , const char * end , int * hour , int * min , int * sec ) {
 if ( ! mime_parse_integer ( buf , end , hour ) ) {
 return 0 ;
 }
 if ( ! mime_parse_integer ( buf , end , min ) ) {
 return 0 ;
 }
 if ( ! mime_parse_integer ( buf , end , sec ) ) {
 return 0 ;
 }
 return 1 ;
 }