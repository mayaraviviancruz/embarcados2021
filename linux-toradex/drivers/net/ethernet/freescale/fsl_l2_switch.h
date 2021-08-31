/*
 *   Copyright 2010-2012 Freescale Semiconductor, Inc
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or (at
 *   your option) any later version.
 *
 */

#ifndef FSL_L2_SWITCH_H
#define	FSL_L2_SWITCH_H

/* Interrupt events/masks */
#define FEC_ENET_HBERR	BIT(31)	/* Heartbeat error */
#define FEC_ENET_BABR	BIT(30)	/* Babbling receiver */
#define FEC_ENET_BABT	BIT(29)	/* Babbling transmitter */
#define FEC_ENET_GRA	BIT(28)	/* Graceful stop complete */
#define FEC_ENET_TXF	BIT(27)	/* Full frame transmitted */
#define FEC_ENET_TXB	BIT(26)	/* A buffer was transmitted */
#define FEC_ENET_RXF	BIT(25)	/* Full frame received */
#define FEC_ENET_RXB	BIT(24)	/* A buffer was received */
#define FEC_ENET_MII	BIT(23)	/* MII interrupt */
#define FEC_ENET_EBERR	BIT(22)	/* SDMA bus error */

#define		NMII	20

/* Make MII read/write commands for the FEC */
#define mk_mii_read(REG)	(0x60020000 | ((REG & 0x1f) << 18))
#define mk_mii_write(REG, VAL)	(0x50020000 | ((REG & 0x1f) << 18) | \
						(VAL & 0xffff))
/* MII MMFR bits definition */
#define ESW_MMFR_ST		BIT(30)
#define ESW_MMFR_OP_READ	(2 << 28)
#define ESW_MMFR_OP_WRITE	BIT(28)
#define ESW_MMFR_PA(v)		((v & 0x1f) << 23)
#define ESW_MMFR_RA(v)		((v & 0x1f) << 18)
#define ESW_MMFR_TA		(2 << 16)
#define ESW_MMFR_DATA(v)	(v & 0xffff)

#define ESW_MII_TIMEOUT		30 /* ms */

/* Transmitter timeout.*/
#define TX_TIMEOUT (2 * HZ)

/* The Switch stores dest/src/type, data,
 * and checksum for receive packets.
 */
#define PKT_MAXBUF_SIZE         1518
#define PKT_MINBUF_SIZE         64
#define PKT_MAXBLR_SIZE         1520

/* The 5441x RX control register also contains maximum frame
 * size bits.
 */
#define OPT_FRAME_SIZE  (PKT_MAXBUF_SIZE << 16)

/* The number of Tx and Rx buffers. These are allocated from the page
 * pool. The code may assume these are power of two, so it it best
 * to keep them that size.
 * We don't need to allocate pages for the transmitter.  We just use
 * the skbuffer directly.
 */
#ifdef CONFIG_SWITCH_DMA_USE_SRAM
#define SWITCH_ENET_RX_PAGES       6
#else
#define SWITCH_ENET_RX_PAGES       8
#endif

#define SWITCH_ENET_RX_FRSIZE	2048
#define SWITCH_ENET_RX_FRPPG	(PAGE_SIZE / SWITCH_ENET_RX_FRSIZE)
#define RX_RING_SIZE		(SWITCH_ENET_RX_FRPPG * SWITCH_ENET_RX_PAGES)
#define SWITCH_ENET_TX_FRSIZE	2048
#define SWITCH_ENET_TX_FRPPG	(PAGE_SIZE / SWITCH_ENET_TX_FRSIZE)

#ifdef CONFIG_SWITCH_DMA_USE_SRAM
#define TX_RING_SIZE		8      /* Must be power of two */
#define TX_RING_MOD_MASK	7      /*   for this to work */
#else
#define TX_RING_SIZE		16      /* Must be power of two */
#define TX_RING_MOD_MASK	15      /*   for this to work */
#endif

#define SWITCH_EPORT_NUMBER	2

#if (((RX_RING_SIZE + TX_RING_SIZE) * 8) > PAGE_SIZE)
#error "L2SWITCH: descriptor ring size constants too large"
#endif

/* memory-mapped register offset */
#define	FEC_ESW_REVISION	0x00
#define	FEC_ESW_SCRATCH		0x04
#define	FEC_ESW_PER		0x08

#define	FEC_ESW_VLANV	0x10
#define	FEC_ESW_DBCR	0x14
#define	FEC_ESW_DMCR	0x18
#define	FEC_ESW_BKLR	0x1C
#define	FEC_ESW_BMPC	0x20
#define	FEC_ESW_MODE	0x24
#define	FEC_ESW_VIMSEL	0x28
#define	FEC_ESW_VOMSEL	0x2C
#define	FEC_ESW_VIMEN	0x30
#define	FEC_ESW_VID	0x34

