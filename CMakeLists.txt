CMAKE_MINIMUM_REQUIRED(VERSION 2.6)
project(amrcc)

# Do not follow symlinks in GLOB_RECURSE
CMAKE_POLICY(SET CMP0009 NEW)
file(GLOB_RECURSE AMRCC_HEADER "*.h")

set(AMRCC_SRC
		  agc.c 
		  amrdecode.c 
		  a_refl.c 
		  b_cn_cod.c 
		  bgnscd.c 
		  c_g_aver.c 
		  d1035pf.c 
		  d2_11pf.c 
		  d2_9pf.c 
		  d3_14pf.c 
		  d4_17pf.c 
		  d8_31pf.c 
		  dec_amr.c 
		  dec_gain.c 
		  dec_input_format_tab.c 
		  dec_lag3.c 
		  dec_lag6.c 
		  d_gain_c.c 
		  d_gain_p.c 
		  d_plsf_3.c 
		  d_plsf_5.c 
		  d_plsf.c 
		  dtx_dec.c 
		  ec_gains.c 
		  ex_ctrl.c 
		  if2_to_ets.c 
		  int_lsf.c 
		  lsp_avg.c 
		  ph_disp.c 
		  post_pro.c 
		  preemph.c 
		  pstfilt.c 
		  qgain475_tab.c 
		  sp_dec.c 
		  wmf_to_ets.c
		  amrencode.c 
		  autocorr.c 
		  c1035pf.c 
		  c2_11pf.c 
		  c2_9pf.c 
		  c3_14pf.c 
		  c4_17pf.c 
		  c8_31pf.c 
		  calc_cor.c 
		  calc_en.c 
		  cbsearch.c 
		  cl_ltp.c 
		  cod_amr.c 
		  convolve.c 
		  cor_h.c 
		  cor_h_x2.c 
		  cor_h_x.c 
		  corrwght_tab.c 
		  div_32.c 
		  dtx_enc.c 
		  enc_lag3.c 
		  enc_lag6.c 
		  enc_output_format_tab.c 
		  ets_to_if2.c 
		  ets_to_wmf.c 
		  g_adapt.c 
		  gain_q.c 
		  g_code.c 
		  g_pitch.c 
		  hp_max.c 
		  inter_36.c 
		  inter_36_tab.c 
		  l_abs.c 
		  lag_wind.c 
		  lag_wind_tab.c 
		  l_comp.c 
		  levinson.c 
		  l_extract.c 
		  lflg_upd.c 
		  l_negate.c 
		  lpc.c 
		  ol_ltp.c 
		  pitch_fr.c 
		  pitch_ol.c 
		  p_ol_wgh.c 
		  pre_big.c 
		  pre_proc.c 
		  prm2bits.c 
		  qgain475.c 
		  qgain795.c 
		  q_gain_c.c 
		  q_gain_p.c 
		  qua_gain.c 
		  s10_8pf.c 
		  set_sign.c 
		  sid_sync.c 
		  sp_enc.c 
		  spreproc.c 
		  spstproc.c 
		  ton_stab.c 
		  vad1.c
		  wrapper.c 
		  add.c 
		  az_lsp.c 
		  bitno_tab.c 
		  bitreorder_tab.c 
		  c2_9pf_tab.c 
		  div_s.c 
		  extract_h.c 
		  extract_l.c 
		  gains_tbl.c 
		  gc_pred.c 
		  get_const_tbls.c 
		  gmed_n.c 
		  gray_tbl.c 
		  grid_tbl.c 
		  int_lpc.c 
		  inv_sqrt.c 
		  inv_sqrt_tbl.c 
		  l_deposit_h.c 
		  l_deposit_l.c 
		  log2.c 
		  log2_norm.c 
		  log2_tbl.c 
		  lsfwt.c 
		  l_shr_r.c 
		  lsp_az.c 
		  lsp.c 
		  lsp_lsf.c 
		  lsp_lsf_tbl.c 
		  lsp_tab.c 
		  mult_r.c 
		  negate.c 
		  norm_l.c 
		  norm_s.c 
		  overflow_tbl.c 
		  ph_disp_tab.c 
		  pow2.c 
		  pow2_tbl.c 
		  pred_lt.c 
		  q_plsf_3.c 
		  q_plsf_3_tbl.c 
		  q_plsf_5.c 
		  q_plsf_5_tbl.c 
		  q_plsf.c 
		  qua_gain_tbl.c 
		  reorder.c 
		  residu.c 
		  round.c 
		  set_zero.c 
		  shr.c 
		  shr_r.c 
		  sqrt_l.c 
		  sqrt_l_tbl.c 
		  sub.c 
		  syn_filt.c 
		  weight_a.c 
		  window_tab.c
	)

set(PORT_SRC libamrcc.c)

include_directories(./)
include_directories(./speex)
include_directories(./speex/include)
include_directories(./speex/libspeex)


set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fvisibility=hidden")

add_definitions(-DFLOAT_POINT)
add_definitions(-DUSE_KISS_FFT)
add_definitions(-DHAVE_CONFIG_H)
add_definitions(-DDISABlE_SPEEX_PREPROCESS)
add_library(amrcc SHARED ${AMRCC_SRC} ${AMRCC_HEADER})

install(TARGETS amrcc DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)

add_symbols(amrcc)

