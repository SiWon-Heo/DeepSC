int ff_find_unused_picture ( MpegEncContext * s , int shared ) {
 int ret = find_unused_picture ( s , shared ) ;
 if ( ret >= 0 && ret < s -> picture_range_end ) {
 if ( s -> picture [ ret ] . needs_realloc ) {
 s -> picture [ ret ] . needs_realloc = 0 ;
 free_picture ( s , & s -> picture [ ret ] ) ;
 avcodec_get_frame_defaults ( & s -> picture [ ret ] . f ) ;
 }
 }
 return ret ;
 }