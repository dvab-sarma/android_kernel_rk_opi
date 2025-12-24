/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Merged with mainline ieee80211.h in Aug 2004.  Original ieee802_11
 * remains copyright by the original authors
 *
 * Portions of the merged code are based on Host AP (software wireless
 * LAN access point) driver for Intersil Prism2/2.5/3.
 *
 * Copyright (c) 2001-2002, SSH Communications Security Corp and Jouni Malinen
 * <j@w1.fi>
 * Copyright (c) 2002-2003, Jouni Malinen <j@w1.fi>
 *
 * Adaption to a generic IEEE 802.11 stack by James Ketrenos
 * <jketreno@linux.intel.com>
 * Copyright (c) 2004-2005, Intel Corporation
 *
 * API Version History
 * 1.0.x -- Initial version
 * 1.1.x -- Added radiotap, QoS, TIM, lib80211_geo APIs,
 *          various structure changes, and crypto API init method
 */
#ifndef LIB80211_H
#define LIB80211_H
#include <linux/if_ether.h>	/* ETH_ALEN */
#include <linux/kernel.h>	/* ARRAY_SIZE */
#include <linux/wireless.h>
#include <linux/ieee80211.h>
#include <net/cfg80211.h>

#define LIB80211_VERSION "git-1.1.13"

#define LIB80211_DATA_LEN		2304
/* Maximum size for the MA-UNITDATA primitive, 802.11 standard section
   6.2.1.1.2.

   The figure in section 7.1.2 suggests a body size of up to 2312
   bytes is allowed, which is a bit confusing, I suspect this
   represents the 2304 bytes of real data, plus a possible 8 bytes of
   WEP IV and ICV. (this interpretation suggested by Ramiro Barreiro) */

#define LIB80211_1ADDR_LEN 10
#define LIB80211_2ADDR_LEN 16
#define LIB80211_3ADDR_LEN 24
#define LIB80211_4ADDR_LEN 30
#define LIB80211_FCS_LEN    4
#define LIB80211_HLEN			(LIB80211_4ADDR_LEN)
#define LIB80211_FRAME_LEN		(LIB80211_DATA_LEN + LIB80211_HLEN)

#define MIN_FRAG_THRESHOLD     256U
#define	MAX_FRAG_THRESHOLD     2346U

/* QOS control */
#define LIB80211_QCTL_TID		0x000F

/* debug macros */

