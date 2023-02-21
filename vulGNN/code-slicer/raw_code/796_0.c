static void isoent_remove_child ( struct isoent * parent , struct isoent * child ) {
 struct isoent * ent ;
 ent = parent -> children . first ;
 while ( ent -> chnext != child ) ent = ent -> chnext ;
 if ( ( ent -> chnext = ent -> chnext -> chnext ) == NULL ) parent -> children . last = & ( ent -> chnext ) ;
 parent -> children . cnt -- ;
 if ( child -> dir ) {
 ent = parent -> subdirs . first ;
 while ( ent -> drnext != child ) ent = ent -> drnext ;
 if ( ( ent -> drnext = ent -> drnext -> drnext ) == NULL ) parent -> subdirs . last = & ( ent -> drnext ) ;
 parent -> subdirs . cnt -- ;
 }
 __archive_rb_tree_remove_node ( & ( parent -> rbtree ) , ( struct archive_rb_node * ) child ) ;
 }