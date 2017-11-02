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

	// Enable temperature (set 0x01 on 0x21 register)	
	i2c_smbus_write_byte_data(file,0x21,0x01);

	__s8 intTemp;
	__s8 fracTemp;
	// Read int temperature from register 0x1F
	intTemp = i2c_smbus_read_byte_data(file, 0x1F);

	// Read frac temperature from register 0x20
	fracTemp = i2c_smbus_read_byte_data(file, 0x20);
	
	printf("On board temperature: %.4f\n",intTemp+(fracTemp*0.0625));
}