#define	FEC_ESW_MCR	0x40
#define	FEC_ESW_EGMAP	0x44
#define	FEC_ESW_INGMAP	0x48
#define	FEC_ESW_INGSAL	0x4C
#define	FEC_ESW_INGSAH	0x50
#define	FEC_ESW_INGDAL	0x54
#define	FEC_ESW_INGDAH	0x58
#define	FEC_ESW_ENGSAL	0x5C
#define	FEC_ESW_ENGSAH	0x60
#define	FEC_ESW_ENGDAL	0x64
#define	FEC_ESW_ENGDAH	0x68
#define	FEC_ESW_MCVAL	0x6C

#define	FEC_ESW_MMSR	0x80
#define	FEC_ESW_LMT	0x84
#define	FEC_ESW_LFC	0x88
#define	FEC_ESW_PCSR	0x8C
#define	FEC_ESW_IOSR	0x90
#define	FEC_ESW_QWT	0x94

#define	FEC_ESW_P0BCT	0x9C

#define	FEC_ESW_P0FFEN		0xBC
#define	FEC_ESW_PSNP(n)		(0xC0 + 4 * n)
#define	FEC_ESW_IPSNP(n)	(0xE0 + 4 * n)
#define	FEC_ESW_PVRES(n)	(0x100 + 4 * n)

#define	FEC_ESW_IPRES	0x140

/* port0-port2 Priority Configuration  0xFC0D_C180-C188 */
#define	FEC_ESW_PRES(n)	(0x180 + n * 4)

/* port0-port2 VLAN ID 0xFC0D_C200-C208 */
#define	FEC_ESW_PID(n)	(0x200 + 4 * n)

/* port0-port2 VLAN domain resolution entry 0xFC0D_C280-C2FC */
#define	FEC_ESW_VRES(n)	(0x280 + n * 4)

#define	FEC_ESW_DISCN	0x300
#define	FEC_ESW_DISCB	0x304
#define	FEC_ESW_NDISCN	0x308
#define	FEC_ESW_NDISCB	0x30C
/* per port statistics 0xFC0DC310_C33C */

#define FEC_ESW_POQC(n)		(0x310 + n * 16)
#define FEC_ESW_PMVID(n)	(0x310 + n * 16 + 0x04)
#define FEC_ESW_PMVTAG(n)	(0x310 + n * 16 + 0x08)
#define FEC_ESW_PBL(n)		(0x310 + n * 16 + 0x0C)

#define	FEC_ESW_ISR	0x400 /* Interrupt event reg */
#define	FEC_ESW_IMR	0x404 /* Interrupt mask reg */
#define	FEC_ESW_RDSR	0x408 /* Receive descriptor ring */
#define	FEC_ESW_TDSR	0x40C /* Transmit descriptor ring */
#define	FEC_ESW_MRBR	0x410 /* Maximum receive buff size */
#define	FEC_ESW_RDAR	0x414 /* Receive descriptor active */
#define	FEC_ESW_TDAR	0x418 /* Transmit descriptor active */

#define	FEC_ESW_LREC0	0x500
#define	FEC_ESW_LREC1	0x504
#define	FEC_ESW_LSR	0x508

#include <linux/phy.h>
struct switch_platform_data {
	phy_interface_t phy;
	unsigned char mac[ETH_ALEN];
};

#define FSL_FEC_MMFR0	(0x40)
#define FSL_FEC_MSCR0	(0x44)
#define FSL_FEC_MSCR1	(0x1044)
#define FSL_FEC_RCR0	(0x84)
#define FSL_FEC_RCR1	(0x1084)
#define FSL_FEC_TCR0	(0xc4)
#define FSL_FEC_TCR1	(0x10c4)
#define FSL_FEC_ECR0	(0x24)
#define FSL_FEC_ECR1	(0x1024)
#define FSL_FEC_EIR0	(0x04)
#define FSL_FEC_EIR1	(0x1004)
#define FSL_FEC_EIMR0   (0x08)
#define FSL_FEC_EIMR1   (0x1008)
#define FSL_FEC_PALR0	(0x0e4)
#define FSL_FEC_PAUR0	(0x0e8)
#define FSL_FEC_PALR1	(0x10e4)
#define FSL_FEC_PAUR1	(0x10e8)
#define FSL_FEC_X_WMRK0	(0x0144)
#define FSL_FEC_X_WMRK1	(0x1144)

#define FSL_FEC_RCR_MII_MODE                 (0x00000004)
#define FSL_FEC_RCR_PROM                     (0x00000008)
#define FSL_FEC_RCR_RMII_MODE                (0x00000100)
#define FSL_FEC_RCR_MAX_FL(x)                (((x) & 0x00003FFF) << 16)
#define FSL_FEC_RCR_CRC_FWD                  (0x00004000)

#define FSL_FEC_TCR_FDEN                     (0x00000004)

#define FSL_FEC_ECR_ETHER_EN                 (0x00000002)
#define FSL_FEC_ECR_ENA_1588                 (0x00000010)

#define LEARNING_AGING_TIMER (10 * HZ)

/* Define the buffer descriptor structure. */
typedef struct bufdesc {
	unsigned short	cbd_datlen;		/* Data length */
	unsigned short	cbd_sc;			/* Control and status info */
	unsigned long	cbd_bufaddr;		/* Buffer address */
} cbd_t;