#ifdef CONFIG_LIB80211_DEBUG
extern u32 lib80211_debug_level;
#define LIB80211_DEBUG(level, fmt, args...) \
do { if (lib80211_debug_level & (level)) \
  printk(KERN_DEBUG "lib80211: %s " fmt, __func__ , ## args); } while (0)
#else
#define LIB80211_DEBUG(level, fmt, args...) do {} while (0)
#endif				/* CONFIG_LIB80211_DEBUG */

/*
 * To use the debug system:
 *
 * If you are defining a new debug classification, simply add it to the #define
 * list here in the form of:
 *
 * #define LIB80211_DL_xxxx VALUE
 *
 * shifting value to the left one bit from the previous entry.  xxxx should be
 * the name of the classification (for example, WEP)
 *
 * You then need to either add a LIB80211_xxxx_DEBUG() macro definition for your
 * classification, or use LIB80211_DEBUG(LIB80211_DL_xxxx, ...) whenever you want
 * to send output to that classification.
 *
 * To add your debug level to the list of levels seen when you perform
 *
 * % cat /proc/net/ieee80211/debug_level
 *
 * you simply need to add your entry to the lib80211_debug_level array.
 *
 * If you do not see debug_level in /proc/net/ieee80211 then you do not have
 * CONFIG_LIB80211_DEBUG defined in your kernel configuration
 *
 */

#define LIB80211_DL_INFO          (1<<0)
#define LIB80211_DL_WX            (1<<1)
#define LIB80211_DL_SCAN          (1<<2)
#define LIB80211_DL_STATE         (1<<3)
#define LIB80211_DL_MGMT          (1<<4)
#define LIB80211_DL_FRAG          (1<<5)
#define LIB80211_DL_DROP          (1<<7)

#define LIB80211_DL_TX            (1<<8)
#define LIB80211_DL_RX            (1<<9)
#define LIB80211_DL_QOS           (1<<31)

#define LIB80211_ERROR(f, a...) printk(KERN_ERR "lib80211: " f, ## a)
#define LIB80211_WARNING(f, a...) printk(KERN_WARNING "lib80211: " f, ## a)
#define LIB80211_DEBUG_INFO(f, a...)   LIB80211_DEBUG(LIB80211_DL_INFO, f, ## a)

#define LIB80211_DEBUG_WX(f, a...)     LIB80211_DEBUG(LIB80211_DL_WX, f, ## a)
#define LIB80211_DEBUG_SCAN(f, a...)   LIB80211_DEBUG(LIB80211_DL_SCAN, f, ## a)
#define LIB80211_DEBUG_STATE(f, a...)  LIB80211_DEBUG(LIB80211_DL_STATE, f, ## a)
#define LIB80211_DEBUG_MGMT(f, a...)  LIB80211_DEBUG(LIB80211_DL_MGMT, f, ## a)
#define LIB80211_DEBUG_FRAG(f, a...)  LIB80211_DEBUG(LIB80211_DL_FRAG, f, ## a)
#define LIB80211_DEBUG_DROP(f, a...)  LIB80211_DEBUG(LIB80211_DL_DROP, f, ## a)
#define LIB80211_DEBUG_TX(f, a...)  LIB80211_DEBUG(LIB80211_DL_TX, f, ## a)
#define LIB80211_DEBUG_RX(f, a...)  LIB80211_DEBUG(LIB80211_DL_RX, f, ## a)
#define LIB80211_DEBUG_QOS(f, a...)  LIB80211_DEBUG(LIB80211_DL_QOS, f, ## a)
#include <linux/netdevice.h>
#include <linux/if_arp.h>	/* ARPHRD_ETHER */

#ifndef WIRELESS_SPY
#define WIRELESS_SPY		/* enable iwspy support */
#endif
#include <net/iw_handler.h>	/* new driver API */

#define ETH_P_PREAUTH 0x88C7	/* IEEE 802.11i pre-authentication */

#ifndef ETH_P_80211_RAW
#define ETH_P_80211_RAW (ETH_P_ECONET + 1)
#endif

/* IEEE 802.11 defines */

#define P80211_OUI_LEN 3

struct lib80211_snap_hdr {

	u8 dsap;		/* always 0xAA */
	u8 ssap;		/* always 0xAA */
	u8 ctrl;		/* always 0x03 */
	u8 oui[P80211_OUI_LEN];	/* organizational universal id */

} __packed;

#define SNAP_SIZE sizeof(struct lib80211_snap_hdr)

#define WLAN_FC_GET_VERS(fc) ((fc) & IEEE80211_FCTL_VERS)
#define WLAN_FC_GET_TYPE(fc) ((fc) & IEEE80211_FCTL_FTYPE)
#define WLAN_FC_GET_STYPE(fc) ((fc) & IEEE80211_FCTL_STYPE)

#define WLAN_GET_SEQ_FRAG(seq) ((seq) & IEEE80211_SCTL_FRAG)
#define WLAN_GET_SEQ_SEQ(seq)  (((seq) & IEEE80211_SCTL_SEQ) >> 4)

#define LIB80211_STATMASK_SIGNAL (1<<0)
#define LIB80211_STATMASK_RSSI (1<<1)
#define LIB80211_STATMASK_NOISE (1<<2)
#define LIB80211_STATMASK_RATE (1<<3)
#define LIB80211_STATMASK_WEMASK 0x7

#define LIB80211_CCK_MODULATION    (1<<0)
#define LIB80211_OFDM_MODULATION   (1<<1)

#define LIB80211_24GHZ_BAND     (1<<0)
#define LIB80211_52GHZ_BAND     (1<<1)

#define LIB80211_CCK_RATE_1MB		        0x02
#define LIB80211_CCK_RATE_2MB		        0x04
#define LIB80211_CCK_RATE_5MB		        0x0B
#define LIB80211_CCK_RATE_11MB		        0x16
#define LIB80211_OFDM_RATE_6MB		        0x0C
#define LIB80211_OFDM_RATE_9MB		        0x12
#define LIB80211_OFDM_RATE_12MB		0x18
#define LIB80211_OFDM_RATE_18MB		0x24
#define LIB80211_OFDM_RATE_24MB		0x30
#define LIB80211_OFDM_RATE_36MB		0x48
#define LIB80211_OFDM_RATE_48MB		0x60
#define LIB80211_OFDM_RATE_54MB		0x6C
#define LIB80211_BASIC_RATE_MASK		0x80

#define LIB80211_CCK_RATE_1MB_MASK		(1<<0)
#define LIB80211_CCK_RATE_2MB_MASK		(1<<1)
#define LIB80211_CCK_RATE_5MB_MASK		(1<<2)
#define LIB80211_CCK_RATE_11MB_MASK		(1<<3)
#define LIB80211_OFDM_RATE_6MB_MASK		(1<<4)
#define LIB80211_OFDM_RATE_9MB_MASK		(1<<5)
#define LIB80211_OFDM_RATE_12MB_MASK		(1<<6)
#define LIB80211_OFDM_RATE_18MB_MASK		(1<<7)
#define LIB80211_OFDM_RATE_24MB_MASK		(1<<8)
#define LIB80211_OFDM_RATE_36MB_MASK		(1<<9)
#define LIB80211_OFDM_RATE_48MB_MASK		(1<<10)
#define LIB80211_OFDM_RATE_54MB_MASK		(1<<11)

#define LIB80211_CCK_RATES_MASK	        0x0000000F
#define LIB80211_CCK_BASIC_RATES_MASK	(LIB80211_CCK_RATE_1MB_MASK | \
	LIB80211_CCK_RATE_2MB_MASK)
#define LIB80211_CCK_DEFAULT_RATES_MASK	(LIB80211_CCK_BASIC_RATES_MASK | \
        LIB80211_CCK_RATE_5MB_MASK | \
        LIB80211_CCK_RATE_11MB_MASK)

#define LIB80211_OFDM_RATES_MASK		0x00000FF0
#define LIB80211_OFDM_BASIC_RATES_MASK	(LIB80211_OFDM_RATE_6MB_MASK | \
	LIB80211_OFDM_RATE_12MB_MASK | \
	LIB80211_OFDM_RATE_24MB_MASK)
