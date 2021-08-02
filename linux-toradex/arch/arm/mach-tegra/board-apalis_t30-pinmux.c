/*
 * arch/arm/mach-tegra/board-apalis_t30-pinmux.c
 *
 * Copyright (C) 2013-2016 Toradex, Inc.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */

#include <linux/init.h>
#include <linux/kernel.h>

#include <mach/pinmux.h>

#include "board-apalis_t30.h"
#include "board.h"
#include "gpio-names.h"

#define DEFAULT_DRIVE(_name)					\
	{							\
		.pingroup = TEGRA_DRIVE_PINGROUP_##_name,	\
		.hsm = TEGRA_HSM_DISABLE,			\
		.schmitt = TEGRA_SCHMITT_ENABLE,		\
		.drive = TEGRA_DRIVE_DIV_1,			\
		.pull_down = TEGRA_PULL_31,			\
		.pull_up = TEGRA_PULL_31,			\
		.slew_rising = TEGRA_SLEW_SLOWEST,		\
		.slew_falling = TEGRA_SLEW_SLOWEST,		\
	}

/* Setting the drive strength of pins
 * hsm: Enable High speed mode (ENABLE/DISABLE)
 * Schimit: Enable/disable schimit (ENABLE/DISABLE)
 * drive: low power mode (DIV_1, DIV_2, DIV_4, DIV_8)
 * pulldn_drive - drive down (falling edge) - Driver Output Pull-Down drive
 *                strength code. Value from 0 to 31.
 * pullup_drive - drive up (rising edge)  - Driver Output Pull-Up drive
 *                strength code. Value from 0 to 31.
 * pulldn_slew -  Driver Output Pull-Up slew control code  - 2bit code
 *                code 11 is least slewing of signal. code 00 is highest
 *                slewing of the signal.
 *                Value - FASTEST, FAST, SLOW, SLOWEST
 * pullup_slew -  Driver Output Pull-Down slew control code -
 *                code 11 is least slewing of signal. code 00 is highest
 *                slewing of the signal.
 *                Value - FASTEST, FAST, SLOW, SLOWEST
 */
#define SET_DRIVE(_name, _hsm, _schmitt, _drive, _pulldn_drive, _pullup_drive, _pulldn_slew, _pullup_slew) \
	{                                               \
		.pingroup = TEGRA_DRIVE_PINGROUP_##_name,   \
		.hsm = TEGRA_HSM_##_hsm,                    \
		.schmitt = TEGRA_SCHMITT_##_schmitt,        \
		.drive = TEGRA_DRIVE_##_drive,              \
		.pull_down = TEGRA_PULL_##_pulldn_drive,    \
		.pull_up = TEGRA_PULL_##_pullup_drive,		\
		.slew_rising = TEGRA_SLEW_##_pulldn_slew,   \
		.slew_falling = TEGRA_SLEW_##_pullup_slew,	\
	}

static __initdata struct tegra_drive_pingroup_config apalis_t30_drive_pinmux[] = {
	/* DEFAULT_DRIVE(<pin_group>), */

	/* Audio codec */
	SET_DRIVE(DAP2, DISABLE, ENABLE, DIV_1, 31, 31, FASTEST, FASTEST),

	/* All I2C pins are driven to maximum drive strength */

	/* GEN1_I2C: I2C_SDA/SCL on SODIMM pin 194/196 (e.g. RTC on carrier board) */
	SET_DRIVE(DBG, DISABLE, ENABLE, DIV_1, 31, 31, FASTEST, FASTEST),

	/* DDC_CLOCK/DATA on X3 pin 15/16 (e.g. display EDID) */
	SET_DRIVE(DDC, DISABLE, ENABLE, DIV_1, 31, 31, FASTEST, FASTEST),

	/* PWR_I2C: power I2C to audio codec, PMIC, temperature sensor and
		    touch screen controller */
	SET_DRIVE(AO1, DISABLE, ENABLE, DIV_1, 31, 31, FASTEST, FASTEST),

	/* SDMMC2 */
	SET_DRIVE(AO2,	DISABLE, DISABLE, DIV_1, 46, 42, FAST, FAST),

	/* eMMC on SDMMC4 */
	SET_DRIVE(GMA, DISABLE, DISABLE, DIV_1, 9, 9, SLOWEST, SLOWEST),
	SET_DRIVE(GMB, DISABLE, DISABLE, DIV_1, 9, 9, SLOWEST, SLOWEST),
	SET_DRIVE(GMC, DISABLE, DISABLE, DIV_1, 9, 9, SLOWEST, SLOWEST),
	SET_DRIVE(GMD, DISABLE, DISABLE, DIV_1, 9, 9, SLOWEST, SLOWEST),
};