typedef struct bufdesc_rx {
	unsigned short	cbd_datlen;		/* Data length */
	unsigned short	cbd_sc;			/* Control and status info */
	unsigned long	cbd_bufaddr;		/* Buffer address */
} cbd_t_r;

/* Forward declarations of some structures to support different PHYs */
typedef struct _phy_cmd_t {
	uint mii_data;
	void (*funct)(uint mii_reg, struct net_device *dev);
} phy_cmd_t;

typedef struct _phy_info_t {
	uint id;
	char *name;

	const phy_cmd_t *config;
	const phy_cmd_t *startup;
	const phy_cmd_t *ack_int;
	const phy_cmd_t *shutdown;
} phy_info_t;

struct port_status {
	/* 1: link is up, 0: link is down */
	int port1_link_status;
	int port2_link_status;
	/* 1: blocking, 0: unblocking */
	int port0_block_status;
	int port1_block_status;
	int port2_block_status;
};

/* The switch buffer descriptors track the ring buffers.  The rx_bd_base and
 * tx_bd_base always point to the base of the buffer descriptors.  The
 * cur_rx and cur_tx point to the currently available buffer.
 * The dirty_tx tracks the current buffer that is being sent by the
 * controller.  The cur_tx and dirty_tx are equal under both completely
 * empty and completely full conditions.  The empty/ready indicator in
 * the buffer descriptor determines the actual condition.
 */
struct switch_enet_private {
	/* Hardware registers of the switch device */
	void __iomem *membase;
	void __iomem *macbase; /* MAC address lookup table */
	void __iomem *enetbase;

	struct clk *clk_esw;
	struct clk *clk_enet;
	struct clk *clk_enet0;
	struct clk *clk_enet1;

	struct net_device *netdev;
	struct platform_device *pdev;

	int	dev_id;

	/* The saved address of a sent-in-place packet/buffer, for skfree(). */
	unsigned char *tx_bounce[TX_RING_SIZE];
	struct  sk_buff *tx_skbuff[TX_RING_SIZE];
	struct	sk_buff *rx_skbuff[RX_RING_SIZE];
	ushort  skb_cur;
	ushort  skb_dirty;

	/* CPM dual port RAM relative addresses */
	dma_addr_t	bd_dma;

	cbd_t   *rx_bd_base;		/* Address of Rx and Tx buffers. */
	cbd_t   *tx_bd_base;
	cbd_t   *cur_rx, *cur_tx;	/* The next free ring entry */
	cbd_t   *dirty_tx;		/* The ring entries to be free()ed. */
	uint    tx_full;
	/* hold while accessing the HW like ringbuffer for tx/rx but not MAC */
	spinlock_t hw_lock;

	uint    sequence_done;

	int     index;
	int     opened;
	int     full_duplex;
	int     msg_enable;

	/* timer related */
	/* current time (for timestamping) */
	int curr_time;
	/* flag set by timer when curr_time changed
	 * and cleared by serving function
	 */
	int time_changed;

	/* Timer for Aging */
	struct timer_list       timer_aging;
	int learning_irqhandle_enable;
};

struct switch_platform_private {
	unsigned long           quirks;
	int                     num_slots;	/* Slots on controller */
	struct switch_enet_private *fep_host[0];	/* Pointers to hosts */
};

/* Receive is empty */
#define BD_SC_EMPTY     ((unsigned short)0x8000)
/* Transmit is ready */
#define BD_SC_READY     ((unsigned short)0x8000)
/* Last buffer descriptor */
#define BD_SC_WRAP      ((unsigned short)0x2000)
/* Interrupt on change */
#define BD_SC_INTRPT    ((unsigned short)0x1000)
/* Continuous mode */
#define BD_SC_CM        ((unsigned short)0x0200)
/* Rec'd too many idles */
#define BD_SC_ID        ((unsigned short)0x0100)
/* xmt preamble */
#define BD_SC_P         ((unsigned short)0x0100)
/* Break received */
#define BD_SC_BR        ((unsigned short)0x0020)
/* Framing error */
#define BD_SC_FR        ((unsigned short)0x0010)
/* Parity error */
#define BD_SC_PR        ((unsigned short)0x0008)
/* Overrun */
#define BD_SC_OV        ((unsigned short)0x0002)
#define BD_SC_CD        ((unsigned short)0x0001)

/* Buffer descriptor control/status used by Ethernet receive.*/
#define BD_ENET_RX_EMPTY        ((unsigned short)0x8000)
#define BD_ENET_RX_WRAP         ((unsigned short)0x2000)
#define BD_ENET_RX_INTR         ((unsigned short)0x1000)
#define BD_ENET_RX_LAST         ((unsigned short)0x0800)
#define BD_ENET_RX_FIRST        ((unsigned short)0x0400)
#define BD_ENET_RX_MISS         ((unsigned short)0x0100)
#define BD_ENET_RX_LG           ((unsigned short)0x0020)
#define BD_ENET_RX_NO           ((unsigned short)0x0010)
#define BD_ENET_RX_SH           ((unsigned short)0x0008)
#define BD_ENET_RX_CR           ((unsigned short)0x0004)
#define BD_ENET_RX_OV           ((unsigned short)0x0002)
#define BD_ENET_RX_CL           ((unsigned short)0x0001)

