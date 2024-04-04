#include <stdint.h>
#include "tl_common.h"
#include "drivers.h"
#include "stack/ble/ble.h"
#include "app.h"
#include "app_config.h"

#if (MCU_CORE_TYPE == MCU_CORE_9518)
	/* default flash is 1M
	 * for 1M Flash, flash_sector_mac_address equals to 0xFF000
	 * for 2M Flash, flash_sector_mac_address equals to 0x1FF000 */
	_attribute_ble_data_retention_	u32 flash_sector_mac_address = CFG_ADR_MAC;
	_attribute_ble_data_retention_	u32 flash_sector_calibration = CFG_ADR_CALIBRATION;
#else
	/* default flash is 512K
	 * for 512K Flash, flash_sector_mac_address equals to 0x76000
	 * for 1M Flash, flash_sector_mac_address equals to 0xFF000
	 * for 2M Flash, flash_sector_mac_address equals to 0x1FF000 */
	_attribute_ble_data_retention_	u32 flash_sector_mac_address = CFG_ADR_MAC;
	_attribute_ble_data_retention_	u32 flash_sector_calibration = CFG_ADR_CALIBRATION;
#endif

/**
 * @brief      This function servers to initialization all gpio.
 * @param[in]  en  -  if mcu wake up from deep retention mode, determine whether it is NOT necessary to reset analog register
 *                    1: set analog register
 *                    0: not set analog register
 * @return     none.
 *
 */