#define LIB80211_OFDM_DEFAULT_RATES_MASK	(LIB80211_OFDM_BASIC_RATES_MASK | \
	LIB80211_OFDM_RATE_9MB_MASK  | \
	LIB80211_OFDM_RATE_18MB_MASK | \
	LIB80211_OFDM_RATE_36MB_MASK | \
	LIB80211_OFDM_RATE_48MB_MASK | \
	LIB80211_OFDM_RATE_54MB_MASK)
#define LIB80211_DEFAULT_RATES_MASK (LIB80211_OFDM_DEFAULT_RATES_MASK | \
                                LIB80211_CCK_DEFAULT_RATES_MASK)

#define LIB80211_NUM_OFDM_RATES	    8
#define LIB80211_NUM_CCK_RATES	            4
#define LIB80211_OFDM_SHIFT_MASK_A         4

/* NOTE: This data is for statistical purposes; not all hardware provides this
 *       information for frames received.
 *       For lib80211_rx_mgt, you need to set at least the 'len' parameter.
 */
struct lib80211_rx_stats {
	u32 mac_time;
	s8 rssi;
	u8 signal;
	u8 noise;
	u16 rate;		/* in 100 kbps */
	u8 received_channel;
	u8 control;
	u8 mask;
	u8 freq;
	u16 len;
	u64 tsf;
	u32 beacon_time;
};

/* IEEE 802.11 requires that STA supports concurrent reception of at least
 * three fragmented frames. This define can be increased to support more
 * concurrent frames, but it should be noted that each entry can consume about
 * 2 kB of RAM and increasing cache size will slow down frame reassembly. */
#define LIB80211_FRAG_CACHE_LEN 4

struct lib80211_frag_entry {
	unsigned long first_frag_time;
	unsigned int seq;
	unsigned int last_frag;
	struct sk_buff *skb;
	u8 src_addr[ETH_ALEN];
	u8 dst_addr[ETH_ALEN];
};

struct lib80211_stats {
	unsigned int tx_unicast_frames;
	unsigned int tx_multicast_frames;
	unsigned int tx_fragments;
	unsigned int tx_unicast_octets;
	unsigned int tx_multicast_octets;
	unsigned int tx_deferred_transmissions;
	unsigned int tx_single_retry_frames;
	unsigned int tx_multiple_retry_frames;
	unsigned int tx_retry_limit_exceeded;
	unsigned int tx_discards;
	unsigned int rx_unicast_frames;
	unsigned int rx_multicast_frames;
	unsigned int rx_fragments;
	unsigned int rx_unicast_octets;
	unsigned int rx_multicast_octets;
	unsigned int rx_fcs_errors;
	unsigned int rx_discards_no_buffer;
	unsigned int tx_discards_wrong_sa;
	unsigned int rx_discards_undecryptable;
	unsigned int rx_message_in_msg_fragments;
	unsigned int rx_message_in_bad_msg_fragments;
};

struct lib80211_device;

#define SEC_KEY_1		(1<<0)
#define SEC_KEY_2		(1<<1)
#define SEC_KEY_3		(1<<2)
#define SEC_KEY_4		(1<<3)
#define SEC_ACTIVE_KEY		(1<<4)
#define SEC_AUTH_MODE		(1<<5)
#define SEC_UNICAST_GROUP	(1<<6)
#define SEC_LEVEL		(1<<7)
#define SEC_ENABLED		(1<<8)
#define SEC_ENCRYPT		(1<<9)

#define SEC_LEVEL_0		0	/* None */
#define SEC_LEVEL_1		1	/* WEP 40 and 104 bit */
#define SEC_LEVEL_2		2	/* Level 1 + TKIP */
#define SEC_LEVEL_2_CKIP	3	/* Level 1 + CKIP */
#define SEC_LEVEL_3		4	/* Level 2 + CCMP */

#define SEC_ALG_NONE		0
#define SEC_ALG_WEP		1
#define SEC_ALG_TKIP		2
#define SEC_ALG_CCMP		3

#define WEP_KEYS		4
#define WEP_KEY_LEN		13
#define SCM_KEY_LEN		32
#define SCM_TEMPORAL_KEY_LENGTH	16

struct lib80211_security {
	u16 active_key:2, enabled:1, unicast_uses_group:1, encrypt:1;
	u8 auth_mode;
	u8 encode_alg[WEP_KEYS];
	u8 key_sizes[WEP_KEYS];
	u8 keys[WEP_KEYS][SCM_KEY_LEN];
	u8 level;
	u16 flags;
} __packed;

/*

 802.11 data frame from AP

      ,-------------------------------------------------------------------.
Bytes |  2   |  2   |    6    |    6    |    6    |  2   | 0..2312 |   4  |
      |------|------|---------|---------|---------|------|---------|------|
Desc. | ctrl | dura |  DA/RA  |   TA    |    SA   | Sequ |  frame  |  fcs |
      |      | tion | (BSSID) |         |         | ence |  data   |      |
      `-------------------------------------------------------------------'

Total: 28-2340 bytes

*/

#define BEACON_PROBE_SSID_ID_POSITION 12

struct lib80211_hdr_1addr {
	__le16 frame_ctl;
	__le16 duration_id;
	u8 addr1[ETH_ALEN];
	u8 payload[];
} __packed;

struct lib80211_hdr_2addr {
	__le16 frame_ctl;
	__le16 duration_id;
	u8 addr1[ETH_ALEN];
	u8 addr2[ETH_ALEN];
	u8 payload[];
} __packed;