/* All status bits */
#define BD_ENET_RX_STATS        ((unsigned short)0x013f)

/* Buffer descriptor control/status used by Ethernet transmit. */
#define BD_ENET_TX_READY        ((unsigned short)0x8000)
#define BD_ENET_TX_PAD          ((unsigned short)0x4000)
#define BD_ENET_TX_WRAP         ((unsigned short)0x2000)
#define BD_ENET_TX_INTR         ((unsigned short)0x1000)
#define BD_ENET_TX_LAST         ((unsigned short)0x0800)
#define BD_ENET_TX_TC           ((unsigned short)0x0400)
#define BD_ENET_TX_DEF          ((unsigned short)0x0200)
#define BD_ENET_TX_HB           ((unsigned short)0x0100)
#define BD_ENET_TX_LC           ((unsigned short)0x0080)
#define BD_ENET_TX_RL           ((unsigned short)0x0040)
#define BD_ENET_TX_RCMASK       ((unsigned short)0x003c)
#define BD_ENET_TX_UN           ((unsigned short)0x0002)
#define BD_ENET_TX_CSL          ((unsigned short)0x0001)

/* All status bits */
#define BD_ENET_TX_STATS        ((unsigned short)0x03ff)

/* Copy from validation code */
#define RX_BUFFER_SIZE 1520
#define TX_BUFFER_SIZE 1520
#define NUM_RXBDS 20
#define NUM_TXBDS 20

#define TX_BD_R                 0x8000
#define TX_BD_TO1               0x4000
#define TX_BD_W                 0x2000
#define TX_BD_TO2               0x1000
#define TX_BD_L                 0x0800
#define TX_BD_TC                0x0400

#define TX_BD_INT		0x40000000
#define TX_BD_TS		0x20000000
#define TX_BD_PINS		0x10000000
#define TX_BD_IINS		0x08000000
#define TX_BD_TXE		0x00008000
#define TX_BD_UE		0x00002000
#define TX_BD_EE		0x00001000
#define TX_BD_FE		0x00000800
#define TX_BD_LCE		0x00000400
#define TX_BD_OE		0x00000200
#define TX_BD_TSE		0x00000100
#define TX_BD_BDU		0x80000000

#define RX_BD_E                 0x8000
#define RX_BD_R01               0x4000
#define RX_BD_W                 0x2000
#define RX_BD_R02               0x1000
#define RX_BD_L                 0x0800
#define RX_BD_M                 0x0100
#define RX_BD_BC                0x0080
#define RX_BD_MC                0x0040
#define RX_BD_LG                0x0020
#define RX_BD_NO                0x0010
#define RX_BD_CR                0x0004
#define RX_BD_OV                0x0002
#define RX_BD_TR                0x0001

#define RX_BD_ME               0x80000000
#define RX_BD_PE               0x04000000
#define RX_BD_CE               0x02000000
#define RX_BD_UC               0x01000000
#define RX_BD_INT              0x00800000
#define RX_BD_ICE              0x00000020
#define RX_BD_PCR              0x00000010
#define RX_BD_VLAN             0x00000004
#define RX_BD_IPV6             0x00000002
#define RX_BD_FRAG             0x00000001
#define RX_BD_BDU              0x80000000

/* Address Table size in bytes(2048 64bit entry ) */
#define ESW_ATABLE_MEM_SIZE         (2048 * 8)
/* How many 64-bit elements fit in the address table */
#define ESW_ATABLE_MEM_NUM_ENTRIES  (2048)
/* Address Table Maximum number of entries in each Slot */
#define ATABLE_ENTRY_PER_SLOT 8
/* log2(ATABLE_ENTRY_PER_SLOT) */
#define ATABLE_ENTRY_PER_SLOT_bits 3
/* entry size in byte */
#define ATABLE_ENTRY_SIZE     8
/*  slot size in byte */
#define ATABLE_SLOT_SIZE    (ATABLE_ENTRY_PER_SLOT * ATABLE_ENTRY_SIZE)
/* width of timestamp variable (bits) within address table entry */
#define AT_DENTRY_TIMESTAMP_WIDTH    10
/* number of bits for port number storage */
#define AT_DENTRY_PORT_WIDTH     4
/* number of bits for port bitmask number storage */
#define AT_SENTRY_PORT_WIDTH     7
/* address table static entry port bitmask start address bit */
#define AT_SENTRY_PORTMASK_shift     21
/* number of bits for port priority storage */
#define AT_SENTRY_PRIO_WIDTH	7
/* address table static entry priority start address bit */
#define AT_SENTRY_PRIO_shift     18
/* address table dynamic entry port start address bit */
#define AT_DENTRY_PORT_shift     28
/* address table dynamic entry timestamp start address bit */
#define AT_DENTRY_TIME_shift     18
/* address table entry record type start address bit */
#define AT_ENTRY_TYPE_shift     17
/* address table entry record type bit: 1 static, 0 dynamic */
#define AT_ENTRY_TYPE_STATIC      1
#define AT_ENTRY_TYPE_DYNAMIC     0
/* address table entry record valid start address bit */
#define AT_ENTRY_VALID_shift     16
#define AT_ENTRY_RECORD_VALID     1

