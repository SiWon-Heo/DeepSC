void mime_init_date_format_table ( ) {
 time_t now_secs ;
 int i , now_days , first_days , last_days , num_days ;
 int m = 0 , d = 0 , y = 0 ;
 time ( & now_secs ) ;
 now_days = ( int ) ( now_secs / ( 60 * 60 * 24 ) ) ;
 first_days = now_days - 366 ;
 last_days = now_days + 366 ;
 num_days = last_days - first_days + 1 ;
 _days_to_mdy_fast_lookup_table = ( MDY * ) ats_malloc ( num_days * sizeof ( MDY ) ) ;
 _days_to_mdy_fast_lookup_table_first_day = first_days ;
 _days_to_mdy_fast_lookup_table_last_day = last_days ;
 for ( i = 0 ;
 i < num_days ;
 i ++ ) {
 mime_days_since_epoch_to_mdy_slowcase ( first_days + i , & m , & d , & y ) ;
 _days_to_mdy_fast_lookup_table [ i ] . m = m ;
 _days_to_mdy_fast_lookup_table [ i ] . d = d ;
 _days_to_mdy_fast_lookup_table [ i ] . y = y ;
 }
 }