#define DEFAULT_PINMUX(_pingroup, _mux, _pupd, _tri, _io)	\
	{							\
		.pingroup	= TEGRA_PINGROUP_##_pingroup,	\
		.func		= TEGRA_MUX_##_mux,		\
		.pupd		= TEGRA_PUPD_##_pupd,		\
/* TRISTATE here means output driver is tri-stated */ \
		.tristate	= TEGRA_TRI_##_tri,		\
/* INPUT here means input driver is enabled vs. OUTPUT where it is disabled */ \
		.io		= TEGRA_PIN_##_io,		\
		.lock		= TEGRA_PIN_LOCK_DEFAULT,	\
		.od		= TEGRA_PIN_OD_DEFAULT,		\
		.ioreset	= TEGRA_PIN_IO_RESET_DEFAULT,	\
	}

#define I2C_PINMUX(_pingroup, _mux, _pupd, _tri, _io, _lock, _od) \
	{							\
		.pingroup	= TEGRA_PINGROUP_##_pingroup,	\
		.func		= TEGRA_MUX_##_mux,		\
		.pupd		= TEGRA_PUPD_##_pupd,		\
		.tristate	= TEGRA_TRI_##_tri,		\
		.io		= TEGRA_PIN_##_io,		\
		.lock		= TEGRA_PIN_LOCK_##_lock,	\
		.od		= TEGRA_PIN_OD_##_od,		\
		.ioreset	= TEGRA_PIN_IO_RESET_DEFAULT,	\
	}

#define VI_PINMUX(_pingroup, _mux, _pupd, _tri, _io, _lock, _ioreset) \
	{							\
		.pingroup	= TEGRA_PINGROUP_##_pingroup,	\
		.func		= TEGRA_MUX_##_mux,		\
		.pupd		= TEGRA_PUPD_##_pupd,		\
		.tristate	= TEGRA_TRI_##_tri,		\
		.io		= TEGRA_PIN_##_io,		\
		.lock		= TEGRA_PIN_LOCK_##_lock,	\
		.od		= TEGRA_PIN_OD_DEFAULT,		\
		.ioreset	= TEGRA_PIN_IO_RESET_##_ioreset	\
	}

