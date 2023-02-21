static struct object_list * * process_tree ( struct tree * tree , struct object_list * * p , struct name_path * path , const char * name ) {
 struct object * obj = & tree -> object ;
 struct tree_desc desc ;
 struct name_entry entry ;
 struct name_path me ;
 obj -> flags |= LOCAL ;
 if ( obj -> flags & ( UNINTERESTING | SEEN ) ) return p ;
 if ( parse_tree ( tree ) < 0 ) die ( "bad tree object %s" , oid_to_hex ( & obj -> oid ) ) ;
 obj -> flags |= SEEN ;
 name = xstrdup ( name ) ;
 p = add_one_object ( obj , p ) ;
 me . up = path ;
 me . elem = name ;
 me . elem_len = strlen ( name ) ;
 init_tree_desc ( & desc , tree -> buffer , tree -> size ) ;
 while ( tree_entry ( & desc , & entry ) ) switch ( object_type ( entry . mode ) ) {
 case OBJ_TREE : p = process_tree ( lookup_tree ( entry . sha1 ) , p , & me , name ) ;
 break ;
 case OBJ_BLOB : p = process_blob ( lookup_blob ( entry . sha1 ) , p , & me , name ) ;
 break ;
 default : break ;
 }
 free_tree_buffer ( tree ) ;
 return p ;
 }