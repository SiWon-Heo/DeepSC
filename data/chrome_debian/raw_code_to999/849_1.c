static int get_delta ( struct rev_info * revs , struct remote_lock * lock ) {
 int i ;
 struct commit * commit ;
 struct object_list * * p = & objects ;
 int count = 0 ;
 while ( ( commit = get_revision ( revs ) ) != NULL ) {
 p = process_tree ( commit -> tree , p , NULL , "" ) ;
 commit -> object . flags |= LOCAL ;
 if ( ! ( commit -> object . flags & UNINTERESTING ) ) count += add_send_request ( & commit -> object , lock ) ;
 }
 for ( i = 0 ;
 i < revs -> pending . nr ;
 i ++ ) {
 struct object_array_entry * entry = revs -> pending . objects + i ;
 struct object * obj = entry -> item ;
 const char * name = entry -> name ;
 if ( obj -> flags & ( UNINTERESTING | SEEN ) ) continue ;
 if ( obj -> type == OBJ_TAG ) {
 obj -> flags |= SEEN ;
 p = add_one_object ( obj , p ) ;
 continue ;
 }
 if ( obj -> type == OBJ_TREE ) {
 p = process_tree ( ( struct tree * ) obj , p , NULL , name ) ;
 continue ;
 }
 if ( obj -> type == OBJ_BLOB ) {
 p = process_blob ( ( struct blob * ) obj , p , NULL , name ) ;
 continue ;
 }
 die ( "unknown pending object %s (%s)" , oid_to_hex ( & obj -> oid ) , name ) ;
 }
 while ( objects ) {
 if ( ! ( objects -> item -> flags & UNINTERESTING ) ) count += add_send_request ( objects -> item , lock ) ;
 objects = objects -> next ;
 }
 return count ;
 }