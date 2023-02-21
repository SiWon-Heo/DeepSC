void mime_field_init ( MIMEField * field ) {
 memset ( field , 0 , sizeof ( MIMEField ) ) ;
 field -> m_readiness = MIME_FIELD_SLOT_READINESS_DETACHED ;
 field -> m_wks_idx = - 1 ;
 }