int mime_field_presence_get ( MIMEHdrImpl * h , int well_known_str_index ) {
 const char * wks = hdrtoken_index_to_wks ( well_known_str_index ) ;
 return mime_field_presence_get ( h , wks ) ;
 }