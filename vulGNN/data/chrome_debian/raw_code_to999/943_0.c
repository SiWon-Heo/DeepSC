int wpa_decrypt_broadcast_key ( struct eapol_key_header * eapol_key , struct rsn_ie_header * rsn_ie , struct wpa_sa * sa ) {
 u_int8 * encrypted_key ;
 u_int16 key_len = 0 ;
 char tmp [ 512 ] ;
 ( void ) rsn_ie ;
 if ( sa -> algo == WPA_KEY_TKIP ) {
 key_len = ntohs ( eapol_key -> key_len ) ;
 }
 else if ( sa -> algo == WPA_KEY_CCMP ) {
 key_len = ntohs ( eapol_key -> key_data_len ) ;
 }
 if ( key_len > sizeof ( struct rsn_ie_header ) || key_len == 0 ) return - E_NOTHANDLED ;
 SAFE_CALLOC ( encrypted_key , key_len , sizeof ( u_int8 ) ) ;
 DEBUG_MSG ( "Encrypted Broadcast key: %s\n" , str_tohex ( encrypted_key , key_len , tmp , sizeof ( tmp ) ) ) ;
 DEBUG_MSG ( "KeyIV: %s\n" , str_tohex ( eapol_key -> key_IV , 16 , tmp , sizeof ( tmp ) ) ) ;
 DEBUG_MSG ( "decryption_key: %s\n" , str_tohex ( sa -> ptk + 16 , 16 , tmp , sizeof ( tmp ) ) ) ;
 # if 0 memcpy ( new_key , pEAPKey -> key_iv , 16 ) ;
 memcpy ( new_key + 16 , decryption_key , 16 ) ;
 DEBUG_DUMP ( "FullDecrKey:" , new_key , 32 ) ;
 if ( key_version == AIRPDCAP_WPA_KEY_VER_NOT_CCMP ) {
 guint8 dummy [ 256 ] ;
 rc4_state_struct rc4_state ;
 crypt_rc4_init ( & rc4_state , new_key , sizeof ( new_key ) ) ;
 crypt_rc4 ( & rc4_state , dummy , 256 ) ;
 crypt_rc4 ( & rc4_state , encrypted_key , key_len ) ;
 }
 else if ( key_version == AIRPDCAP_WPA_KEY_VER_AES_CCMP ) {
 guint8 key_found ;
 guint16 key_index ;
 guint8 * decrypted_data ;
 decrypted_data = ( guint8 * ) g_malloc ( key_len ) ;
 AES_unwrap ( decryption_key , 16 , encrypted_key , key_len , decrypted_data ) ;
 key_found = FALSE ;
 key_index = 0 ;
 while ( key_index < key_len && ! key_found ) {
 guint8 rsn_id ;
 rsn_id = decrypted_data [ key_index ] ;
 if ( rsn_id != 0xdd ) {
 key_index += decrypted_data [ key_index + 1 ] + 2 ;
 }
 else {
 key_found = TRUE ;
 }
 }
 if ( key_found ) {
 memcpy ( encrypted_key , decrypted_data + key_index + 8 , key_len - key_index - 8 ) ;
 }
 g_free ( decrypted_data ) ;
 }
 DEBUG_DUMP ( "Broadcast key:" , encrypted_key , key_len ) ;
 sa -> key = & dummy_key ;
 sa -> validKey = TRUE ;
 sa -> wpa . key_ver = key_version ;
 memset ( sa -> wpa . ptk , 0 , sizeof ( sa -> wpa . ptk ) ) ;
 memcpy ( sa -> wpa . ptk + 32 , szEncryptedKey , key_len ) ;
 g_free ( szEncryptedKey ) ;
 # endif SAFE_FREE ( encrypted_key ) ;
 return E_SUCCESS ;
 }