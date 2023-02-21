int mime_field_length_get ( MIMEField * field ) {
 if ( field -> m_n_v_raw_printable ) {
 return ( field -> m_len_name + field -> m_len_value + field -> m_n_v_raw_printable_pad ) ;
 }
 else {
 return ( field -> m_len_name + field -> m_len_value + 4 ) ;
 }
 }