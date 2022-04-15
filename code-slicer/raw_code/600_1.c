static int find_unused_picture ( MpegEncContext * s , int shared ) {
 int i ;
 if ( shared ) {
 for ( i = s -> picture_range_start ;
 i < s -> picture_range_end ;
 i ++ ) {
 if ( s -> picture [ i ] . f . data [ 0 ] == NULL && s -> picture [ i ] . f . type == 0 ) return i ;
 }
 }
 else {
 for ( i = s -> picture_range_start ;
 i < s -> picture_range_end ;
 i ++ ) {
 if ( pic_is_unused ( s , & s -> picture [ i ] ) && s -> picture [ i ] . f . type != 0 ) return i ;
 }
 for ( i = s -> picture_range_start ;
 i < s -> picture_range_end ;
 i ++ ) {
 if ( pic_is_unused ( s , & s -> picture [ i ] ) ) return i ;
 }
 }
 return AVERROR_INVALIDDATA ;
 }