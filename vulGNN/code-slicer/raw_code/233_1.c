int set_wep_key ( char * string ) {
 int bit = 0 ;
 char * p , type ;
 char * tok ;
 char s [ strlen ( string ) + 1 ] ;
 u_char tmp_wkey [ 512 ] ;
 size_t tmp_wkey_len ;
 char tmp [ 128 ] ;
 memset ( GBL_WIFI -> wkey , 0 , sizeof ( GBL_WIFI -> wkey ) ) ;
 GBL_WIFI -> wkey_len = 0 ;
 strcpy ( s , string ) ;
 p = ec_strtok ( s , ":" , & tok ) ;
 if ( p == NULL ) SEMIFATAL_ERROR ( "Invalid parsing of the WEP key" ) ;
 bit = atoi ( p ) ;
 if ( bit <= 0 ) SEMIFATAL_ERROR ( "Unsupported WEP key length" ) ;
 tmp_wkey_len = bit / 8 - WEP_IV_LEN ;
 if ( bit != 64 && bit != 128 ) SEMIFATAL_ERROR ( "Unsupported WEP key length" ) ;
 p = ec_strtok ( NULL , ":" , & tok ) ;
 if ( p == NULL ) SEMIFATAL_ERROR ( "Invalid parsing of the WEP key" ) ;
 type = * p ;
 p = ec_strtok ( NULL , ":" , & tok ) ;
 if ( p == NULL ) SEMIFATAL_ERROR ( "Invalid parsing of the WEP key" ) ;
 if ( type == 's' ) {
 if ( strescape ( ( char * ) tmp_wkey , p ) != ( int ) tmp_wkey_len ) SEMIFATAL_ERROR ( "Specified WEP key length does not match the given string" ) ;
 }
 else if ( type == 'p' ) {
 if ( bit == 64 ) make_key_64 ( ( u_char * ) p , tmp_wkey ) ;
 else if ( bit == 128 ) make_key_128 ( ( u_char * ) p , tmp_wkey ) ;
 }
 else {
 SEMIFATAL_ERROR ( "Invalid parsing of the WEP key" ) ;
 }
 USER_MSG ( "Using WEP key: %s\n" , str_tohex ( tmp_wkey , tmp_wkey_len , tmp , sizeof ( tmp ) ) ) ;
 memcpy ( GBL_WIFI -> wkey , tmp_wkey , sizeof ( GBL_WIFI -> wkey ) ) ;
 GBL_WIFI -> wkey_len = tmp_wkey_len ;
 return E_SUCCESS ;
 }