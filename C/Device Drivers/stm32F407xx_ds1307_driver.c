#include"stm32F407xx.h"
#include"stm32F407xx_ds1307_driver.h"
#include<string.h>

I2C_Handle DS_1307;

// Private Functions
static void ds1307_i2c_pin_config(void);
static void ds1307_i2c_config(void);
static uint8_t ds1307_read(uint8_t reg_addr);
static void ds1307_write(uint8_t value,uint8_t reg_addr);
static uint8_t bcd_to_binary(uint8_t value);
static uint8_t binary_to_bcd(uint8_t value);





void ds1307_set_current_time(RTC_time *rtc_time)
{
	uint8_t seconds, hrs;

    //write seconds value to the register
	seconds = binary_to_bcd(rtc_time->seconds);  //binary to bcd
	seconds &= ~( 1 << 7);                       //
	ds1307_write(seconds, DS1307_ADD_SEC);       //write opertaion

    //write minutes value to the register
	ds1307_write(binary_to_bcd(rtc_time->minutes), DS1307_ADD_MIN);


	hrs = binary_to_bcd(rtc_time->hours);         //binary to bcd

    //adjust value based on time format of hours
	if(rtc_time->time_format == TIME_FORMAT_24HRS)
    {
		hrs &= ~(1 << 6);
	}
    else
    {
		hrs |= (1 << 6);
		hrs = (rtc_time->time_format == TIME_FORMAT_12HRS_PM)?(hrs | (1 << 5)):(hrs & ~(1<<5));  //set value based on AM or PM
	}

    //write hours value to register
	ds1307_write(hrs,DS1307_ADD_HRS);
}


//set date
void ds1307_set_current_date(RTC_date *rtc_date)
{
	ds1307_write(binary_to_bcd(rtc_date->date),DS1307_ADD_DATE);     //date
	ds1307_write(binary_to_bcd(rtc_date->month),DS1307_ADD_MONTH);   //month
	ds1307_write(binary_to_bcd(rtc_date->year),DS1307_ADD_YEAR);     //year
	ds1307_write(binary_to_bcd(rtc_date->day),DS1307_ADD_DAY);       //day
}


//get current time
void ds1307_get_current_time(RTC_time *rtc_time)
{

	uint8_t seconds,hrs;
	seconds = ds1307_read(DS1307_ADD_SEC);   //read seconds
	seconds &= ~(1 << 7);                    //

	rtc_time->seconds = bcd_to_binary(seconds);                     //seconds - convert bcd to binary
	rtc_time->minutes = bcd_to_binary(ds1307_read(DS1307_ADD_MIN)); //minutes - read and convert

	hrs = ds1307_read(DS1307_ADD_HRS);       //read hours

	if(hrs&(1 << 6)) //check if format is 12 hours
    {
		rtc_time->time_format = !((hrs & (1 << 5))==0);   //assign format value for AM / PM (0 - AM & 1 - PM)
		hrs &= ~(0x3 << 5);                               //Clear bits 5 & 6
	}
    else             //24 hr format
    {
		rtc_time->time_format = TIME_FORMAT_24HRS;
	}

    //hours - convert bcd to binary
	rtc_time->hours = bcd_to_binary(hrs);
}


//get current date
void ds1307_get_current_date(RTC_date *rtc_date)
{
	rtc_date->day =  bcd_to_binary(ds1307_read(DS1307_ADD_DAY));
	rtc_date->date = bcd_to_binary(ds1307_read(DS1307_ADD_DATE));
	rtc_date->month = bcd_to_binary(ds1307_read(DS1307_ADD_MONTH));
	rtc_date->year = bcd_to_binary(ds1307_read(DS1307_ADD_YEAR));
}






