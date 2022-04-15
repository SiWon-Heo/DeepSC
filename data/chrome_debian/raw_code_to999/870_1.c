kadm5_ret_t kadm5_get_policy ( void * server_handle , kadm5_policy_t name , kadm5_policy_ent_t entry ) {
 osa_policy_ent_t t ;
 kadm5_ret_t ret ;
 kadm5_server_handle_t handle = server_handle ;
 memset ( entry , 0 , sizeof ( * entry ) ) ;
 CHECK_HANDLE ( server_handle ) ;
 krb5_clear_error_message ( handle -> context ) ;
 if ( name == ( kadm5_policy_t ) NULL ) return EINVAL ;
 if ( strlen ( name ) == 0 ) return KADM5_BAD_POLICY ;
 ret = krb5_db_get_policy ( handle -> context , name , & t ) ;
 if ( ret == KRB5_KDB_NOENTRY ) return KADM5_UNK_POLICY ;
 else if ( ret ) return ret ;
 if ( ( entry -> policy = strdup ( t -> name ) ) == NULL ) {
 ret = ENOMEM ;
 goto cleanup ;
 }
 entry -> pw_min_life = t -> pw_min_life ;
 entry -> pw_max_life = t -> pw_max_life ;
 entry -> pw_min_length = t -> pw_min_length ;
 entry -> pw_min_classes = t -> pw_min_classes ;
 entry -> pw_history_num = t -> pw_history_num ;
 entry -> policy_refcnt = t -> policy_refcnt ;
 if ( handle -> api_version >= KADM5_API_VERSION_3 ) {
 entry -> pw_max_fail = t -> pw_max_fail ;
 entry -> pw_failcnt_interval = t -> pw_failcnt_interval ;
 entry -> pw_lockout_duration = t -> pw_lockout_duration ;
 }
 if ( handle -> api_version >= KADM5_API_VERSION_4 ) {
 entry -> attributes = t -> attributes ;
 entry -> max_life = t -> max_life ;
 entry -> max_renewable_life = t -> max_renewable_life ;
 if ( t -> allowed_keysalts ) {
 entry -> allowed_keysalts = strdup ( t -> allowed_keysalts ) ;
 if ( ! entry -> allowed_keysalts ) {
 ret = ENOMEM ;
 goto cleanup ;
 }
 }
 ret = copy_tl_data ( t -> n_tl_data , t -> tl_data , & entry -> tl_data ) ;
 if ( ret ) goto cleanup ;
 entry -> n_tl_data = t -> n_tl_data ;
 }
 ret = 0 ;
 cleanup : if ( ret ) kadm5_free_policy_ent ( handle , entry ) ;
 krb5_db_free_policy ( handle -> context , t ) ;
 return ret ;
 }