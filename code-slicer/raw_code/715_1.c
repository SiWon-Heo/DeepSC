static int pefromupx ( const char * src , uint32_t ssize , char * dst , uint32_t * dsize , uint32_t ep , uint32_t upx0 , uint32_t upx1 , uint32_t * magic , uint32_t dend ) {
 char * imports , * sections = NULL , * pehdr = NULL , * newbuf ;
 unsigned int sectcnt = 0 , upd = 1 ;
 uint32_t realstuffsz = 0 , valign = 0 ;
 uint32_t foffset = 0xd0 + 0xf8 ;
 if ( ( dst == NULL ) || ( src == NULL ) ) return 0 ;
 while ( ( valign = magic [ sectcnt ++ ] ) ) {
 if ( ep - upx1 + valign <= ssize - 5 && src [ ep - upx1 + valign - 2 ] == '\x8d' && src [ ep - upx1 + valign - 1 ] == '\xbe' ) break ;
 }
 if ( ! valign && ep - upx1 + 0x80 < ssize - 8 ) {
 const char * pt = & src [ ep - upx1 + 0x80 ] ;
 cli_dbgmsg ( "UPX: bad magic - scanning for imports\n" ) ;
 while ( ( pt = cli_memstr ( pt , ssize - ( pt - src ) - 8 , "\x8d\xbe" , 2 ) ) ) {
 if ( pt [ 6 ] == '\x8b' && pt [ 7 ] == '\x07' ) {
 valign = pt - src + 2 - ep + upx1 ;
 break ;
 }
 pt ++ ;
 }
 }
 if ( valign && CLI_ISCONTAINED ( src , ssize , src + ep - upx1 + valign , 4 ) ) {
 imports = dst + cli_readint32 ( src + ep - upx1 + valign ) ;
 realstuffsz = imports - dst ;
 if ( realstuffsz >= * dsize ) {
 cli_dbgmsg ( "UPX: wrong realstuff size\n" ) ;
 }
 else {
 pehdr = imports ;
 while ( CLI_ISCONTAINED ( dst , * dsize , pehdr , 8 ) && cli_readint32 ( pehdr ) ) {
 pehdr += 8 ;
 while ( CLI_ISCONTAINED ( dst , * dsize , pehdr , 2 ) && * pehdr ) {
 pehdr ++ ;
 while ( CLI_ISCONTAINED ( dst , * dsize , pehdr , 2 ) && * pehdr ) pehdr ++ ;
 pehdr ++ ;
 }
 pehdr ++ ;
 }
 pehdr += 4 ;
 if ( ! ( sections = checkpe ( dst , * dsize , pehdr , & valign , & sectcnt ) ) ) pehdr = NULL ;
 }
 }
 if ( ! pehdr && dend > 0xf8 + 0x28 ) {
 cli_dbgmsg ( "UPX: no luck - scanning for PE\n" ) ;
 pehdr = & dst [ dend - 0xf8 - 0x28 ] ;
 while ( pehdr > dst ) {
 if ( ( sections = checkpe ( dst , * dsize , pehdr , & valign , & sectcnt ) ) ) break ;
 pehdr -- ;
 }
 if ( ! ( realstuffsz = pehdr - dst ) ) pehdr = NULL ;
 }
 if ( ! pehdr ) {
 uint32_t rebsz = PESALIGN ( dend , 0x1000 ) ;
 cli_dbgmsg ( "UPX: no luck - brutally crafing a reasonable PE\n" ) ;
 if ( ! ( newbuf = ( char * ) cli_calloc ( rebsz + 0x200 , sizeof ( char ) ) ) ) {
 cli_dbgmsg ( "UPX: malloc failed - giving up rebuild\n" ) ;
 return 0 ;
 }
 memcpy ( newbuf , HEADERS , 0xd0 ) ;
 memcpy ( newbuf + 0xd0 , FAKEPE , 0x120 ) ;
 memcpy ( newbuf + 0x200 , dst , dend ) ;
 memcpy ( dst , newbuf , dend + 0x200 ) ;
 free ( newbuf ) ;
 cli_writeint32 ( dst + 0xd0 + 0x50 , rebsz + 0x1000 ) ;
 cli_writeint32 ( dst + 0xd0 + 0x100 , rebsz ) ;
 cli_writeint32 ( dst + 0xd0 + 0x108 , rebsz ) ;
 * dsize = rebsz + 0x200 ;
 cli_dbgmsg ( "UPX: PE structure added to uncompressed data\n" ) ;
 return 1 ;
 }
 if ( ! sections ) sectcnt = 0 ;
 foffset = PESALIGN ( foffset + 0x28 * sectcnt , valign ) ;
 for ( upd = 0 ;
 upd < sectcnt ;
 upd ++ ) {
 uint32_t vsize = PESALIGN ( ( uint32_t ) cli_readint32 ( sections + 8 ) , valign ) ;
 uint32_t urva = PEALIGN ( ( uint32_t ) cli_readint32 ( sections + 12 ) , valign ) ;
 if ( ! CLI_ISCONTAINED ( upx0 , realstuffsz , urva , vsize ) ) {
 cli_dbgmsg ( "UPX: Sect %d out of bounds - giving up rebuild\n" , upd ) ;
 return 0 ;
 }
 cli_writeint32 ( sections + 8 , vsize ) ;
 cli_writeint32 ( sections + 12 , urva ) ;
 cli_writeint32 ( sections + 16 , vsize ) ;
 cli_writeint32 ( sections + 20 , foffset ) ;
 if ( foffset + vsize < foffset ) {
 return 0 ;
 }
 foffset += vsize ;
 sections += 0x28 ;
 }
 cli_writeint32 ( pehdr + 8 , 0x4d414c43 ) ;
 cli_writeint32 ( pehdr + 0x3c , valign ) ;
 if ( ! ( newbuf = ( char * ) cli_calloc ( foffset , sizeof ( char ) ) ) ) {
 cli_dbgmsg ( "UPX: malloc failed - giving up rebuild\n" ) ;
 return 0 ;
 }
 memcpy ( newbuf , HEADERS , 0xd0 ) ;
 memcpy ( newbuf + 0xd0 , pehdr , 0xf8 + 0x28 * sectcnt ) ;
 sections = pehdr + 0xf8 ;
 for ( upd = 0 ;
 upd < sectcnt ;
 upd ++ ) {
 uint32_t offset1 , offset2 , offset3 ;
 offset1 = ( uint32_t ) cli_readint32 ( sections + 20 ) ;
 offset2 = ( uint32_t ) cli_readint32 ( sections + 16 ) ;
 if ( offset1 > foffset || offset2 > foffset || offset1 + offset2 > foffset ) {
 free ( newbuf ) ;
 return 1 ;
 }
 offset3 = ( uint32_t ) cli_readint32 ( sections + 12 ) ;
 if ( offset3 - upx0 > * dsize ) {
 free ( newbuf ) ;
 return 1 ;
 }
 memcpy ( newbuf + offset1 , dst + offset3 - upx0 , offset2 ) ;
 sections += 0x28 ;
 }
 if ( foffset > * dsize + 8192 ) {
 cli_dbgmsg ( "UPX: wrong raw size - giving up rebuild\n" ) ;
 free ( newbuf ) ;
 return 0 ;
 }
 memcpy ( dst , newbuf , foffset ) ;
 * dsize = foffset ;
 free ( newbuf ) ;
 cli_dbgmsg ( "UPX: PE structure rebuilt from compressed file\n" ) ;
 return 1 ;
 }