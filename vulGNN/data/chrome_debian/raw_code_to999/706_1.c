const mbfl_encoding * mbfl_identify_encoding ( mbfl_string * string , enum mbfl_no_encoding * elist , int elistsz , int strict ) {
 int i , n , num , bad ;
 unsigned char * p ;
 mbfl_identify_filter * flist , * filter ;
 const mbfl_encoding * encoding ;
 flist = ( mbfl_identify_filter * ) mbfl_calloc ( elistsz , sizeof ( mbfl_identify_filter ) ) ;
 if ( flist == NULL ) {
 return NULL ;
 }
 num = 0 ;
 if ( elist != NULL ) {
 for ( i = 0 ;
 i < elistsz ;
 i ++ ) {
 if ( ! mbfl_identify_filter_init ( & flist [ num ] , elist [ i ] ) ) {
 num ++ ;
 }
 }
 }
 n = string -> len ;
 p = string -> val ;
 if ( p != NULL ) {
 bad = 0 ;
 while ( n > 0 ) {
 for ( i = 0 ;
 i < num ;
 i ++ ) {
 filter = & flist [ i ] ;
 if ( ! filter -> flag ) {
 ( * filter -> filter_function ) ( * p , filter ) ;
 if ( filter -> flag ) {
 bad ++ ;
 }
 }
 }
 if ( ( num - 1 ) <= bad && ! strict ) {
 break ;
 }
 p ++ ;
 n -- ;
 }
 }
 encoding = NULL ;
 for ( i = 0 ;
 i < num ;
 i ++ ) {
 filter = & flist [ i ] ;
 if ( ! filter -> flag ) {
 if ( strict && filter -> status ) {
 continue ;
 }
 encoding = filter -> encoding ;
 break ;
 }
 }
 if ( ! encoding ) {
 for ( i = 0 ;
 i < num ;
 i ++ ) {
 filter = & flist [ i ] ;
 if ( ! filter -> flag && ( ! strict || ! filter -> status ) ) {
 encoding = filter -> encoding ;
 break ;
 }
 }
 }
 i = num ;
 while ( -- i >= 0 ) {
 mbfl_identify_filter_cleanup ( & flist [ i ] ) ;
 }
 mbfl_free ( ( void * ) flist ) ;
 return encoding ;
 }