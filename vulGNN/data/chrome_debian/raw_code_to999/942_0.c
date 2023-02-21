int wpa_check_MIC ( struct eapol_header * eapol , struct eapol_key_header * eapol_key , size_t eapol_len , u_char * kck , int algo ) {
 u_char mic [ WPA_MICKEY_LEN ] ;
 u_int len ;
 u_char hmac_mic [ 20 ] ;
 memcpy ( mic , eapol_key -> key_MIC , WPA_MICKEY_LEN ) ;
 memset ( eapol_key -> key_MIC , 0 , WPA_MICKEY_LEN ) ;
 if ( algo == WPA_KEY_TKIP ) {
 HMAC ( EVP_md5 ( ) , kck , WPA_KCK_LEN , ( u_char * ) eapol , eapol_len , hmac_mic , & len ) ;
 }
 else if ( algo == WPA_KEY_CCMP ) {
 HMAC ( EVP_sha1 ( ) , kck , WPA_KCK_LEN , ( u_char * ) eapol , eapol_len , hmac_mic , & len ) ;
 }
 else return - E_INVALID ;
 memcpy ( eapol_key -> key_MIC , mic , WPA_MICKEY_LEN ) ;
 return memcmp ( mic , hmac_mic , WPA_MICKEY_LEN ) ;
 }