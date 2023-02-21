static inline void relocate ( MIMEField * field , MIMEFieldBlockImpl * dest_block , MIMEFieldBlockImpl * src_block ) {
 for ( ;
 src_block ;
 src_block = src_block -> m_next , dest_block = dest_block -> m_next ) {
 ink_release_assert ( dest_block ) ;
 if ( field -> m_next_dup >= src_block -> m_field_slots && field -> m_next_dup < src_block -> m_field_slots + src_block -> m_freetop ) {
 field -> m_next_dup = rebase ( field -> m_next_dup , dest_block -> m_field_slots , src_block -> m_field_slots ) ;
 return ;
 }
 }
 }