MIMEFieldBlockImpl * _mime_field_block_copy ( MIMEFieldBlockImpl * s_fblock , HdrHeap * , HdrHeap * d_heap ) {
 MIMEFieldBlockImpl * d_fblock ;
 d_fblock = ( MIMEFieldBlockImpl * ) d_heap -> allocate_obj ( sizeof ( MIMEFieldBlockImpl ) , HDR_HEAP_OBJ_FIELD_BLOCK ) ;
 memcpy ( d_fblock , s_fblock , sizeof ( MIMEFieldBlockImpl ) ) ;
 return d_fblock ;
 }