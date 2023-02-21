void mime_hdr_presence_set ( MIMEHdrImpl * h , int well_known_str_index ) {
 const char * wks = hdrtoken_index_to_wks ( well_known_str_index ) ;
 mime_hdr_presence_set ( h , wks ) ;
 }