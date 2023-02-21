kadm5_ret_t kadm5_delete_policy ( void * server_handle , kadm5_policy_t name ) {
 kadm5_server_handle_t handle = server_handle ;
 osa_policy_ent_t entry ;
 int ret ;
 CHECK_HANDLE ( server_handle ) ;
 krb5_clear_error_message ( handle -> context ) ;
 if ( name == ( kadm5_policy_t ) NULL ) return EINVAL ;
 if ( strlen ( name ) == 0 ) return KADM5_BAD_POLICY ;
 ret = krb5_db_get_policy ( handle -> context , name , & entry ) ;
 if ( ret == KRB5_KDB_NOENTRY ) return KADM5_UNK_POLICY ;
 else if ( ret ) return ret ;
 if ( entry -> policy_refcnt != 0 ) {
 krb5_db_free_policy ( handle -> context , entry ) ;
 return KADM5_POLICY_REF ;
 }
 krb5_db_free_policy ( handle -> context , entry ) ;
 ret = krb5_db_delete_policy ( handle -> context , name ) ;
 if ( ret == KRB5_KDB_POLICY_REF ) ret = KADM5_POLICY_REF ;
 return ( ret == 0 ) ? KADM5_OK : ret ;
 }