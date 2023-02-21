HTTPHdrImpl * http_hdr_create ( HdrHeap * heap , HTTPType polarity ) {
 HTTPHdrImpl * hh ;
 hh = ( HTTPHdrImpl * ) heap -> allocate_obj ( sizeof ( HTTPHdrImpl ) , HDR_HEAP_OBJ_HTTP_HEADER ) ;
 http_hdr_init ( heap , hh , polarity ) ;
 return ( hh ) ;
 }