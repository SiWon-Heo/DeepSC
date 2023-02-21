static int zsethalftone5 ( i_ctx_t * i_ctx_p ) {
 os_ptr op = osp ;
 uint count ;
 gs_halftone_component * phtc = 0 ;
 gs_halftone_component * pc ;
 int code = 0 ;
 int j ;
 bool have_default ;
 gs_halftone * pht = 0 ;
 gx_device_halftone * pdht = 0 ;
 ref sprocs [ GS_CLIENT_COLOR_MAX_COMPONENTS + 1 ] ;
 ref tprocs [ GS_CLIENT_COLOR_MAX_COMPONENTS + 1 ] ;
 gs_memory_t * mem ;
 uint edepth = ref_stack_count ( & e_stack ) ;
 int npop = 2 ;
 int dict_enum = dict_first ( op ) ;
 ref rvalue [ 2 ] ;
 int cname , colorant_number ;
 byte * pname ;
 uint name_size ;
 int halftonetype , type = 0 ;
 gs_gstate * pgs = igs ;
 int space_index = r_space_index ( op - 1 ) ;
 mem = ( gs_memory_t * ) idmemory -> spaces_indexed [ space_index ] ;
 check_type ( * op , t_dictionary ) ;
 check_dict_read ( * op ) ;
 check_type ( op [ - 1 ] , t_dictionary ) ;
 check_dict_read ( op [ - 1 ] ) ;
 code = dict_int_param ( op - 1 , "HalftoneType" , 1 , 100 , 0 , & type ) ;
 if ( code < 0 ) return code ;
 halftonetype = ( type == 2 || type == 4 ) ? ht_type_multiple_colorscreen : ht_type_multiple ;
 have_default = false ;
 for ( count = 0 ;
 ;
 ) {
 if ( ( dict_enum = dict_next ( op , dict_enum , rvalue ) ) == - 1 ) break ;
 if ( ! r_has_type ( & rvalue [ 0 ] , t_name ) ) continue ;
 if ( ! r_has_type ( & rvalue [ 1 ] , t_dictionary ) ) continue ;
 cname = name_index ( mem , & rvalue [ 0 ] ) ;
 code = gs_get_colorname_string ( mem , cname , & pname , & name_size ) ;
 if ( code < 0 ) break ;
 colorant_number = gs_cname_to_colorant_number ( pgs , pname , name_size , halftonetype ) ;
 if ( colorant_number < 0 ) continue ;
 else if ( colorant_number == GX_DEVICE_COLOR_MAX_COMPONENTS ) {
 if ( have_default ) return_error ( gs_error_rangecheck ) ;
 have_default = true ;
 }
 count ++ ;
 if ( count > GS_CLIENT_COLOR_MAX_COMPONENTS + 1 ) {
 code = gs_note_error ( gs_error_rangecheck ) ;
 break ;
 }
 }
 if ( count == 0 || ( halftonetype == ht_type_multiple && ! have_default ) ) code = gs_note_error ( gs_error_rangecheck ) ;
 if ( code >= 0 ) {
 check_estack ( 5 ) ;
 refset_null ( sprocs , count ) ;
 refset_null ( tprocs , count ) ;
 rc_alloc_struct_0 ( pht , gs_halftone , & st_halftone , imemory , pht = 0 , ".sethalftone5" ) ;
 phtc = gs_alloc_struct_array ( mem , count , gs_halftone_component , & st_ht_component_element , ".sethalftone5" ) ;
 rc_alloc_struct_0 ( pdht , gx_device_halftone , & st_device_halftone , imemory , pdht = 0 , ".sethalftone5" ) ;
 if ( pht == 0 || phtc == 0 || pdht == 0 ) {
 j = 0 ;
 code = gs_note_error ( gs_error_VMerror ) ;
 }
 }
 if ( code >= 0 ) {
 dict_enum = dict_first ( op ) ;
 for ( j = 0 , pc = phtc ;
 ;
 ) {
 int type ;
 if ( ( dict_enum = dict_next ( op , dict_enum , rvalue ) ) == - 1 ) break ;
 if ( ! r_has_type ( & rvalue [ 0 ] , t_name ) ) continue ;
 if ( ! r_has_type ( & rvalue [ 1 ] , t_dictionary ) ) continue ;
 cname = name_index ( mem , & rvalue [ 0 ] ) ;
 code = gs_get_colorname_string ( mem , cname , & pname , & name_size ) ;
 if ( code < 0 ) break ;
 colorant_number = gs_cname_to_colorant_number ( pgs , pname , name_size , halftonetype ) ;
 if ( colorant_number < 0 ) continue ;
 pc -> cname = cname ;
 pc -> comp_number = colorant_number ;
 check_dict_read ( rvalue [ 1 ] ) ;
 if ( dict_int_param ( & rvalue [ 1 ] , "HalftoneType" , 1 , 7 , 0 , & type ) < 0 ) {
 code = gs_note_error ( gs_error_typecheck ) ;
 break ;
 }
 switch ( type ) {
 default : code = gs_note_error ( gs_error_rangecheck ) ;
 break ;
 case 1 : code = dict_spot_params ( & rvalue [ 1 ] , & pc -> params . spot , sprocs + j , tprocs + j , mem ) ;
 pc -> params . spot . screen . spot_function = spot1_dummy ;
 pc -> type = ht_type_spot ;
 break ;
 case 3 : code = dict_threshold_params ( & rvalue [ 1 ] , & pc -> params . threshold , tprocs + j ) ;
 pc -> type = ht_type_threshold ;
 break ;
 case 7 : code = dict_threshold2_params ( & rvalue [ 1 ] , & pc -> params . threshold2 , tprocs + j , imemory ) ;
 pc -> type = ht_type_threshold2 ;
 break ;
 }
 if ( code < 0 ) break ;
 pc ++ ;
 j ++ ;
 }
 }
 if ( code >= 0 ) {
 pht -> type = halftonetype ;
 pht -> params . multiple . components = phtc ;
 pht -> params . multiple . num_comp = j ;
 pht -> params . multiple . get_colorname_string = gs_get_colorname_string ;
 code = gs_sethalftone_prepare ( igs , pht , pdht ) ;
 }
 if ( code >= 0 ) {
 dict_enum = dict_first ( op ) ;
 for ( pc = phtc ;
 ;
 ) {
 if ( ( dict_enum = dict_next ( op , dict_enum , rvalue ) ) == - 1 ) break ;
 if ( ! r_has_type ( & rvalue [ 0 ] , t_name ) ) continue ;
 if ( ! r_has_type ( & rvalue [ 1 ] , t_dictionary ) ) continue ;
 cname = name_index ( mem , & rvalue [ 0 ] ) ;
 code = gs_get_colorname_string ( mem , cname , & pname , & name_size ) ;
 if ( code < 0 ) break ;
 colorant_number = gs_cname_to_colorant_number ( pgs , pname , name_size , halftonetype ) ;
 if ( colorant_number < 0 ) continue ;
 if ( pc -> type == ht_type_spot ) {
 code = dict_spot_results ( i_ctx_p , & rvalue [ 1 ] , & pc -> params . spot ) ;
 if ( code < 0 ) break ;
 }
 pc ++ ;
 }
 }
 if ( code >= 0 ) {
 uint odepth = ref_stack_count ( & o_stack ) ;
 ref odict , odict5 ;
 odict = op [ - 1 ] ;
 odict5 = * op ;
 pop ( 2 ) ;
 op = osp ;
 esp += 5 ;
 make_mark_estack ( esp - 4 , es_other , sethalftone_cleanup ) ;
 esp [ - 3 ] = odict ;
 make_istruct ( esp - 2 , 0 , pht ) ;
 make_istruct ( esp - 1 , 0 , pdht ) ;
 make_op_estack ( esp , sethalftone_finish ) ;
 for ( j = 0 ;
 j < count ;
 j ++ ) {
 gx_ht_order * porder = NULL ;
 if ( pdht -> components == 0 ) porder = & pdht -> order ;
 else {
 int k ;
 int comp_number = phtc [ j ] . comp_number ;
 for ( k = 0 ;
 k < count ;
 k ++ ) {
 if ( pdht -> components [ k ] . comp_number == comp_number ) {
 porder = & pdht -> components [ k ] . corder ;
 break ;
 }
 }
 }
 switch ( phtc [ j ] . type ) {
 case ht_type_spot : code = zscreen_enum_init ( i_ctx_p , porder , & phtc [ j ] . params . spot . screen , & sprocs [ j ] , 0 , 0 , space_index ) ;
 if ( code < 0 ) break ;
 case ht_type_threshold : if ( ! r_has_type ( tprocs + j , t__invalid ) ) {
 check_ostack ( zcolor_remap_one_ostack ) ;
 check_estack ( zcolor_remap_one_estack ) ;
 code = zcolor_remap_one ( i_ctx_p , tprocs + j , porder -> transfer , igs , zcolor_remap_one_finish ) ;
 op = osp ;
 }
 break ;
 default : ;
 }
 if ( code < 0 ) {
 ref_stack_pop_to ( & o_stack , odepth ) ;
 ref_stack_pop_to ( & e_stack , edepth ) ;
 op = osp ;
 op [ - 1 ] = odict ;
 * op = odict5 ;
 break ;
 }
 npop = 0 ;
 }
 }
 if ( code < 0 ) {
 gs_free_object ( mem , pdht , ".sethalftone5" ) ;
 gs_free_object ( mem , phtc , ".sethalftone5" ) ;
 gs_free_object ( mem , pht , ".sethalftone5" ) ;
 return code ;
 }
 pop ( npop ) ;
 return ( ref_stack_count ( & e_stack ) > edepth ? o_push_estack : 0 ) ;
 }