MIMEField * mime_hdr_prepare_for_value_set ( HdrHeap * heap , MIMEHdrImpl * mh , const char * name , int name_length ) {
 int wks_idx ;
 MIMEField * field ;
 field = mime_hdr_field_find ( mh , name , name_length ) ;
 if ( field == nullptr ) {
 wks_idx = hdrtoken_tokenize ( name , name_length ) ;
 field = mime_field_create ( heap , mh ) ;
 mime_field_name_set ( heap , mh , field , wks_idx , name , name_length , true ) ;
 mime_hdr_field_attach ( mh , field , 0 , nullptr ) ;
 }
 else if ( field -> m_next_dup ) {
 wks_idx = field -> m_wks_idx ;
 mime_hdr_field_delete ( heap , mh , field , true ) ;
 field = mime_field_create ( heap , mh ) ;
 mime_field_name_set ( heap , mh , field , wks_idx , name , name_length , true ) ;
 mime_hdr_field_attach ( mh , field , 0 , nullptr ) ;
 }
 return field ;
 }