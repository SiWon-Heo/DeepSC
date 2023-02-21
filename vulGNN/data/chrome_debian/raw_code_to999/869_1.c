kadm5_ret_t kadm5_create_policy_internal ( void * server_handle , kadm5_policy_ent_t entry , long mask ) {
 kadm5_server_handle_t handle = server_handle ;
 osa_policy_ent_rec pent ;
 int ret ;
 char * p ;
 CHECK_HANDLE ( server_handle ) ;
 if ( ( entry == ( kadm5_policy_ent_t ) NULL ) || ( entry -> policy == NULL ) ) return EINVAL ;
 if ( strlen ( entry -> policy ) == 0 ) return KADM5_BAD_POLICY ;
 if ( ! ( mask & KADM5_POLICY ) ) return KADM5_BAD_MASK ;
 if ( ( mask & KADM5_POLICY_ALLOWED_KEYSALTS ) && entry -> allowed_keysalts != NULL ) {
 ret = validate_allowed_keysalts ( entry -> allowed_keysalts ) ;
 if ( ret ) return ret ;
 }
 memset ( & pent , 0 , sizeof ( pent ) ) ;
 pent . name = entry -> policy ;
 p = entry -> policy ;
 while ( * p != '\0' ) {
 if ( * p < ' ' || * p > '~' ) return KADM5_BAD_POLICY ;
 else p ++ ;
 }
 if ( ! ( mask & KADM5_PW_MAX_LIFE ) ) pent . pw_max_life = 0 ;
 else pent . pw_max_life = entry -> pw_max_life ;
 if ( ! ( mask & KADM5_PW_MIN_LIFE ) ) pent . pw_min_life = 0 ;
 else {
 if ( ( mask & KADM5_PW_MAX_LIFE ) ) {
 if ( entry -> pw_min_life > entry -> pw_max_life && entry -> pw_max_life != 0 ) return KADM5_BAD_MIN_PASS_LIFE ;
 }
 pent . pw_min_life = entry -> pw_min_life ;
 }
 if ( ! ( mask & KADM5_PW_MIN_LENGTH ) ) pent . pw_min_length = MIN_PW_LENGTH ;
 else {
 if ( entry -> pw_min_length < MIN_PW_LENGTH ) return KADM5_BAD_LENGTH ;
 pent . pw_min_length = entry -> pw_min_length ;
 }
 if ( ! ( mask & KADM5_PW_MIN_CLASSES ) ) pent . pw_min_classes = MIN_PW_CLASSES ;
 else {
 if ( entry -> pw_min_classes > MAX_PW_CLASSES || entry -> pw_min_classes < MIN_PW_CLASSES ) return KADM5_BAD_CLASS ;
 pent . pw_min_classes = entry -> pw_min_classes ;
 }
 if ( ! ( mask & KADM5_PW_HISTORY_NUM ) ) pent . pw_history_num = MIN_PW_HISTORY ;
 else {
 if ( entry -> pw_history_num < MIN_PW_HISTORY ) return KADM5_BAD_HISTORY ;
 else pent . pw_history_num = entry -> pw_history_num ;
 }
 if ( ! ( mask & KADM5_REF_COUNT ) ) pent . policy_refcnt = 0 ;
 else pent . policy_refcnt = entry -> policy_refcnt ;
 if ( handle -> api_version >= KADM5_API_VERSION_4 ) {
 if ( ! ( mask & KADM5_POLICY_ATTRIBUTES ) ) pent . attributes = 0 ;
 else pent . attributes = entry -> attributes ;
 if ( ! ( mask & KADM5_POLICY_MAX_LIFE ) ) pent . max_life = 0 ;
 else pent . max_life = entry -> max_life ;
 if ( ! ( mask & KADM5_POLICY_MAX_RLIFE ) ) pent . max_renewable_life = 0 ;
 else pent . max_renewable_life = entry -> max_renewable_life ;
 if ( ! ( mask & KADM5_POLICY_ALLOWED_KEYSALTS ) ) pent . allowed_keysalts = 0 ;
 else pent . allowed_keysalts = entry -> allowed_keysalts ;
 if ( ! ( mask & KADM5_POLICY_TL_DATA ) ) {
 pent . n_tl_data = 0 ;
 pent . tl_data = NULL ;
 }
 else {
 pent . n_tl_data = entry -> n_tl_data ;
 pent . tl_data = entry -> tl_data ;
 }
 }
 if ( handle -> api_version >= KADM5_API_VERSION_3 ) {
 if ( ! ( mask & KADM5_PW_MAX_FAILURE ) ) pent . pw_max_fail = 0 ;
 else pent . pw_max_fail = entry -> pw_max_fail ;
 if ( ! ( mask & KADM5_PW_FAILURE_COUNT_INTERVAL ) ) pent . pw_failcnt_interval = 0 ;
 else pent . pw_failcnt_interval = entry -> pw_failcnt_interval ;
 if ( ! ( mask & KADM5_PW_LOCKOUT_DURATION ) ) pent . pw_lockout_duration = 0 ;
 else pent . pw_lockout_duration = entry -> pw_lockout_duration ;
 }
 if ( ( ret = krb5_db_create_policy ( handle -> context , & pent ) ) ) return ret ;
 else return KADM5_OK ;
 }