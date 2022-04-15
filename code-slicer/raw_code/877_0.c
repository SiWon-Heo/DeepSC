static int set_directory_record_rr ( unsigned char * bp , int dr_len , struct isoent * isoent , struct iso9660 * iso9660 , enum dir_rec_type t ) {
 unsigned char rr_flag ;
 # define RR_USE_PX 0x01 # define RR_USE_PN 0x02 # define RR_USE_SL 0x04 # define RR_USE_NM 0x08 # define RR_USE_CL 0x10 # define RR_USE_PL 0x20 # define RR_USE_RE 0x40 # define RR_USE_TF 0x80 int length ;
 struct ctl_extr_rec ctl ;
 struct isoent * rr_parent , * pxent ;
 struct isofile * file ;
 bp = extra_open_record ( bp , dr_len , isoent , & ctl ) ;
 if ( t == DIR_REC_PARENT ) {
 rr_parent = isoent -> rr_parent ;
 pxent = isoent -> parent ;
 if ( rr_parent != NULL ) isoent = rr_parent ;
 else isoent = isoent -> parent ;
 }
 else {
 rr_parent = NULL ;
 pxent = isoent ;
 }
 file = isoent -> file ;
 if ( t != DIR_REC_NORMAL ) {
 rr_flag = RR_USE_PX | RR_USE_TF ;
 if ( rr_parent != NULL ) rr_flag |= RR_USE_PL ;
 }
 else {
 rr_flag = RR_USE_PX | RR_USE_NM | RR_USE_TF ;
 if ( archive_entry_filetype ( file -> entry ) == AE_IFLNK ) rr_flag |= RR_USE_SL ;
 if ( isoent -> rr_parent != NULL ) rr_flag |= RR_USE_RE ;
 if ( isoent -> rr_child != NULL ) rr_flag |= RR_USE_CL ;
 if ( archive_entry_filetype ( file -> entry ) == AE_IFCHR || archive_entry_filetype ( file -> entry ) == AE_IFBLK ) rr_flag |= RR_USE_PN ;
 # ifdef COMPAT_MKISOFS if ( isoent -> virtual && isoent -> parent == iso9660 -> primary . rootent && strcmp ( isoent -> file -> basename . s , "rr_moved" ) == 0 ) rr_flag |= RR_USE_RE ;
 # endif }
 if ( t == DIR_REC_SELF && isoent == isoent -> parent ) {
 length = 7 ;
 if ( bp != NULL ) {
 bp [ 1 ] = 'S' ;
 bp [ 2 ] = 'P' ;
 bp [ 3 ] = length ;
 bp [ 4 ] = 1 ;
 bp [ 5 ] = 0xBE ;
 bp [ 6 ] = 0xEF ;
 bp [ 7 ] = 0 ;
 bp += length ;
 }
 extra_tell_used_size ( & ctl , length ) ;
 }
 length = 5 ;
 if ( extra_space ( & ctl ) < length ) bp = extra_next_record ( & ctl , length ) ;
 if ( bp != NULL ) {
 bp [ 1 ] = 'R' ;
 bp [ 2 ] = 'R' ;
 bp [ 3 ] = length ;
 bp [ 4 ] = 1 ;
 bp [ 5 ] = rr_flag ;
 bp += length ;
 }
 extra_tell_used_size ( & ctl , length ) ;
 if ( rr_flag & RR_USE_NM ) {
 size_t nmlen = file -> basename . length ;
 const char * nm = file -> basename . s ;
 size_t nmmax ;
 if ( extra_space ( & ctl ) < 6 ) bp = extra_next_record ( & ctl , 6 ) ;
 if ( bp != NULL ) {
 bp [ 1 ] = 'N' ;
 bp [ 2 ] = 'M' ;
 bp [ 4 ] = 1 ;
 }
 nmmax = extra_space ( & ctl ) ;
 if ( nmmax > 0xff ) nmmax = 0xff ;
 while ( nmlen + 5 > nmmax ) {
 length = ( int ) nmmax ;
 if ( bp != NULL ) {
 bp [ 3 ] = length ;
 bp [ 5 ] = 0x01 ;
 memcpy ( bp + 6 , nm , length - 5 ) ;
 bp += length ;
 }
 nmlen -= length - 5 ;
 nm += length - 5 ;
 extra_tell_used_size ( & ctl , length ) ;
 if ( extra_space ( & ctl ) < 6 ) {
 bp = extra_next_record ( & ctl , 6 ) ;
 nmmax = extra_space ( & ctl ) ;
 if ( nmmax > 0xff ) nmmax = 0xff ;
 }
 if ( bp != NULL ) {
 bp [ 1 ] = 'N' ;
 bp [ 2 ] = 'M' ;
 bp [ 4 ] = 1 ;
 }
 }
 length = 5 + ( int ) nmlen ;
 if ( bp != NULL ) {
 bp [ 3 ] = length ;
 bp [ 5 ] = 0 ;
 memcpy ( bp + 6 , nm , nmlen ) ;
 bp += length ;
 }
 extra_tell_used_size ( & ctl , length ) ;
 }
 if ( rr_flag & RR_USE_PX ) {
 length = 44 ;
 if ( extra_space ( & ctl ) < length ) bp = extra_next_record ( & ctl , length ) ;
 if ( bp != NULL ) {
 mode_t mode ;
 int64_t uid ;
 int64_t gid ;
 mode = archive_entry_mode ( file -> entry ) ;
 uid = archive_entry_uid ( file -> entry ) ;
 gid = archive_entry_gid ( file -> entry ) ;
 if ( iso9660 -> opt . rr == OPT_RR_USEFUL ) {
 mode |= 0444 ;
 # if ! defined ( _WIN32 ) && ! defined ( __CYGWIN__ ) if ( mode & 0111 ) # endif mode |= 0111 ;
 mode &= ~ 0222 ;
 mode &= ~ 07000 ;
 }
 bp [ 1 ] = 'P' ;
 bp [ 2 ] = 'X' ;
 bp [ 3 ] = length ;
 bp [ 4 ] = 1 ;
 set_num_733 ( bp + 5 , mode ) ;
 set_num_733 ( bp + 13 , archive_entry_nlink ( file -> entry ) ) ;
 set_num_733 ( bp + 21 , ( uint32_t ) uid ) ;
 set_num_733 ( bp + 29 , ( uint32_t ) gid ) ;
 if ( pxent -> dir ) set_num_733 ( bp + 37 , pxent -> dir_location ) ;
 else if ( file -> hardlink_target != NULL ) set_num_733 ( bp + 37 , file -> hardlink_target -> cur_content -> location ) ;
 else set_num_733 ( bp + 37 , file -> cur_content -> location ) ;
 bp += length ;
 }
 extra_tell_used_size ( & ctl , length ) ;
 }
 if ( rr_flag & RR_USE_SL ) {
 const char * sl ;
 char sl_last ;
 if ( extra_space ( & ctl ) < 7 ) bp = extra_next_record ( & ctl , 7 ) ;
 sl = file -> symlink . s ;
 sl_last = '\0' ;
 if ( bp != NULL ) {
 bp [ 1 ] = 'S' ;
 bp [ 2 ] = 'L' ;
 bp [ 4 ] = 1 ;
 }
 for ( ;
 ;
 ) {
 unsigned char * nc , * cf , * cl , cldmy = 0 ;
 int sllen , slmax ;
 slmax = extra_space ( & ctl ) ;
 if ( slmax > 0xff ) slmax = 0xff ;
 if ( bp != NULL ) nc = & bp [ 6 ] ;
 else nc = NULL ;
 cf = cl = NULL ;
 sllen = 0 ;
 while ( * sl && sllen + 11 < slmax ) {
 if ( sl_last == '\0' && sl [ 0 ] == '/' ) {
 if ( nc != NULL ) {
 cf = nc ++ ;
 * cf = 0x08 ;
 * nc ++ = 0 ;
 }
 sllen += 2 ;
 sl ++ ;
 sl_last = '/' ;
 cl = NULL ;
 continue ;
 }
 if ( ( ( sl_last == '\0' || sl_last == '/' ) && sl [ 0 ] == '.' && sl [ 1 ] == '.' && ( sl [ 2 ] == '/' || sl [ 2 ] == '\0' ) ) || ( sl [ 0 ] == '/' && sl [ 1 ] == '.' && sl [ 2 ] == '.' && ( sl [ 3 ] == '/' || sl [ 3 ] == '\0' ) ) ) {
 if ( nc != NULL ) {
 cf = nc ++ ;
 * cf = 0x04 ;
 * nc ++ = 0 ;
 }
 sllen += 2 ;
 if ( sl [ 0 ] == '/' ) sl += 3 ;
 else sl += 2 ;
 sl_last = '.' ;
 cl = NULL ;
 continue ;
 }
 if ( ( ( sl_last == '\0' || sl_last == '/' ) && sl [ 0 ] == '.' && ( sl [ 1 ] == '/' || sl [ 1 ] == '\0' ) ) || ( sl [ 0 ] == '/' && sl [ 1 ] == '.' && ( sl [ 2 ] == '/' || sl [ 2 ] == '\0' ) ) ) {
 if ( nc != NULL ) {
 cf = nc ++ ;
 * cf = 0x02 ;
 * nc ++ = 0 ;
 }
 sllen += 2 ;
 if ( sl [ 0 ] == '/' ) sl += 2 ;
 else sl ++ ;
 sl_last = '.' ;
 cl = NULL ;
 continue ;
 }
 if ( sl [ 0 ] == '/' || cl == NULL ) {
 if ( nc != NULL ) {
 cf = nc ++ ;
 * cf = 0 ;
 cl = nc ++ ;
 * cl = 0 ;
 }
 else cl = & cldmy ;
 sllen += 2 ;
 if ( sl [ 0 ] == '/' ) {
 sl_last = * sl ++ ;
 continue ;
 }
 }
 sl_last = * sl ++ ;
 if ( nc != NULL ) {
 * nc ++ = sl_last ;
 ( * cl ) ++ ;
 }
 sllen ++ ;
 }
 if ( * sl ) {
 length = 5 + sllen ;
 if ( bp != NULL ) {
 * cf |= 0x01 ;
 bp [ 3 ] = length ;
 bp [ 5 ] = 0x01 ;
 bp += length ;
 }
 extra_tell_used_size ( & ctl , length ) ;
 if ( extra_space ( & ctl ) < 11 ) bp = extra_next_record ( & ctl , 11 ) ;
 if ( bp != NULL ) {
 bp [ 1 ] = 'S' ;
 bp [ 2 ] = 'L' ;
 bp [ 4 ] = 1 ;
 }
 }
 else {
 length = 5 + sllen ;
 if ( bp != NULL ) {
 bp [ 3 ] = length ;
 bp [ 5 ] = 0 ;
 bp += length ;
 }
 extra_tell_used_size ( & ctl , length ) ;
 break ;
 }
 }
 }
 if ( rr_flag & RR_USE_TF ) {
 # define TF_CREATION 0x01 # define TF_MODIFY 0x02 # define TF_ACCESS 0x04 # define TF_ATTRIBUTES 0x08 # define TF_BACKUP 0x10 # define TF_EXPIRATION 0x20 # define TF_EFFECTIVE 0x40 # define TF_LONG_FORM 0x80 unsigned char tf_flags ;
 length = 5 ;
 tf_flags = 0 ;
 # ifndef COMPAT_MKISOFS if ( archive_entry_birthtime_is_set ( file -> entry ) && archive_entry_birthtime ( file -> entry ) <= archive_entry_mtime ( file -> entry ) ) {
 length += 7 ;
 tf_flags |= TF_CREATION ;
 }
 # endif if ( archive_entry_mtime_is_set ( file -> entry ) ) {
 length += 7 ;
 tf_flags |= TF_MODIFY ;
 }
 if ( archive_entry_atime_is_set ( file -> entry ) ) {
 length += 7 ;
 tf_flags |= TF_ACCESS ;
 }
 if ( archive_entry_ctime_is_set ( file -> entry ) ) {
 length += 7 ;
 tf_flags |= TF_ATTRIBUTES ;
 }
 if ( extra_space ( & ctl ) < length ) bp = extra_next_record ( & ctl , length ) ;
 if ( bp != NULL ) {
 bp [ 1 ] = 'T' ;
 bp [ 2 ] = 'F' ;
 bp [ 3 ] = length ;
 bp [ 4 ] = 1 ;
 bp [ 5 ] = tf_flags ;
 bp += 5 ;
 if ( tf_flags & TF_CREATION ) {
 set_time_915 ( bp + 1 , archive_entry_birthtime ( file -> entry ) ) ;
 bp += 7 ;
 }
 if ( tf_flags & TF_MODIFY ) {
 set_time_915 ( bp + 1 , archive_entry_mtime ( file -> entry ) ) ;
 bp += 7 ;
 }
 if ( tf_flags & TF_ACCESS ) {
 set_time_915 ( bp + 1 , archive_entry_atime ( file -> entry ) ) ;
 bp += 7 ;
 }
 if ( tf_flags & TF_ATTRIBUTES ) {
 set_time_915 ( bp + 1 , archive_entry_ctime ( file -> entry ) ) ;
 bp += 7 ;
 }
 }
 extra_tell_used_size ( & ctl , length ) ;
 }
 if ( rr_flag & RR_USE_RE ) {
 length = 4 ;
 if ( extra_space ( & ctl ) < length ) bp = extra_next_record ( & ctl , length ) ;
 if ( bp != NULL ) {
 bp [ 1 ] = 'R' ;
 bp [ 2 ] = 'E' ;
 bp [ 3 ] = length ;
 bp [ 4 ] = 1 ;
 bp += length ;
 }
 extra_tell_used_size ( & ctl , length ) ;
 }
 if ( rr_flag & RR_USE_PL ) {
 length = 12 ;
 if ( extra_space ( & ctl ) < length ) bp = extra_next_record ( & ctl , length ) ;
 if ( bp != NULL ) {
 bp [ 1 ] = 'P' ;
 bp [ 2 ] = 'L' ;
 bp [ 3 ] = length ;
 bp [ 4 ] = 1 ;
 set_num_733 ( bp + 5 , rr_parent -> dir_location ) ;
 bp += length ;
 }
 extra_tell_used_size ( & ctl , length ) ;
 }
 if ( rr_flag & RR_USE_CL ) {
 length = 12 ;
 if ( extra_space ( & ctl ) < length ) bp = extra_next_record ( & ctl , length ) ;
 if ( bp != NULL ) {
 bp [ 1 ] = 'C' ;
 bp [ 2 ] = 'L' ;
 bp [ 3 ] = length ;
 bp [ 4 ] = 1 ;
 set_num_733 ( bp + 5 , isoent -> rr_child -> dir_location ) ;
 bp += length ;
 }
 extra_tell_used_size ( & ctl , length ) ;
 }
 if ( rr_flag & RR_USE_PN ) {
 length = 20 ;
 if ( extra_space ( & ctl ) < length ) bp = extra_next_record ( & ctl , length ) ;
 if ( bp != NULL ) {
 uint64_t dev ;
 bp [ 1 ] = 'P' ;
 bp [ 2 ] = 'N' ;
 bp [ 3 ] = length ;
 bp [ 4 ] = 1 ;
 dev = ( uint64_t ) archive_entry_rdev ( file -> entry ) ;
 set_num_733 ( bp + 5 , ( uint32_t ) ( dev >> 32 ) ) ;
 set_num_733 ( bp + 13 , ( uint32_t ) ( dev & 0xFFFFFFFF ) ) ;
 bp += length ;
 }
 extra_tell_used_size ( & ctl , length ) ;
 }
 if ( file -> zisofs . header_size ) {
 length = 16 ;
 if ( extra_space ( & ctl ) < length ) bp = extra_next_record ( & ctl , length ) ;
 if ( bp != NULL ) {
 bp [ 1 ] = 'Z' ;
 bp [ 2 ] = 'F' ;
 bp [ 3 ] = length ;
 bp [ 4 ] = 1 ;
 bp [ 5 ] = 'p' ;
 bp [ 6 ] = 'z' ;
 bp [ 7 ] = file -> zisofs . header_size ;
 bp [ 8 ] = file -> zisofs . log2_bs ;
 set_num_733 ( bp + 9 , file -> zisofs . uncompressed_size ) ;
 bp += length ;
 }
 extra_tell_used_size ( & ctl , length ) ;
 }
 if ( t == DIR_REC_SELF && isoent == isoent -> parent ) {
 length = RR_CE_SIZE ;
 if ( bp != NULL ) set_SUSP_CE ( bp + 1 , iso9660 -> location_rrip_er , 0 , RRIP_ER_SIZE ) ;
 extra_tell_used_size ( & ctl , length ) ;
 }
 extra_close_record ( & ctl , 0 ) ;
 return ( ctl . dr_len ) ;
 }