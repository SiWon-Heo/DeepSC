time_t mime_parse_date ( const char * buf , const char * end ) {
 static const int DAYS_OFFSET = 25508 ;
 static const int days [ 12 ] = {
 305 , 336 , - 1 , 30 , 60 , 91 , 121 , 152 , 183 , 213 , 244 , 274 }
 ;
 struct tm tp ;
 time_t t ;
 int year ;
 int month ;
 int mday ;
 if ( ! buf ) {
 return ( time_t ) 0 ;
 }
 while ( ( buf != end ) && is_ws ( * buf ) ) {
 buf += 1 ;
 }
 if ( ( buf != end ) && is_digit ( * buf ) ) {
 if ( ! mime_parse_mday ( buf , end , & tp . tm_mday ) ) {
 return ( time_t ) 0 ;
 }
 if ( ! mime_parse_month ( buf , end , & tp . tm_mon ) ) {
 return ( time_t ) 0 ;
 }
 if ( ! mime_parse_year ( buf , end , & tp . tm_year ) ) {
 return ( time_t ) 0 ;
 }
 if ( ! mime_parse_time ( buf , end , & tp . tm_hour , & tp . tm_min , & tp . tm_sec ) ) {
 return ( time_t ) 0 ;
 }
 }
 else if ( end && ( end - buf >= 29 ) && ( buf [ 3 ] == ',' ) ) {
 if ( ! mime_parse_rfc822_date_fastcase ( buf , end - buf , & tp ) ) {
 return ( time_t ) 0 ;
 }
 }
 else {
 if ( ! mime_parse_day ( buf , end , & tp . tm_wday ) ) {
 return ( time_t ) 0 ;
 }
 while ( ( buf != end ) && is_ws ( * buf ) ) {
 buf += 1 ;
 }
 if ( ( buf != end ) && ( ( * buf == ',' ) || is_digit ( * buf ) ) ) {
 if ( ! mime_parse_mday ( buf , end , & tp . tm_mday ) ) {
 return ( time_t ) 0 ;
 }
 if ( ! mime_parse_month ( buf , end , & tp . tm_mon ) ) {
 return ( time_t ) 0 ;
 }
 if ( ! mime_parse_year ( buf , end , & tp . tm_year ) ) {
 return ( time_t ) 0 ;
 }
 if ( ! mime_parse_time ( buf , end , & tp . tm_hour , & tp . tm_min , & tp . tm_sec ) ) {
 return ( time_t ) 0 ;
 }
 }
 else {
 if ( ! mime_parse_month ( buf , end , & tp . tm_mon ) ) {
 return ( time_t ) 0 ;
 }
 if ( ! mime_parse_mday ( buf , end , & tp . tm_mday ) ) {
 return ( time_t ) 0 ;
 }
 if ( ! mime_parse_time ( buf , end , & tp . tm_hour , & tp . tm_min , & tp . tm_sec ) ) {
 return ( time_t ) 0 ;
 }
 if ( ! mime_parse_year ( buf , end , & tp . tm_year ) ) {
 return ( time_t ) 0 ;
 }
 }
 }
 year = tp . tm_year ;
 month = tp . tm_mon ;
 mday = tp . tm_mday ;
 if ( year > 137 ) {
 return ( time_t ) INT_MAX ;
 }
 if ( year < 70 ) {
 return ( time_t ) 0 ;
 }
 mday += days [ month ] ;
 if ( month < 2 ) {
 year -= 1 ;
 }
 mday += ( year * 365 ) + ( year / 4 ) - ( year / 100 ) + ( year / 100 + 3 ) / 4 ;
 mday -= DAYS_OFFSET ;
 t = ( ( mday * 24 + tp . tm_hour ) * 60 + tp . tm_min ) * 60 + tp . tm_sec ;
 return t ;
 }