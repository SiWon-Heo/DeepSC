static krb5_error_code randkey_princ ( krb5_principal princ , krb5_boolean keepold , int n_ks , krb5_key_salt_tuple * ks ) {
 if ( keepold || ks ) {
 return kadm5_randkey_principal_3 ( handle , princ , keepold , n_ks , ks , NULL , NULL ) ;
 }
 else return kadm5_randkey_principal ( handle , princ , NULL , NULL ) ;
 }