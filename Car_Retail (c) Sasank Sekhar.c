 #include <stdio.h>
 #include<stdlib.h>
 


struct car_retail
{
		struct car_kind ob;
		char customer_name[50];
		long long int mob_no;
};

struct car_detail{
	int engine_no;
	int chasis_no;
	
};

struct car_kind{
	char kind_name[50];
	struct car_detail ob;
};

void main()
{
	
}
