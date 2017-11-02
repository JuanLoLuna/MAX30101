#include<stdio.h>
#include<fcntl.h>
#include<sys/ioctl.h>
//#include<linux/i1c.h>
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

	// Device register to acces
	__u8 reg = 0x02;
	__u8 res;
	// Byte to be written on register
	__u8 write = 0x00;

	/* Read using SMBus commands */
	res = i2c_smbus_read_byte_data(file, reg);
	if (res < 0) {
	/* ERROR HANDLING: i2c transaction failed */
	} else {
	/* res contains the read word */
		printf("Result: %x\n",res);
	}

	/* Wrtie using SMBus commands */
	i2c_smbus_write_byte_data(file,reg,write);
}
