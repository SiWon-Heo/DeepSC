int mime_format_date ( char * buffer , time_t value ) {
 static const char * daystrs [ ] = {
 "Sun" , "Mon" , "Tue" , "Wed" , "Thu" , "Fri" , "Sat" }
 ;
 static const char * monthstrs [ ] = {
 "Jan" , "Feb" , "Mar" , "Apr" , "May" , "Jun" , "Jul" , "Aug" , "Sep" , "Oct" , "Nov" , "Dec" }
 ;
 static const char * digitstrs [ ] = {
 "00" , "01" , "02" , "03" , "04" , "05" , "06" , "07" , "08" , "09" , "10" , "11" , "12" , "13" , "14" , "15" , "16" , "17" , "18" , "19" , "20" , "21" , "22" , "23" , "24" , "25" , "26" , "27" , "28" , "29" , "30" , "31" , "32" , "33" , "34" , "35" , "36" , "37" , "38" , "39" , "40" , "41" , "42" , "43" , "44" , "45" , "46" , "47" , "48" , "49" , "50" , "51" , "52" , "53" , "54" , "55" , "56" , "57" , "58" , "59" , "60" , "61" , "62" , "63" , "64" , "65" , "66" , "67" , "68" , "69" , "70" , "71" , "72" , "73" , "74" , "75" , "76" , "77" , "78" , "79" , "80" , "81" , "82" , "83" , "84" , "85" , "86" , "87" , "88" , "89" , "90" , "91" , "92" , "93" , "94" , "95" , "96" , "97" , "98" , "99" , }
 ;
 char * buf ;
 int sec , min , hour , wday , mday = 0 , year = 0 , month = 0 ;
 buf = buffer ;
 sec = ( int ) ( value % 60 ) ;
 value /= 60 ;
 min = ( int ) ( value % 60 ) ;
 value /= 60 ;
 hour = ( int ) ( value % 24 ) ;
 value /= 24 ;
 wday = ( int ) ( ( 4 + value ) % 7 ) ;
 # if MIME_FORMAT_DATE_USE_LOOKUP_TABLE mime_days_since_epoch_to_mdy ( value , & month , & mday , & year ) ;
 # else mime_days_since_epoch_to_mdy_slowcase ( value , & month , & mday , & year ) ;
 # endif ink_assert ( ( mday >= 0 ) && ( mday <= 99 ) ) ;
 ink_assert ( ( hour >= 0 ) && ( hour <= 99 ) ) ;
 ink_assert ( ( min >= 0 ) && ( min <= 99 ) ) ;
 ink_assert ( ( sec >= 0 ) && ( sec <= 99 ) ) ;
 const char * three_char_day = daystrs [ wday ] ;
 buf [ 0 ] = three_char_day [ 0 ] ;
 buf [ 1 ] = three_char_day [ 1 ] ;
 buf [ 2 ] = three_char_day [ 2 ] ;
 buf += 3 ;
 buf [ 0 ] = ',' ;
 buf [ 1 ] = ' ' ;
 buf += 2 ;
 buf [ 0 ] = digitstrs [ mday ] [ 0 ] ;
 buf [ 1 ] = digitstrs [ mday ] [ 1 ] ;
 buf [ 2 ] = ' ' ;
 buf += 3 ;
 const char * three_char_month = monthstrs [ month ] ;
 buf [ 0 ] = three_char_month [ 0 ] ;
 buf [ 1 ] = three_char_month [ 1 ] ;
 buf [ 2 ] = three_char_month [ 2 ] ;
 buf += 3 ;
 buf [ 0 ] = ' ' ;
 if ( ( year >= 2000 ) && ( year <= 2009 ) ) {
 buf [ 1 ] = '2' ;
 buf [ 2 ] = '0' ;
 buf [ 3 ] = '0' ;
 buf [ 4 ] = ( year - 2000 ) + '0' ;
 }
 else if ( ( year >= 1990 ) && ( year <= 1999 ) ) {
 buf [ 1 ] = '1' ;
 buf [ 2 ] = '9' ;
 buf [ 3 ] = '9' ;
 buf [ 4 ] = ( year - 1990 ) + '0' ;
 }
 else {
 buf [ 4 ] = ( year % 10 ) + '0' ;
 year /= 10 ;
 buf [ 3 ] = ( year % 10 ) + '0' ;
 year /= 10 ;
 buf [ 2 ] = ( year % 10 ) + '0' ;
 year /= 10 ;
 buf [ 1 ] = ( year % 10 ) + '0' ;
 }
 buf [ 5 ] = ' ' ;
 buf += 6 ;
 buf [ 0 ] = digitstrs [ hour ] [ 0 ] ;
 buf [ 1 ] = digitstrs [ hour ] [ 1 ] ;
 buf [ 2 ] = ':' ;
 buf += 3 ;
 buf [ 0 ] = digitstrs [ min ] [ 0 ] ;
 buf [ 1 ] = digitstrs [ min ] [ 1 ] ;
 buf [ 2 ] = ':' ;
 buf += 3 ;
 buf [ 0 ] = digitstrs [ sec ] [ 0 ] ;
 buf [ 1 ] = digitstrs [ sec ] [ 1 ] ;
 buf [ 2 ] = ' ' ;
 buf += 3 ;
 buf [ 0 ] = 'G' ;
 buf [ 1 ] = 'M' ;
 buf [ 2 ] = 'T' ;
 buf [ 3 ] = '\0' ;
 buf += 3 ;
 return buf - buffer ;
 }