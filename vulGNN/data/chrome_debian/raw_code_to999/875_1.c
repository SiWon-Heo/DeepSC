void kadmin_getprinc ( int argc , char * argv [ ] ) {
 kadm5_principal_ent_rec dprinc ;
 krb5_principal princ = NULL ;
 krb5_error_code retval ;
 char * canon = NULL , * princstr = NULL , * modprincstr = NULL ;
 int i ;
 size_t j ;
 if ( ! ( argc == 2 || ( argc == 3 && ! strcmp ( "-terse" , argv [ 1 ] ) ) ) ) {
 fprintf ( stderr , _ ( "usage: get_principal [-terse] principal\n" ) ) ;
 return ;
 }
 memset ( & dprinc , 0 , sizeof ( dprinc ) ) ;
 retval = kadmin_parse_name ( argv [ argc - 1 ] , & princ ) ;
 if ( retval ) {
 com_err ( "get_principal" , retval , _ ( "while parsing principal" ) ) ;
 return ;
 }
 retval = krb5_unparse_name ( context , princ , & canon ) ;
 if ( retval ) {
 com_err ( "get_principal" , retval , _ ( "while canonicalizing principal" ) ) ;
 goto cleanup ;
 }
 retval = kadm5_get_principal ( handle , princ , & dprinc , KADM5_PRINCIPAL_NORMAL_MASK | KADM5_KEY_DATA ) ;
 if ( retval ) {
 com_err ( "get_principal" , retval , _ ( "while retrieving \"%s\"." ) , canon ) ;
 goto cleanup ;
 }
 retval = krb5_unparse_name ( context , dprinc . principal , & princstr ) ;
 if ( retval ) {
 com_err ( "get_principal" , retval , _ ( "while unparsing principal" ) ) ;
 goto cleanup ;
 }
 retval = krb5_unparse_name ( context , dprinc . mod_name , & modprincstr ) ;
 if ( retval ) {
 com_err ( "get_principal" , retval , _ ( "while unparsing principal" ) ) ;
 goto cleanup ;
 }
 if ( argc == 2 ) {
 printf ( _ ( "Principal: %s\n" ) , princstr ) ;
 printf ( _ ( "Expiration date: %s\n" ) , dprinc . princ_expire_time ? strdate ( dprinc . princ_expire_time ) : _ ( "[never]" ) ) ;
 printf ( _ ( "Last password change: %s\n" ) , dprinc . last_pwd_change ? strdate ( dprinc . last_pwd_change ) : _ ( "[never]" ) ) ;
 printf ( _ ( "Password expiration date: %s\n" ) , dprinc . pw_expiration ? strdate ( dprinc . pw_expiration ) : _ ( "[none]" ) ) ;
 printf ( _ ( "Maximum ticket life: %s\n" ) , strdur ( dprinc . max_life ) ) ;
 printf ( _ ( "Maximum renewable life: %s\n" ) , strdur ( dprinc . max_renewable_life ) ) ;
 printf ( _ ( "Last modified: %s (%s)\n" ) , strdate ( dprinc . mod_date ) , modprincstr ) ;
 printf ( _ ( "Last successful authentication: %s\n" ) , dprinc . last_success ? strdate ( dprinc . last_success ) : _ ( "[never]" ) ) ;
 printf ( "Last failed authentication: %s\n" , dprinc . last_failed ? strdate ( dprinc . last_failed ) : "[never]" ) ;
 printf ( _ ( "Failed password attempts: %d\n" ) , dprinc . fail_auth_count ) ;
 printf ( _ ( "Number of keys: %d\n" ) , dprinc . n_key_data ) ;
 for ( i = 0 ;
 i < dprinc . n_key_data ;
 i ++ ) {
 krb5_key_data * key_data = & dprinc . key_data [ i ] ;
 char enctype [ BUFSIZ ] , salttype [ BUFSIZ ] ;
 if ( krb5_enctype_to_name ( key_data -> key_data_type [ 0 ] , FALSE , enctype , sizeof ( enctype ) ) ) snprintf ( enctype , sizeof ( enctype ) , _ ( "<Encryption type 0x%x>" ) , key_data -> key_data_type [ 0 ] ) ;
 printf ( "Key: vno %d, %s, " , key_data -> key_data_kvno , enctype ) ;
 if ( key_data -> key_data_ver > 1 ) {
 if ( krb5_salttype_to_string ( key_data -> key_data_type [ 1 ] , salttype , sizeof ( salttype ) ) ) snprintf ( salttype , sizeof ( salttype ) , _ ( "<Salt type 0x%x>" ) , key_data -> key_data_type [ 1 ] ) ;
 printf ( "%s\n" , salttype ) ;
 }
 else printf ( _ ( "no salt\n" ) ) ;
 }
 printf ( _ ( "MKey: vno %d\n" ) , dprinc . mkvno ) ;
 printf ( _ ( "Attributes:" ) ) ;
 for ( j = 0 ;
 j < sizeof ( prflags ) / sizeof ( char * ) ;
 j ++ ) {
 if ( dprinc . attributes & ( krb5_flags ) 1 << j ) printf ( " %s" , prflags [ j ] ) ;
 }
 printf ( "\n" ) ;
 printf ( _ ( "Policy: %s\n" ) , dprinc . policy ? dprinc . policy : _ ( "[none]" ) ) ;
 }
 else {
 printf ( "\"%s\"\t%d\t%d\t%d\t%d\t\"%s\"\t%d\t%d\t%d\t%d\t\"%s\"" "\t%d\t%d\t%d\t%d\t%d" , princstr , dprinc . princ_expire_time , dprinc . last_pwd_change , dprinc . pw_expiration , dprinc . max_life , modprincstr , dprinc . mod_date , dprinc . attributes , dprinc . kvno , dprinc . mkvno , dprinc . policy ? dprinc . policy : "[none]" , dprinc . max_renewable_life , dprinc . last_success , dprinc . last_failed , dprinc . fail_auth_count , dprinc . n_key_data ) ;
 for ( i = 0 ;
 i < dprinc . n_key_data ;
 i ++ ) printf ( "\t%d\t%d\t%d\t%d" , dprinc . key_data [ i ] . key_data_ver , dprinc . key_data [ i ] . key_data_kvno , dprinc . key_data [ i ] . key_data_type [ 0 ] , dprinc . key_data [ i ] . key_data_type [ 1 ] ) ;
 printf ( "\n" ) ;
 }
 cleanup : krb5_free_principal ( context , princ ) ;
 kadm5_free_principal_ent ( handle , & dprinc ) ;
 free ( canon ) ;
 free ( princstr ) ;
 free ( modprincstr ) ;
 }