_attribute_ram_code_ void _gpio_init(int anaRes_init_en) {
	//PA group
	reg_gpio_pa_setting1 =
		(PA0_INPUT_ENABLE<<8) 	| (PA1_INPUT_ENABLE<<9)	| (PA2_INPUT_ENABLE<<10)	| (PA3_INPUT_ENABLE<<11) |
		(PA4_INPUT_ENABLE<<12)	| (PA5_INPUT_ENABLE<<13)	| (PA6_INPUT_ENABLE<<14)	| (PA7_INPUT_ENABLE<<15) |
		((PA0_OUTPUT_ENABLE?0:1)<<16)	| ((PA1_OUTPUT_ENABLE?0:1)<<17) | ((PA2_OUTPUT_ENABLE?0:1)<<18)	| ((PA3_OUTPUT_ENABLE?0:1)<<19) |
		((PA4_OUTPUT_ENABLE?0:1)<<20)	| ((PA5_OUTPUT_ENABLE?0:1)<<21) | ((PA6_OUTPUT_ENABLE?0:1)<<22)	| ((PA7_OUTPUT_ENABLE?0:1)<<23) |
		(PA0_DATA_OUT<<24)	| (PA1_DATA_OUT<<25)	| (PA2_DATA_OUT<<26)	| (PA3_DATA_OUT<<27) |
		(PA4_DATA_OUT<<28)	| (PA5_DATA_OUT<<29)	| (PA6_DATA_OUT<<30)	| (PA7_DATA_OUT<<31) ;
	reg_gpio_pa_setting2 =
		(PA0_DATA_STRENGTH<<8)		| (PA1_DATA_STRENGTH<<9)| (PA2_DATA_STRENGTH<<10)	| (PA3_DATA_STRENGTH<<11) |
		(PA4_DATA_STRENGTH<<12)	| (PA5_DATA_STRENGTH<<13)	| (PA6_DATA_STRENGTH<<14)	| (PA7_DATA_STRENGTH<<15) |
		(PA0_FUNC==AS_GPIO ? BIT(16):0)	| (PA1_FUNC==AS_GPIO ? BIT(17):0)| (PA2_FUNC==AS_GPIO ? BIT(18):0)| (PA3_FUNC==AS_GPIO ? BIT(19):0) |
		(PA4_FUNC==AS_GPIO ? BIT(20):0)	| (PA5_FUNC==AS_GPIO ? BIT(21):0)| (PA6_FUNC==AS_GPIO ? BIT(22):0)| (PA7_FUNC==AS_GPIO ? BIT(23):0);

	//PB group
	//ie
	analog_write(areg_gpio_pb_ie, 	(PB0_INPUT_ENABLE<<0) 	| (PB1_INPUT_ENABLE<<1)	| (PB2_INPUT_ENABLE<<2)	| (PB3_INPUT_ENABLE<<3) |
									(PB4_INPUT_ENABLE<<4)	| (PB5_INPUT_ENABLE<<5) | (PB6_INPUT_ENABLE<<6)	| (PB7_INPUT_ENABLE<<7) );
	//dataO
	reg_gpio_pb_out =
		(PB0_DATA_OUT<<0)	| (PB1_DATA_OUT<<1)	| (PB2_DATA_OUT<<2)	| (PB3_DATA_OUT<<3) |
		(PB4_DATA_OUT<<4)	| (PB5_DATA_OUT<<5)	| (PB6_DATA_OUT<<6)	| (PB7_DATA_OUT<<7) ;
	//oen
	reg_gpio_pb_oen =
		((PB0_OUTPUT_ENABLE?0:1)<<0)	| ((PB1_OUTPUT_ENABLE?0:1)<<1) | ((PB2_OUTPUT_ENABLE?0:1)<<2)	| ((PB3_OUTPUT_ENABLE?0:1)<<3) |
		((PB4_OUTPUT_ENABLE?0:1)<<4)	| ((PB5_OUTPUT_ENABLE?0:1)<<5) | ((PB6_OUTPUT_ENABLE?0:1)<<6)	| ((PB7_OUTPUT_ENABLE?0:1)<<7);
	//ds
	analog_write(areg_gpio_pb_ds, 	(PB0_DATA_STRENGTH<<0) 	| (PB1_DATA_STRENGTH<<1)  | (PB2_DATA_STRENGTH<<2)	| (PB3_DATA_STRENGTH<<3) |
									(PB4_DATA_STRENGTH<<4)	| (PB5_DATA_STRENGTH<<5)  | (PB6_DATA_STRENGTH<<6)	| (PB7_DATA_STRENGTH<<7) );
	//func
	reg_gpio_pb_gpio =
		(PB0_FUNC==AS_GPIO ? BIT(0):0)	| (PB1_FUNC==AS_GPIO ? BIT(1):0)| (PB2_FUNC==AS_GPIO ? BIT(2):0)| (PB3_FUNC==AS_GPIO ? BIT(3):0) |
		(PB4_FUNC==AS_GPIO ? BIT(4):0)	| (PB5_FUNC==AS_GPIO ? BIT(5):0)| (PB6_FUNC==AS_GPIO ? BIT(6):0)| (PB7_FUNC==AS_GPIO ? BIT(7):0);

	//PC group
	//ie
	analog_write(areg_gpio_pc_ie, 	(PC0_INPUT_ENABLE<<0) 	| (PC1_INPUT_ENABLE<<1)	| (PC2_INPUT_ENABLE<<2)	| (PC3_INPUT_ENABLE<<3) |
									(PC4_INPUT_ENABLE<<4)	| (PC5_INPUT_ENABLE<<5) | (PC6_INPUT_ENABLE<<6)	| (PC7_INPUT_ENABLE<<7) );
	//dataO
	reg_gpio_pc_out =
		(PC0_DATA_OUT<<0)	| (PC1_DATA_OUT<<1)	| (PC2_DATA_OUT<<2)	| (PC3_DATA_OUT<<3) |
		(PC4_DATA_OUT<<4)	| (PC5_DATA_OUT<<5)	| (PC6_DATA_OUT<<6)	| (PC7_DATA_OUT<<7) ;
	//oen
	reg_gpio_pc_oen =
		((PC0_OUTPUT_ENABLE?0:1)<<0)	| ((PC1_OUTPUT_ENABLE?0:1)<<1) | ((PC2_OUTPUT_ENABLE?0:1)<<2)	| ((PC3_OUTPUT_ENABLE?0:1)<<3) |
		((PC4_OUTPUT_ENABLE?0:1)<<4)	| ((PC5_OUTPUT_ENABLE?0:1)<<5) | ((PC6_OUTPUT_ENABLE?0:1)<<6)	| ((PC7_OUTPUT_ENABLE?0:1)<<7);
	//ds
	analog_write(areg_gpio_pc_ds, 	(PC0_DATA_STRENGTH<<0) 	| (PC1_DATA_STRENGTH<<1)  | (PC2_DATA_STRENGTH<<2)	| (PC3_DATA_STRENGTH<<3) |
									(PC4_DATA_STRENGTH<<4)	| (PC5_DATA_STRENGTH<<5)  | (PC6_DATA_STRENGTH<<6)	| (PC7_DATA_STRENGTH<<7) );
	reg_gpio_pc_gpio =
		(PC0_FUNC==AS_GPIO ? BIT(0):0)	| (PC1_FUNC==AS_GPIO ? BIT(1):0)| (PC2_FUNC==AS_GPIO ? BIT(2):0)| (PC3_FUNC==AS_GPIO ? BIT(3):0) |
		(PC4_FUNC==AS_GPIO ? BIT(4):0)	| (PC5_FUNC==AS_GPIO ? BIT(5):0)| (PC6_FUNC==AS_GPIO ? BIT(6):0)| (PC7_FUNC==AS_GPIO ? BIT(7):0);

	//PD group
	reg_gpio_pd_setting1 =
		(PD0_INPUT_ENABLE<<8) 	| (PD1_INPUT_ENABLE<<9)	| (PD2_INPUT_ENABLE<<10)	| (PD3_INPUT_ENABLE<<11) |
		(PD4_INPUT_ENABLE<<12)	| (PD5_INPUT_ENABLE<<13)| (PD6_INPUT_ENABLE<<14)	| (PD7_INPUT_ENABLE<<15) |
		((PD0_OUTPUT_ENABLE?0:1)<<16)	| ((PD1_OUTPUT_ENABLE?0:1)<<17) | ((PD2_OUTPUT_ENABLE?0:1)<<18)	| ((PD3_OUTPUT_ENABLE?0:1)<<19) |
		((PD4_OUTPUT_ENABLE?0:1)<<20)	| ((PD5_OUTPUT_ENABLE?0:1)<<21) | ((PD6_OUTPUT_ENABLE?0:1)<<22)	| ((PD7_OUTPUT_ENABLE?0:1)<<23) |
		(PD0_DATA_OUT<<24)	| (PD1_DATA_OUT<<25)	| (PD2_DATA_OUT<<26)	| (PD3_DATA_OUT<<27) |
		(PD4_DATA_OUT<<28)	| (PD5_DATA_OUT<<29)	| (PD6_DATA_OUT<<30)	| (PD7_DATA_OUT<<31) ;
	reg_gpio_pd_setting2 =
		(PD0_DATA_STRENGTH<<8)	| (PD1_DATA_STRENGTH<<9)	| (PD2_DATA_STRENGTH<<10)	| (PD3_DATA_STRENGTH<<11) |
		(PD4_DATA_STRENGTH<<12)	| (PD5_DATA_STRENGTH<<13)	| (PD6_DATA_STRENGTH<<14)	| (PD7_DATA_STRENGTH<<15) |
		(PD0_FUNC==AS_GPIO ? BIT(16):0)	| (PD1_FUNC==AS_GPIO ? BIT(17):0)| (PD2_FUNC==AS_GPIO ? BIT(18):0)| (PD3_FUNC==AS_GPIO ? BIT(19):0) |
		(PD4_FUNC==AS_GPIO ? BIT(20):0)	| (PD5_FUNC==AS_GPIO ? BIT(21):0)| (PD6_FUNC==AS_GPIO ? BIT(22):0)| (PD7_FUNC==AS_GPIO ? BIT(23):0);

	//PE group
	reg_gpio_pe_ie = (PE0_INPUT_ENABLE<<0)	| (PE1_INPUT_ENABLE<<1)| (PE2_INPUT_ENABLE<<2)	| (PE3_INPUT_ENABLE<<3);
	reg_gpio_pe_out = (PE0_DATA_OUT<<0)	| (PE1_DATA_OUT<<1)	| (PE2_DATA_OUT<<2)	| (PE3_DATA_OUT<<3);
	reg_gpio_pe_oen = ((PE0_OUTPUT_ENABLE?0:1)<<0)	| ((PE1_OUTPUT_ENABLE?0:1)<<1) | ((PE2_OUTPUT_ENABLE?0:1)<<2)	| ((PE3_OUTPUT_ENABLE?0:1)<<3);
	reg_gpio_pe_ds = (PE0_DATA_STRENGTH<<0)	| (PE1_DATA_STRENGTH<<1)	| (PE2_DATA_STRENGTH<<2)	| (PE3_DATA_STRENGTH<<3);
	reg_gpio_pe_gpio = (PE0_FUNC==AS_GPIO ? BIT(0):0)	| (PE1_FUNC==AS_GPIO ? BIT(1):0)| (PE2_FUNC==AS_GPIO ? BIT(2):0)| (PE3_FUNC==AS_GPIO ? BIT(3):0);

	if(anaRes_init_en)	{
		analog_write (0x0e,  PULL_WAKEUP_SRC_PA0 |
							(PULL_WAKEUP_SRC_PA1<<2) |
							(PULL_WAKEUP_SRC_PA2<<4) |
							(PULL_WAKEUP_SRC_PA3<<6));

		analog_write (0x0f,  PULL_WAKEUP_SRC_PA4 |
							(PULL_WAKEUP_SRC_PA5<<2) |
							(PULL_WAKEUP_SRC_PA6<<4) |
							(PULL_WAKEUP_SRC_PA7<<6));


		analog_write (0x10,  PULL_WAKEUP_SRC_PB0 |
							(PULL_WAKEUP_SRC_PB1<<2) |
							(PULL_WAKEUP_SRC_PB2<<4) |
							(PULL_WAKEUP_SRC_PB3<<6));

		analog_write (0x11,  PULL_WAKEUP_SRC_PB4 |
							(PULL_WAKEUP_SRC_PB5<<2) |
							(PULL_WAKEUP_SRC_PB6<<4) |
							(PULL_WAKEUP_SRC_PB7<<6));


		analog_write (0x12,  PULL_WAKEUP_SRC_PC0 |
							(PULL_WAKEUP_SRC_PC1<<2) |
							(PULL_WAKEUP_SRC_PC2<<4) |
							(PULL_WAKEUP_SRC_PC3<<6));

		analog_write (0x13,  PULL_WAKEUP_SRC_PC4 |
							(PULL_WAKEUP_SRC_PC5<<2) |
							(PULL_WAKEUP_SRC_PC6<<4) |
							(PULL_WAKEUP_SRC_PC7<<6));


		analog_write (0x14,  PULL_WAKEUP_SRC_PD0 |
							(PULL_WAKEUP_SRC_PD1<<2) |
							(PULL_WAKEUP_SRC_PD2<<4) |
							(PULL_WAKEUP_SRC_PD3<<6));

		analog_write (0x15,  PULL_WAKEUP_SRC_PD4 |
							(PULL_WAKEUP_SRC_PD5<<2) |
							(PULL_WAKEUP_SRC_PD6<<4) |
							(PULL_WAKEUP_SRC_PD7<<6));
	}
}

