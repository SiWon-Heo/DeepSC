SplineChar * PSCharStringToSplines ( uint8 * type1 , int len , struct pscontext * context , struct pschars * subrs , struct pschars * gsubrs , const char * name ) {
 int is_type2 = context -> is_type2 ;
 real stack [ 50 ] ;
 int sp = 0 , v ;
 real transient [ 32 ] ;
 SplineChar * ret = SplineCharCreate ( 2 ) ;
 SplinePointList * cur = NULL , * oldcur = NULL ;
 RefChar * r1 , * r2 , * rlast = NULL ;
 DBasePoint current ;
 real dx , dy , dx2 , dy2 , dx3 , dy3 , dx4 , dy4 , dx5 , dy5 , dx6 , dy6 ;
 SplinePoint * pt ;
 struct substate {
 unsigned char * type1 ;
 int len ;
 int subnum ;
 }
 pcstack [ 11 ] ;
 int pcsp = 0 ;
 StemInfo * hint , * hp ;
 real pops [ 30 ] ;
 int popsp = 0 ;
 int base , polarity ;
 real coord ;
 struct pschars * s ;
 int hint_cnt = 0 ;
 StemInfo * activeh = NULL , * activev = NULL , * sameh ;
 HintMask * pending_hm = NULL ;
 HintMask * counters [ 96 ] ;
 int cp = 0 ;
 real unblended [ 2 ] [ MmMax ] ;
 int last_was_b1 = false , old_last_was_b1 ;
 if ( ! is_type2 && context -> instance_count > 1 ) memset ( unblended , 0 , sizeof ( unblended ) ) ;
 ret -> name = copy ( name ) ;
 ret -> unicodeenc = - 1 ;
 ret -> width = ( int16 ) 0x8000 ;
 if ( name == NULL ) name = "unnamed" ;
 ret -> manualhints = true ;
 current . x = current . y = 0 ;
 while ( len > 0 ) {
 if ( sp > 48 ) {
 LogError ( _ ( "Stack got too big in %s\n" ) , name ) ;
 sp = 48 ;
 }
 base = 0 ;
 -- len ;
 if ( ( v = * type1 ++ ) >= 32 ) {
 if ( v <= 246 ) {
 stack [ sp ++ ] = v - 139 ;
 }
 else if ( v <= 250 ) {
 stack [ sp ++ ] = ( v - 247 ) * 256 + * type1 ++ + 108 ;
 -- len ;
 }
 else if ( v <= 254 ) {
 stack [ sp ++ ] = - ( v - 251 ) * 256 - * type1 ++ - 108 ;
 -- len ;
 }
 else {
 int val = ( * type1 << 24 ) | ( type1 [ 1 ] << 16 ) | ( type1 [ 2 ] << 8 ) | type1 [ 3 ] ;
 stack [ sp ++ ] = val ;
 type1 += 4 ;
 len -= 4 ;
 if ( is_type2 ) {
 # ifndef PSFixed_Is_TTF stack [ sp - 1 ] /= 65536. ;
 # else int mant = val & 0xffff ;
 stack [ sp - 1 ] = ( val >> 16 ) + mant / 65536. ;
 # endif }
 }
 }
 else if ( v == 28 ) {
 stack [ sp ++ ] = ( short ) ( ( type1 [ 0 ] << 8 ) | type1 [ 1 ] ) ;
 type1 += 2 ;
 len -= 2 ;
 }
 else if ( v == 12 ) {
 old_last_was_b1 = last_was_b1 ;
 last_was_b1 = false ;
 v = * type1 ++ ;
 -- len ;
 switch ( v ) {
 case 0 : if ( is_type2 ) LogError ( _ ( "%s\'s dotsection operator is deprecated for Type2\n" ) , name ) ;
 sp = 0 ;
 break ;
 case 1 : if ( sp < 6 ) LogError ( _ ( "Stack underflow on vstem3 in %s\n" ) , name ) ;
 if ( is_type2 ) LogError ( _ ( "%s\'s vstem3 operator is not supported for Type2\n" ) , name ) ;
 sameh = NULL ;
 if ( ! is_type2 ) sameh = SameH ( ret -> vstem , stack [ 0 ] + ret -> lsidebearing , stack [ 1 ] , unblended , 0 ) ;
 hint = HintNew ( stack [ 0 ] + ret -> lsidebearing , stack [ 1 ] ) ;
 hint -> hintnumber = sameh != NULL ? sameh -> hintnumber : hint_cnt ++ ;
 if ( activev == NULL ) activev = hp = hint ;
 else {
 for ( hp = activev ;
 hp -> next != NULL ;
 hp = hp -> next ) ;
 hp -> next = hint ;
 hp = hint ;
 }
 sameh = NULL ;
 if ( ! is_type2 ) sameh = SameH ( ret -> vstem , stack [ 2 ] + ret -> lsidebearing , stack [ 3 ] , unblended , 0 ) ;
 hp -> next = HintNew ( stack [ 2 ] + ret -> lsidebearing , stack [ 3 ] ) ;
 hp -> next -> hintnumber = sameh != NULL ? sameh -> hintnumber : hint_cnt ++ ;
 if ( ! is_type2 ) sameh = SameH ( ret -> vstem , stack [ 4 ] + ret -> lsidebearing , stack [ 5 ] , unblended , 0 ) ;
 hp -> next -> next = HintNew ( stack [ 4 ] + ret -> lsidebearing , stack [ 5 ] ) ;
 hp -> next -> next -> hintnumber = sameh != NULL ? sameh -> hintnumber : hint_cnt ++ ;
 if ( ! is_type2 && hp -> next -> next -> hintnumber < 96 ) {
 if ( pending_hm == NULL ) pending_hm = chunkalloc ( sizeof ( HintMask ) ) ;
 ( * pending_hm ) [ hint -> hintnumber >> 3 ] |= 0x80 >> ( hint -> hintnumber & 0x7 ) ;
 ( * pending_hm ) [ hint -> next -> hintnumber >> 3 ] |= 0x80 >> ( hint -> next -> hintnumber & 0x7 ) ;
 ( * pending_hm ) [ hint -> next -> next -> hintnumber >> 3 ] |= 0x80 >> ( hint -> next -> next -> hintnumber & 0x7 ) ;
 }
 hp = hp -> next -> next ;
 sp = 0 ;
 break ;
 case 2 : if ( sp < 6 ) LogError ( _ ( "Stack underflow on hstem3 in %s\n" ) , name ) ;
 if ( is_type2 ) LogError ( _ ( "%s\'s vstem3 operator is not supported for Type2\n" ) , name ) ;
 sameh = NULL ;
 if ( ! is_type2 ) sameh = SameH ( ret -> hstem , stack [ 0 ] , stack [ 1 ] , unblended , 0 ) ;
 hint = HintNew ( stack [ 0 ] , stack [ 1 ] ) ;
 hint -> hintnumber = sameh != NULL ? sameh -> hintnumber : hint_cnt ++ ;
 if ( activeh == NULL ) activeh = hp = hint ;
 else {
 for ( hp = activeh ;
 hp -> next != NULL ;
 hp = hp -> next ) ;
 hp -> next = hint ;
 hp = hint ;
 }
 sameh = NULL ;
 if ( ! is_type2 ) sameh = SameH ( ret -> hstem , stack [ 2 ] , stack [ 3 ] , unblended , 0 ) ;
 hp -> next = HintNew ( stack [ 2 ] , stack [ 3 ] ) ;
 hp -> next -> hintnumber = sameh != NULL ? sameh -> hintnumber : hint_cnt ++ ;
 sameh = NULL ;
 if ( ! is_type2 ) sameh = SameH ( ret -> hstem , stack [ 4 ] , stack [ 5 ] , unblended , 0 ) ;
 hp -> next -> next = HintNew ( stack [ 4 ] , stack [ 5 ] ) ;
 hp -> next -> next -> hintnumber = sameh != NULL ? sameh -> hintnumber : hint_cnt ++ ;
 if ( ! is_type2 && hp -> next -> next -> hintnumber < 96 ) {
 if ( pending_hm == NULL ) pending_hm = chunkalloc ( sizeof ( HintMask ) ) ;
 ( * pending_hm ) [ hint -> hintnumber >> 3 ] |= 0x80 >> ( hint -> hintnumber & 0x7 ) ;
 ( * pending_hm ) [ hint -> next -> hintnumber >> 3 ] |= 0x80 >> ( hint -> next -> hintnumber & 0x7 ) ;
 ( * pending_hm ) [ hint -> next -> next -> hintnumber >> 3 ] |= 0x80 >> ( hint -> next -> next -> hintnumber & 0x7 ) ;
 }
 hp = hp -> next -> next ;
 sp = 0 ;
 break ;
 case 6 : seac : if ( sp < 5 ) LogError ( _ ( "Stack underflow on seac in %s\n" ) , name ) ;
 if ( is_type2 ) {
 if ( v == 6 ) LogError ( _ ( "%s\'s SEAC operator is invalid for Type2\n" ) , name ) ;
 else LogError ( _ ( "%s\'s SEAC-like endchar operator is deprecated for Type2\n" ) , name ) ;
 }
 r1 = RefCharCreate ( ) ;
 r2 = RefCharCreate ( ) ;
 r2 -> transform [ 0 ] = 1 ;
 r2 -> transform [ 3 ] = 1 ;
 r2 -> transform [ 4 ] = stack [ 1 ] - ( stack [ 0 ] - ret -> lsidebearing ) ;
 r2 -> transform [ 5 ] = stack [ 2 ] ;
 r1 -> transform [ 0 ] = 1 ;
 r1 -> transform [ 3 ] = 1 ;
 r1 -> adobe_enc = stack [ 3 ] ;
 r2 -> adobe_enc = stack [ 4 ] ;
 if ( stack [ 3 ] < 0 || stack [ 3 ] >= 256 || stack [ 4 ] < 0 || stack [ 4 ] >= 256 ) {
 LogError ( _ ( "Reference encoding out of bounds in %s\n" ) , name ) ;
 r1 -> adobe_enc = 0 ;
 r2 -> adobe_enc = 0 ;
 }
 r1 -> next = r2 ;
 if ( rlast != NULL ) rlast -> next = r1 ;
 else ret -> layers [ ly_fore ] . refs = r1 ;
 ret -> changedsincelasthinted = true ;
 rlast = r2 ;
 sp = 0 ;
 break ;
 case 7 : if ( sp < 4 ) LogError ( _ ( "Stack underflow on sbw in %s\n" ) , name ) ;
 if ( is_type2 ) LogError ( _ ( "%s\'s sbw operator is not supported for Type2\n" ) , name ) ;
 ret -> lsidebearing = stack [ 0 ] ;
 ret -> width = stack [ 2 ] ;
 sp = 0 ;
 break ;
 case 5 : case 9 : case 14 : case 26 : if ( sp < 1 ) LogError ( _ ( "Stack underflow on unary operator in %s\n" ) , name ) ;
 switch ( v ) {
 case 5 : stack [ sp - 1 ] = ( stack [ sp - 1 ] == 0 ) ;
 break ;
 case 9 : if ( stack [ sp - 1 ] < 0 ) stack [ sp - 1 ] = - stack [ sp - 1 ] ;
 break ;
 case 14 : stack [ sp - 1 ] = - stack [ sp - 1 ] ;
 break ;
 case 26 : stack [ sp - 1 ] = sqrt ( stack [ sp - 1 ] ) ;
 break ;
 default : break ;
 }
 break ;
 case 3 : case 4 : case 10 : case 11 : case 12 : case 15 : case 24 : if ( sp < 2 ) LogError ( _ ( "Stack underflow on binary operator in %s\n" ) , name ) ;
 else switch ( v ) {
 case 3 : stack [ sp - 2 ] = ( stack [ sp - 1 ] != 0 && stack [ sp - 2 ] != 0 ) ;
 break ;
 case 4 : stack [ sp - 2 ] = ( stack [ sp - 1 ] != 0 || stack [ sp - 2 ] != 0 ) ;
 break ;
 case 10 : stack [ sp - 2 ] += stack [ sp - 1 ] ;
 break ;
 case 11 : stack [ sp - 2 ] -= stack [ sp - 1 ] ;
 break ;
 case 12 : stack [ sp - 2 ] /= stack [ sp - 1 ] ;
 break ;
 case 24 : stack [ sp - 2 ] *= stack [ sp - 1 ] ;
 break ;
 case 15 : stack [ sp - 2 ] = ( stack [ sp - 1 ] == stack [ sp - 2 ] ) ;
 break ;
 default : break ;
 }
 -- sp ;
 break ;
 case 22 : if ( sp < 4 ) LogError ( _ ( "Stack underflow on ifelse in %s\n" ) , name ) ;
 else {
 if ( stack [ sp - 2 ] > stack [ sp - 1 ] ) stack [ sp - 4 ] = stack [ sp - 3 ] ;
 sp -= 3 ;
 }
 break ;
 case 23 : do {
 stack [ sp ] = ( rand ( ) / ( RAND_MAX - 1 ) ) ;
 }
 while ( stack [ sp ] == 0 || stack [ sp ] > 1 ) ;
 ++ sp ;
 break ;
 case 16 : if ( is_type2 ) LogError ( _ ( "Type2 fonts do not support the Type1 callothersubrs operator" ) ) ;
 if ( sp < 2 || sp < 2 + stack [ sp - 2 ] ) {
 LogError ( _ ( "Stack underflow on callothersubr in %s\n" ) , name ) ;
 sp = 0 ;
 }
 else {
 int tot = stack [ sp - 2 ] , i , k , j ;
 popsp = 0 ;
 for ( k = sp - 3 ;
 k >= sp - 2 - tot ;
 -- k ) pops [ popsp ++ ] = stack [ k ] ;
 switch ( ( int ) stack [ sp - 1 ] ) {
 case 3 : {
 ret -> manualhints = false ;
 ret -> hstem = HintsAppend ( ret -> hstem , activeh ) ;
 activeh = NULL ;
 ret -> vstem = HintsAppend ( ret -> vstem , activev ) ;
 activev = NULL ;
 }
 break ;
 case 1 : {
 is_type2 = false ;
 if ( cur != NULL ) {
 oldcur = cur ;
 cur -> next = NULL ;
 }
 else LogError ( _ ( "Bad flex subroutine in %s\n" ) , name ) ;
 }
 break ;
 case 2 : {
 ;
 }
 break ;
 case 0 : if ( oldcur != NULL ) {
 SplinePointList * spl = oldcur -> next ;
 if ( spl != NULL && spl -> next != NULL && spl -> next -> next != NULL && spl -> next -> next -> next != NULL && spl -> next -> next -> next -> next != NULL && spl -> next -> next -> next -> next -> next != NULL && spl -> next -> next -> next -> next -> next -> next != NULL ) {
 BasePoint old_nextcp , mid_prevcp , mid , mid_nextcp , end_prevcp , end ;
 old_nextcp = spl -> next -> first -> me ;
 mid_prevcp = spl -> next -> next -> first -> me ;
 mid = spl -> next -> next -> next -> first -> me ;
 mid_nextcp = spl -> next -> next -> next -> next -> first -> me ;
 end_prevcp = spl -> next -> next -> next -> next -> next -> first -> me ;
 end = spl -> next -> next -> next -> next -> next -> next -> first -> me ;
 cur = oldcur ;
 if ( cur != NULL && cur -> first != NULL && ( cur -> first != cur -> last || cur -> first -> next == NULL ) ) {
 cur -> last -> nextcp = old_nextcp ;
 cur -> last -> nonextcp = false ;
 pt = chunkalloc ( sizeof ( SplinePoint ) ) ;
 pt -> hintmask = pending_hm ;
 pending_hm = NULL ;
 pt -> prevcp = mid_prevcp ;
 pt -> me = mid ;
 pt -> nextcp = mid_nextcp ;
 CheckMake ( cur -> last , pt ) ;
 SplineMake3 ( cur -> last , pt ) ;
 cur -> last = pt ;
 pt = chunkalloc ( sizeof ( SplinePoint ) ) ;
 pt -> prevcp = end_prevcp ;
 pt -> me = end ;
 pt -> nonextcp = true ;
 CheckMake ( cur -> last , pt ) ;
 SplineMake3 ( cur -> last , pt ) ;
 cur -> last = pt ;
 }
 else LogError ( _ ( "No previous point on path in curveto from flex 0 in %s\n" ) , name ) ;
 }
 else {
 pt = chunkalloc ( sizeof ( SplinePoint ) ) ;
 pt -> me . x = pops [ 1 ] ;
 pt -> me . y = pops [ 0 ] ;
 pt -> noprevcp = true ;
 pt -> nonextcp = true ;
 SplinePointListFree ( oldcur -> next ) ;
 oldcur -> next = NULL ;
 spl = NULL ;
 cur = oldcur ;
 if ( cur != NULL && cur -> first != NULL && ( cur -> first != cur -> last || cur -> first -> next == NULL ) ) {
 CheckMake ( cur -> last , pt ) ;
 SplineMake3 ( cur -> last , pt ) ;
 cur -> last = pt ;
 }
 else LogError ( _ ( "No previous point on path in lineto from flex 0 in %s\n" ) , name ) ;
 }
 -- popsp ;
 cur -> next = NULL ;
 SplinePointListsFree ( spl ) ;
 oldcur = NULL ;
 }
 else LogError ( _ ( "Bad flex subroutine in %s\n" ) , name ) ;
 is_type2 = context -> is_type2 ;
 break ;
 case 14 : case 15 : case 16 : case 17 : case 18 : {
 int cnt = stack [ sp - 1 ] - 13 ;
 if ( cnt == 5 ) cnt = 6 ;
 if ( context -> instance_count == 0 ) LogError ( _ ( "Attempt to use a multiple master subroutine in a non-mm font in %s.\n" ) , name ) ;
 else if ( tot != cnt * context -> instance_count ) LogError ( _ ( "Multiple master subroutine called with the wrong number of arguments in %s.\n" ) , name ) ;
 else {
 if ( cnt == 1 && ! is_type2 ) {
 if ( sp - 2 - tot >= 1 && ( ! old_last_was_b1 || stack [ 0 ] != Blend ( unblended [ 1 ] , context ) ) ) {
 unblended [ 0 ] [ 0 ] = stack [ 0 ] ;
 for ( i = 1 ;
 i < context -> instance_count ;
 ++ i ) unblended [ 0 ] [ i ] = 0 ;
 }
 else memcpy ( unblended , unblended + 1 , context -> instance_count * sizeof ( real ) ) ;
 for ( j = 0 ;
 j < context -> instance_count ;
 ++ j ) unblended [ 1 ] [ j ] = stack [ sp - 2 - tot + j ] ;
 }
 else if ( cnt == 2 && ! is_type2 ) {
 unblended [ 0 ] [ 0 ] = stack [ sp - 2 - tot ] ;
 unblended [ 1 ] [ 0 ] = stack [ sp - 2 - tot + 1 ] ;
 for ( i = 0 ;
 i < 2 ;
 ++ i ) for ( j = 1 ;
 j < context -> instance_count ;
 ++ j ) unblended [ i ] [ j ] = stack [ sp - 2 - tot + 2 + i * ( context -> instance_count - 1 ) + ( j - 1 ) ] ;
 }
 popsp = 0 ;
 for ( i = 0 ;
 i < cnt ;
 ++ i ) {
 double sum = stack [ sp - 2 - tot + i ] ;
 for ( j = 1 ;
 j < context -> instance_count ;
 ++ j ) sum += context -> blend_values [ j ] * stack [ sp - 2 - tot + cnt + i * ( context -> instance_count - 1 ) + j - 1 ] ;
 pops [ cnt - 1 - popsp ++ ] = sum ;
 }
 }
 }
 break ;
 }
 sp = k + 1 ;
 }
 break ;
 case 20 : if ( sp < 2 ) LogError ( _ ( "Too few items on stack for put in %s\n" ) , name ) ;
 else if ( stack [ sp - 1 ] < 0 || stack [ sp - 1 ] >= 32 ) LogError ( _ ( "Reference to transient memory out of bounds in put in %s\n" ) , name ) ;
 else {
 transient [ ( int ) stack [ sp - 1 ] ] = stack [ sp - 2 ] ;
 sp -= 2 ;
 }
 break ;
 case 21 : if ( sp < 1 ) LogError ( _ ( "Too few items on stack for get in %s\n" ) , name ) ;
 else if ( stack [ sp - 1 ] < 0 || stack [ sp - 1 ] >= 32 ) LogError ( _ ( "Reference to transient memory out of bounds in put in %s\n" ) , name ) ;
 else stack [ sp - 1 ] = transient [ ( int ) stack [ sp - 1 ] ] ;
 break ;
 case 17 : if ( popsp <= 0 ) LogError ( _ ( "Pop stack underflow on pop in %s\n" ) , name ) ;
 else stack [ sp ++ ] = pops [ -- popsp ] ;
 break ;
 case 18 : if ( sp > 0 ) -- sp ;
 break ;
 case 27 : if ( sp >= 1 ) {
 stack [ sp ] = stack [ sp - 1 ] ;
 ++ sp ;
 }
 break ;
 case 28 : if ( sp >= 2 ) {
 real temp = stack [ sp - 1 ] ;
 stack [ sp - 1 ] = stack [ sp - 2 ] ;
 stack [ sp - 2 ] = temp ;
 }
 break ;
 case 29 : if ( sp >= 1 ) {
 int index = stack [ -- sp ] ;
 if ( index < 0 || sp < index + 1 ) LogError ( _ ( "Index out of range in %s\n" ) , name ) ;
 else {
 stack [ sp ] = stack [ sp - index - 1 ] ;
 ++ sp ;
 }
 }
 break ;
 case 30 : if ( sp >= 2 ) {
 int j = stack [ sp - 1 ] , N = stack [ sp - 2 ] ;
 if ( N > sp || j >= N || j < 0 || N < 0 ) LogError ( _ ( "roll out of range in %s\n" ) , name ) ;
 else if ( j == 0 || N == 0 ) ;
 else {
 real * temp = malloc ( N * sizeof ( real ) ) ;
 int i ;
 for ( i = 0 ;
 i < N ;
 ++ i ) temp [ i ] = stack [ sp - N + i ] ;
 for ( i = 0 ;
 i < N ;
 ++ i ) stack [ sp - N + i ] = temp [ ( i + j ) % N ] ;
 free ( temp ) ;
 }
 }
 break ;
 case 33 : if ( is_type2 ) LogError ( _ ( "Type2 fonts do not support the Type1 setcurrentpoint operator" ) ) ;
 if ( sp < 2 ) LogError ( _ ( "Stack underflow on setcurrentpoint in %s\n" ) , name ) ;
 else {
 current . x = stack [ 0 ] ;
 current . y = stack [ 1 ] ;
 }
 sp = 0 ;
 break ;
 case 34 : case 35 : case 36 : case 37 : dy = dy3 = dy4 = dy5 = dy6 = 0 ;
 dx = stack [ base ++ ] ;
 if ( v != 34 ) dy = stack [ base ++ ] ;
 dx2 = stack [ base ++ ] ;
 dy2 = stack [ base ++ ] ;
 dx3 = stack [ base ++ ] ;
 if ( v != 34 && v != 36 ) dy3 = stack [ base ++ ] ;
 dx4 = stack [ base ++ ] ;
 if ( v != 34 && v != 36 ) dy4 = stack [ base ++ ] ;
 dx5 = stack [ base ++ ] ;
 if ( v == 34 ) dy5 = - dy2 ;
 else dy5 = stack [ base ++ ] ;
 switch ( v ) {
 real xt , yt ;
 case 35 : dx6 = stack [ base ++ ] ;
 dy6 = stack [ base ++ ] ;
 break ;
 case 34 : dx6 = stack [ base ++ ] ;
 break ;
 case 36 : dx6 = stack [ base ++ ] ;
 dy6 = - dy - dy2 - dy5 ;
 break ;
 case 37 : xt = dx + dx2 + dx3 + dx4 + dx5 ;
 yt = dy + dy2 + dy3 + dy4 + dy5 ;
 if ( xt < 0 ) xt = - xt ;
 if ( yt < 0 ) yt = - yt ;
 if ( xt > yt ) {
 dx6 = stack [ base ++ ] ;
 dy6 = - dy - dy2 - dy3 - dy4 - dy5 ;
 }
 else {
 dy6 = stack [ base ++ ] ;
 dx6 = - dx - dx2 - dx3 - dx4 - dx5 ;
 }
 break ;
 }
 if ( cur != NULL && cur -> first != NULL && ( cur -> first != cur -> last || cur -> first -> next == NULL ) ) {
 current . x = rint ( ( current . x + dx ) * 1024 ) / 1024 ;
 current . y = rint ( ( current . y + dy ) * 1024 ) / 1024 ;
 cur -> last -> nextcp . x = current . x ;
 cur -> last -> nextcp . y = current . y ;
 cur -> last -> nonextcp = false ;
 current . x = rint ( ( current . x + dx2 ) * 1024 ) / 1024 ;
 current . y = rint ( ( current . y + dy2 ) * 1024 ) / 1024 ;
 pt = chunkalloc ( sizeof ( SplinePoint ) ) ;
 pt -> hintmask = pending_hm ;
 pending_hm = NULL ;
 pt -> prevcp . x = current . x ;
 pt -> prevcp . y = current . y ;
 current . x = rint ( ( current . x + dx3 ) * 1024 ) / 1024 ;
 current . y = rint ( ( current . y + dy3 ) * 1024 ) / 1024 ;
 pt -> me . x = current . x ;
 pt -> me . y = current . y ;
 pt -> nonextcp = true ;
 CheckMake ( cur -> last , pt ) ;
 SplineMake3 ( cur -> last , pt ) ;
 cur -> last = pt ;
 current . x = rint ( ( current . x + dx4 ) * 1024 ) / 1024 ;
 current . y = rint ( ( current . y + dy4 ) * 1024 ) / 1024 ;
 cur -> last -> nextcp . x = current . x ;
 cur -> last -> nextcp . y = current . y ;
 cur -> last -> nonextcp = false ;
 current . x = rint ( ( current . x + dx5 ) * 1024 ) / 1024 ;
 current . y = rint ( ( current . y + dy5 ) * 1024 ) / 1024 ;
 pt = chunkalloc ( sizeof ( SplinePoint ) ) ;
 pt -> prevcp . x = current . x ;
 pt -> prevcp . y = current . y ;
 current . x = rint ( ( current . x + dx6 ) * 1024 ) / 1024 ;
 current . y = rint ( ( current . y + dy6 ) * 1024 ) / 1024 ;
 pt -> me . x = current . x ;
 pt -> me . y = current . y ;
 pt -> nonextcp = true ;
 CheckMake ( cur -> last , pt ) ;
 SplineMake3 ( cur -> last , pt ) ;
 cur -> last = pt ;
 }
 else LogError ( _ ( "No previous point on path in flex operator in %s\n" ) , name ) ;
 sp = 0 ;
 break ;
 default : LogError ( _ ( "Uninterpreted opcode 12,%d in %s\n" ) , v , name ) ;
 break ;
 }
 }
 else {
 last_was_b1 = false ;
 switch ( v ) {
 case 1 : case 18 : base = 0 ;
 if ( ( sp & 1 ) && ret -> width == ( int16 ) 0x8000 ) ret -> width = stack [ 0 ] ;
 if ( sp & 1 ) base = 1 ;
 if ( sp - base < 2 ) LogError ( _ ( "Stack underflow on hstem in %s\n" ) , name ) ;
 coord = 0 ;
 hp = NULL ;
 if ( activeh != NULL ) for ( hp = activeh ;
 hp -> next != NULL ;
 hp = hp -> next ) ;
 while ( sp - base >= 2 ) {
 sameh = NULL ;
 if ( ! is_type2 ) sameh = SameH ( ret -> hstem , stack [ base ] + coord , stack [ base + 1 ] , unblended , context -> instance_count ) ;
 hint = HintNew ( stack [ base ] + coord , stack [ base + 1 ] ) ;
 hint -> hintnumber = sameh != NULL ? sameh -> hintnumber : hint_cnt ++ ;
 if ( ! is_type2 && context -> instance_count != 0 ) {
 hint -> u . unblended = chunkalloc ( sizeof ( real [ 2 ] [ MmMax ] ) ) ;
 memcpy ( hint -> u . unblended , unblended , sizeof ( real [ 2 ] [ MmMax ] ) ) ;
 }
 if ( activeh == NULL ) activeh = hint ;
 else hp -> next = hint ;
 hp = hint ;
 if ( ! is_type2 && hint -> hintnumber < 96 ) {
 if ( pending_hm == NULL ) pending_hm = chunkalloc ( sizeof ( HintMask ) ) ;
 ( * pending_hm ) [ hint -> hintnumber >> 3 ] |= 0x80 >> ( hint -> hintnumber & 0x7 ) ;
 }
 base += 2 ;
 coord = hint -> start + hint -> width ;
 }
 sp = 0 ;
 break ;
 case 19 : case 20 : case 3 : case 23 : base = 0 ;
 if ( cur == NULL || v == 3 || v == 23 ) {
 if ( ( sp & 1 ) && is_type2 && ret -> width == ( int16 ) 0x8000 ) {
 ret -> width = stack [ 0 ] ;
 }
 if ( sp & 1 ) base = 1 ;
 if ( sp - base < 2 && v != 19 && v != 20 ) LogError ( _ ( "Stack underflow on vstem in %s\n" ) , name ) ;
 coord = ret -> lsidebearing ;
 hp = NULL ;
 if ( activev != NULL ) for ( hp = activev ;
 hp -> next != NULL ;
 hp = hp -> next ) ;
 while ( sp - base >= 2 ) {
 sameh = NULL ;
 if ( ! is_type2 ) sameh = SameH ( ret -> vstem , stack [ base ] + coord , stack [ base + 1 ] , unblended , context -> instance_count ) ;
 hint = HintNew ( stack [ base ] + coord , stack [ base + 1 ] ) ;
 hint -> hintnumber = sameh != NULL ? sameh -> hintnumber : hint_cnt ++ ;
 if ( ! is_type2 && context -> instance_count != 0 ) {
 hint -> u . unblended = chunkalloc ( sizeof ( real [ 2 ] [ MmMax ] ) ) ;
 memcpy ( hint -> u . unblended , unblended , sizeof ( real [ 2 ] [ MmMax ] ) ) ;
 }
 if ( ! is_type2 && hint -> hintnumber < 96 ) {
 if ( pending_hm == NULL ) pending_hm = chunkalloc ( sizeof ( HintMask ) ) ;
 ( * pending_hm ) [ hint -> hintnumber >> 3 ] |= 0x80 >> ( hint -> hintnumber & 0x7 ) ;
 }
 if ( activev == NULL ) activev = hint ;
 else hp -> next = hint ;
 hp = hint ;
 base += 2 ;
 coord = hint -> start + hint -> width ;
 }
 sp = 0 ;
 }
 if ( v == 19 || v == 20 ) {
 int bytes = ( hint_cnt + 7 ) / 8 ;
 if ( bytes > sizeof ( HintMask ) ) bytes = sizeof ( HintMask ) ;
 if ( v == 19 ) {
 ret -> hstem = HintsAppend ( ret -> hstem , activeh ) ;
 activeh = NULL ;
 ret -> vstem = HintsAppend ( ret -> vstem , activev ) ;
 activev = NULL ;
 if ( pending_hm == NULL ) pending_hm = chunkalloc ( sizeof ( HintMask ) ) ;
 memcpy ( pending_hm , type1 , bytes ) ;
 }
 else if ( cp < sizeof ( counters ) / sizeof ( counters [ 0 ] ) ) {
 counters [ cp ] = chunkalloc ( sizeof ( HintMask ) ) ;
 memcpy ( counters [ cp ] , type1 , bytes ) ;
 ++ cp ;
 }
 if ( bytes != hint_cnt / 8 ) {
 int mask = 0xff >> ( hint_cnt & 7 ) ;
 if ( type1 [ bytes - 1 ] & mask ) LogError ( _ ( "Hint mask (or counter mask) with too many hints in %s\n" ) , name ) ;
 }
 type1 += bytes ;
 len -= bytes ;
 }
 break ;
 case 14 : if ( ( sp & 1 ) && is_type2 && ret -> width == ( int16 ) 0x8000 ) ret -> width = stack [ 0 ] ;
 if ( context -> painttype != 2 ) closepath ( cur , is_type2 ) ;
 pcsp = 0 ;
 if ( sp == 4 ) {
 stack [ 4 ] = stack [ 3 ] ;
 stack [ 3 ] = stack [ 2 ] ;
 stack [ 2 ] = stack [ 1 ] ;
 stack [ 1 ] = stack [ 0 ] ;
 stack [ 0 ] = 0 ;
 sp = 5 ;
 goto seac ;
 }
 else if ( sp == 5 ) {
 stack [ 0 ] = 0 ;
 goto seac ;
 }
 goto done ;
 break ;
 case 13 : if ( sp < 2 ) LogError ( _ ( "Stack underflow on hsbw in %s\n" ) , name ) ;
 ret -> lsidebearing = stack [ 0 ] ;
 current . x = stack [ 0 ] ;
 ret -> width = stack [ 1 ] ;
 sp = 0 ;
 break ;
 case 9 : sp = 0 ;
 closepath ( cur , is_type2 ) ;
 break ;
 case 21 : case 22 : case 4 : if ( is_type2 ) {
 if ( ( ( v == 21 && sp == 3 ) || ( v != 21 && sp == 2 ) ) && ret -> width == ( int16 ) 0x8000 ) ret -> width = stack [ 0 ] ;
 if ( v == 21 && sp > 2 ) {
 stack [ 0 ] = stack [ sp - 2 ] ;
 stack [ 1 ] = stack [ sp - 1 ] ;
 sp = 2 ;
 }
 else if ( v != 21 && sp > 1 ) {
 stack [ 0 ] = stack [ sp - 1 ] ;
 sp = 1 ;
 }
 if ( context -> painttype != 2 ) closepath ( cur , true ) ;
 }
 case 5 : case 6 : case 7 : polarity = 0 ;
 base = 0 ;
 while ( base < sp ) {
 dx = dy = 0 ;
 if ( v == 5 || v == 21 ) {
 if ( sp < base + 2 ) {
 LogError ( _ ( "Stack underflow on rlineto/rmoveto in %s\n" ) , name ) ;
 break ;
 }
 dx = stack [ base ++ ] ;
 dy = stack [ base ++ ] ;
 }
 else if ( ( v == 6 && ! ( polarity & 1 ) ) || ( v == 7 && ( polarity & 1 ) ) || v == 22 ) {
 if ( sp <= base ) {
 LogError ( _ ( "Stack underflow on hlineto/hmoveto in %s\n" ) , name ) ;
 break ;
 }
 dx = stack [ base ++ ] ;
 }
 else {
 if ( sp <= base ) {
 LogError ( _ ( "Stack underflow on vlineto/vmoveto in %s\n" ) , name ) ;
 break ;
 }
 dy = stack [ base ++ ] ;
 }
 ++ polarity ;
 current . x = rint ( ( current . x + dx ) * 1024 ) / 1024 ;
 current . y = rint ( ( current . y + dy ) * 1024 ) / 1024 ;
 pt = chunkalloc ( sizeof ( SplinePoint ) ) ;
 pt -> hintmask = pending_hm ;
 pending_hm = NULL ;
 pt -> me . x = current . x ;
 pt -> me . y = current . y ;
 pt -> noprevcp = true ;
 pt -> nonextcp = true ;
 if ( v == 4 || v == 21 || v == 22 ) {
 if ( cur != NULL && cur -> first == cur -> last && cur -> first -> prev == NULL && is_type2 ) {
 cur -> first -> me . x = current . x ;
 cur -> first -> me . y = current . y ;
 SplinePointFree ( pt ) ;
 }
 else {
 SplinePointList * spl = chunkalloc ( sizeof ( SplinePointList ) ) ;
 spl -> first = spl -> last = pt ;
 if ( cur != NULL ) cur -> next = spl ;
 else ret -> layers [ ly_fore ] . splines = spl ;
 cur = spl ;
 }
 break ;
 }
 else {
 if ( cur != NULL && cur -> first != NULL && ( cur -> first != cur -> last || cur -> first -> next == NULL ) ) {
 CheckMake ( cur -> last , pt ) ;
 SplineMake3 ( cur -> last , pt ) ;
 cur -> last = pt ;
 }
 else LogError ( _ ( "No previous point on path in lineto in %s\n" ) , name ) ;
 if ( ! is_type2 ) break ;
 }
 }
 sp = 0 ;
 break ;
 case 25 : base = 0 ;
 while ( sp > base + 6 ) {
 current . x = rint ( ( current . x + stack [ base ++ ] ) * 1024 ) / 1024 ;
 current . y = rint ( ( current . y + stack [ base ++ ] ) * 1024 ) / 1024 ;
 if ( cur != NULL ) {
 pt = chunkalloc ( sizeof ( SplinePoint ) ) ;
 pt -> hintmask = pending_hm ;
 pending_hm = NULL ;
 pt -> me . x = current . x ;
 pt -> me . y = current . y ;
 pt -> noprevcp = true ;
 pt -> nonextcp = true ;
 CheckMake ( cur -> last , pt ) ;
 SplineMake3 ( cur -> last , pt ) ;
 cur -> last = pt ;
 }
 }
 case 24 : case 8 : case 31 : case 30 : case 27 : case 26 : polarity = 0 ;
 while ( sp > base + 2 ) {
 dx = dy = dx2 = dy2 = dx3 = dy3 = 0 ;
 if ( v == 8 || v == 25 || v == 24 ) {
 if ( sp < 6 + base ) {
 LogError ( _ ( "Stack underflow on rrcurveto in %s\n" ) , name ) ;
 base = sp ;
 }
 else {
 dx = stack [ base ++ ] ;
 dy = stack [ base ++ ] ;
 dx2 = stack [ base ++ ] ;
 dy2 = stack [ base ++ ] ;
 dx3 = stack [ base ++ ] ;
 dy3 = stack [ base ++ ] ;
 }
 }
 else if ( v == 27 ) {
 if ( sp < 4 + base ) {
 LogError ( _ ( "Stack underflow on hhcurveto in %s\n" ) , name ) ;
 base = sp ;
 }
 else {
 if ( ( sp - base ) & 1 ) dy = stack [ base ++ ] ;
 dx = stack [ base ++ ] ;
 dx2 = stack [ base ++ ] ;
 dy2 = stack [ base ++ ] ;
 dx3 = stack [ base ++ ] ;
 }
 }
 else if ( v == 26 ) {
 if ( sp < 4 + base ) {
 LogError ( _ ( "Stack underflow on hhcurveto in %s\n" ) , name ) ;
 base = sp ;
 }
 else {
 if ( ( sp - base ) & 1 ) dx = stack [ base ++ ] ;
 dy = stack [ base ++ ] ;
 dx2 = stack [ base ++ ] ;
 dy2 = stack [ base ++ ] ;
 dy3 = stack [ base ++ ] ;
 }
 }
 else if ( ( v == 31 && ! ( polarity & 1 ) ) || ( v == 30 && ( polarity & 1 ) ) ) {
 if ( sp < 4 + base ) {
 LogError ( _ ( "Stack underflow on hvcurveto in %s\n" ) , name ) ;
 base = sp ;
 }
 else {
 dx = stack [ base ++ ] ;
 dx2 = stack [ base ++ ] ;
 dy2 = stack [ base ++ ] ;
 dy3 = stack [ base ++ ] ;
 if ( sp == base + 1 ) dx3 = stack [ base ++ ] ;
 }
 }
 else {
 if ( sp < 4 + base ) {
 LogError ( _ ( "Stack underflow on vhcurveto in %s\n" ) , name ) ;
 base = sp ;
 }
 else {
 dy = stack [ base ++ ] ;
 dx2 = stack [ base ++ ] ;
 dy2 = stack [ base ++ ] ;
 dx3 = stack [ base ++ ] ;
 if ( sp == base + 1 ) dy3 = stack [ base ++ ] ;
 }
 }
 ++ polarity ;
 if ( cur != NULL && cur -> first != NULL && ( cur -> first != cur -> last || cur -> first -> next == NULL ) ) {
 current . x = rint ( ( current . x + dx ) * 1024 ) / 1024 ;
 current . y = rint ( ( current . y + dy ) * 1024 ) / 1024 ;
 cur -> last -> nextcp . x = current . x ;
 cur -> last -> nextcp . y = current . y ;
 cur -> last -> nonextcp = false ;
 current . x = rint ( ( current . x + dx2 ) * 1024 ) / 1024 ;
 current . y = rint ( ( current . y + dy2 ) * 1024 ) / 1024 ;
 pt = chunkalloc ( sizeof ( SplinePoint ) ) ;
 pt -> hintmask = pending_hm ;
 pending_hm = NULL ;
 pt -> prevcp . x = current . x ;
 pt -> prevcp . y = current . y ;
 current . x = rint ( ( current . x + dx3 ) * 1024 ) / 1024 ;
 current . y = rint ( ( current . y + dy3 ) * 1024 ) / 1024 ;
 pt -> me . x = current . x ;
 pt -> me . y = current . y ;
 pt -> nonextcp = true ;
 CheckMake ( cur -> last , pt ) ;
 SplineMake3 ( cur -> last , pt ) ;
 cur -> last = pt ;
 }
 else LogError ( _ ( "No previous point on path in curveto in %s\n" ) , name ) ;
 }
 if ( v == 24 ) {
 current . x = rint ( ( current . x + stack [ base ++ ] ) * 1024 ) / 1024 ;
 current . y = rint ( ( current . y + stack [ base ++ ] ) * 1024 ) / 1024 ;
 if ( cur != NULL ) {
 pt = chunkalloc ( sizeof ( SplinePoint ) ) ;
 pt -> hintmask = pending_hm ;
 pending_hm = NULL ;
 pt -> me . x = current . x ;
 pt -> me . y = current . y ;
 pt -> noprevcp = true ;
 pt -> nonextcp = true ;
 CheckMake ( cur -> last , pt ) ;
 SplineMake3 ( cur -> last , pt ) ;
 cur -> last = pt ;
 }
 }
 sp = 0 ;
 break ;
 case 29 : case 10 : if ( sp < 1 ) {
 LogError ( _ ( "Stack underflow on callsubr in %s\n" ) , name ) ;
 break ;
 }
 else if ( pcsp > 10 ) {
 LogError ( _ ( "Too many subroutine calls in %s\n" ) , name ) ;
 break ;
 }
 s = subrs ;
 if ( v == 29 ) s = gsubrs ;
 if ( s != NULL ) stack [ sp - 1 ] += s -> bias ;
 if ( s == NULL || stack [ sp - 1 ] >= s -> cnt || stack [ sp - 1 ] < 0 || s -> values [ ( int ) stack [ sp - 1 ] ] == NULL ) LogError ( _ ( "Subroutine number out of bounds in %s\n" ) , name ) ;
 else {
 pcstack [ pcsp ] . type1 = type1 ;
 pcstack [ pcsp ] . len = len ;
 pcstack [ pcsp ] . subnum = stack [ sp - 1 ] ;
 ++ pcsp ;
 type1 = s -> values [ ( int ) stack [ sp - 1 ] ] ;
 len = s -> lens [ ( int ) stack [ sp - 1 ] ] ;
 }
 if ( -- sp < 0 ) sp = 0 ;
 break ;
 case 11 : if ( pcsp < 1 ) LogError ( _ ( "return when not in subroutine in %s\n" ) , name ) ;
 else {
 -- pcsp ;
 type1 = pcstack [ pcsp ] . type1 ;
 len = pcstack [ pcsp ] . len ;
 }
 break ;
 case 16 : {
 int cnt , i , j ;
 if ( context -> instance_count == 0 ) LogError ( _ ( "Attempt to use a multiple master subroutine in a non-mm font.\n" ) ) ;
 else if ( sp < 1 || sp < context -> instance_count * stack [ sp - 1 ] + 1 ) LogError ( _ ( "Too few items on stack for blend in %s\n" ) , name ) ;
 else {
 if ( ! context -> blend_warn ) {
 LogError ( _ ( "Use of obsolete blend operator.\n" ) ) ;
 context -> blend_warn = true ;
 }
 cnt = stack [ sp - 1 ] ;
 sp -= context -> instance_count * stack [ sp - 1 ] + 1 ;
 for ( i = 0 ;
 i < cnt ;
 ++ i ) {
 for ( j = 1 ;
 j < context -> instance_count ;
 ++ j ) stack [ sp + i ] += context -> blend_values [ j ] * stack [ sp + cnt + i * ( context -> instance_count - 1 ) + j - 1 ] ;
 }
 sp += cnt ;
 }
 }
 break ;
 default : LogError ( _ ( "Uninterpreted opcode %d in %s\n" ) , v , name ) ;
 break ;
 }
 }
 }
 done : if ( pcsp != 0 ) LogError ( _ ( "end of subroutine reached with no return in %s\n" ) , name ) ;
 SCCategorizePoints ( ret ) ;
 ret -> hstem = HintsAppend ( ret -> hstem , activeh ) ;
 activeh = NULL ;
 ret -> vstem = HintsAppend ( ret -> vstem , activev ) ;
 activev = NULL ;
 if ( cp != 0 ) {
 int i ;
 ret -> countermasks = malloc ( cp * sizeof ( HintMask ) ) ;
 ret -> countermask_cnt = cp ;
 for ( i = 0 ;
 i < cp ;
 ++ i ) {
 memcpy ( & ret -> countermasks [ i ] , counters [ i ] , sizeof ( HintMask ) ) ;
 chunkfree ( counters [ i ] , sizeof ( HintMask ) ) ;
 }
 }
 if ( ! is_type2 && ! context -> painttype ) for ( cur = ret -> layers [ ly_fore ] . splines ;
 cur != NULL ;
 cur = cur -> next ) if ( cur -> first -> prev == NULL ) {
 CheckMake ( cur -> last , cur -> first ) ;
 SplineMake3 ( cur -> last , cur -> first ) ;
 cur -> last = cur -> first ;
 }
 for ( cur = ret -> layers [ ly_fore ] . splines ;
 cur != NULL ;
 cur = cur -> next ) SplineSetReverse ( cur ) ;
 if ( ret -> hstem == NULL && ret -> vstem == NULL ) ret -> manualhints = false ;
 if ( ! is_type2 && context -> instance_count != 0 ) {
 UnblendFree ( ret -> hstem ) ;
 UnblendFree ( ret -> vstem ) ;
 }
 ret -> hstem = HintCleanup ( ret -> hstem , true , context -> instance_count ) ;
 ret -> vstem = HintCleanup ( ret -> vstem , true , context -> instance_count ) ;
 SCGuessHHintInstancesList ( ret , ly_fore ) ;
 SCGuessVHintInstancesList ( ret , ly_fore ) ;
 ret -> hconflicts = StemListAnyConflicts ( ret -> hstem ) ;
 ret -> vconflicts = StemListAnyConflicts ( ret -> vstem ) ;
 if ( context -> instance_count == 1 && ! ret -> hconflicts && ! ret -> vconflicts ) SCClearHintMasks ( ret , ly_fore , false ) ;
 HintsRenumber ( ret ) ;
 if ( name != NULL && strcmp ( name , ".notdef" ) != 0 ) ret -> widthset = true ;
 return ( ret ) ;
 }