int unupack ( int upack , char * dest , uint32_t dsize , char * buff , uint32_t vma , uint32_t ep , uint32_t base , uint32_t va , int file ) {
 int j , searchval ;
 char * loc_esi , * loc_edi = NULL , * loc_ebx , * end_edi , * save_edi , * alvalue ;
 char * paddr , * pushed_esi , * save2 ;
 uint32_t save1 , save3 , loc_ecx , count , shlsize , original_ep , ret , loc_ebx_u ;
 struct cli_exe_section section ;
 int upack_version = UPACK_399 ;
 if ( upack ) {
 uint32_t aljump , shroff , lngjmpoff ;
 if ( buff [ 5 ] == '\xff' && buff [ 6 ] == '\x36' ) upack_version = UPACK_0297729 ;
 loc_esi = dest + ( cli_readint32 ( buff + 1 ) - vma ) ;
 if ( ! CLI_ISCONTAINED ( dest , dsize , loc_esi , 12 ) ) return - 1 ;
 original_ep = cli_readint32 ( loc_esi ) ;
 loc_esi += 4 ;
 loc_esi += 4 ;
 original_ep -= vma ;
 cli_dbgmsg ( "Upack: EP: %08x original: %08X || %08x\n" , ep , original_ep , cli_readint32 ( loc_esi - 8 ) ) ;
 if ( upack_version == UPACK_399 ) {
 loc_edi = dest + ( cli_readint32 ( loc_esi ) - vma ) ;
 if ( ! CLI_ISCONTAINED ( dest , dsize , dest + ep + 0xa , 2 ) || dest [ ep + 0xa ] != '\xeb' ) return - 1 ;
 loc_esi = dest + * ( dest + ep + 0xb ) + ep + 0xc ;
 alvalue = loc_esi + 0x1a ;
 if ( ! CLI_ISCONTAINED ( dest , dsize , alvalue , 2 ) || * alvalue != '\xeb' ) return - 1 ;
 alvalue ++ ;
 alvalue += ( * alvalue & 0xff ) + 1 + 0xa ;
 lngjmpoff = 8 ;
 }
 else {
 if ( ! CLI_ISCONTAINED ( dest , dsize , dest + ep + 7 , 5 ) || dest [ ep + 7 ] != '\xe9' ) return - 1 ;
 loc_esi = dest + cli_readint32 ( dest + ep + 8 ) + ep + 0xc ;
 alvalue = loc_esi + 0x25 ;
 lngjmpoff = 10 ;
 }
 if ( ! CLI_ISCONTAINED ( dest , dsize , alvalue , 2 ) || * alvalue != '\xb5' ) return - 1 ;
 alvalue ++ ;
 count = * alvalue & 0xff ;
 if ( ! CLI_ISCONTAINED ( dest , dsize , alvalue , lngjmpoff + 5 ) || * ( alvalue + lngjmpoff ) != '\xe9' ) return - 1 ;
 shlsize = cli_readint32 ( alvalue + lngjmpoff + 1 ) ;
 if ( upack_version == UPACK_399 ) shlsize = shlsize + ( loc_esi - dest ) + * ( loc_esi + 0x1b ) + 0x1c + 0x018 ;
 else shlsize = shlsize + ( loc_esi - dest ) + 0x035 ;
 alvalue = dest + shlsize + 43 ;
 aljump = 8 ;
 shroff = 24 ;
 if ( ! CLI_ISCONTAINED ( dest , dsize , alvalue - 1 , 2 ) || * ( alvalue - 1 ) != '\xe3' ) {
 alvalue = dest + shlsize + 46 ;
 if ( ! CLI_ISCONTAINED ( dest , dsize , alvalue - 1 , 2 ) || * ( alvalue - 1 ) != '\xe3' ) return - 1 ;
 else {
 if ( upack_version != UPACK_0297729 ) upack_version = UPACK_0151477 ;
 aljump = 7 ;
 shroff = 26 ;
 }
 }
 alvalue += ( * alvalue & 0xff ) + 1 ;
 if ( ! CLI_ISCONTAINED ( dest , dsize , alvalue , aljump + 5 ) || * ( alvalue + aljump ) != '\xe9' ) return - 1 ;
 ret = cli_readint32 ( alvalue + aljump + 1 ) ;
 alvalue += ret + aljump + 1 + 4 + 27 ;
 if ( upack_version == UPACK_0297729 ) alvalue += 2 ;
 if ( ! CLI_ISCONTAINED ( dest , dsize , dest + shlsize + shroff , 3 ) || * ( dest + shlsize + shroff ) != '\xc1' || * ( dest + shlsize + shroff + 1 ) != '\xed' ) return - 1 ;
 shlsize = ( * ( dest + shlsize + shroff + 2 ) ) & 0xff ;
 count *= 0x100 ;
 if ( shlsize < 2 || shlsize > 8 ) {
 cli_dbgmsg ( "Upack: context bits out of bounds\n" ) ;
 return - 1 ;
 }
 cli_dbgmsg ( "Upack: Context Bits parameter used with lzma: %02x, %02x\n" , shlsize , count ) ;
 if ( upack_version == UPACK_0297729 ) {
 if ( ! CLI_ISCONTAINED ( dest , dsize , loc_esi + 6 , 10 ) || * ( loc_esi + 6 ) != '\xbe' || * ( loc_esi + 11 ) != '\xbf' ) return - 1 ;
 if ( ( uint32_t ) cli_readint32 ( loc_esi + 7 ) < base || ( uint32_t ) cli_readint32 ( loc_esi + 7 ) > vma ) return - 1 ;
 loc_edi = dest + ( cli_readint32 ( loc_esi + 12 ) - vma ) ;
 loc_esi = dest + ( cli_readint32 ( loc_esi + 7 ) - base ) ;
 }
 else {
 if ( ! CLI_ISCONTAINED ( dest , dsize , loc_esi + 7 , 5 ) || * ( loc_esi + 7 ) != '\xbe' ) return - 1 ;
 loc_esi = dest + ( cli_readint32 ( loc_esi + 8 ) - vma ) ;
 }
 if ( upack_version == UPACK_0297729 ) {
 if ( ! CLI_ISCONTAINED ( dest , dsize , loc_edi , ( 0x58 + 24 + 4 * count ) ) || ! CLI_ISCONTAINED ( dest , dsize , loc_esi , ( 0x58 + 0x64 + 4 ) ) ) return - 1 ;
 for ( j = 0 ;
 j < 0x16 ;
 j ++ , loc_esi += 4 , loc_edi += 4 ) cli_writeint32 ( loc_edi , cli_readint32 ( loc_esi ) ) ;
 }
 else {
 if ( ! CLI_ISCONTAINED ( dest , dsize , loc_edi , ( 0x9c + 24 + 4 * count ) ) || ! CLI_ISCONTAINED ( dest , dsize , loc_esi , ( 0x9c + 0x34 + 4 ) ) ) return - 1 ;
 for ( j = 0 ;
 j < 0x27 ;
 j ++ , loc_esi += 4 , loc_edi += 4 ) cli_writeint32 ( loc_edi , cli_readint32 ( loc_esi ) ) ;
 }
 save3 = cli_readint32 ( loc_esi + 4 ) ;
 paddr = dest + ( ( uint32_t ) cli_readint32 ( loc_edi - 4 ) ) - vma ;
 loc_ebx = loc_edi ;
 cli_writeint32 ( loc_edi , 0xffffffff ) ;
 loc_edi += 4 ;
 cli_writeint32 ( loc_edi , 0 ) ;
 loc_edi += 4 ;
 for ( j = 0 ;
 j < 4 ;
 j ++ , loc_edi += 4 ) cli_writeint32 ( loc_edi , ( 1 ) ) ;
 for ( j = 0 ;
 ( unsigned int ) j < count ;
 j ++ , loc_edi += 4 ) cli_writeint32 ( loc_edi , 0x400 ) ;
 loc_edi = dest + cli_readint32 ( loc_esi + 0xc ) - vma ;
 if ( upack_version == UPACK_0297729 ) loc_edi = dest + vma - base ;
 pushed_esi = loc_edi ;
 end_edi = dest + cli_readint32 ( loc_esi + 0x34 ) - vma ;
 if ( upack_version == UPACK_0297729 ) {
 end_edi = dest + cli_readint32 ( loc_esi + 0x64 ) - vma ;
 save3 = cli_readint32 ( loc_esi + 0x40 ) ;
 }
 cli_dbgmsg ( "Upack: data initialized, before upack lzma call!\n" ) ;
 if ( ( ret = ( uint32_t ) unupack399 ( dest , dsize , 0 , loc_ebx , 0 , loc_edi , end_edi , shlsize , paddr ) ) == 0xffffffff ) return - 1 ;
 }
 else {
 int ep_jmp_offs , rep_stosd_count_offs , context_bits_offs ;
 loc_esi = dest + vma + ep ;
 if ( buff [ 0 ] == '\xbe' && buff [ 5 ] == '\xad' && buff [ 6 ] == '\x8b' && buff [ 7 ] == '\xf8' ) upack_version = UPACK_11_12 ;
 if ( upack_version == UPACK_11_12 ) {
 ep_jmp_offs = 0x1a4 ;
 rep_stosd_count_offs = 0x1b ;
 context_bits_offs = 0x41 ;
 alvalue = loc_esi + 0x184 ;
 }
 else {
 ep_jmp_offs = 0x217 ;
 rep_stosd_count_offs = 0x3a ;
 context_bits_offs = 0x5f ;
 alvalue = loc_esi + 0x1c1 ;
 }
 if ( ! CLI_ISCONTAINED ( dest , dsize , loc_esi , ep_jmp_offs + 4 ) ) return - 1 ;
 save1 = cli_readint32 ( loc_esi + ep_jmp_offs ) ;
 original_ep = ( loc_esi - dest ) + ep_jmp_offs + 4 ;
 original_ep += ( int32_t ) save1 ;
 cli_dbgmsg ( "Upack: EP: %08x original %08x\n" , ep , original_ep ) ;
 count = ( * ( loc_esi + rep_stosd_count_offs ) ) & 0xff ;
 shlsize = ( * ( loc_esi + context_bits_offs ) ) & 0xff ;
 shlsize = 8 - shlsize ;
 if ( shlsize < 2 || shlsize > 8 ) {
 cli_dbgmsg ( "Upack: context bits out of bounds\n" ) ;
 return - 1 ;
 }
 count *= 0x100 ;
 cli_dbgmsg ( "Upack: Context Bits parameter used with lzma: %02x, %02x\n" , shlsize , count ) ;
 if ( upack_version == UPACK_399 ) {
 loc_esi += 4 ;
 loc_ecx = cli_readint32 ( loc_esi + 2 ) ;
 cli_writeint32 ( loc_esi + 2 , 0 ) ;
 if ( ! loc_ecx ) {
 cli_dbgmsg ( "Upack: something's wrong, report back\n" ) ;
 return - 1 ;
 }
 loc_esi -= ( loc_ecx - 2 ) ;
 if ( ! CLI_ISCONTAINED ( dest , dsize , loc_esi , 12 ) ) return - 1 ;
 cli_dbgmsg ( "Upack: %p %p %08x %08x\n" , loc_esi , dest , cli_readint32 ( loc_esi ) , base ) ;
 loc_ebx_u = loc_esi - ( dest + cli_readint32 ( loc_esi ) - base ) ;
 cli_dbgmsg ( "Upack: EBX: %08x\n" , loc_ebx_u ) ;
 loc_esi += 4 ;
 save2 = loc_edi = dest + cli_readint32 ( loc_esi ) - base ;
 cli_dbgmsg ( "Upack: DEST: %08x, %08x\n" , cli_readint32 ( loc_esi ) , cli_readint32 ( loc_esi ) - base ) ;
 loc_esi += 4 ;
 j = cli_readint32 ( loc_esi ) ;
 if ( j < 0 ) {
 cli_dbgmsg ( "Upack: probably hand-crafted data, report back\n" ) ;
 return - 1 ;
 }
 loc_esi += 4 ;
 cli_dbgmsg ( "Upack: ecx counter: %08x\n" , j ) ;
 if ( ! CLI_ISCONTAINED ( dest , dsize , loc_esi , ( j * 4 ) ) || ! CLI_ISCONTAINED ( dest , dsize , loc_edi , ( ( j + count ) * 4 ) ) ) return - 1 ;
 for ( ;
 j -- ;
 loc_edi += 4 , loc_esi += 4 ) cli_writeint32 ( loc_edi , cli_readint32 ( loc_esi ) ) ;
 if ( ! CLI_ISCONTAINED ( dest , dsize , save2 , 8 ) ) return - 1 ;
 loc_ecx = cli_readint32 ( save2 ) ;
 save2 += 4 ;
 loc_esi = save2 ;
 do {
 loc_esi += loc_ebx_u ;
 loc_esi += 4 ;
 }
 while ( -- loc_ecx ) ;
 if ( ! CLI_ISCONTAINED ( dest , dsize , loc_esi , 4 ) ) return - 1 ;
 save1 = cli_readint32 ( loc_esi ) ;
 loc_esi += 4 ;
 for ( j = 0 ;
 ( uint32_t ) j < count ;
 j ++ , loc_edi += 4 ) cli_writeint32 ( loc_edi , ( save1 ) ) ;
 if ( ! CLI_ISCONTAINED ( dest , dsize , ( loc_esi + 0x10 ) , 4 ) ) return - 1 ;
 cli_writeint32 ( loc_esi + 0x10 , ( uint32_t ) cli_readint32 ( loc_esi + 0x10 ) + loc_ebx_u ) ;
 loc_ebx = loc_esi + 0x14 ;
 loc_esi = save2 ;
 save_edi = loc_edi = dest + ( ( uint32_t ) cli_readint32 ( loc_esi ) - base ) ;
 loc_esi += 4 ;
 cli_dbgmsg ( "Upack: before_fixing\n" ) ;
 if ( ! CLI_ISCONTAINED ( dest , dsize , loc_ebx - 4 , ( 12 + 4 * 4 ) ) || ! CLI_ISCONTAINED ( dest , dsize , loc_esi + 0x24 , 4 ) || ! CLI_ISCONTAINED ( dest , dsize , loc_esi + 0x40 , 4 ) ) return - 1 ;
 for ( j = 2 ;
 j < 6 ;
 j ++ ) cli_writeint32 ( loc_ebx + ( j << 2 ) , cli_readint32 ( loc_ebx + ( j << 2 ) ) ) ;
 paddr = dest + cli_readint32 ( loc_ebx - 4 ) - base ;
 save1 = loc_ecx ;
 pushed_esi = loc_edi ;
 end_edi = dest + cli_readint32 ( loc_esi + 0x24 ) - base ;
 vma = cli_readint32 ( loc_ebx ) ;
 cli_writeint32 ( loc_ebx , cli_readint32 ( loc_ebx + 4 ) ) ;
 cli_writeint32 ( ( loc_ebx + 4 ) , vma ) ;
 }
 else if ( upack_version == UPACK_11_12 ) {
 cli_dbgmsg ( "Upack v 1.1/1.2\n" ) ;
 loc_esi = dest + 0x148 ;
 loc_edi = dest + cli_readint32 ( loc_esi ) - base ;
 loc_esi += 4 ;
 save_edi = loc_edi ;
 paddr = dest + ( ( uint32_t ) cli_readint32 ( loc_esi ) ) - base ;
 loc_esi += 4 ;
 loc_edi += 4 ;
 loc_ebx = loc_edi ;
 if ( ! CLI_ISCONTAINED ( dest , dsize , loc_edi , ( ( 6 + count ) * 4 ) ) ) return - 1 ;
 cli_writeint32 ( loc_edi , 0xffffffff ) ;
 loc_edi += 4 ;
 cli_writeint32 ( loc_edi , 0 ) ;
 loc_edi += 4 ;
 for ( j = 0 ;
 j < 4 ;
 j ++ , loc_edi += 4 ) cli_writeint32 ( loc_edi , ( 1 ) ) ;
 for ( j = 0 ;
 ( uint32_t ) j < count ;
 j ++ , loc_edi += 4 ) cli_writeint32 ( loc_edi , 0x400 ) ;
 loc_edi = dest + cli_readint32 ( loc_esi ) - base ;
 pushed_esi = loc_edi ;
 loc_esi += 4 ;
 loc_ecx = 0 ;
 loc_esi += 4 ;
 end_edi = dest + cli_readint32 ( loc_esi - 0x28 ) - base ;
 loc_esi = save_edi ;
 }
 cli_dbgmsg ( "Upack: data initialized, before upack lzma call!\n" ) ;
 if ( ( ret = ( uint32_t ) unupack399 ( dest , dsize , loc_ecx , loc_ebx , loc_ecx , loc_edi , end_edi , shlsize , paddr ) ) == 0xffffffff ) return - 1 ;
 if ( upack_version == UPACK_399 ) save3 = cli_readint32 ( loc_esi + 0x40 ) ;
 else if ( upack_version == UPACK_11_12 ) save3 = cli_readint32 ( dest + vma + ep + 0x174 ) ;
 }
 loc_ecx = 0 ;
 if ( ! CLI_ISCONTAINED ( dest , dsize , alvalue , 1 ) ) {
 cli_dbgmsg ( "Upack: alvalue out of bounds\n" ) ;
 return - 1 ;
 }
 searchval = * alvalue & 0xff ;
 cli_dbgmsg ( "Upack: loops: %08x search value: %02x\n" , save3 , searchval ) ;
 while ( save3 ) {
 if ( ! CLI_ISCONTAINED ( dest , dsize , pushed_esi + loc_ecx , 1 ) ) {
 cli_dbgmsg ( "Upack: callfixerr %p %08x = %p, %p\n" , dest , dsize , dest + dsize , pushed_esi + loc_ecx ) ;
 return - 1 ;
 }
 if ( pushed_esi [ loc_ecx ] == '\xe8' || pushed_esi [ loc_ecx ] == '\xe9' ) {
 char * adr = ( pushed_esi + loc_ecx + 1 ) ;
 loc_ecx ++ ;
 if ( ! CLI_ISCONTAINED ( dest , dsize , adr , 4 ) ) {
 cli_dbgmsg ( "Upack: callfixerr\n" ) ;
 return - 1 ;
 }
 if ( ( cli_readint32 ( adr ) & 0xff ) != searchval ) continue ;
 cli_writeint32 ( adr , EC32 ( CE32 ( ( uint32_t ) ( cli_readint32 ( adr ) & 0xffffff00 ) ) ) - loc_ecx - 4 ) ;
 loc_ecx += 4 ;
 save3 -- ;
 }
 else loc_ecx ++ ;
 }
 section . raw = 0 ;
 section . rva = va ;
 section . rsz = end_edi - loc_edi ;
 section . vsz = end_edi - loc_edi ;
 if ( ! cli_rebuildpe ( dest + ( upack ? 0 : va ) , & section , 1 , base , original_ep , 0 , 0 , file ) ) {
 cli_dbgmsg ( "Upack: Rebuilding failed\n" ) ;
 return 0 ;
 }
 return 1 ;
 }