struct lib80211_hdr_3addr {
	/* New members MUST be added within the __struct_group() macro below. */
	__struct_group(lib80211_hdr_3addr_hdr, hdr, __packed,
		__le16 frame_ctl;
		__le16 duration_id;
		u8 addr1[ETH_ALEN];
		u8 addr2[ETH_ALEN];
		u8 addr3[ETH_ALEN];
		__le16 seq_ctl;
	);
	u8 payload[];
} __packed;
static_assert(offsetof(struct lib80211_hdr_3addr, payload) == sizeof(struct lib80211_hdr_3addr_hdr),
	      "struct member likely outside of __struct_group()");

struct lib80211_hdr_4addr {
	__le16 frame_ctl;
	__le16 duration_id;
	u8 addr1[ETH_ALEN];
	u8 addr2[ETH_ALEN];
	u8 addr3[ETH_ALEN];
	__le16 seq_ctl;
	u8 addr4[ETH_ALEN];
	u8 payload[];
} __packed;

struct lib80211_hdr_3addrqos {
	__le16 frame_ctl;
	__le16 duration_id;
	u8 addr1[ETH_ALEN];
	u8 addr2[ETH_ALEN];
	u8 addr3[ETH_ALEN];
	__le16 seq_ctl;
	u8 payload[0];
	__le16 qos_ctl;
} __packed;

struct lib80211_info_element {
	u8 id;
	u8 len;
	u8 data[];
} __packed;

/*
 * These are the data types that can make up management packets
 *
	u16 auth_algorithm;
	u16 auth_sequence;
	u16 beacon_interval;
	u16 capability;
	u8 current_ap[ETH_ALEN];
	u16 listen_interval;
	struct {
		u16 association_id:14, reserved:2;
	} __packed;
	u32 time_stamp[2];
	u16 reason;
	u16 status;
*/

struct lib80211_auth {
	struct lib80211_hdr_3addr_hdr header;
	__le16 algorithm;
	__le16 transaction;
	__le16 status;
	/* challenge */
	u8 variable[];
} __packed;

struct lib80211_channel_switch {
	u8 id;
	u8 len;
	u8 mode;
	u8 channel;
	u8 count;
} __packed;

struct lib80211_action {
	struct lib80211_hdr_3addr_hdr header;
	u8 category;
	u8 action;
	union {
		struct lib80211_action_exchange {
			u8 token;
		} exchange;
		struct lib80211_channel_switch channel_switch;

	} format;
} __packed;

struct lib80211_disassoc {
	struct lib80211_hdr_3addr_hdr header;
	__le16 reason;
} __packed;

/* Alias deauth for disassoc */
#define lib80211_deauth lib80211_disassoc

struct lib80211_probe_request {
	struct lib80211_hdr_3addr_hdr header;
	/* SSID, supported rates */
	u8 variable[];
} __packed;

struct lib80211_probe_response {
	struct lib80211_hdr_3addr_hdr header;
	__le32 time_stamp[2];
	__le16 beacon_interval;
	__le16 capability;
	/* SSID, supported rates, FH params, DS params,
	 * CF params, IBSS params, TIM (if beacon), RSN */
	u8 variable[];
} __packed;

/* Alias beacon for probe_response */
#define lib80211_beacon lib80211_probe_response

struct lib80211_reassoc_request {
	struct lib80211_hdr_3addr_hdr header;
	__le16 capability;
	__le16 listen_interval;
	u8 current_ap[ETH_ALEN];
	u8 variable[];
} __packed;

struct lib80211_assoc_response {
	struct lib80211_hdr_3addr_hdr header;
	__le16 capability;
	__le16 status;
	__le16 aid;
	/* supported rates */
	u8 variable[];
} __packed;

struct lib80211_txb {
	u8 nr_frags;
	u8 encrypted;
	u8 rts_included;
	u8 reserved;
	u16 frag_size;
	u16 payload_size;
	struct sk_buff *fragments[] __counted_by(nr_frags);
};

/* SWEEP TABLE ENTRIES NUMBER */
#define MAX_SWEEP_TAB_ENTRIES		  42
#define MAX_SWEEP_TAB_ENTRIES_PER_PACKET  7
/* MAX_RATES_LENGTH needs to be 12.  The spec says 8, and many APs
 * only use 8, and then use extended rates for the remaining supported
 * rates.  Other APs, however, stick all of their supported rates on the
 * main rates information element... */
#define MAX_RATES_LENGTH                  ((u8)12)
#define MAX_RATES_EX_LENGTH               ((u8)16)
#define MAX_NETWORK_COUNT                  128

#define CRC_LENGTH                 4U

#define MAX_WPA_IE_LEN 64

#define NETWORK_HAS_OFDM       (1<<1)
#define NETWORK_HAS_CCK        (1<<2)

/* QoS structure */
#define NETWORK_HAS_QOS_PARAMETERS      (1<<3)
#define NETWORK_HAS_QOS_INFORMATION     (1<<4)
#define NETWORK_HAS_QOS_MASK            (NETWORK_HAS_QOS_PARAMETERS | \
					 NETWORK_HAS_QOS_INFORMATION)

/* 802.11h */
#define NETWORK_HAS_POWER_CONSTRAINT    (1<<5)
#define NETWORK_HAS_CSA                 (1<<6)
#define NETWORK_HAS_QUIET               (1<<7)
#define NETWORK_HAS_IBSS_DFS            (1<<8)
#define NETWORK_HAS_TPC_REPORT          (1<<9)

