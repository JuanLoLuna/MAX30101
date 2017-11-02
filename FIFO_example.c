#include<stdio.h>
#include<fcntl.h>
#include<sys/ioctl.h>
//#include<linux/i2c.h>
#include<linux/i2c-dev.h>
#include<unistd.h>

int main(){
	int file;
	char *filename = "/dev/i2c-1";
	printf(" Starting the MAX30101 test application\n");
	if(( file = open(filename, O_RDWR)) < 0){
		perror(" failed to open the bus\n");
		return 1;
	}
	if( ioctl( file, I2C_SLAVE, 0x57) < 0){ 
		perror(" Failed to connect to the sensor\n");
		return 1; 
	}
	
	// Set the FIFO writer pointer location
	i2c_smbus_write_byte_data(file,0x04,0x00);
	// Set the FIFO reader pointer location
	i2c_smbus_write_byte_data(file,0x06,0x00);
	// Enable PPG_RDY_EN interruption
	i2c_smbus_write_byte_data(file,0x02,0x40);	

	// TEST: only green LED configuration
	// Set LED pulse amplitude, testing 50mA
	i2c_smbus_write_byte_data(file,0x0E,0xFF);
	i2c_smbus_write_byte_data(file,0x10,0xFF);	
	// Set only red LED on SLOT1 in MultiLED configuration register
	i2c_smbus_write_byte_data(file,0x11,0x03);
	// Set MultiLED mode
	i2c_smbus_write_byte_data(file,0x09,0x07);	
	
	int i;
	__s8 PPG_RDY_flag;
	__s8 LEDbyte1;
	__s8 LEDbyte2;
	__s8 LEDbyte3;
	
	for(i=1;i<100;i++){
		PPG_RDY_flag = i2c_smbus_read_byte_data(file,0x00);
		while( PPG_RDY_flag != 0x40 ){
			PPG_RDY_flag = i2c_smbus_read_byte_data(file,0x00);	
		}	
	
		LEDbyte3 = i2c_smbus_read_byte_data(file,0x07);
		LEDbyte2 = i2c_smbus_read_byte_data(file,0x07);
		LEDbyte1 = i2c_smbus_read_byte_data(file,0x07);

		printf("Data: %d\n",LEDbyte3+LEDbyte2+LEDbyte1);	
	}
}