#define AT_EXTRACT_VALID(x)   \
	((x >> AT_ENTRY_VALID_shift) & AT_ENTRY_RECORD_VALID)

#define AT_EXTRACT_PORTMASK(x)  \
	((x >> AT_SENTRY_PORTMASK_shift) & AT_SENTRY_PORT_WIDTH)

#define AT_EXTRACT_PRIO(x)  \
	((x >> AT_SENTRY_PRIO_shift) & AT_SENTRY_PRIO_WIDTH)

/* return block corresponding to the 8 bit hash value calculated */
#define GET_BLOCK_PTR(hash)  (hash << 3)
#define AT_EXTRACT_TIMESTAMP(x) \
	((x >> AT_DENTRY_TIME_shift) & ((1 << AT_DENTRY_TIMESTAMP_WIDTH) - 1))
#define AT_EXTRACT_PORT(x)   \
	((x >> AT_DENTRY_PORT_shift) & ((1 << AT_DENTRY_PORT_WIDTH) - 1))
#define AT_SEXTRACT_PORT(x)  \
	((~((x >> AT_SENTRY_PORTMASK_shift) &  \
	   ((1 << AT_DENTRY_PORT_WIDTH) - 1))) >> 1)
#define TIMEDELTA(newtime, oldtime) \
	 ((newtime - oldtime) & \
	  ((1 << AT_DENTRY_TIMESTAMP_WIDTH) - 1))

#define AT_EXTRACT_IP_PROTOCOL(x) ((x >> 8) & 0xff)
#define AT_EXTRACT_TCP_UDP_PORT(x) ((x >> 16) & 0xffff)

/* increment time value respecting modulo. */
#define TIMEINCREMENT(time) \
	((time) = ((time)+1) & ((1 << AT_DENTRY_TIMESTAMP_WIDTH)-1))

/* Bit definitions and macros for FSL_ESW_REVISION */
#define FSL_ESW_REVISION_CORE_REVISION(x)      (((x)&0x0000FFFF) << 0)
#define FSL_ESW_REVISION_CUSTOMER_REVISION(x)  (((x)&0x0000FFFF) << 16)

/* Bit definitions and macros for FSL_ESW_PER */
#define FSL_ESW_PER_TE0                        (0x00000001)
#define FSL_ESW_PER_TE1                        (0x00000002)
#define FSL_ESW_PER_TE2                        (0x00000004)
#define FSL_ESW_PER_RE0                        (0x00010000)
#define FSL_ESW_PER_RE1                        (0x00020000)
#define FSL_ESW_PER_RE2                        (0x00040000)

/* Bit definitions and macros for FSL_ESW_VLANV */
#define FSL_ESW_VLANV_VV0                      (0x00000001)
#define FSL_ESW_VLANV_VV1                      (0x00000002)
#define FSL_ESW_VLANV_VV2                      (0x00000004)
#define FSL_ESW_VLANV_DU0                      (0x00010000)
#define FSL_ESW_VLANV_DU1                      (0x00020000)
#define FSL_ESW_VLANV_DU2                      (0x00040000)

/* Bit definitions and macros for FSL_ESW_DBCR */
#define FSL_ESW_DBCR_P0                        (0x00000001)
#define FSL_ESW_DBCR_P1                        (0x00000002)
#define FSL_ESW_DBCR_P2                        (0x00000004)

/* Bit definitions and macros for FSL_ESW_DMCR */
#define FSL_ESW_DMCR_P0                        (0x00000001)
#define FSL_ESW_DMCR_P1                        (0x00000002)
#define FSL_ESW_DMCR_P2                        (0x00000004)

/* Bit definitions and macros for FSL_ESW_BKLR */
#define FSL_ESW_BKLR_BE0			(0x00000001)
#define FSL_ESW_BKLR_BE1			(0x00000002)
#define FSL_ESW_BKLR_BE2			(0x00000004)
#define FSL_ESW_BKLR_LD0			(0x00010000)
#define FSL_ESW_BKLR_LD1			(0x00020000)
#define FSL_ESW_BKLR_LD2			(0x00040000)
#define	FSL_ESW_BKLR_LDX			(0x00070007)

/* Bit definitions and macros for FSL_ESW_BMPC */
#define FSL_ESW_BMPC_PORT(x)                   (((x) & 0x0000000F) << 0)
#define FSL_ESW_BMPC_MSG_TX                    (0x00000020)
#define FSL_ESW_BMPC_EN                        (0x00000040)
#define FSL_ESW_BMPC_DIS                       (0x00000080)
#define FSL_ESW_BMPC_PRIORITY(x)               (((x) & 0x00000007) << 13)
#define FSL_ESW_BMPC_PORTMASK(x)               (((x) & 0x00000007) << 16)

