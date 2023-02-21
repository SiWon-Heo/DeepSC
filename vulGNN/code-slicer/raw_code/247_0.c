static ossl_inline void lh_ ## type ## _node_usage_stats_bio ( const LHASH_OF ( type ) * lh , BIO * out ) {
 OPENSSL_LH_node_usage_stats_bio ( ( const OPENSSL_LHASH * ) lh , out ) ;
 }
 static ossl_inline void lh_ ## type ## _stats_bio ( const LHASH_OF ( type ) * lh , BIO * out ) {
 OPENSSL_LH_stats_bio ( ( const OPENSSL_LHASH * ) lh , out ) ;
 }
 static ossl_inline unsigned long lh_ ## type ## _get_down_load ( LHASH_OF ( type ) * lh ) {
 return OPENSSL_LH_get_down_load ( ( OPENSSL_LHASH * ) lh ) ;
 }
 static ossl_inline void lh_ ## type ## _set_down_load ( LHASH_OF ( type ) * lh , unsigned long dl ) {
 OPENSSL_LH_set_down_load ( ( OPENSSL_LHASH * ) lh , dl ) ;
 }
 static ossl_inline void lh_ ## type ## _doall ( LHASH_OF ( type ) * lh , void ( * doall ) ( type * ) ) {
 OPENSSL_LH_doall ( ( OPENSSL_LHASH * ) lh , ( OPENSSL_LH_DOALL_FUNC ) doall ) ;
 }
 LHASH_OF ( type ) # define IMPLEMENT_LHASH_DOALL_ARG_CONST ( type , argtype ) int_implement_lhash_doall ( type , argtype , const type ) # define IMPLEMENT_LHASH_DOALL_ARG ( type , argtype ) int_implement_lhash_doall ( type , argtype , type ) # define int_implement_lhash_doall ( type , argtype , cbargtype ) static ossl_inline void lh_ ## type ## _doall_ ## argtype ( LHASH_OF ( type ) * lh , void ( * fn ) ( cbargtype * , argtype * ) , argtype * arg ) {
 OPENSSL_LH_doall_arg ( ( OPENSSL_LHASH * ) lh , ( OPENSSL_LH_DOALL_FUNCARG ) fn , ( void * ) arg ) ;
 }
 LHASH_OF ( type ) DEFINE_LHASH_OF ( OPENSSL_STRING )