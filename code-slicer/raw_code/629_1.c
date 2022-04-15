static int decode_13 ( AVCodecContext * avctx , DxaDecContext * c , uint8_t * dst , uint8_t * src , uint8_t * ref ) {
 uint8_t * code , * data , * mv , * msk , * tmp , * tmp2 ;
 int i , j , k ;
 int type , x , y , d , d2 ;
 int stride = c -> pic . linesize [ 0 ] ;
 uint32_t mask ;
 code = src + 12 ;
 data = code + ( ( avctx -> width * avctx -> height ) >> 4 ) ;
 mv = data + AV_RB32 ( src + 0 ) ;
 msk = mv + AV_RB32 ( src + 4 ) ;
 for ( j = 0 ;
 j < avctx -> height ;
 j += 4 ) {
 for ( i = 0 ;
 i < avctx -> width ;
 i += 4 ) {
 tmp = dst + i ;
 tmp2 = ref + i ;
 type = * code ++ ;
 switch ( type ) {
 case 4 : x = ( * mv ) >> 4 ;
 if ( x & 8 ) x = 8 - x ;
 y = ( * mv ++ ) & 0xF ;
 if ( y & 8 ) y = 8 - y ;
 tmp2 += x + y * stride ;
 case 0 : case 5 : for ( y = 0 ;
 y < 4 ;
 y ++ ) {
 memcpy ( tmp , tmp2 , 4 ) ;
 tmp += stride ;
 tmp2 += stride ;
 }
 break ;
 case 1 : case 10 : case 11 : case 12 : case 13 : case 14 : case 15 : if ( type == 1 ) {
 mask = AV_RB16 ( msk ) ;
 msk += 2 ;
 }
 else {
 type -= 10 ;
 mask = ( ( msk [ 0 ] & 0xF0 ) << shift1 [ type ] ) | ( ( msk [ 0 ] & 0xF ) << shift2 [ type ] ) ;
 msk ++ ;
 }
 for ( y = 0 ;
 y < 4 ;
 y ++ ) {
 for ( x = 0 ;
 x < 4 ;
 x ++ ) {
 tmp [ x ] = ( mask & 0x8000 ) ? * data ++ : tmp2 [ x ] ;
 mask <<= 1 ;
 }
 tmp += stride ;
 tmp2 += stride ;
 }
 break ;
 case 2 : for ( y = 0 ;
 y < 4 ;
 y ++ ) {
 memset ( tmp , data [ 0 ] , 4 ) ;
 tmp += stride ;
 }
 data ++ ;
 break ;
 case 3 : for ( y = 0 ;
 y < 4 ;
 y ++ ) {
 memcpy ( tmp , data , 4 ) ;
 data += 4 ;
 tmp += stride ;
 }
 break ;
 case 8 : mask = * msk ++ ;
 for ( k = 0 ;
 k < 4 ;
 k ++ ) {
 d = ( ( k & 1 ) << 1 ) + ( ( k & 2 ) * stride ) ;
 d2 = ( ( k & 1 ) << 1 ) + ( ( k & 2 ) * stride ) ;
 tmp2 = ref + i + d2 ;
 switch ( mask & 0xC0 ) {
 case 0x80 : x = ( * mv ) >> 4 ;
 if ( x & 8 ) x = 8 - x ;
 y = ( * mv ++ ) & 0xF ;
 if ( y & 8 ) y = 8 - y ;
 tmp2 += x + y * stride ;
 case 0x00 : tmp [ d + 0 ] = tmp2 [ 0 ] ;
 tmp [ d + 1 ] = tmp2 [ 1 ] ;
 tmp [ d + 0 + stride ] = tmp2 [ 0 + stride ] ;
 tmp [ d + 1 + stride ] = tmp2 [ 1 + stride ] ;
 break ;
 case 0x40 : tmp [ d + 0 ] = data [ 0 ] ;
 tmp [ d + 1 ] = data [ 0 ] ;
 tmp [ d + 0 + stride ] = data [ 0 ] ;
 tmp [ d + 1 + stride ] = data [ 0 ] ;
 data ++ ;
 break ;
 case 0xC0 : tmp [ d + 0 ] = * data ++ ;
 tmp [ d + 1 ] = * data ++ ;
 tmp [ d + 0 + stride ] = * data ++ ;
 tmp [ d + 1 + stride ] = * data ++ ;
 break ;
 }
 mask <<= 2 ;
 }
 break ;
 case 32 : mask = AV_RB16 ( msk ) ;
 msk += 2 ;
 for ( y = 0 ;
 y < 4 ;
 y ++ ) {
 for ( x = 0 ;
 x < 4 ;
 x ++ ) {
 tmp [ x ] = data [ mask & 1 ] ;
 mask >>= 1 ;
 }
 tmp += stride ;
 tmp2 += stride ;
 }
 data += 2 ;
 break ;
 case 33 : case 34 : mask = AV_RB32 ( msk ) ;
 msk += 4 ;
 for ( y = 0 ;
 y < 4 ;
 y ++ ) {
 for ( x = 0 ;
 x < 4 ;
 x ++ ) {
 tmp [ x ] = data [ mask & 3 ] ;
 mask >>= 2 ;
 }
 tmp += stride ;
 tmp2 += stride ;
 }
 data += type - 30 ;
 break ;
 default : av_log ( avctx , AV_LOG_ERROR , "Unknown opcode %d\n" , type ) ;
 return AVERROR_INVALIDDATA ;
 }
 }
 dst += stride * 4 ;
 ref += stride * 4 ;
 }
 return 0 ;
 }