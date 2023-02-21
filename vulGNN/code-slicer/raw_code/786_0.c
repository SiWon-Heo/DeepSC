static int isoent_gen_iso9660_identifier ( struct archive_write * a , struct isoent * isoent , struct idr * idr ) {
 struct iso9660 * iso9660 ;
 struct isoent * np ;
 char * p ;
 int l , r ;
 const char * char_map ;
 char allow_ldots , allow_multidot , allow_period , allow_vernum ;
 int fnmax , ffmax , dnmax ;
 static const struct archive_rb_tree_ops rb_ops = {
 isoent_cmp_node_iso9660 , isoent_cmp_key_iso9660 }
 ;
 if ( isoent -> children . cnt == 0 ) return ( 0 ) ;
 iso9660 = a -> format_data ;
 char_map = idr -> char_map ;
 if ( iso9660 -> opt . iso_level <= 3 ) {
 allow_ldots = 0 ;
 allow_multidot = 0 ;
 allow_period = 1 ;
 allow_vernum = iso9660 -> opt . allow_vernum ;
 if ( iso9660 -> opt . iso_level == 1 ) {
 fnmax = 8 ;
 ffmax = 12 ;
 dnmax = 8 ;
 }
 else {
 fnmax = 30 ;
 ffmax = 31 ;
 dnmax = 31 ;
 }
 }
 else {
 allow_ldots = allow_multidot = 1 ;
 allow_period = allow_vernum = 0 ;
 if ( iso9660 -> opt . rr ) fnmax = ffmax = dnmax = 193 ;
 else fnmax = ffmax = dnmax = 207 ;
 }
 r = idr_start ( a , idr , isoent -> children . cnt , ffmax , 3 , 1 , & rb_ops ) ;
 if ( r < 0 ) return ( r ) ;
 for ( np = isoent -> children . first ;
 np != NULL ;
 np = np -> chnext ) {
 char * dot , * xdot ;
 int ext_off , noff , weight ;
 l = ( int ) np -> file -> basename . length ;
 p = malloc ( l + 31 + 2 + 1 ) ;
 if ( p == NULL ) {
 archive_set_error ( & a -> archive , ENOMEM , "Can't allocate memory" ) ;
 return ( ARCHIVE_FATAL ) ;
 }
 memcpy ( p , np -> file -> basename . s , l ) ;
 p [ l ] = '\0' ;
 np -> identifier = p ;
 dot = xdot = NULL ;
 if ( ! allow_ldots ) {
 if ( * p == '.' ) * p ++ = '_' ;
 }
 for ( ;
 * p ;
 p ++ ) {
 if ( * p & 0x80 ) {
 * p = '_' ;
 continue ;
 }
 if ( char_map [ ( unsigned char ) * p ] ) {
 if ( * p == '.' ) {
 xdot = dot ;
 dot = p ;
 }
 continue ;
 }
 if ( * p >= 'a' && * p <= 'z' ) {
 * p -= 'a' - 'A' ;
 continue ;
 }
 if ( * p == '.' ) {
 xdot = dot ;
 dot = p ;
 if ( allow_multidot ) continue ;
 }
 * p = '_' ;
 }
 p = np -> identifier ;
 weight = - 1 ;
 if ( dot == NULL ) {
 int nammax ;
 if ( np -> dir ) nammax = dnmax ;
 else nammax = fnmax ;
 if ( l > nammax ) {
 p [ nammax ] = '\0' ;
 weight = nammax ;
 ext_off = nammax ;
 }
 else ext_off = l ;
 }
 else {
 * dot = '.' ;
 ext_off = ( int ) ( dot - p ) ;
 if ( iso9660 -> opt . iso_level == 1 ) {
 if ( dot - p <= 8 ) {
 if ( strlen ( dot ) > 4 ) {
 dot [ 4 ] = '\0' ;
 weight = 0 ;
 }
 }
 else {
 p [ 8 ] = dot [ 0 ] ;
 p [ 9 ] = dot [ 1 ] ;
 p [ 10 ] = dot [ 2 ] ;
 p [ 11 ] = dot [ 3 ] ;
 p [ 12 ] = '\0' ;
 weight = 8 ;
 ext_off = 8 ;
 }
 }
 else if ( np -> dir ) {
 if ( l > dnmax ) {
 p [ dnmax ] = '\0' ;
 weight = dnmax ;
 if ( ext_off > dnmax ) ext_off = dnmax ;
 }
 }
 else if ( l > ffmax ) {
 int extlen = ( int ) strlen ( dot ) ;
 int xdoff ;
 if ( xdot != NULL ) xdoff = ( int ) ( xdot - p ) ;
 else xdoff = 0 ;
 if ( extlen > 1 && xdoff < fnmax - 1 ) {
 int off ;
 if ( extlen > ffmax ) extlen = ffmax ;
 off = ffmax - extlen ;
 if ( off == 0 ) {
 off ++ ;
 extlen -- ;
 }
 memmove ( p + off , dot , extlen ) ;
 p [ ffmax ] = '\0' ;
 ext_off = off ;
 weight = off ;
 # ifdef COMPAT_MKISOFS }
 else if ( xdoff >= fnmax - 1 ) {
 p [ fnmax - 1 ] = '\0' ;
 ext_off = fnmax - 1 ;
 weight = fnmax - 1 ;
 # endif }
 else {
 p [ fnmax ] = '\0' ;
 ext_off = fnmax ;
 weight = fnmax ;
 }
 }
 }
 np -> ext_off = ext_off ;
 np -> ext_len = ( int ) strlen ( & p [ ext_off ] ) ;
 np -> id_len = l = ext_off + np -> ext_len ;
 if ( iso9660 -> opt . iso_level == 1 ) {
 if ( ext_off >= 5 ) noff = 5 ;
 else noff = ext_off ;
 }
 else {
 if ( l == ffmax ) noff = ext_off - 3 ;
 else if ( l == ffmax - 1 ) noff = ext_off - 2 ;
 else if ( l == ffmax - 2 ) noff = ext_off - 1 ;
 else noff = ext_off ;
 }
 idr_register ( idr , np , weight , noff ) ;
 }
 idr_resolve ( idr , idr_set_num ) ;
 for ( np = isoent -> children . first ;
 np != NULL ;
 np = np -> chnext ) {
 if ( ! np -> dir && np -> rr_child == NULL ) {
 p = np -> identifier + np -> ext_off + np -> ext_len ;
 if ( np -> ext_len == 0 && allow_period ) {
 * p ++ = '.' ;
 np -> ext_len = 1 ;
 }
 if ( np -> ext_len == 1 && ! allow_period ) {
 * -- p = '\0' ;
 np -> ext_len = 0 ;
 }
 np -> id_len = np -> ext_off + np -> ext_len ;
 if ( allow_vernum ) {
 * p ++ = ';
' ;
 * p ++ = '1' ;
 np -> id_len += 2 ;
 }
 * p = '\0' ;
 }
 else np -> id_len = np -> ext_off + np -> ext_len ;
 np -> mb_len = np -> id_len ;
 }
 return ( ARCHIVE_OK ) ;
 }