/* Bit definitions and macros for FSL_ESW_MODE */
#define FSL_ESW_MODE_SW_RST                    (0x00000001)
#define FSL_ESW_MODE_SW_EN                     (0x00000002)
#define FSL_ESW_MODE_STOP                      (0x00000080)
#define FSL_ESW_MODE_CRC_TRAN                  (0x00000100)
#define FSL_ESW_MODE_P0CT                      (0x00000200)
#define FSL_ESW_MODE_STATRST                   (0x80000000)

/* Bit definitions and macros for FSL_ESW_VIMSEL */
#define FSL_ESW_VIMSEL_IM0(x)                  (((x) & 0x00000003) << 0)
#define FSL_ESW_VIMSEL_IM1(x)                  (((x) & 0x00000003) << 2)
#define FSL_ESW_VIMSEL_IM2(x)                  (((x) & 0x00000003) << 4)

/* Bit definitions and macros for FSL_ESW_VOMSEL */
#define FSL_ESW_VOMSEL_OM0(x)                  (((x) & 0x00000003) << 0)
#define FSL_ESW_VOMSEL_OM1(x)                  (((x) & 0x00000003) << 2)
#define FSL_ESW_VOMSEL_OM2(x)                  (((x) & 0x00000003) << 4)

/* Bit definitions and macros for FSL_ESW_VIMEN */
#define FSL_ESW_VIMEN_EN0                      (0x00000001)
#define FSL_ESW_VIMEN_EN1                      (0x00000002)
#define FSL_ESW_VIMEN_EN2                      (0x00000004)

/* Bit definitions and macros for FSL_ESW_VID */
#define FSL_ESW_VID_TAG(x)                     (((x) & 0xFFFFFFFF) << 0)

/* Bit definitions and macros for FSL_ESW_MCR */
#define FSL_ESW_MCR_PORT(x)                    (((x) & 0x0000000F) << 0)
#define FSL_ESW_MCR_MEN                        (0x00000010)
#define FSL_ESW_MCR_INGMAP                     (0x00000020)
#define FSL_ESW_MCR_EGMAP                      (0x00000040)
#define FSL_ESW_MCR_INGSA                      (0x00000080)
#define FSL_ESW_MCR_INGDA                      (0x00000100)
#define FSL_ESW_MCR_EGSA                       (0x00000200)
#define FSL_ESW_MCR_EGDA                       (0x00000400)

/* Bit definitions and macros for FSL_ESW_EGMAP */
#define FSL_ESW_EGMAP_EG0                      (0x00000001)
#define FSL_ESW_EGMAP_EG1                      (0x00000002)
#define FSL_ESW_EGMAP_EG2                      (0x00000004)

/* Bit definitions and macros for FSL_ESW_INGMAP */
#define FSL_ESW_INGMAP_ING0                    (0x00000001)
#define FSL_ESW_INGMAP_ING1                    (0x00000002)
#define FSL_ESW_INGMAP_ING2                    (0x00000004)

/* Bit definitions and macros for FSL_ESW_INGSAL */
#define FSL_ESW_INGSAL_ADDLOW(x)               (((x) & 0xFFFFFFFF) << 0)

/* Bit definitions and macros for FSL_ESW_INGSAH */
#define FSL_ESW_INGSAH_ADDHIGH(x)              (((x) & 0x0000FFFF) << 0)

/* Bit definitions and macros for FSL_ESW_INGDAL */
#define FSL_ESW_INGDAL_ADDLOW(x)               (((x) & 0xFFFFFFFF) << 0)

/* Bit definitions and macros for FSL_ESW_INGDAH */
#define FSL_ESW_INGDAH_ADDHIGH(x)              (((x) & 0x0000FFFF) << 0)

/* Bit definitions and macros for FSL_ESW_ENGSAL */
#define FSL_ESW_ENGSAL_ADDLOW(x)               (((x) & 0xFFFFFFFF) << 0)

/* Bit definitions and macros for FSL_ESW_ENGSAH */
#define FSL_ESW_ENGSAH_ADDHIGH(x)              (((x) & 0x0000FFFF) << 0)

/* Bit definitions and macros for FSL_ESW_ENGDAL */
#define FSL_ESW_ENGDAL_ADDLOW(x)               (((x) & 0xFFFFFFFF) << 0)

/* Bit definitions and macros for FSL_ESW_ENGDAH */
#define FSL_ESW_ENGDAH_ADDHIGH(x)              (((x) & 0x0000FFFF) << 0)

/* Bit definitions and macros for FSL_ESW_MCVAL */
#define FSL_ESW_MCVAL_COUNT(x)                 (((x) & 0x000000FF) << 0)

/* Bit definitions and macros for FSL_ESW_MMSR */
#define FSL_ESW_MMSR_BUSY                      (0x00000001)
#define FSL_ESW_MMSR_NOCELL                    (0x00000002)
#define FSL_ESW_MMSR_MEMFULL                   (0x00000004)
#define FSL_ESW_MMSR_MFLATCH                   (0x00000008)
#define FSL_ESW_MMSR_DQ_GRNT                   (0x00000040)
#define FSL_ESW_MMSR_CELLS_AVAIL(x)            (((x) & 0x000000FF) << 16)

