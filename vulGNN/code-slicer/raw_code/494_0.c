MIMEField * mime_field_create_named ( HdrHeap * heap , MIMEHdrImpl * mh , const char * name , int length ) {
 MIMEField * field = mime_field_create ( heap , mh ) ;
 int field_name_wks_idx = hdrtoken_tokenize ( name , length ) ;
 mime_field_name_set ( heap , mh , field , field_name_wks_idx , name , length , true ) ;
 return field ;
 }