static void ds1307_i2c_pin_config()
{
    GPIO_Handle I2C_SDA, I2C_SCL;

    memset(&I2C_SDA, 0, sizeof(I2C_SDA));                    //clearing the structure member's values
    memset(&I2C_SCL, 0, sizeof(I2C_SCL));                    //clearing the structure member's values

    I2C_SDA.pGPIOx = DS1307_I2C_GPIO_PORT;
    I2C_SDA.GPIOx_PinConfig.PinAltFuncMode = 4;
    I2C_SDA.GPIOx_PinConfig.PinMode = GPIO_MODE_ALTFUN;
    I2C_SDA.GPIOx_PinConfig.PinNo = DS1307_I2C_SDA_PIN;
    I2C_SDA.GPIOx_PinConfig.PinOPType = GPIO_OP_TYPE_OD;
    I2C_SDA.GPIOx_PinConfig.PinPUPDCtrl = DS1307_I2C_PUPD;
    I2C_SDA.GPIOx_PinConfig.PinSpeed = GPIO_OP_SPEED_FAST;
    GPIO_Init(&I2C_SDA);

    I2C_SCL.pGPIOx = DS1307_I2C_GPIO_PORT;
    I2C_SCL.GPIOx_PinConfig.PinAltFuncMode = 4;
    I2C_SCL.GPIOx_PinConfig.PinMode = GPIO_MODE_ALTFUN;
    I2C_SCL.GPIOx_PinConfig.PinNo = DS1307_I2C_SCL_PIN;
    I2C_SCL.GPIOx_PinConfig.PinOPType = GPIO_OP_TYPE_OD;
    I2C_SCL.GPIOx_PinConfig.PinPUPDCtrl = DS1307_I2C_PUPD;
    I2C_SCL.GPIOx_PinConfig.PinSpeed = GPIO_OP_SPEED_FAST;
    GPIO_Init(&I2C_SCL);
}



static void ds1307_i2c_config()
{
    DS_1307.pI2Cx = DS1307_I2C;
    DS_1307.I2C_Config.I2C_AckControl = I2C_ACK_ENABLE;
    DS_1307.I2C_Config.I2C_SCLSpeed = DS1307_I2C_SPEED;
    I2C_Init(&DS_1307);
}



uint8_t ds1307_init()
{
    //initialize GPIO for I2C
    ds1307_i2c_pin_config();

    //initialize I2C
    ds1307_i2c_config();

    //enable I2C peripheral
    I2C_PeripheralControl(DS1307_I2C, ENABLE);

    //4. Make clock halt = 0;
    ds1307_write(0x00,DS1307_ADD_SEC);

    //5. Read back clock halt bit
    uint8_t clock_state = ds1307_read(DS1307_ADD_SEC);

    return ((clock_state >> 7 ) & 0x1);
}





//write values to register in DS 1307
static void ds1307_write(uint8_t value,uint8_t reg_addr)
{
	uint8_t tx[2];
	tx[0] = reg_addr;
	tx[1] = value;
	I2C_MasterSendData(&DS_1307, tx, 2, DS1307_I2C_ADDRESS, 0);  //send register address and value to be written
}



//read register values in DS 1307
static uint8_t ds1307_read(uint8_t reg_addr)
{
	uint8_t data;
    I2C_MasterSendData(&DS_1307, &reg_addr, 1, DS1307_I2C_ADDRESS, 0); //send register address
    I2C_MasterReceiveData(&DS_1307, &data, 1, DS1307_I2C_ADDRESS, 0);  //receive register value

    return data;
}



//conversion of binary to BCD
static uint8_t binary_to_bcd(uint8_t value)
{
	uint8_t m,n;
	uint8_t bcd;

	bcd = value;       //for decimal values less than 10, bcd value is the same

	if(value >= 10)    //for decimal values greater than 10..
	{
		m = value/10;             //extract tenth's place
		n = value % 10;           //extract one's place
		bcd = (m << 4)|n;         //shift the tenth's place value by 4 bits and OR it with the one's place value
	}

	return bcd;
}



//conversion of BCD to binary
static uint8_t bcd_to_binary(uint8_t value)
{
	uint8_t m,n;
	m = (uint8_t) ((value>>4)*10);   //extract the tenth's place value from bcd
	n =  value & (uint8_t)0x0F;      //extract the one's place value from bcd
	return (m+n);                    //add both the values to get the equivaent binary value
}