#define NETWORK_HAS_ERP_VALUE           (1<<10)

#define QOS_QUEUE_NUM                   4
#define QOS_OUI_LEN                     3
#define QOS_OUI_TYPE                    2
#define QOS_ELEMENT_ID                  221
#define QOS_OUI_INFO_SUB_TYPE           0
#define QOS_OUI_PARAM_SUB_TYPE          1
#define QOS_VERSION_1                   1
#define QOS_AIFSN_MIN_VALUE             2

struct lib80211_qos_information_element {
	u8 elementID;
	u8 length;
	u8 qui[QOS_OUI_LEN];
	u8 qui_type;
	u8 qui_subtype;
	u8 version;
	u8 ac_info;
} __packed;

struct lib80211_qos_ac_parameter {
	u8 aci_aifsn;
	u8 ecw_min_max;
	__le16 tx_op_limit;
} __packed;

struct lib80211_qos_parameter_info {
	struct lib80211_qos_information_element info_element;
	u8 reserved;
	struct lib80211_qos_ac_parameter ac_params_record[QOS_QUEUE_NUM];
} __packed;

struct lib80211_qos_parameters {
	__le16 cw_min[QOS_QUEUE_NUM];
	__le16 cw_max[QOS_QUEUE_NUM];
	u8 aifs[QOS_QUEUE_NUM];
	u8 flag[QOS_QUEUE_NUM];
	__le16 tx_op_limit[QOS_QUEUE_NUM];
} __packed;

struct lib80211_qos_data {
	struct lib80211_qos_parameters parameters;
	int active;
	int supported;
	u8 param_count;
	u8 old_param_count;
};

struct lib80211_tim_parameters {
	u8 tim_count;
	u8 tim_period;
} __packed;

/*******************************************************/

struct lib80211_tpc_report {
	u8 transmit_power;
	u8 link_margin;
} __packed;

struct lib80211_channel_map {
	u8 channel;
	u8 map;
} __packed;

struct lib80211_csa {
	u8 mode;
	u8 channel;
	u8 count;
} __packed;

struct lib80211_quiet {
	u8 count;
	u8 period;
	u8 duration;
	u8 offset;
} __packed;

struct lib80211_network {
	/* These entries are used to identify a unique network */
	u8 bssid[ETH_ALEN];
	u8 channel;
	/* Ensure null-terminated for any debug msgs */
	u8 ssid[IW_ESSID_MAX_SIZE + 1];
	u8 ssid_len;

	struct lib80211_qos_data qos_data;

	/* These are network statistics */
	struct lib80211_rx_stats stats;
	u16 capability;
	u8 rates[MAX_RATES_LENGTH];
	u8 rates_len;
	u8 rates_ex[MAX_RATES_EX_LENGTH];
	u8 rates_ex_len;
	unsigned long last_scanned;
	u8 mode;
	u32 flags;
	u32 last_associate;
	u32 time_stamp[2];
	u16 beacon_interval;
	u16 listen_interval;
	u16 atim_window;
	u8 erp_value;
	u8 wpa_ie[MAX_WPA_IE_LEN];
	size_t wpa_ie_len;
	u8 rsn_ie[MAX_WPA_IE_LEN];
	size_t rsn_ie_len;
	struct lib80211_tim_parameters tim;

	/* 802.11h info */

	/* Power Constraint - mandatory if spctrm mgmt required */
	u8 power_constraint;

	/* TPC Report - mandatory if spctrm mgmt required */
	struct lib80211_tpc_report tpc_report;

	/* Channel Switch Announcement - optional if spctrm mgmt required */
	struct lib80211_csa csa;

	/* Quiet - optional if spctrm mgmt required */
	struct lib80211_quiet quiet;

	struct list_head list;
};

enum lib80211_state {
	LIB80211_UNINITIALIZED = 0,
	LIB80211_INITIALIZED,
	LIB80211_ASSOCIATING,
	LIB80211_ASSOCIATED,
	LIB80211_AUTHENTICATING,
	LIB80211_AUTHENTICATED,
	LIB80211_SHUTDOWN
};

#define DEFAULT_MAX_SCAN_AGE (15 * HZ)
#define DEFAULT_FTS 2346

#define CFG_LIB80211_RESERVE_FCS (1<<0)
#define CFG_LIB80211_COMPUTE_FCS (1<<1)
#define CFG_LIB80211_RTS (1<<2)

#define LIB80211_24GHZ_MIN_CHANNEL 1
#define LIB80211_24GHZ_MAX_CHANNEL 14
#define LIB80211_24GHZ_CHANNELS (LIB80211_24GHZ_MAX_CHANNEL - \
				  LIB80211_24GHZ_MIN_CHANNEL + 1)

#define LIB80211_52GHZ_MIN_CHANNEL 34
#define LIB80211_52GHZ_MAX_CHANNEL 165
#define LIB80211_52GHZ_CHANNELS (LIB80211_52GHZ_MAX_CHANNEL - \
				  LIB80211_52GHZ_MIN_CHANNEL + 1)

enum {
	LIB80211_CH_PASSIVE_ONLY = (1 << 0),
	LIB80211_CH_80211H_RULES = (1 << 1),
	LIB80211_CH_B_ONLY = (1 << 2),
	LIB80211_CH_NO_IBSS = (1 << 3),
	LIB80211_CH_UNIFORM_SPREADING = (1 << 4),
	LIB80211_CH_RADAR_DETECT = (1 << 5),
	LIB80211_CH_INVALID = (1 << 6),
};