/**
 * @brief   IRQ handler
 * @param   none.
 * @return  none.
 */
_attribute_ram_code_ void irq_handler(void)
{
    DBG_CHN15_HIGH;

	blc_sdk_irq_handler ();

	DBG_CHN15_LOW;
}

/**
 * @brief		This is main function
 * @param[in]	none
 * @return      none
 */
_attribute_ram_code_ int main (void) {    //must run in ramcode
	blc_pm_select_internal_32k_crystal();

	#if(MCU_CORE_TYPE == MCU_CORE_825x)
		cpu_wakeup_init();
	#elif(MCU_CORE_TYPE == MCU_CORE_827x)
		cpu_wakeup_init(DCDC_MODE, EXTERNAL_XTAL_24M);
	#endif

	/* detect if MCU is wake_up from deep retention mode */
	int deepRetWakeUp = pm_is_MCU_deepRetentionWakeup();  //MCU deep retention wakeUp

	clock_init(SYS_CLK_TYPE);

	rf_drv_init(RF_MODE_BLE_1M);

	_gpio_init(!deepRetWakeUp);  // analog resistance will keep available in deepSleep mode, so no need to initialize it again

    // Initialize ports for the used LEDs
    int leds[] = { GPIO_LED_B, GPIO_LED_R, GPIO_LED_G, GPIO_LED_E, GPIO_LED_W };
    for (size_t i = 0; i < sizeof(leds)/sizeof(leds[0]); i++) {
        gpio_set_func(leds[i], AS_GPIO);
        gpio_set_output_en(leds[i], 1);
        gpio_set_data_strength(leds[i], 1);
    }

    // Initialize UART RX port
    gpio_set_func(GPIO_RX, AS_GPIO);
    gpio_set_input_en(GPIO_RX, 1);
    gpio_setup_up_down_resistor(GPIO_RX, PM_PIN_PULLUP_1M);

    // Initialize UART TX port
    gpio_set_func(GPIO_TX, AS_GPIO);
    gpio_set_output_en(GPIO_TX, 1);
    gpio_set_data_strength(GPIO_TX, 1);
    gpio_setup_up_down_resistor(GPIO_TX, PM_PIN_PULLUP_1M);

	adc_power_on_sar_adc(0); // - 0.4 mA
	lpc_power_down();

	user_init_normal();

	/* load customized freq_offset cap value.
	 */
	blc_app_loadCustomizedParameters();

    irq_enable();
	while(1) {
		main_loop();
	}
	return 0;
}
