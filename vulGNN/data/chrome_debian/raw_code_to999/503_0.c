int mime_parse_rfc822_date_fastcase ( const char * buf , int length , struct tm * tp ) {
 unsigned int three_char_wday , three_char_mon ;
 ink_assert ( length >= 29 ) ;
 ink_assert ( ! is_ws ( buf [ 0 ] ) ) ;
 ink_assert ( buf [ 3 ] == ',' ) ;
 tp -> tm_wday = - 1 ;
 three_char_wday = ( buf [ 0 ] << 16 ) | ( buf [ 1 ] << 8 ) | buf [ 2 ] ;
 if ( three_char_wday <= 0x53756E ) {
 if ( three_char_wday == 0x467269 ) {
 tp -> tm_wday = 5 ;
 }
 else if ( three_char_wday == 0x4D6F6E ) {
 tp -> tm_wday = 1 ;
 }
 else if ( three_char_wday == 0x536174 ) {
 tp -> tm_wday = 6 ;
 }
 else if ( three_char_wday == 0x53756E ) {
 tp -> tm_wday = 0 ;
 }
 }
 else {
 if ( three_char_wday == 0x546875 ) {
 tp -> tm_wday = 4 ;
 }
 else if ( three_char_wday == 0x547565 ) {
 tp -> tm_wday = 2 ;
 }
 else if ( three_char_wday == 0x576564 ) {
 tp -> tm_wday = 3 ;
 }
 }
 if ( tp -> tm_wday < 0 ) {
 tp -> tm_wday = day_names_dfa -> match ( buf , length ) ;
 if ( tp -> tm_wday < 0 ) {
 return 0 ;
 }
 }
 tp -> tm_mday = ( buf [ 5 ] - '0' ) * 10 + ( buf [ 6 ] - '0' ) ;
 tp -> tm_mon = - 1 ;
 three_char_mon = ( buf [ 8 ] << 16 ) | ( buf [ 9 ] << 8 ) | buf [ 10 ] ;
 if ( three_char_mon <= 0x4A756C ) {
 if ( three_char_mon <= 0x446563 ) {
 if ( three_char_mon == 0x417072 ) {
 tp -> tm_mon = 3 ;
 }
 else if ( three_char_mon == 0x417567 ) {
 tp -> tm_mon = 7 ;
 }
 else if ( three_char_mon == 0x446563 ) {
 tp -> tm_mon = 11 ;
 }
 }
 else {
 if ( three_char_mon == 0x466562 ) {
 tp -> tm_mon = 1 ;
 }
 else if ( three_char_mon == 0x4A616E ) {
 tp -> tm_mon = 0 ;
 }
 else if ( three_char_mon == 0x4A756C ) {
 tp -> tm_mon = 6 ;
 }
 }
 }
 else {
 if ( three_char_mon <= 0x4D6179 ) {
 if ( three_char_mon == 0x4A756E ) {
 tp -> tm_mon = 5 ;
 }
 else if ( three_char_mon == 0x4D6172 ) {
 tp -> tm_mon = 2 ;
 }
 else if ( three_char_mon == 0x4D6179 ) {
 tp -> tm_mon = 4 ;
 }
 }
 else {
 if ( three_char_mon == 0x4E6F76 ) {
 tp -> tm_mon = 10 ;
 }
 else if ( three_char_mon == 0x4F6374 ) {
 tp -> tm_mon = 9 ;
 }
 else if ( three_char_mon == 0x536570 ) {
 tp -> tm_mon = 8 ;
 }
 }
 }
 if ( tp -> tm_mon < 0 ) {
 tp -> tm_mon = month_names_dfa -> match ( buf , length ) ;
 if ( tp -> tm_mon < 0 ) {
 return 0 ;
 }
 }
 tp -> tm_year = ( ( buf [ 12 ] - '0' ) * 1000 + ( buf [ 13 ] - '0' ) * 100 + ( buf [ 14 ] - '0' ) * 10 + ( buf [ 15 ] - '0' ) ) - 1900 ;
 tp -> tm_hour = ( buf [ 17 ] - '0' ) * 10 + ( buf [ 18 ] - '0' ) ;
 tp -> tm_min = ( buf [ 20 ] - '0' ) * 10 + ( buf [ 21 ] - '0' ) ;
 tp -> tm_sec = ( buf [ 23 ] - '0' ) * 10 + ( buf [ 24 ] - '0' ) ;
 if ( ( buf [ 19 ] != ':' ) || ( buf [ 22 ] != ':' ) ) {
 return 0 ;
 }
 return 1 ;
 }