/* Bit definitions and macros for FSL_ESW_LMT */
#define FSL_ESW_LMT_THRESH(x)                  (((x) & 0x000000FF) << 0)

/* Bit definitions and macros for FSL_ESW_LFC */
#define FSL_ESW_LFC_COUNT(x)                   (((x) & 0xFFFFFFFF) << 0)

/* Bit definitions and macros for FSL_ESW_PCSR */
#define FSL_ESW_PCSR_PC0                       (0x00000001)
#define FSL_ESW_PCSR_PC1                       (0x00000002)
#define FSL_ESW_PCSR_PC2                       (0x00000004)

/* Bit definitions and macros for FSL_ESW_IOSR */
#define FSL_ESW_IOSR_OR0                       (0x00000001)
#define FSL_ESW_IOSR_OR1                       (0x00000002)
#define FSL_ESW_IOSR_OR2                       (0x00000004)

/* Bit definitions and macros for FSL_ESW_QWT */
#define FSL_ESW_QWT_Q0WT(x)                    (((x) & 0x0000001F) << 0)
#define FSL_ESW_QWT_Q1WT(x)                    (((x) & 0x0000001F) << 8)
#define FSL_ESW_QWT_Q2WT(x)                    (((x) & 0x0000001F) << 16)
#define FSL_ESW_QWT_Q3WT(x)                    (((x) & 0x0000001F) << 24)

/* Bit definitions and macros for FSL_ESW_P0BCT */
#define FSL_ESW_P0BCT_THRESH(x)                (((x) & 0x000000FF) << 0)

/* Bit definitions and macros for FSL_ESW_P0FFEN */
#define FSL_ESW_P0FFEN_FEN                     (0x00000001)
#define FSL_ESW_P0FFEN_FD(x)                   (((x) & 0x00000003) << 2)

/* Bit definitions and macros for FSL_ESW_PSNP */
#define FSL_ESW_PSNP_EN                        (0x00000001)
#define FSL_ESW_PSNP_MODE(x)                   (((x) & 0x00000003) << 1)
#define FSL_ESW_PSNP_CD                        (0x00000008)
#define FSL_ESW_PSNP_CS                        (0x00000010)
#define FSL_ESW_PSNP_PORT_COMPARE(x)           (((x) & 0x0000FFFF) << 16)

/* Bit definitions and macros for FSL_ESW_IPSNP */
#define FSL_ESW_IPSNP_EN                       (0x00000001)
#define FSL_ESW_IPSNP_MODE(x)                  (((x) & 0x00000003) << 1)
#define FSL_ESW_IPSNP_PROTOCOL(x)              (((x) & 0x000000FF) << 8)

/* Bit definitions and macros for FSL_ESW_PVRES */
#define FSL_ESW_PVRES_PRI0(x)                  (((x) & 0x00000007) << 0)
#define FSL_ESW_PVRES_PRI1(x)                  (((x) & 0x00000007) << 3)
#define FSL_ESW_PVRES_PRI2(x)                  (((x) & 0x00000007) << 6)
#define FSL_ESW_PVRES_PRI3(x)                  (((x) & 0x00000007) << 9)
#define FSL_ESW_PVRES_PRI4(x)                  (((x) & 0x00000007) << 12)
#define FSL_ESW_PVRES_PRI5(x)                  (((x) & 0x00000007) << 15)
#define FSL_ESW_PVRES_PRI6(x)                  (((x) & 0x00000007) << 18)
#define FSL_ESW_PVRES_PRI7(x)                  (((x) & 0x00000007) << 21)

/* Bit definitions and macros for FSL_ESW_IPRES */
#define FSL_ESW_IPRES_ADDRESS(x)               (((x) & 0x000000FF) << 0)
#define FSL_ESW_IPRES_IPV4SEL                  (0x00000100)
#define FSL_ESW_IPRES_PRI0(x)                  (((x) & 0x00000003) << 9)
#define FSL_ESW_IPRES_PRI1(x)                  (((x) & 0x00000003) << 11)
#define FSL_ESW_IPRES_PRI2(x)                   (((x) & 0x00000003) << 13)
#define FSL_ESW_IPRES_READ                     (0x80000000)

/* Bit definitions and macros for FSL_ESW_PRES */
#define FSL_ESW_PRES_VLAN                      (0x00000001)
#define FSL_ESW_PRES_IP                        (0x00000002)
#define FSL_ESW_PRES_MAC                       (0x00000004)
#define FSL_ESW_PRES_DFLT_PRI(x)               (((x) & 0x00000007) << 4)

/* Bit definitions and macros for FSL_ESW_PID */
#define FSL_ESW_PID_VLANID(x)                  (((x) & 0x0000FFFF) << 0)

/* Bit definitions and macros for FSL_ESW_VRES */
#define FSL_ESW_VRES_P0                        (0x00000001)
#define FSL_ESW_VRES_P1                        (0x00000002)
#define FSL_ESW_VRES_P2                        (0x00000004)
#define FSL_ESW_VRES_VLANID(x)                 (((x) & 0x00000FFF) << 3)