struct lib80211_channel {
	u32 freq;	/* in MHz */
	u8 channel;
	u8 flags;
	u8 max_power;	/* in dBm */
};

struct lib80211_geo {
	u8 name[4];
	u8 bg_channels;
	u8 a_channels;
	struct lib80211_channel bg[LIB80211_24GHZ_CHANNELS];
	struct lib80211_channel a[LIB80211_52GHZ_CHANNELS];
};

#define NUM_WEP_KEYS	4

enum {
	IEEE80211_CRYPTO_TKIP_COUNTERMEASURES = (1 << 0),
};

struct module;

struct lib80211_crypto_ops {
	const char *name;
	struct list_head list;

	/* init new crypto context (e.g., allocate private data space,
	 * select IV, etc.); returns NULL on failure or pointer to allocated
	 * private data on success */
	void *(*init) (int keyidx);

	/* deinitialize crypto context and free allocated private data */
	void (*deinit) (void *priv);

	/* encrypt/decrypt return < 0 on error or >= 0 on success. The return
	 * value from decrypt_mpdu is passed as the keyidx value for
	 * decrypt_msdu. skb must have enough head and tail room for the
	 * encryption; if not, error will be returned; these functions are
	 * called for all MPDUs (i.e., fragments).
	 */
	int (*encrypt_mpdu) (struct sk_buff * skb, int hdr_len, void *priv);
	int (*decrypt_mpdu) (struct sk_buff * skb, int hdr_len, void *priv);

	/* These functions are called for full MSDUs, i.e. full frames.
	 * These can be NULL if full MSDU operations are not needed. */
	int (*encrypt_msdu) (struct sk_buff * skb, int hdr_len, void *priv);
	int (*decrypt_msdu) (struct sk_buff * skb, int keyidx, int hdr_len,
			     void *priv);

	int (*set_key) (void *key, int len, u8 * seq, void *priv);
	int (*get_key) (void *key, int len, u8 * seq, void *priv);

	/* procfs handler for printing out key information and possible
	 * statistics */
	void (*print_stats) (struct seq_file *m, void *priv);

	/* Crypto specific flag get/set for configuration settings */
	unsigned long (*get_flags) (void *priv);
	unsigned long (*set_flags) (unsigned long flags, void *priv);

	/* maximum number of bytes added by encryption; encrypt buf is
	 * allocated with extra_prefix_len bytes, copy of in_buf, and
	 * extra_postfix_len; encrypt need not use all this space, but
	 * the result must start at the beginning of the buffer and correct
	 * length must be returned */
	int extra_mpdu_prefix_len, extra_mpdu_postfix_len;
	int extra_msdu_prefix_len, extra_msdu_postfix_len;

	struct module *owner;
};

struct lib80211_crypt_info {
	char *name;
	/* Most clients will already have a lock,
	   so just point to that. */
	spinlock_t *lock;

	struct lib80211_crypt_data *crypt[NUM_WEP_KEYS];
	int tx_keyidx;		/* default TX key index (crypt[tx_keyidx]) */
	struct list_head crypt_deinit_list;
	struct timer_list crypt_deinit_timer;
	int crypt_quiesced;
};

struct lib80211_device {
	struct net_device *dev;
	struct wireless_dev wdev;
	struct lib80211_security sec;

	/* Bookkeeping structures */
	struct lib80211_stats ieee_stats;

	struct lib80211_geo geo;
	struct ieee80211_supported_band bg_band;
	struct ieee80211_supported_band a_band;

	/* Probe / Beacon management */
	struct list_head network_free_list;
	struct list_head network_list;
	struct lib80211_network *networks[MAX_NETWORK_COUNT];
	int scans;
	int scan_age;

	int iw_mode;		/* operating mode (IW_MODE_*) */
	struct iw_spy_data spy_data;	/* iwspy support */
	bool spy_enabled;

	spinlock_t lock;

	int tx_headroom;	/* Set to size of any additional room needed at front
				 * of allocated Tx SKBs */
	u32 config;

	/* WEP and other encryption related settings at the device level */
	int open_wep;		/* Set to 1 to allow unencrypted frames */

	/* If the host performs {en,de}cryption, then set to 1 */
	int host_encrypt;
	int host_encrypt_msdu;
	int host_decrypt;
	/* host performs multicast decryption */
	int host_mc_decrypt;

	/* host should strip IV and ICV from protected frames */
	/* meaningful only when hardware decryption is being used */
	int host_strip_iv_icv;

	int host_open_frag;
	int ieee802_1x;		/* is IEEE 802.1X used */

	/* WPA data */
	int wpa_enabled;
	int drop_unencrypted;
	int privacy_invoked;
	size_t wpa_ie_len;
	u8 *wpa_ie;

	struct lib80211_crypt_info crypt_info;

	int bcrx_sta_key;	/* use individual keys to override default keys even
				 * with RX of broad/multicast frames */

	/* Fragmentation structures */
	struct lib80211_frag_entry frag_cache[LIB80211_FRAG_CACHE_LEN];
	unsigned int frag_next_idx;
	u16 fts;		/* Fragmentation Threshold */
	u16 rts;		/* RTS threshold */

	/* Association info */
	u8 bssid[ETH_ALEN];

