void mime_field_value_set_int64 ( HdrHeap * heap , MIMEHdrImpl * mh , MIMEField * field , int64_t value ) {
 char buf [ 20 ] ;
 int len = mime_format_int64 ( buf , value , sizeof ( buf ) ) ;
 mime_field_value_set ( heap , mh , field , buf , len , true ) ;
 }