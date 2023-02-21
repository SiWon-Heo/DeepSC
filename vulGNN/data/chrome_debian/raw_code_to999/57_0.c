static void copy_prev_row ( Jbig2Image * image , int row ) {
 if ( ! row ) {
 memset ( image -> data , 0 , image -> stride ) ;
 }
 else {
 uint8_t * src = image -> data + ( row - 1 ) * image -> stride ;
 memcpy ( src + image -> stride , src , image -> stride ) ;
 }
 }