	enum lib80211_state state;

	int mode;		/* A, B, G */
	int modulation;		/* CCK, OFDM */
	int freq_band;		/* 2.4Ghz, 5.2Ghz, Mixed */
	int abg_true;		/* ABG flag              */

	int perfect_rssi;
	int worst_rssi;

	u16 prev_seq_ctl;	/* used to drop duplicate frames */

	/* Callback functions */
	void (*set_security) (struct net_device * dev,
			      struct lib80211_security * sec);
	netdev_tx_t (*hard_start_xmit) (struct lib80211_txb * txb,
					struct net_device * dev, int pri);
	int (*is_queue_full) (struct net_device * dev, int pri);

	int (*handle_management) (struct net_device * dev,
				  struct lib80211_network * network, u16 type);
	int (*is_qos_active) (struct net_device *dev, struct sk_buff *skb);

	/* Typical STA methods */
	int (*handle_auth) (struct net_device * dev,
			    struct lib80211_auth * auth);
	int (*handle_deauth) (struct net_device * dev,
			      struct lib80211_deauth * auth);
	int (*handle_action) (struct net_device * dev,
			      struct lib80211_action * action,
			      struct lib80211_rx_stats * stats);
	int (*handle_disassoc) (struct net_device * dev,
				struct lib80211_disassoc * assoc);
	int (*handle_beacon) (struct net_device * dev,
			      struct lib80211_beacon * beacon,
			      struct lib80211_network * network);
	int (*handle_probe_response) (struct net_device * dev,
				      struct lib80211_probe_response * resp,
				      struct lib80211_network * network);
	int (*handle_probe_request) (struct net_device * dev,
				     struct lib80211_probe_request * req,
				     struct lib80211_rx_stats * stats);
	int (*handle_assoc_response) (struct net_device * dev,
				      struct lib80211_assoc_response * resp,
				      struct lib80211_network * network);

	/* Typical AP methods */
	int (*handle_assoc_request) (struct net_device * dev);
	int (*handle_reassoc_request) (struct net_device * dev,
				       struct lib80211_reassoc_request * req);

	/* This must be the last item so that it points to the data
	 * allocated beyond this structure by alloc_lib80211 */
	u8 priv[];
};

#define IEEE_A            (1<<0)
#define IEEE_B            (1<<1)
#define IEEE_G            (1<<2)
#define IEEE_MODE_MASK    (IEEE_A|IEEE_B|IEEE_G)

static inline void *lib80211_priv(struct net_device *dev)
{
	return ((struct lib80211_device *)netdev_priv(dev))->priv;
}

static inline int lib80211_is_valid_mode(struct lib80211_device *ieee,
					  int mode)
{
	/*
	 * It is possible for both access points and our device to support
	 * combinations of modes, so as long as there is one valid combination
	 * of ap/device supported modes, then return success
	 *
	 */
	if ((mode & IEEE_A) &&
	    (ieee->modulation & LIB80211_OFDM_MODULATION) &&
	    (ieee->freq_band & LIB80211_52GHZ_BAND))
		return 1;

	if ((mode & IEEE_G) &&
	    (ieee->modulation & LIB80211_OFDM_MODULATION) &&
	    (ieee->freq_band & LIB80211_24GHZ_BAND))
		return 1;

	if ((mode & IEEE_B) &&
	    (ieee->modulation & LIB80211_CCK_MODULATION) &&
	    (ieee->freq_band & LIB80211_24GHZ_BAND))
		return 1;

	return 0;
}

static inline int lib80211_get_hdrlen(u16 fc)
{
	int hdrlen = LIB80211_3ADDR_LEN;
	u16 stype = WLAN_FC_GET_STYPE(fc);

	switch (WLAN_FC_GET_TYPE(fc)) {
	case IEEE80211_FTYPE_DATA:
		if ((fc & IEEE80211_FCTL_FROMDS) && (fc & IEEE80211_FCTL_TODS))
			hdrlen = LIB80211_4ADDR_LEN;
		if (stype & IEEE80211_STYPE_QOS_DATA)
			hdrlen += 2;
		break;
	case IEEE80211_FTYPE_CTL:
		switch (WLAN_FC_GET_STYPE(fc)) {
		case IEEE80211_STYPE_CTS:
		case IEEE80211_STYPE_ACK:
			hdrlen = LIB80211_1ADDR_LEN;
			break;
		default:
			hdrlen = LIB80211_2ADDR_LEN;
			break;
		}
		break;
	}

	return hdrlen;
}

static inline u8 *lib80211_get_payload(struct ieee80211_hdr *hdr)
{
	switch (lib80211_get_hdrlen(le16_to_cpu(hdr->frame_control))) {
	case LIB80211_1ADDR_LEN:
		return ((struct lib80211_hdr_1addr *)hdr)->payload;
	case LIB80211_2ADDR_LEN:
		return ((struct lib80211_hdr_2addr *)hdr)->payload;
	case LIB80211_3ADDR_LEN:
		return ((struct lib80211_hdr_3addr *)hdr)->payload;
	case LIB80211_4ADDR_LEN:
		return ((struct lib80211_hdr_4addr *)hdr)->payload;
	}
	return NULL;
}

