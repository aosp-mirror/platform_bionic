/****************************************************************************
 ****************************************************************************
 ***
 ***   This header was automatically generated from a Linux kernel header
 ***   of the same name, to make information necessary for userspace to
 ***   call into the kernel available to libc.  It contains only constants,
 ***   structures, and macros generated from the original header, and thus,
 ***   contains no copyrightable information.
 ***
 ***   To edit the content of this header, modify the corresponding
 ***   source file (e.g. under external/kernel-headers/original/) then
 ***   run bionic/libc/kernel/tools/update_all.py
 ***
 ***   Any manual change here will be lost the next time this script will
 ***   be run. You've been warned!
 ***
 ****************************************************************************
 ****************************************************************************/
#ifndef _FC_ELS_H_
#define _FC_ELS_H_
#include <linux/types.h>
enum fc_els_cmd {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 ELS_LS_RJT = 0x01,
 ELS_LS_ACC = 0x02,
 ELS_PLOGI = 0x03,
 ELS_FLOGI = 0x04,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 ELS_LOGO = 0x05,
 ELS_ABTX = 0x06,
 ELS_RCS = 0x07,
 ELS_RES = 0x08,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 ELS_RSS = 0x09,
 ELS_RSI = 0x0a,
 ELS_ESTS = 0x0b,
 ELS_ESTC = 0x0c,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 ELS_ADVC = 0x0d,
 ELS_RTV = 0x0e,
 ELS_RLS = 0x0f,
 ELS_ECHO = 0x10,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 ELS_TEST = 0x11,
 ELS_RRQ = 0x12,
 ELS_REC = 0x13,
 ELS_SRR = 0x14,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 ELS_PRLI = 0x20,
 ELS_PRLO = 0x21,
 ELS_SCN = 0x22,
 ELS_TPLS = 0x23,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 ELS_TPRLO = 0x24,
 ELS_LCLM = 0x25,
 ELS_GAID = 0x30,
 ELS_FACT = 0x31,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 ELS_FDACDT = 0x32,
 ELS_NACT = 0x33,
 ELS_NDACT = 0x34,
 ELS_QOSR = 0x40,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 ELS_RVCS = 0x41,
 ELS_PDISC = 0x50,
 ELS_FDISC = 0x51,
 ELS_ADISC = 0x52,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 ELS_RNC = 0x53,
 ELS_FARP_REQ = 0x54,
 ELS_FARP_REPL = 0x55,
 ELS_RPS = 0x56,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 ELS_RPL = 0x57,
 ELS_RPBC = 0x58,
 ELS_FAN = 0x60,
 ELS_RSCN = 0x61,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 ELS_SCR = 0x62,
 ELS_RNFT = 0x63,
 ELS_CSR = 0x68,
 ELS_CSU = 0x69,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 ELS_LINIT = 0x70,
 ELS_LSTS = 0x72,
 ELS_RNID = 0x78,
 ELS_RLIR = 0x79,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 ELS_LIRR = 0x7a,
 ELS_SRL = 0x7b,
 ELS_SBRP = 0x7c,
 ELS_RPSC = 0x7d,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 ELS_QSA = 0x7e,
 ELS_EVFP = 0x7f,
 ELS_LKA = 0x80,
 ELS_AUTH_ELS = 0x90,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
#define FC_ELS_CMDS_INIT {   [ELS_LS_RJT] = "LS_RJT",   [ELS_LS_ACC] = "LS_ACC",   [ELS_PLOGI] = "PLOGI",   [ELS_FLOGI] = "FLOGI",   [ELS_LOGO] = "LOGO",   [ELS_ABTX] = "ABTX",   [ELS_RCS] = "RCS",   [ELS_RES] = "RES",   [ELS_RSS] = "RSS",   [ELS_RSI] = "RSI",   [ELS_ESTS] = "ESTS",   [ELS_ESTC] = "ESTC",   [ELS_ADVC] = "ADVC",   [ELS_RTV] = "RTV",   [ELS_RLS] = "RLS",   [ELS_ECHO] = "ECHO",   [ELS_TEST] = "TEST",   [ELS_RRQ] = "RRQ",   [ELS_REC] = "REC",   [ELS_SRR] = "SRR",   [ELS_PRLI] = "PRLI",   [ELS_PRLO] = "PRLO",   [ELS_SCN] = "SCN",   [ELS_TPLS] = "TPLS",   [ELS_TPRLO] = "TPRLO",   [ELS_LCLM] = "LCLM",   [ELS_GAID] = "GAID",   [ELS_FACT] = "FACT",   [ELS_FDACDT] = "FDACDT",   [ELS_NACT] = "NACT",   [ELS_NDACT] = "NDACT",   [ELS_QOSR] = "QOSR",   [ELS_RVCS] = "RVCS",   [ELS_PDISC] = "PDISC",   [ELS_FDISC] = "FDISC",   [ELS_ADISC] = "ADISC",   [ELS_RNC] = "RNC",   [ELS_FARP_REQ] = "FARP_REQ",   [ELS_FARP_REPL] = "FARP_REPL",   [ELS_RPS] = "RPS",   [ELS_RPL] = "RPL",   [ELS_RPBC] = "RPBC",   [ELS_FAN] = "FAN",   [ELS_RSCN] = "RSCN",   [ELS_SCR] = "SCR",   [ELS_RNFT] = "RNFT",   [ELS_CSR] = "CSR",   [ELS_CSU] = "CSU",   [ELS_LINIT] = "LINIT",   [ELS_LSTS] = "LSTS",   [ELS_RNID] = "RNID",   [ELS_RLIR] = "RLIR",   [ELS_LIRR] = "LIRR",   [ELS_SRL] = "SRL",   [ELS_SBRP] = "SBRP",   [ELS_RPSC] = "RPSC",   [ELS_QSA] = "QSA",   [ELS_EVFP] = "EVFP",   [ELS_LKA] = "LKA",   [ELS_AUTH_ELS] = "AUTH_ELS",  }
struct fc_els_ls_acc {
 __u8 la_cmd;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u8 la_resv[3];
};
struct fc_els_ls_rjt {
 __u8 er_cmd;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u8 er_resv[4];
 __u8 er_reason;
 __u8 er_explan;
 __u8 er_vendor;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
enum fc_els_rjt_reason {
 ELS_RJT_NONE = 0,
 ELS_RJT_INVAL = 0x01,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 ELS_RJT_LOGIC = 0x03,
 ELS_RJT_BUSY = 0x05,
 ELS_RJT_PROT = 0x07,
 ELS_RJT_UNAB = 0x09,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 ELS_RJT_UNSUP = 0x0b,
 ELS_RJT_INPROG = 0x0e,
 ELS_RJT_FIP = 0x20,
 ELS_RJT_VENDOR = 0xff,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
enum fc_els_rjt_explan {
 ELS_EXPL_NONE = 0x00,
 ELS_EXPL_SPP_OPT_ERR = 0x01,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 ELS_EXPL_SPP_ICTL_ERR = 0x03,
 ELS_EXPL_AH = 0x11,
 ELS_EXPL_AH_REQ = 0x13,
 ELS_EXPL_SID = 0x15,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 ELS_EXPL_OXID_RXID = 0x17,
 ELS_EXPL_INPROG = 0x19,
 ELS_EXPL_PLOGI_REQD = 0x1e,
 ELS_EXPL_INSUF_RES = 0x29,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 ELS_EXPL_UNAB_DATA = 0x2a,
 ELS_EXPL_UNSUPR = 0x2c,
 ELS_EXPL_INV_LEN = 0x2d,
 ELS_EXPL_NOT_NEIGHBOR = 0x62,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct fc_els_csp {
 __u8 sp_hi_ver;
 __u8 sp_lo_ver;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __be16 sp_bb_cred;
 __be16 sp_features;
 __be16 sp_bb_data;
 union {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct {
 __be16 _sp_tot_seq;
 __be16 _sp_rel_off;
 } sp_plogi;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct {
 __be32 _sp_r_a_tov;
 } sp_flogi_acc;
 } sp_u;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __be32 sp_e_d_tov;
};
#define sp_tot_seq sp_u.sp_plogi._sp_tot_seq
#define sp_rel_off sp_u.sp_plogi._sp_rel_off
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define sp_r_a_tov sp_u.sp_flogi_acc._sp_r_a_tov
#define FC_SP_BB_DATA_MASK 0xfff
#define FC_SP_MIN_MAX_PAYLOAD FC_MIN_MAX_PAYLOAD
#define FC_SP_MAX_MAX_PAYLOAD FC_MAX_PAYLOAD
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define FC_SP_FT_NPIV 0x8000
#define FC_SP_FT_CIRO 0x8000
#define FC_SP_FT_CLAD 0x8000
#define FC_SP_FT_RAND 0x4000
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define FC_SP_FT_VAL 0x2000
#define FC_SP_FT_NPIV_ACC 0x2000
#define FC_SP_FT_FPORT 0x1000
#define FC_SP_FT_ABB 0x0800
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define FC_SP_FT_EDTR 0x0400
#define FC_SP_FT_MCAST 0x0200
#define FC_SP_FT_BCAST 0x0100
#define FC_SP_FT_HUNT 0x0080
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define FC_SP_FT_SIMP 0x0040
#define FC_SP_FT_SEC 0x0020
#define FC_SP_FT_CSYN 0x0010
#define FC_SP_FT_RTTOV 0x0008
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define FC_SP_FT_HALF 0x0004
#define FC_SP_FT_SEQC 0x0002
#define FC_SP_FT_PAYL 0x0001
struct fc_els_cssp {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __be16 cp_class;
 __be16 cp_init;
 __be16 cp_recip;
 __be16 cp_rdfs;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __be16 cp_con_seq;
 __be16 cp_ee_cred;
 __u8 cp_resv1;
 __u8 cp_open_seq;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u8 _cp_resv2[2];
};
#define FC_CPC_VALID 0x8000
#define FC_CPC_IMIX 0x4000
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define FC_CPC_SEQ 0x0800
#define FC_CPC_CAMP 0x0200
#define FC_CPC_PRI 0x0080
#define FC_CPI_CSYN 0x0010
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define FC_CPR_CSYN 0x0008
struct fc_els_flogi {
 __u8 fl_cmd;
 __u8 _fl_resvd[3];
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 struct fc_els_csp fl_csp;
 __be64 fl_wwpn;
 __be64 fl_wwnn;
 struct fc_els_cssp fl_cssp[4];
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u8 fl_vend[16];
} __attribute__((__packed__));
struct fc_els_spp {
 __u8 spp_type;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u8 spp_type_ext;
 __u8 spp_flags;
 __u8 _spp_resvd;
 __be32 spp_orig_pa;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __be32 spp_resp_pa;
 __be32 spp_params;
};
#define FC_SPP_OPA_VAL 0x80
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define FC_SPP_RPA_VAL 0x40
#define FC_SPP_EST_IMG_PAIR 0x20
#define FC_SPP_RESP_MASK 0x0f
enum fc_els_spp_resp {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 FC_SPP_RESP_ACK = 1,
 FC_SPP_RESP_RES = 2,
 FC_SPP_RESP_INIT = 3,
 FC_SPP_RESP_NO_PA = 4,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 FC_SPP_RESP_CONF = 5,
 FC_SPP_RESP_COND = 6,
 FC_SPP_RESP_MULT = 7,
 FC_SPP_RESP_INVL = 8,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct fc_els_rrq {
 __u8 rrq_cmd;
 __u8 rrq_zero[3];
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u8 rrq_resvd;
 __u8 rrq_s_id[3];
 __be16 rrq_ox_id;
 __be16 rrq_rx_id;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct fc_els_rec {
 __u8 rec_cmd;
 __u8 rec_zero[3];
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u8 rec_resvd;
 __u8 rec_s_id[3];
 __be16 rec_ox_id;
 __be16 rec_rx_id;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct fc_els_rec_acc {
 __u8 reca_cmd;
 __u8 reca_zero[3];
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __be16 reca_ox_id;
 __be16 reca_rx_id;
 __u8 reca_resvd1;
 __u8 reca_ofid[3];
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u8 reca_resvd2;
 __u8 reca_rfid[3];
 __be32 reca_fc4value;
 __be32 reca_e_stat;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct fc_els_prli {
 __u8 prli_cmd;
 __u8 prli_spp_len;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __be16 prli_len;
};
struct fc_els_prlo {
 __u8 prlo_cmd;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u8 prlo_obs;
 __be16 prlo_len;
};
struct fc_els_adisc {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u8 adisc_cmd;
 __u8 adisc_resv[3];
 __u8 adisc_resv1;
 __u8 adisc_hard_addr[3];
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __be64 adisc_wwpn;
 __be64 adisc_wwnn;
 __u8 adisc_resv2;
 __u8 adisc_port_id[3];
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
} __attribute__((__packed__));
struct fc_els_logo {
 __u8 fl_cmd;
 __u8 fl_zero[3];
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u8 fl_resvd;
 __u8 fl_n_port_id[3];
 __be64 fl_n_port_wwn;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct fc_els_rtv {
 __u8 rtv_cmd;
 __u8 rtv_zero[3];
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct fc_els_rtv_acc {
 __u8 rtv_cmd;
 __u8 rtv_zero[3];
 __be32 rtv_r_a_tov;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __be32 rtv_e_d_tov;
 __be32 rtv_toq;
};
#define FC_ELS_RTV_EDRES (1 << 26)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define FC_ELS_RTV_RTTOV (1 << 19)
struct fc_els_scr {
 __u8 scr_cmd;
 __u8 scr_resv[6];
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u8 scr_reg_func;
};
enum fc_els_scr_func {
 ELS_SCRF_FAB = 1,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 ELS_SCRF_NPORT = 2,
 ELS_SCRF_FULL = 3,
 ELS_SCRF_CLEAR = 255,
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct fc_els_rscn {
 __u8 rscn_cmd;
 __u8 rscn_page_len;
 __be16 rscn_plen;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct fc_els_rscn_page {
 __u8 rscn_page_flags;
 __u8 rscn_fid[3];
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
#define ELS_RSCN_EV_QUAL_BIT 2
#define ELS_RSCN_EV_QUAL_MASK 0xf
#define ELS_RSCN_ADDR_FMT_BIT 0
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define ELS_RSCN_ADDR_FMT_MASK 0x3
enum fc_els_rscn_ev_qual {
 ELS_EV_QUAL_NONE = 0,
 ELS_EV_QUAL_NS_OBJ = 1,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 ELS_EV_QUAL_PORT_ATTR = 2,
 ELS_EV_QUAL_SERV_OBJ = 3,
 ELS_EV_QUAL_SW_CONFIG = 4,
 ELS_EV_QUAL_REM_OBJ = 5,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
enum fc_els_rscn_addr_fmt {
 ELS_ADDR_FMT_PORT = 0,
 ELS_ADDR_FMT_AREA = 1,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 ELS_ADDR_FMT_DOM = 2,
 ELS_ADDR_FMT_FAB = 3,
};
struct fc_els_rnid {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u8 rnid_cmd;
 __u8 rnid_resv[3];
 __u8 rnid_fmt;
 __u8 rnid_resv2[3];
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
enum fc_els_rnid_fmt {
 ELS_RNIDF_NONE = 0,
 ELS_RNIDF_GEN = 0xdf,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct fc_els_rnid_resp {
 __u8 rnid_cmd;
 __u8 rnid_resv[3];
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u8 rnid_fmt;
 __u8 rnid_cid_len;
 __u8 rnid_resv2;
 __u8 rnid_sid_len;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct fc_els_rnid_cid {
 __be64 rnid_wwpn;
 __be64 rnid_wwnn;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct fc_els_rnid_gen {
 __u8 rnid_vend_id[16];
 __be32 rnid_atype;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __be32 rnid_phys_port;
 __be32 rnid_att_nodes;
 __u8 rnid_node_mgmt;
 __u8 rnid_ip_ver;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __be16 rnid_prot_port;
 __be32 rnid_ip_addr[4];
 __u8 rnid_resvd[2];
 __be16 rnid_vend_spec;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
enum fc_els_rnid_atype {
 ELS_RNIDA_UNK = 0x01,
 ELS_RNIDA_OTHER = 0x02,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 ELS_RNIDA_HUB = 0x03,
 ELS_RNIDA_SWITCH = 0x04,
 ELS_RNIDA_GATEWAY = 0x05,
 ELS_RNIDA_CONV = 0x06,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 ELS_RNIDA_HBA = 0x07,
 ELS_RNIDA_PROXY = 0x08,
 ELS_RNIDA_STORAGE = 0x09,
 ELS_RNIDA_HOST = 0x0a,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 ELS_RNIDA_SUBSYS = 0x0b,
 ELS_RNIDA_ACCESS = 0x0e,
 ELS_RNIDA_NAS = 0x11,
 ELS_RNIDA_BRIDGE = 0x12,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 ELS_RNIDA_VIRT = 0x13,
 ELS_RNIDA_MF = 0xff,
 ELS_RNIDA_MF_HUB = 1UL << 31,
 ELS_RNIDA_MF_SW = 1UL << 30,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 ELS_RNIDA_MF_GW = 1UL << 29,
 ELS_RNIDA_MF_ST = 1UL << 28,
 ELS_RNIDA_MF_HOST = 1UL << 27,
 ELS_RNIDA_MF_SUB = 1UL << 26,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 ELS_RNIDA_MF_ACC = 1UL << 25,
 ELS_RNIDA_MF_WDM = 1UL << 24,
 ELS_RNIDA_MF_NAS = 1UL << 23,
 ELS_RNIDA_MF_BR = 1UL << 22,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 ELS_RNIDA_MF_VIRT = 1UL << 21,
};
enum fc_els_rnid_mgmt {
 ELS_RNIDM_SNMP = 0,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 ELS_RNIDM_TELNET = 1,
 ELS_RNIDM_HTTP = 2,
 ELS_RNIDM_HTTPS = 3,
 ELS_RNIDM_XML = 4,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
enum fc_els_rnid_ipver {
 ELS_RNIDIP_NONE = 0,
 ELS_RNIDIP_V4 = 1,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 ELS_RNIDIP_V6 = 2,
};
struct fc_els_rpl {
 __u8 rpl_cmd;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u8 rpl_resv[5];
 __be16 rpl_max_size;
 __u8 rpl_resv1;
 __u8 rpl_index[3];
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct fc_els_pnb {
 __be32 pnb_phys_pn;
 __u8 pnb_resv;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u8 pnb_port_id[3];
 __be64 pnb_wwpn;
};
struct fc_els_rpl_resp {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u8 rpl_cmd;
 __u8 rpl_resv1;
 __be16 rpl_plen;
 __u8 rpl_resv2;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u8 rpl_llen[3];
 __u8 rpl_resv3;
 __u8 rpl_index[3];
 struct fc_els_pnb rpl_pnb[1];
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct fc_els_lesb {
 __be32 lesb_link_fail;
 __be32 lesb_sync_loss;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __be32 lesb_sig_loss;
 __be32 lesb_prim_err;
 __be32 lesb_inv_word;
 __be32 lesb_inv_crc;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct fc_els_rps {
 __u8 rps_cmd;
 __u8 rps_resv[2];
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u8 rps_flag;
 __be64 rps_port_spec;
};
enum fc_els_rps_flag {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 FC_ELS_RPS_DID = 0x00,
 FC_ELS_RPS_PPN = 0x01,
 FC_ELS_RPS_WWPN = 0x02,
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct fc_els_rps_resp {
 __u8 rps_cmd;
 __u8 rps_resv[2];
 __u8 rps_flag;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u8 rps_resv2[2];
 __be16 rps_status;
 struct fc_els_lesb rps_lesb;
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
enum fc_els_rps_resp_flag {
 FC_ELS_RPS_LPEV = 0x01,
};
enum fc_els_rps_resp_status {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 FC_ELS_RPS_PTP = 1 << 5,
 FC_ELS_RPS_LOOP = 1 << 4,
 FC_ELS_RPS_FAB = 1 << 3,
 FC_ELS_RPS_NO_SIG = 1 << 2,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 FC_ELS_RPS_NO_SYNC = 1 << 1,
 FC_ELS_RPS_RESET = 1 << 0,
};
struct fc_els_lirr {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u8 lirr_cmd;
 __u8 lirr_resv[3];
 __u8 lirr_func;
 __u8 lirr_fmt;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u8 lirr_resv2[2];
};
enum fc_els_lirr_func {
 ELS_LIRR_SET_COND = 0x01,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 ELS_LIRR_SET_UNCOND = 0x02,
 ELS_LIRR_CLEAR = 0xff
};
struct fc_els_srl {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u8 srl_cmd;
 __u8 srl_resv[3];
 __u8 srl_flag;
 __u8 srl_flag_param[3];
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
enum fc_els_srl_flag {
 FC_ELS_SRL_ALL = 0x00,
 FC_ELS_SRL_ONE = 0x01,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 FC_ELS_SRL_EN_PER = 0x02,
 FC_ELS_SRL_DIS_PER = 0x03,
};
struct fc_els_rls {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u8 rls_cmd;
 __u8 rls_resv[4];
 __u8 rls_port_id[3];
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct fc_els_rls_resp {
 __u8 rls_cmd;
 __u8 rls_resv[3];
 struct fc_els_lesb rls_lesb;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct fc_els_rlir {
 __u8 rlir_cmd;
 __u8 rlir_resv[3];
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u8 rlir_fmt;
 __u8 rlir_clr_len;
 __u8 rlir_cld_len;
 __u8 rlir_slr_len;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
struct fc_els_clir {
 __be64 clir_wwpn;
 __be64 clir_wwnn;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u8 clir_port_type;
 __u8 clir_port_id[3];
 __be64 clir_conn_wwpn;
 __be64 clir_conn_wwnn;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __be64 clir_fab_name;
 __be32 clir_phys_port;
 __be32 clir_trans_id;
 __u8 clir_resv[3];
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 __u8 clir_ts_fmt;
 __be64 clir_timestamp;
};
enum fc_els_clir_ts_fmt {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 ELS_CLIR_TS_UNKNOWN = 0,
 ELS_CLIR_TS_SEC_FRAC = 1,
 ELS_CLIR_TS_CSU = 2,
};
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
struct fc_els_clid {
 __u8 clid_iq;
 __u8 clid_ic;
 __be16 clid_epai;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
enum fc_els_clid_iq {
 ELS_CLID_SWITCH = 0x20,
 ELS_CLID_E_PORT = 0x10,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 ELS_CLID_SEV_MASK = 0x0c,
 ELS_CLID_SEV_INFO = 0x00,
 ELS_CLID_SEV_INOP = 0x08,
 ELS_CLID_SEV_DEG = 0x04,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 ELS_CLID_LASER = 0x02,
 ELS_CLID_FRU = 0x01,
};
enum fc_els_clid_ic {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 ELS_CLID_IC_IMPL = 1,
 ELS_CLID_IC_BER = 2,
 ELS_CLID_IC_LOS = 3,
 ELS_CLID_IC_NOS = 4,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
 ELS_CLID_IC_PST = 5,
 ELS_CLID_IC_INVAL = 6,
 ELS_CLID_IC_LOOP_TO = 7,
 ELS_CLID_IC_LIP = 8,
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
};
#endif
