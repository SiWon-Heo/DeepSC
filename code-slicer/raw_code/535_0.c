void mime_field_destroy ( MIMEHdrImpl * , MIMEField * field ) {
 ink_assert ( field -> m_readiness == MIME_FIELD_SLOT_READINESS_DETACHED ) ;
 field -> m_readiness = MIME_FIELD_SLOT_READINESS_DELETED ;
 }