static inline int lib80211_is_ofdm_rate(u8 rate)
{
	switch (rate & ~LIB80211_BASIC_RATE_MASK) {
	case LIB80211_OFDM_RATE_6MB:
	case LIB80211_OFDM_RATE_9MB:
	case LIB80211_OFDM_RATE_12MB:
	case LIB80211_OFDM_RATE_18MB:
	case LIB80211_OFDM_RATE_24MB:
	case LIB80211_OFDM_RATE_36MB:
	case LIB80211_OFDM_RATE_48MB:
	case LIB80211_OFDM_RATE_54MB:
		return 1;
	}
	return 0;
}

static inline int lib80211_is_cck_rate(u8 rate)
{
	switch (rate & ~LIB80211_BASIC_RATE_MASK) {
	case LIB80211_CCK_RATE_1MB:
	case LIB80211_CCK_RATE_2MB:
	case LIB80211_CCK_RATE_5MB:
	case LIB80211_CCK_RATE_11MB:
		return 1;
	}
	return 0;
}

/* lib80211.c */
void free_lib80211(struct net_device *dev, int monitor);
struct net_device *alloc_lib80211(int sizeof_priv, int monitor);

void lib80211_networks_age(struct lib80211_device *ieee, unsigned long age_secs);

int lib80211_set_encryption(struct lib80211_device *ieee);

/* lib80211_tx.c */
netdev_tx_t lib80211_xmit(struct sk_buff *skb, struct net_device *dev);
void lib80211_txb_free(struct lib80211_txb *);

/* lib80211_rx.c */
int lib80211_rx(struct lib80211_device *ieee, struct sk_buff *skb,
	      struct lib80211_rx_stats *rx_stats);
/* make sure to set stats->len */
void lib80211_rx_mgt(struct lib80211_device *ieee, struct lib80211_hdr_4addr *header,
		   struct lib80211_rx_stats *stats);

/* lib80211_geo.c */
const struct lib80211_geo *lib80211_get_geo(struct lib80211_device *ieee);
void lib80211_set_geo(struct lib80211_device *ieee, const struct lib80211_geo *geo);

int lib80211_is_valid_channel(struct lib80211_device *ieee, u8 channel);
int lib80211_channel_to_index(struct lib80211_device *ieee, u8 channel);
u8 lib80211_freq_to_channel(struct lib80211_device *ieee, u32 freq);
u8 lib80211_get_channel_flags(struct lib80211_device *ieee, u8 channel);
const struct lib80211_channel *lib80211_get_channel(struct lib80211_device *ieee,
						u8 channel);
u32 lib80211_channel_to_freq(struct lib80211_device *ieee, u8 channel);

/* lib80211_wx.c */
int lib80211_wx_get_scan(struct lib80211_device *ieee, struct iw_request_info *info,
		       union iwreq_data *wrqu, char *key);
int lib80211_wx_set_encode(struct lib80211_device *ieee,
			 struct iw_request_info *info, union iwreq_data *wrqu,
			 char *key);
int lib80211_wx_get_encode(struct lib80211_device *ieee,
			 struct iw_request_info *info, union iwreq_data *wrqu,
			 char *key);
int lib80211_wx_set_encodeext(struct lib80211_device *ieee,
			    struct iw_request_info *info,
			    union iwreq_data *wrqu, char *extra);
int lib80211_wx_get_encodeext(struct lib80211_device *ieee,
			    struct iw_request_info *info,
			    union iwreq_data *wrqu, char *extra);

static inline void lib80211_increment_scans(struct lib80211_device *ieee)
{
	ieee->scans++;
}

static inline int lib80211_get_scans(struct lib80211_device *ieee)
{
	return ieee->scans;
}

struct lib80211_crypt_data {
	struct list_head list;	/* delayed deletion list */
	const struct lib80211_crypto_ops *ops;
	void *priv;
	atomic_t refcnt;
};

int lib80211_crypt_info_init(struct lib80211_crypt_info *info, char *name,
			   spinlock_t *lock);
void lib80211_crypt_info_free(struct lib80211_crypt_info *info);
int lib80211_register_crypto_ops(const struct lib80211_crypto_ops *ops);
int lib80211_unregister_crypto_ops(const struct lib80211_crypto_ops *ops);
const struct lib80211_crypto_ops *lib80211_get_crypto_ops(const char *name);
void lib80211_crypt_delayed_deinit(struct lib80211_crypt_info *info,
				 struct lib80211_crypt_data **crypt);

/* must be called in the listed order */
int lib80211_crypto_init(void);
int lib80211_crypto_ccmp_init(void);
int lib80211_crypto_tkip_init(void);
int lib80211_crypto_wep_init(void);

void lib80211_crypto_wep_exit(void);
void lib80211_crypto_tkip_exit(void);
void lib80211_crypto_ccmp_exit(void);
void lib80211_crypto_exit(void);

#if 0
int lib80211_wx_set_spy(struct net_device *dev, struct iw_request_info *info,
		   union iwreq_data *wrqu, char *extra);
int lib80211_wx_get_spy(struct net_device *dev, struct iw_request_info *info,
		   union iwreq_data *wrqu, char *extra);
int lib80211_wx_set_thrspy(struct net_device *dev, struct iw_request_info *info,
		      union iwreq_data *wrqu, char *extra);
int lib80211_wx_get_thrspy(struct net_device *dev, struct iw_request_info *info,
		      union iwreq_data *wrqu, char *extra);
void lib80211_spy_update(struct net_device *dev, unsigned char *address,
		       struct iw_quality *wstats);
#endif

#endif				/* LIB80211_H */
