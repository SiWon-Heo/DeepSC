static int matroska_read_seek ( AVFormatContext * s , int stream_index , int64_t timestamp , int flags ) {
 MatroskaDemuxContext * matroska = s -> priv_data ;
 MatroskaTrack * tracks = matroska -> tracks . elem ;
 AVStream * st = s -> streams [ stream_index ] ;
 int i , index , index_sub , index_min ;
 if ( matroska -> cues_parsing_deferred ) {
 matroska_parse_cues ( matroska ) ;
 matroska -> cues_parsing_deferred = 0 ;
 }
 if ( ! st -> nb_index_entries ) return 0 ;
 timestamp = FFMAX ( timestamp , st -> index_entries [ 0 ] . timestamp ) ;
 if ( ( index = av_index_search_timestamp ( st , timestamp , flags ) ) < 0 ) {
 avio_seek ( s -> pb , st -> index_entries [ st -> nb_index_entries - 1 ] . pos , SEEK_SET ) ;
 matroska -> current_id = 0 ;
 while ( ( index = av_index_search_timestamp ( st , timestamp , flags ) ) < 0 ) {
 matroska_clear_queue ( matroska ) ;
 if ( matroska_parse_cluster ( matroska ) < 0 ) break ;
 }
 }
 matroska_clear_queue ( matroska ) ;
 if ( index < 0 ) return 0 ;
 index_min = index ;
 for ( i = 0 ;
 i < matroska -> tracks . nb_elem ;
 i ++ ) {
 tracks [ i ] . audio . pkt_cnt = 0 ;
 tracks [ i ] . audio . sub_packet_cnt = 0 ;
 tracks [ i ] . audio . buf_timecode = AV_NOPTS_VALUE ;
 tracks [ i ] . end_timecode = 0 ;
 if ( tracks [ i ] . type == MATROSKA_TRACK_TYPE_SUBTITLE && tracks [ i ] . stream -> discard != AVDISCARD_ALL ) {
 index_sub = av_index_search_timestamp ( tracks [ i ] . stream , st -> index_entries [ index ] . timestamp , AVSEEK_FLAG_BACKWARD ) ;
 if ( index_sub >= 0 && st -> index_entries [ index_sub ] . pos < st -> index_entries [ index_min ] . pos && st -> index_entries [ index ] . timestamp - st -> index_entries [ index_sub ] . timestamp < 30000000000 / matroska -> time_scale ) index_min = index_sub ;
 }
 }
 avio_seek ( s -> pb , st -> index_entries [ index_min ] . pos , SEEK_SET ) ;
 matroska -> current_id = 0 ;
 matroska -> skip_to_keyframe = ! ( flags & AVSEEK_FLAG_ANY ) ;
 matroska -> skip_to_timecode = st -> index_entries [ index ] . timestamp ;
 matroska -> done = 0 ;
 ff_update_cur_dts ( s , st , st -> index_entries [ index ] . timestamp ) ;
 return 0 ;
 }