/* Bit definitions and macros for FSL_ESW_DISCN */
#define FSL_ESW_DISCN_COUNT(x)                 (((x) & 0xFFFFFFFF) << 0)

/* Bit definitions and macros for FSL_ESW_DISCB */
#define FSL_ESW_DISCB_COUNT(x)                 (((x) & 0xFFFFFFFF) << 0)

/* Bit definitions and macros for FSL_ESW_NDISCN */
#define FSL_ESW_NDISCN_COUNT(x)                (((x) & 0xFFFFFFFF) << 0)

/* Bit definitions and macros for FSL_ESW_NDISCB */
#define FSL_ESW_NDISCB_COUNT(x)                (((x) & 0xFFFFFFFF) << 0)

/* Bit definitions and macros for FSL_ESW_POQC */
#define FSL_ESW_POQC_COUNT(x)                  (((x) & 0xFFFFFFFF) << 0)

/* Bit definitions and macros for FSL_ESW_PMVID */
#define FSL_ESW_PMVID_COUNT(x)                 (((x) & 0xFFFFFFFF) << 0)

/* Bit definitions and macros for FSL_ESW_PMVTAG */
#define FSL_ESW_PMVTAG_COUNT(x)                (((x) & 0xFFFFFFFF) << 0)

/* Bit definitions and macros for FSL_ESW_PBL */
#define FSL_ESW_PBL_COUNT(x)                   (((x) & 0xFFFFFFFF) << 0)

/* Bit definitions and macros for FSL_ESW_ISR */
#define FSL_ESW_ISR_EBERR                      (0x00000001)
#define FSL_ESW_ISR_RXB                        (0x00000002)
#define FSL_ESW_ISR_RXF                        (0x00000004)
#define FSL_ESW_ISR_TXB                        (0x00000008)
#define FSL_ESW_ISR_TXF                        (0x00000010)
#define FSL_ESW_ISR_QM                         (0x00000020)
#define FSL_ESW_ISR_OD0                        (0x00000040)
#define FSL_ESW_ISR_OD1                        (0x00000080)
#define FSL_ESW_ISR_OD2                        (0x00000100)
#define FSL_ESW_ISR_LRN                        (0x00000200)

/* Bit definitions and macros for FSL_ESW_IMR */
#define FSL_ESW_IMR_EBERR                      (0x00000001)
#define FSL_ESW_IMR_RXB                        (0x00000002)
#define FSL_ESW_IMR_RXF                        (0x00000004)
#define FSL_ESW_IMR_TXB                        (0x00000008)
#define FSL_ESW_IMR_TXF                        (0x00000010)
#define FSL_ESW_IMR_QM                         (0x00000020)
#define FSL_ESW_IMR_OD0                        (0x00000040)
#define FSL_ESW_IMR_OD1                        (0x00000080)
#define FSL_ESW_IMR_OD2                        (0x00000100)
#define FSL_ESW_IMR_LRN                        (0x00000200)

/* Bit definitions and macros for FSL_ESW_RDSR */
#define FSL_ESW_RDSR_ADDRESS(x)                (((x) & 0x3FFFFFFF) << 2)

/* Bit definitions and macros for FSL_ESW_TDSR */
#define FSL_ESW_TDSR_ADDRESS(x)                (((x) & 0x3FFFFFFF) << 2)

/* Bit definitions and macros for FSL_ESW_MRBR */
#define FSL_ESW_MRBR_SIZE(x)                   (((x) & 0x000003FF) << 4)

/* Bit definitions and macros for FSL_ESW_RDAR */
#define FSL_ESW_RDAR_R_DES_ACTIVE              (0x01000000)

/* Bit definitions and macros for FSL_ESW_TDAR */
#define FSL_ESW_TDAR_X_DES_ACTIVE              (0x01000000)

/* Bit definitions and macros for FSL_ESW_LREC0 */
#define FSL_ESW_LREC0_MACADDR0(x)              (((x) & 0xFFFFFFFF) << 0)

/* Bit definitions and macros for FSL_ESW_LREC1 */
#define FSL_ESW_LREC1_MACADDR1(x)              (((x) & 0x0000FFFF) << 0)
#define FSL_ESW_LREC1_HASH(x)                  (((x) & 0x000000FF) << 16)
#define FSL_ESW_LREC1_SWPORT(x)                (((x) & 0x00000003) << 24)

/* Bit definitions and macros for FSL_ESW_LSR */
#define FSL_ESW_LSR_DA                         (0x00000001)

/* port mirroring port number match */
#define MIRROR_EGRESS_PORT_MATCH		1
#define MIRROR_INGRESS_PORT_MATCH		2

/* port mirroring mac address match */
#define MIRROR_EGRESS_SOURCE_MATCH		1
#define MIRROR_INGRESS_SOURCE_MATCH		2
#define MIRROR_EGRESS_DESTINATION_MATCH		3
#define MIRROR_INGRESS_DESTINATION_MATCH	4

#endif /* FSL_L2_SWITCH_H */