static __initdata struct tegra_pingroup_config apalis_t30_pinmux[] = {
	I2C_PINMUX(CAM_I2C_SCL, I2C3, NORMAL, NORMAL, INPUT, DEFAULT, ENABLE),
	I2C_PINMUX(CAM_I2C_SDA, I2C3, NORMAL, NORMAL, INPUT, DEFAULT, ENABLE),
	DEFAULT_PINMUX(CAM_MCLK, VI_ALT2, NORMAL, NORMAL, OUTPUT),

	DEFAULT_PINMUX(CLK1_OUT, EXTPERIPH1, NORMAL, NORMAL, OUTPUT),
	DEFAULT_PINMUX(CLK1_REQ, HDA, NORMAL, NORMAL, OUTPUT),

	DEFAULT_PINMUX(CLK2_OUT, EXTPERIPH2, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(CLK2_REQ, RSVD1, NORMAL, NORMAL, INPUT),

	DEFAULT_PINMUX(CLK3_OUT, RSVD1, PULL_DOWN, TRISTATE, OUTPUT),	/* NC */
	DEFAULT_PINMUX(CLK3_REQ, RSVD1, PULL_DOWN, TRISTATE, OUTPUT),	/* NC */

	DEFAULT_PINMUX(CLK_32K_OUT, RSVD1, PULL_DOWN, TRISTATE, OUTPUT),/* NC */

	DEFAULT_PINMUX(CRT_HSYNC, CRT, NORMAL, NORMAL, OUTPUT),
	DEFAULT_PINMUX(CRT_VSYNC, CRT, NORMAL, NORMAL, OUTPUT),

	DEFAULT_PINMUX(DAP1_DIN, HDA, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(DAP1_DOUT, HDA, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(DAP1_FS, HDA, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(DAP1_SCLK, HDA, NORMAL, NORMAL, INPUT),

//multiplexed CAN1/2_RX/TX
	DEFAULT_PINMUX(DAP2_DIN, RSVD2, NORMAL, TRISTATE, OUTPUT),
	DEFAULT_PINMUX(DAP2_DOUT, RSVD2, NORMAL, TRISTATE, OUTPUT),
	DEFAULT_PINMUX(DAP2_FS, RSVD2, NORMAL, TRISTATE, OUTPUT),
	DEFAULT_PINMUX(DAP2_SCLK, RSVD2, NORMAL, TRISTATE, OUTPUT),

	DEFAULT_PINMUX(DAP3_DIN, I2S2, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(DAP3_DOUT, I2S2, NORMAL, NORMAL, OUTPUT),
//requires INPUT when operating as I2S slave
	DEFAULT_PINMUX(DAP3_FS, I2S2, NORMAL, NORMAL, OUTPUT),
//requires INPUT even when operating as I2S master
	DEFAULT_PINMUX(DAP3_SCLK, I2S2, NORMAL, NORMAL, INPUT),

	DEFAULT_PINMUX(DAP4_DIN, RSVD1, PULL_DOWN, TRISTATE, OUTPUT),	/* NC */
	DEFAULT_PINMUX(DAP4_DOUT, RSVD1, PULL_DOWN, TRISTATE, OUTPUT),	/* NC */
	DEFAULT_PINMUX(DAP4_FS, RSVD1, PULL_DOWN, TRISTATE, OUTPUT),	/* NC */
	DEFAULT_PINMUX(DAP4_SCLK, RSVD1, PULL_DOWN, TRISTATE, OUTPUT),	/* NC */

	I2C_PINMUX(DDC_SCL, I2C4, NORMAL, NORMAL, INPUT, DEFAULT, ENABLE),
	I2C_PINMUX(DDC_SDA, I2C4, NORMAL, NORMAL, INPUT, DEFAULT, ENABLE),

	I2C_PINMUX(GEN1_I2C_SCL, I2C1, NORMAL, NORMAL, INPUT, DEFAULT, ENABLE),
	I2C_PINMUX(GEN1_I2C_SDA, I2C1, NORMAL, NORMAL, INPUT, DEFAULT, ENABLE),

	I2C_PINMUX(GEN2_I2C_SCL, RSVD3, NORMAL, NORMAL, INPUT, DEFAULT, DISABLE),
	I2C_PINMUX(GEN2_I2C_SDA, RSVD3, NORMAL, NORMAL, INPUT, DEFAULT, DISABLE),

	DEFAULT_PINMUX(GMI_A16, SPI4, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(GMI_A17, SPI4, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(GMI_A18, SPI4, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(GMI_A19, SPI4, NORMAL, NORMAL, INPUT),

	DEFAULT_PINMUX(GMI_AD0, RSVD1, PULL_DOWN, TRISTATE, OUTPUT),	/* NC */
	DEFAULT_PINMUX(GMI_AD1, RSVD1, PULL_DOWN, TRISTATE, OUTPUT),	/* NC */
	DEFAULT_PINMUX(GMI_AD2, RSVD1, PULL_DOWN, TRISTATE, OUTPUT),	/* NC */
	DEFAULT_PINMUX(GMI_AD3, RSVD1, PULL_DOWN, TRISTATE, OUTPUT),	/* NC */
	DEFAULT_PINMUX(GMI_AD4, RSVD1, PULL_DOWN, TRISTATE, OUTPUT),	/* NC */
	DEFAULT_PINMUX(GMI_AD5, RSVD1, PULL_DOWN, TRISTATE, OUTPUT),	/* NC */
	DEFAULT_PINMUX(GMI_AD6, RSVD1, PULL_DOWN, TRISTATE, OUTPUT),	/* NC */
	DEFAULT_PINMUX(GMI_AD7, RSVD1, PULL_DOWN, TRISTATE, OUTPUT),	/* NC */
	DEFAULT_PINMUX(GMI_AD8, RSVD2, PULL_DOWN, TRISTATE, OUTPUT),	/* NC */
	DEFAULT_PINMUX(GMI_AD9, RSVD2, PULL_DOWN, TRISTATE, OUTPUT),	/* NC */
	DEFAULT_PINMUX(GMI_AD10, RSVD2, PULL_DOWN, TRISTATE, OUTPUT),	/* NC */
	DEFAULT_PINMUX(GMI_AD11, RSVD2, PULL_DOWN, TRISTATE, OUTPUT),	/* NC */
	DEFAULT_PINMUX(GMI_AD12, RSVD1, PULL_DOWN, TRISTATE, OUTPUT),	/* NC */
	DEFAULT_PINMUX(GMI_AD13, RSVD1, PULL_DOWN, TRISTATE, OUTPUT),	/* NC */
	DEFAULT_PINMUX(GMI_AD14, RSVD1, PULL_DOWN, TRISTATE, OUTPUT),	/* NC */
	DEFAULT_PINMUX(GMI_AD15, RSVD1, PULL_DOWN, TRISTATE, OUTPUT),	/* NC */
	DEFAULT_PINMUX(GMI_ADV_N, RSVD1, PULL_DOWN, TRISTATE, OUTPUT),	/* NC */
	DEFAULT_PINMUX(GMI_CLK, RSVD1, PULL_DOWN, TRISTATE, OUTPUT),	/* NC */
	DEFAULT_PINMUX(GMI_CS0_N, RSVD1, PULL_DOWN, TRISTATE, OUTPUT),	/* NC */
	DEFAULT_PINMUX(GMI_CS1_N, RSVD1, PULL_DOWN, TRISTATE, OUTPUT),	/* NC */
	DEFAULT_PINMUX(GMI_CS2_N, RSVD1, PULL_DOWN, TRISTATE, OUTPUT),	/* NC */
	DEFAULT_PINMUX(GMI_CS3_N, RSVD1, PULL_DOWN, TRISTATE, OUTPUT),	/* NC */
	DEFAULT_PINMUX(GMI_CS4_N, RSVD1, PULL_DOWN, TRISTATE, OUTPUT),	/* NC */
	DEFAULT_PINMUX(GMI_CS6_N, GMI, PULL_DOWN, TRISTATE, OUTPUT),	/* NC */
	DEFAULT_PINMUX(GMI_CS7_N, GMI, PULL_DOWN, TRISTATE, OUTPUT),	/* NC */
	DEFAULT_PINMUX(GMI_DQS, RSVD1, PULL_DOWN, TRISTATE, OUTPUT),	/* NC */
	DEFAULT_PINMUX(GMI_IORDY, RSVD1, PULL_DOWN, TRISTATE, OUTPUT),	/* NC */

	DEFAULT_PINMUX(GMI_OE_N, RSVD1, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(GMI_RST_N, RSVD3, NORMAL, NORMAL, INPUT),

	DEFAULT_PINMUX(GMI_WAIT, RSVD1, PULL_DOWN, TRISTATE, OUTPUT),	/* NC */
	DEFAULT_PINMUX(GMI_WP_N, RSVD1, PULL_DOWN, TRISTATE, OUTPUT),	/* NC */
	DEFAULT_PINMUX(GMI_WR_N, RSVD1, PULL_DOWN, TRISTATE, OUTPUT),	/* NC */

	DEFAULT_PINMUX(GPIO_PBB0, RSVD1, NORMAL, NORMAL, OUTPUT),
	DEFAULT_PINMUX(GPIO_PBB3, VGP3, NORMAL, NORMAL, OUTPUT),
	DEFAULT_PINMUX(GPIO_PBB4, VGP4, NORMAL, NORMAL, OUTPUT),
	DEFAULT_PINMUX(GPIO_PBB5, VGP5, NORMAL, NORMAL, OUTPUT),
	DEFAULT_PINMUX(GPIO_PBB6, VGP6, NORMAL, NORMAL, OUTPUT),
	DEFAULT_PINMUX(GPIO_PBB7, RSVD1, NORMAL, NORMAL, OUTPUT),
	DEFAULT_PINMUX(GPIO_PCC1, RSVD1, NORMAL, NORMAL, OUTPUT),
	DEFAULT_PINMUX(GPIO_PCC2, RSVD1, NORMAL, NORMAL, OUTPUT),

	DEFAULT_PINMUX(GPIO_PU0, GMI, PULL_DOWN, NORMAL, OUTPUT),	/* NC */
	DEFAULT_PINMUX(GPIO_PU1, GMI, PULL_DOWN, NORMAL, OUTPUT),	/* NC */
	DEFAULT_PINMUX(GPIO_PU2, GMI, PULL_DOWN, NORMAL, OUTPUT),	/* NC */

	DEFAULT_PINMUX(GPIO_PU3, PWM0, NORMAL, NORMAL, OUTPUT),
	DEFAULT_PINMUX(GPIO_PU4, PWM1, NORMAL, NORMAL, OUTPUT),
	DEFAULT_PINMUX(GPIO_PU5, PWM2, NORMAL, NORMAL, OUTPUT),
	DEFAULT_PINMUX(GPIO_PU6, PWM3, NORMAL, NORMAL, OUTPUT),

	DEFAULT_PINMUX(GPIO_PV0, RSVD, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(GPIO_PV1, RSVD, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(GPIO_PV2, RSVD1, NORMAL, NORMAL, INPUT),
#ifdef IXORA
	DEFAULT_PINMUX(GPIO_PV3, RSVD1, NORMAL, NORMAL, OUTPUT),	/* UART2_3_RS232_FOFF_N */
#else
	DEFAULT_PINMUX(GPIO_PV3, RSVD1, NORMAL, NORMAL, INPUT),
#endif

	DEFAULT_PINMUX(HDMI_CEC, CEC, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(HDMI_INT, RSVD0, NORMAL, NORMAL, INPUT),

	DEFAULT_PINMUX(JTAG_RTCK, RTCK, NORMAL, NORMAL, OUTPUT),

//multiplexed OWR
	DEFAULT_PINMUX(KB_COL0, KBC, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(KB_COL1, KBC, NORMAL, NORMAL, INPUT),

//multiplexed VI_PCLK, VI_VSYNC and VI_HSYNC
	DEFAULT_PINMUX(KB_COL2, KBC, PULL_DOWN, TRISTATE, INPUT),
	DEFAULT_PINMUX(KB_COL3, KBC, PULL_DOWN, TRISTATE, INPUT),
	DEFAULT_PINMUX(KB_COL4, KBC, PULL_DOWN, TRISTATE, INPUT),

//multiplexed VI_D11
	DEFAULT_PINMUX(KB_COL5, KBC, PULL_DOWN, NORMAL, INPUT),
//multiplexed VI_D10
	DEFAULT_PINMUX(KB_COL6, KBC, PULL_DOWN, NORMAL, INPUT),
	DEFAULT_PINMUX(KB_COL7, KBC, NORMAL, NORMAL, INPUT),

//multiplexed VI_D2, VI_D3, VI_D4, VI_D5, VI_D6, VI_D7, VI_D8 and VI_D9
	DEFAULT_PINMUX(KB_ROW0, RSVD2, PULL_DOWN, TRISTATE, INPUT),
	DEFAULT_PINMUX(KB_ROW1, RSVD2, PULL_DOWN, TRISTATE, INPUT),
	DEFAULT_PINMUX(KB_ROW2, RSVD2, PULL_DOWN, TRISTATE, INPUT),
	DEFAULT_PINMUX(KB_ROW3, RSVD2, PULL_DOWN, TRISTATE, INPUT),
	DEFAULT_PINMUX(KB_ROW4, RSVD3, PULL_DOWN, TRISTATE, INPUT),
	DEFAULT_PINMUX(KB_ROW5, KBC, PULL_DOWN, TRISTATE, INPUT),
	DEFAULT_PINMUX(KB_ROW6, KBC, PULL_DOWN, TRISTATE, INPUT),
	DEFAULT_PINMUX(KB_ROW7, KBC, PULL_DOWN, TRISTATE, INPUT),

//multiplexed VI_D0
	DEFAULT_PINMUX(KB_ROW8, KBC, PULL_DOWN, NORMAL, INPUT),
//multiplexed VI_D1
	DEFAULT_PINMUX(KB_ROW9, KBC, PULL_DOWN, NORMAL, INPUT),

	DEFAULT_PINMUX(KB_ROW10, KBC, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(KB_ROW11, KBC, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(KB_ROW12, KBC, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(KB_ROW13, KBC, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(KB_ROW14, KBC, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(KB_ROW15, KBC, NORMAL, NORMAL, INPUT),

	DEFAULT_PINMUX(LCD_CS0_N, SPI5, NORMAL, NORMAL, INPUT),

	DEFAULT_PINMUX(LCD_CS1_N, RSVD2, PULL_DOWN, TRISTATE, OUTPUT),	/* NC */

	DEFAULT_PINMUX(LCD_D0, DISPLAYA, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(LCD_D1, DISPLAYA, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(LCD_D2, DISPLAYA, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(LCD_D3, DISPLAYA, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(LCD_D4, DISPLAYA, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(LCD_D5, DISPLAYA, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(LCD_D6, DISPLAYA, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(LCD_D7, DISPLAYA, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(LCD_D8, DISPLAYA, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(LCD_D9, DISPLAYA, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(LCD_D10, DISPLAYA, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(LCD_D11, DISPLAYA, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(LCD_D12, DISPLAYA, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(LCD_D13, DISPLAYA, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(LCD_D14, DISPLAYA, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(LCD_D15, DISPLAYA, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(LCD_D16, DISPLAYA, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(LCD_D17, DISPLAYA, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(LCD_D18, DISPLAYA, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(LCD_D19, DISPLAYA, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(LCD_D20, DISPLAYA, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(LCD_D21, DISPLAYA, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(LCD_D22, DISPLAYA, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(LCD_D23, DISPLAYA, NORMAL, NORMAL, INPUT),

	DEFAULT_PINMUX(LCD_DC0, RSVD1, PULL_DOWN, TRISTATE, OUTPUT),	/* NC */

	DEFAULT_PINMUX(LCD_DC1, RSVD1, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(LCD_DE, DISPLAYA, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(LCD_HSYNC, DISPLAYA, NORMAL, NORMAL, INPUT),

	DEFAULT_PINMUX(LCD_M1, RSVD1, PULL_DOWN, TRISTATE, OUTPUT),	/* NC */

	DEFAULT_PINMUX(LCD_PCLK, DISPLAYA, NORMAL, NORMAL, INPUT),

	DEFAULT_PINMUX(LCD_PWR0, DISPLAYB, PULL_DOWN, TRISTATE, OUTPUT),/* NC */
	DEFAULT_PINMUX(LCD_PWR1, RSVD1, PULL_DOWN, TRISTATE, OUTPUT),	/* NC */
	DEFAULT_PINMUX(LCD_PWR2, RSVD, PULL_DOWN, TRISTATE, OUTPUT),	/* NC */

	DEFAULT_PINMUX(LCD_SCK, SPI5, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(LCD_SDIN, SPI5, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(LCD_SDOUT, SPI5, NORMAL, NORMAL, INPUT),

	DEFAULT_PINMUX(LCD_VSYNC, DISPLAYA, NORMAL, NORMAL, INPUT),

	DEFAULT_PINMUX(LCD_WR_N, RSVD, PULL_DOWN, TRISTATE, OUTPUT),	/* NC */

//multiplexed KB_COL0
	DEFAULT_PINMUX(OWR, OWR, NORMAL, NORMAL, INPUT),

	DEFAULT_PINMUX(PEX_L0_CLKREQ_N, RSVD2, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(PEX_L0_PRSNT_N, RSVD2, NORMAL, NORMAL, OUTPUT),
	DEFAULT_PINMUX(PEX_L0_RST_N, RSVD2, NORMAL, NORMAL, OUTPUT),

	DEFAULT_PINMUX(PEX_L1_CLKREQ_N, RSVD2, PULL_DOWN, TRISTATE, OUTPUT),	/* NC */
	DEFAULT_PINMUX(PEX_L1_PRSNT_N, RSVD2, PULL_DOWN, TRISTATE, OUTPUT),	/* NC */
	DEFAULT_PINMUX(PEX_L1_RST_N, RSVD2, PULL_DOWN, TRISTATE, OUTPUT),	/* NC */

	DEFAULT_PINMUX(PEX_L2_CLKREQ_N, PCIE, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(PEX_L2_PRSNT_N, PCIE, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(PEX_L2_RST_N, PCIE, NORMAL, NORMAL, OUTPUT),
	DEFAULT_PINMUX(PEX_WAKE_N, PCIE, NORMAL, NORMAL, INPUT),

	/* Power I2C pinmux */
	I2C_PINMUX(PWR_I2C_SCL, I2CPWR, NORMAL, NORMAL, INPUT, DEFAULT, ENABLE),
	I2C_PINMUX(PWR_I2C_SDA, I2CPWR, NORMAL, NORMAL, INPUT, DEFAULT, ENABLE),

	DEFAULT_PINMUX(SDMMC1_CLK, SDMMC1, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(SDMMC1_CMD, SDMMC1, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(SDMMC1_DAT0, SDMMC1, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(SDMMC1_DAT1, SDMMC1, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(SDMMC1_DAT2, SDMMC1, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(SDMMC1_DAT3, SDMMC1, NORMAL, NORMAL, INPUT),

#ifdef IXORA
	DEFAULT_PINMUX(SDMMC3_CLK, PWM2, PULL_DOWN, TRISTATE, OUTPUT),	/* NC */
	DEFAULT_PINMUX(SDMMC3_CMD, PWM3, PULL_DOWN, TRISTATE, OUTPUT),	/* NC */
	DEFAULT_PINMUX(SDMMC3_DAT0, RSVD1, NORMAL, TRISTATE, INPUT),	/* SW3 */
	DEFAULT_PINMUX(SDMMC3_DAT1, RSVD0, NORMAL, NORMAL, OUTPUT),	/* LED4_GREEN */
	DEFAULT_PINMUX(SDMMC3_DAT2, PWM1, NORMAL, NORMAL, OUTPUT),	/* PCIE1_WDISABLE_N */
	DEFAULT_PINMUX(SDMMC3_DAT3, RSVD0, NORMAL, NORMAL, OUTPUT),	/* LED4_RED */
	DEFAULT_PINMUX(SDMMC3_DAT4, PWM1, PULL_DOWN, TRISTATE, OUTPUT),	/* NC */
	DEFAULT_PINMUX(SDMMC3_DAT5, PWM0, NORMAL, NORMAL, OUTPUT),	/* LED5_GREEN */
	DEFAULT_PINMUX(SDMMC3_DAT6, SPDIF, NORMAL, NORMAL, OUTPUT),	/* LED5_RED */
	DEFAULT_PINMUX(SDMMC3_DAT7, SPDIF, PULL_DOWN, TRISTATE, OUTPUT),/* NC */
#else /* IXORA */
	DEFAULT_PINMUX(SDMMC3_CLK, SDMMC3, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(SDMMC3_CMD, SDMMC3, PULL_UP, NORMAL, INPUT),
	DEFAULT_PINMUX(SDMMC3_DAT0, SDMMC3, PULL_UP, NORMAL, INPUT),
	DEFAULT_PINMUX(SDMMC3_DAT1, SDMMC3, PULL_UP, NORMAL, INPUT),
	DEFAULT_PINMUX(SDMMC3_DAT2, SDMMC3, PULL_UP, NORMAL, INPUT),
	DEFAULT_PINMUX(SDMMC3_DAT3, SDMMC3, PULL_UP, NORMAL, INPUT),
	DEFAULT_PINMUX(SDMMC3_DAT4, SDMMC3, PULL_UP, NORMAL, INPUT),
	DEFAULT_PINMUX(SDMMC3_DAT5, SDMMC3, PULL_UP, NORMAL, INPUT),
	DEFAULT_PINMUX(SDMMC3_DAT6, SDMMC3, PULL_UP, NORMAL, INPUT),
	DEFAULT_PINMUX(SDMMC3_DAT7, SDMMC3, PULL_UP, NORMAL, INPUT),
#endif /* IXORA */

	DEFAULT_PINMUX(SDMMC4_CLK, SDMMC4, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(SDMMC4_CMD, SDMMC4, PULL_UP, NORMAL, INPUT),
	DEFAULT_PINMUX(SDMMC4_DAT0, SDMMC4, PULL_UP, NORMAL, INPUT),
	DEFAULT_PINMUX(SDMMC4_DAT1, SDMMC4, PULL_UP, NORMAL, INPUT),
	DEFAULT_PINMUX(SDMMC4_DAT2, SDMMC4, PULL_UP, NORMAL, INPUT),
	DEFAULT_PINMUX(SDMMC4_DAT3, SDMMC4, PULL_UP, NORMAL, INPUT),
	DEFAULT_PINMUX(SDMMC4_DAT4, SDMMC4, PULL_UP, NORMAL, INPUT),
	DEFAULT_PINMUX(SDMMC4_DAT5, SDMMC4, PULL_UP, NORMAL, INPUT),
	DEFAULT_PINMUX(SDMMC4_DAT6, SDMMC4, PULL_UP, NORMAL, INPUT),
	DEFAULT_PINMUX(SDMMC4_DAT7, SDMMC4, PULL_UP, NORMAL, INPUT),
	DEFAULT_PINMUX(SDMMC4_RST_N, RSVD1, PULL_DOWN, NORMAL, INPUT),

	DEFAULT_PINMUX(SPDIF_IN, SPDIF, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(SPDIF_OUT, SPDIF, NORMAL, NORMAL, OUTPUT),

	DEFAULT_PINMUX(SPI1_CS0_N, SPI1, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(SPI1_MISO, SPI1, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(SPI1_MOSI, SPI1, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(SPI1_SCK, SPI1, NORMAL, NORMAL, INPUT),

	DEFAULT_PINMUX(SPI2_CS0_N, SPI2, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(SPI2_CS1_N, SPI2, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(SPI2_CS2_N, SPI2, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(SPI2_MISO, SPI2, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(SPI2_MOSI, SPI2, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(SPI2_SCK, SPI2, NORMAL, NORMAL, INPUT),

	DEFAULT_PINMUX(SYS_CLK_REQ, SYSCLK, NORMAL, NORMAL, INPUT),

	/* EN_+3.3_SDMMC3 */
	DEFAULT_PINMUX(UART2_CTS_N, GMI, NORMAL, NORMAL, OUTPUT),

	DEFAULT_PINMUX(UART2_RTS_N, GMI, PULL_DOWN, TRISTATE, OUTPUT),	/* NC */

	DEFAULT_PINMUX(UART2_RXD, IRDA, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(UART2_TXD, IRDA, NORMAL, NORMAL, OUTPUT),

//disable BKL1_PWM_EN# (e.g. PMIC PWM backlight enable) for now
	DEFAULT_PINMUX(UART3_CTS_N, RSVD1, PULL_UP, NORMAL, OUTPUT),
	DEFAULT_PINMUX(UART3_RTS_N, PWM0, NORMAL, NORMAL, OUTPUT),
	DEFAULT_PINMUX(UART3_RXD, UARTC, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(UART3_TXD, UARTC, NORMAL, NORMAL, OUTPUT),

	DEFAULT_PINMUX(ULPI_CLK, UARTD, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(ULPI_DATA0, UARTA, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(ULPI_DATA1, UARTA, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(ULPI_DATA2, UARTA, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(ULPI_DATA3, UARTA, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(ULPI_DATA4, UARTA, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(ULPI_DATA5, UARTA, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(ULPI_DATA6, UARTA, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(ULPI_DATA7, UARTA, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(ULPI_DIR, UARTD, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(ULPI_NXT, UARTD, NORMAL, NORMAL, INPUT),
	DEFAULT_PINMUX(ULPI_STP, UARTD, NORMAL, NORMAL, INPUT),

	/* VI pins are all level-shifted */
	VI_PINMUX(VI_D0, VI, NORMAL, NORMAL, INPUT, DISABLE, DISABLE),
	VI_PINMUX(VI_D1, VI, NORMAL, NORMAL, INPUT, DISABLE, DISABLE),
	VI_PINMUX(VI_D2, VI, NORMAL, NORMAL, INPUT, DISABLE, DISABLE),
	VI_PINMUX(VI_D3, VI, NORMAL, NORMAL, INPUT, DISABLE, DISABLE),
	VI_PINMUX(VI_D4, VI, NORMAL, NORMAL, INPUT, DISABLE, DISABLE),
	VI_PINMUX(VI_D5, VI, NORMAL, NORMAL, INPUT, DISABLE, DISABLE),
	VI_PINMUX(VI_D6, VI, NORMAL, NORMAL, INPUT, DISABLE, DISABLE),
	VI_PINMUX(VI_D7, VI, NORMAL, NORMAL, INPUT, DISABLE, DISABLE),
	VI_PINMUX(VI_D8, VI, NORMAL, NORMAL, INPUT, DISABLE, DISABLE),
	VI_PINMUX(VI_D9, VI, NORMAL, NORMAL, INPUT, DISABLE, DISABLE),
	VI_PINMUX(VI_D10, VI, NORMAL, NORMAL, INPUT, DISABLE, DISABLE),
	VI_PINMUX(VI_D11, VI, NORMAL, NORMAL, INPUT, DISABLE, DISABLE),
	VI_PINMUX(VI_HSYNC, VI, NORMAL, NORMAL, INPUT, DISABLE, DISABLE),
	/* GPIO T1: VI_LevelShifter_DIR */
	DEFAULT_PINMUX(VI_MCLK, VI, NORMAL, NORMAL, OUTPUT),
	VI_PINMUX(VI_PCLK, VI, PULL_UP, NORMAL, INPUT, DISABLE, DISABLE),
	VI_PINMUX(VI_VSYNC, VI, NORMAL, NORMAL, INPUT, DISABLE, DISABLE),
};

#define GPIO_INIT_PIN_MODE(_gpio, _is_input, _value)	\
	{					\
		.gpio_nr	= _gpio,	\
		.is_input	= _is_input,	\
		.value		= _value,	\
	}

static struct gpio_init_pin_info apalis_t30_init_gpio_mode[] = {
};

static void __init apalis_t30_gpio_init_configure(void)
{
	int len;
	int i;
	struct gpio_init_pin_info *pins_info;

	len = ARRAY_SIZE(apalis_t30_init_gpio_mode);
	pins_info = apalis_t30_init_gpio_mode;

	for (i = 0; i < len; ++i) {
		tegra_gpio_init_configure(pins_info->gpio_nr,
					  pins_info->is_input,
					  pins_info->value);
		pins_info++;
	}
}

int __init apalis_t30_pinmux_init(void)
{
	apalis_t30_gpio_init_configure();

	tegra_pinmux_config_table(apalis_t30_pinmux,
				  ARRAY_SIZE(apalis_t30_pinmux));
	tegra_drive_pinmux_config_table(apalis_t30_drive_pinmux,
					ARRAY_SIZE(apalis_t30_drive_pinmux));

	/* VI level-shifter direction (output low => default direction input) */
	gpio_request(TEGRA_GPIO_PT1, "VI_LevelShifter_DIR");
	gpio_direction_output(TEGRA_GPIO_PT1, 0);

	return 0;
}
