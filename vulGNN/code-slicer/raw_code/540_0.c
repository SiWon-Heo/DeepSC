void mime_days_since_epoch_to_mdy ( unsigned int days_since_jan_1_1970 , int * m_return , int * d_return , int * y_return ) {
 ink_assert ( _days_to_mdy_fast_lookup_table != nullptr ) ;
 if ( ( days_since_jan_1_1970 >= _days_to_mdy_fast_lookup_table_first_day ) && ( days_since_jan_1_1970 <= _days_to_mdy_fast_lookup_table_last_day ) ) {
 int i = days_since_jan_1_1970 - _days_to_mdy_fast_lookup_table_first_day ;
 * m_return = _days_to_mdy_fast_lookup_table [ i ] . m ;
 * d_return = _days_to_mdy_fast_lookup_table [ i ] . d ;
 * y_return = _days_to_mdy_fast_lookup_table [ i ] . y ;
 return ;
 }
 mime_days_since_epoch_to_mdy_slowcase ( days_since_jan_1_1970 , m_return , d_return , y_return ) ;
 }