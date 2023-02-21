MIMEHdrImpl * mime_hdr_create ( HdrHeap * heap ) {
 MIMEHdrImpl * mh ;
 mh = ( MIMEHdrImpl * ) heap -> allocate_obj ( sizeof ( MIMEHdrImpl ) , HDR_HEAP_OBJ_MIME_HEADER ) ;
 mime_hdr_init ( mh ) ;
 return mh ;
 }