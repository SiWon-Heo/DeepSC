WRITE_CLASS_ENCODER ( AuthCapsInfo ) struct AuthTicket {
 EntityName name ;
 uint64_t global_id ;
 uint64_t auid ;
 utime_t created , renew_after , expires ;
 AuthCapsInfo caps ;
 __u32 flags ;
 AuthTicket ( ) : global_id ( 0 ) , auid ( CEPH_AUTH_UID_DEFAULT ) , flags ( 0 ) {
 }
 void init_timestamps ( utime_t now , double ttl ) {
 created = now ;
 expires = now ;
 expires += ttl ;
 renew_after = now ;
 renew_after += ttl / 2.0 ;
 }
 void encode ( bufferlist & bl ) const {
 __u8 struct_v = 2 ;
 : : encode ( struct_v , bl ) ;
 : : encode ( name , bl ) ;
 : : encode ( global_id , bl ) ;
 : : encode ( auid , bl ) ;
 : : encode ( created , bl ) ;
 : : encode ( expires , bl ) ;
 : : encode ( caps , bl ) ;
 : : encode ( flags , bl ) ;
 }
 void decode ( bufferlist : : iterator & bl ) {
 __u8 struct_v ;
 : : decode ( struct_v , bl ) ;
 : : decode ( name , bl ) ;
 : : decode ( global_id , bl ) ;
 if ( struct_v >= 2 ) : : decode ( auid , bl ) ;
 else auid = CEPH_AUTH_UID_DEFAULT ;
 : : decode ( created , bl ) ;
 : : decode ( expires , bl ) ;
 : : decode ( caps , bl ) ;
 : : decode ( flags , bl ) ;
 }
 }
 ;
 WRITE_CLASS_ENCODER ( AuthTicket ) struct AuthAuthorizer {
 __u32 protocol ;
 bufferlist bl ;
 CryptoKey session_key ;
 explicit AuthAuthorizer ( __u32 p ) : protocol ( p ) {
 }
 virtual ~ AuthAuthorizer ( ) {
 }
 virtual bool verify_reply ( bufferlist : : iterator & reply ) = 0 ;
 }