void _mime_field_block_destroy ( HdrHeap * heap , MIMEFieldBlockImpl * fblock ) {
 heap -> deallocate_obj ( fblock ) ;
 }