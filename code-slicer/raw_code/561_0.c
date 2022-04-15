void mime_hdr_field_block_list_adjust ( int , MIMEFieldBlockImpl * old_list , MIMEFieldBlockImpl * new_list ) {
 for ( MIMEFieldBlockImpl * new_blk = new_list ;
 new_blk ;
 new_blk = new_blk -> m_next ) {
 for ( MIMEField * field = new_blk -> m_field_slots , * end = field + new_blk -> m_freetop ;
 field != end ;
 ++ field ) {
 if ( field -> is_live ( ) && field -> m_next_dup ) {
 relocate ( field , new_list , old_list ) ;
 }
 }
 }
 }