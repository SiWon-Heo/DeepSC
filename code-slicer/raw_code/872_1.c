kadm5_ret_t kadm5_modify_policy_internal ( void * server_handle , kadm5_policy_ent_t entry , long mask ) {
 kadm5_server_handle_t handle = server_handle ;
 krb5_tl_data * tl ;
 osa_policy_ent_t p ;
 int ret ;
 size_t len ;
 CHECK_HANDLE ( server_handle ) ;
 if ( ( entry == ( kadm5_policy_ent_t ) NULL ) || ( entry -> policy == NULL ) ) return EINVAL ;
 if ( strlen ( entry -> policy ) == 0 ) return KADM5_BAD_POLICY ;
 if ( ( mask & KADM5_POLICY ) ) return KADM5_BAD_MASK ;
 if ( ( mask & KADM5_POLICY_ALLOWED_KEYSALTS ) && entry -> allowed_keysalts != NULL ) {
 ret = validate_allowed_keysalts ( entry -> allowed_keysalts ) ;
 if ( ret ) return ret ;
 }
 if ( ( mask & KADM5_POLICY_TL_DATA ) ) {
 tl = entry -> tl_data ;
 while ( tl != NULL ) {
 if ( tl -> tl_data_type < 256 ) return KADM5_BAD_TL_TYPE ;
 tl = tl -> tl_data_next ;
 }
 }
 ret = krb5_db_get_policy ( handle -> context , entry -> policy , & p ) ;
 if ( ret == KRB5_KDB_NOENTRY ) return KADM5_UNK_POLICY ;
 else if ( ret ) return ret ;
 if ( ( mask & KADM5_PW_MAX_LIFE ) ) p -> pw_max_life = entry -> pw_max_life ;
 if ( ( mask & KADM5_PW_MIN_LIFE ) ) {
 if ( entry -> pw_min_life > p -> pw_max_life && p -> pw_max_life != 0 ) {
 krb5_db_free_policy ( handle -> context , p ) ;
 return KADM5_BAD_MIN_PASS_LIFE ;
 }
 p -> pw_min_life = entry -> pw_min_life ;
 }
 if ( ( mask & KADM5_PW_MIN_LENGTH ) ) {
 if ( entry -> pw_min_length < MIN_PW_LENGTH ) {
 krb5_db_free_policy ( handle -> context , p ) ;
 return KADM5_BAD_LENGTH ;
 }
 p -> pw_min_length = entry -> pw_min_length ;
 }
 if ( ( mask & KADM5_PW_MIN_CLASSES ) ) {
 if ( entry -> pw_min_classes > MAX_PW_CLASSES || entry -> pw_min_classes < MIN_PW_CLASSES ) {
 krb5_db_free_policy ( handle -> context , p ) ;
 return KADM5_BAD_CLASS ;
 }
 p -> pw_min_classes = entry -> pw_min_classes ;
 }
 if ( ( mask & KADM5_PW_HISTORY_NUM ) ) {
 if ( entry -> pw_history_num < MIN_PW_HISTORY ) {
 krb5_db_free_policy ( handle -> context , p ) ;
 return KADM5_BAD_HISTORY ;
 }
 p -> pw_history_num = entry -> pw_history_num ;
 }
 if ( ( mask & KADM5_REF_COUNT ) ) p -> policy_refcnt = entry -> policy_refcnt ;
 if ( handle -> api_version >= KADM5_API_VERSION_3 ) {
 if ( ( mask & KADM5_PW_MAX_FAILURE ) ) p -> pw_max_fail = entry -> pw_max_fail ;
 if ( ( mask & KADM5_PW_FAILURE_COUNT_INTERVAL ) ) p -> pw_failcnt_interval = entry -> pw_failcnt_interval ;
 if ( ( mask & KADM5_PW_LOCKOUT_DURATION ) ) p -> pw_lockout_duration = entry -> pw_lockout_duration ;
 }
 if ( handle -> api_version >= KADM5_API_VERSION_4 ) {
 if ( ( mask & KADM5_POLICY_ATTRIBUTES ) ) p -> attributes = entry -> attributes ;
 if ( ( mask & KADM5_POLICY_MAX_LIFE ) ) p -> max_life = entry -> max_life ;
 if ( ( mask & KADM5_POLICY_MAX_RLIFE ) ) p -> max_renewable_life = entry -> max_renewable_life ;
 if ( ( mask & KADM5_POLICY_ALLOWED_KEYSALTS ) ) {
 krb5_db_free ( handle -> context , p -> allowed_keysalts ) ;
 p -> allowed_keysalts = NULL ;
 if ( entry -> allowed_keysalts != NULL ) {
 len = strlen ( entry -> allowed_keysalts ) + 1 ;
 p -> allowed_keysalts = krb5_db_alloc ( handle -> context , NULL , len ) ;
 if ( p -> allowed_keysalts == NULL ) {
 ret = ENOMEM ;
 goto cleanup ;
 }
 memcpy ( p -> allowed_keysalts , entry -> allowed_keysalts , len ) ;
 }
 }
 if ( ( mask & KADM5_POLICY_TL_DATA ) ) {
 for ( tl = entry -> tl_data ;
 tl != NULL ;
 tl = tl -> tl_data_next ) {
 ret = krb5_db_update_tl_data ( handle -> context , & p -> n_tl_data , & p -> tl_data , tl ) ;
 if ( ret ) goto cleanup ;
 }
 }
 }
 ret = krb5_db_put_policy ( handle -> context , p ) ;
 cleanup : krb5_db_free_policy ( handle -> context , p ) ;
 return ret ;
 }