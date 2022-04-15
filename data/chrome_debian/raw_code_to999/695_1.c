WRITE_CLASS_ENCODER ( CephXServiceTicketRequest ) struct CephXAuthorizeReply {
 uint64_t nonce_plus_one ;
 void encode ( bufferlist & bl ) const {
 __u8 struct_v = 1 ;
 : : encode ( struct_v , bl ) ;
 : : encode ( nonce_plus_one , bl ) ;
 }
 void decode ( bufferlist : : iterator & bl ) {
 __u8 struct_v ;
 : : decode ( struct_v , bl ) ;
 : : decode ( nonce_plus_one , bl ) ;
 }
 }
 ;
 WRITE_CLASS_ENCODER ( CephXAuthorizeReply ) struct CephXAuthorizer : public AuthAuthorizer {
 private : CephContext * cct ;
 public : uint64_t nonce ;
 explicit CephXAuthorizer ( CephContext * cct_ ) : AuthAuthorizer ( CEPH_AUTH_CEPHX ) , cct ( cct_ ) , nonce ( 0 ) {
 }
 bool build_authorizer ( ) ;
 bool verify_reply ( bufferlist : : iterator & reply ) override ;
 }