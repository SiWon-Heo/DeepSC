const char * mime_field_value_str_from_strlist ( HdrHeap * heap , int * new_str_len_return , StrList * list ) {
 Str * cell ;
 char * new_value , * dest ;
 int i , new_value_len ;
 HdrHeap : : HeapGuard guard ( heap , list -> head -> str ) ;
 new_value_len = 0 ;
 cell = list -> head ;
 for ( i = 0 ;
 i < list -> count ;
 i ++ ) {
 new_value_len += cell -> len ;
 cell = cell -> next ;
 }
 if ( list -> count > 1 ) {
 new_value_len += ( 2 * ( list -> count - 1 ) ) ;
 }
 new_value = heap -> allocate_str ( new_value_len ) ;
 dest = new_value ;
 cell = list -> head ;
 for ( i = 0 ;
 i < list -> count ;
 i ++ ) {
 if ( i != 0 ) {
 * dest ++ = ',' ;
 * dest ++ = ' ' ;
 }
 memcpy ( dest , cell -> str , cell -> len ) ;
 dest += cell -> len ;
 cell = cell -> next ;
 }
 ink_assert ( dest - new_value == new_value_len ) ;
 * new_str_len_return = new_value_len ;
 return new_value ;
 }