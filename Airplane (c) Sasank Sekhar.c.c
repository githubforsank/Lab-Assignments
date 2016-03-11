#include<stdio.h>
#include<stdlib.h>
#include<time.h>


struct flight_arrival
{
	int flight_no;
	struct flight_arrival *add;
	int fuel_time;

} *start,*end,*current;



struct flight_departure
{
	int flight_no;
	struct flight_departure *add;

}*st,*en,*cr;




void create_departing_queue(struct flight_arrival *current)
{
 	if(st==NULL)
  	{
		 st=malloc(sizeof(struct flight_departure));
		 st->flight_no=current->flight_no;
		 en=st;
	}
	else
	{
		en->add=malloc(sizeof(struct flight_departure));
		en->flight_no=current->flight_no;
		en=en->add;
	}
}


int t_f_no,t_f_arrival,t_f_time;

    void sort()
    {
    	current=start;
    	while(current->add!=NULL)
    	{
    		if(current->fuel_time>current->add->fuel_time)
    		{
    			t_f_no=current->flight_no          ;
    			t_f_time=current->fuel_time      ;
    			current->flight_no=current->add->flight_no;
    			current->fuel_time=current->add->fuel_time;
    			current->add->flight_no=t_f_no;
    			current->add->fuel_time=t_f_time;
			}

	}	current=current->add;
		}

	void update_fuel()
	{
		current=start;
                if(current==NULL)
                {
                    return;
                }
                else
                {
                    while(current->add!=NULL)
                    {

       			current->fuel_time-=1;
                current=current->add;


                    }
			}

	}

	int calc_landing_q_size()
	{
		int c=0;
		current=start;
		while(current->add!=NULL)
		{
			c++;
			current=current->add;
		}
		return c;
	}
	int calc_departing_q_size()
	{
		int c=0;
		cr=st;
		while(cr->add!=NULL)
		{
			c++;
			cr=cr->add;
		}
		return c;
	}
	 void landing_q_trav()
	 {
	 	printf("Current Landing Queue \n");
	 	current=start;
	 	while(current->add!=NULL)
	 	{
	 		printf("Flight NO : %d \n",current->flight_no);
	 		printf("Fuel Time left %d \n",current->fuel_time);
	 		current=current->add;
		 }
	 }
	 void departing_q_trav()
	 {
	 	printf("Current Departing Queue \n");
	 	cr=st;
	 	while(cr->add!=NULL)
	 	{
	 		printf("Flight NO : %d \n",cr->flight_no);
	 		cr=cr->add;
		 }
	 }
	 int successful_arrivals=0;
	void flight_land()
	{
		current=start;
		if(current->fuel_time<10)//The 10 parameter can be changed as per the priority of the hovering flights to be landed.
		{  
		if(current==NULL)
		{
		printf("No flights to land");
		return;
	}
		if(current!=end)
		{
			
			printf("Flight no %d landed \n",current->flight_no);
			successful_arrivals++;
			create_departing_queue(current);
			current=current->add;
			free(current);

		}
		else
		{
			printf("Flight no %d landed \n",start->flight_no);
			create_departing_queue(current);
			successful_arrivals++;
			free(start);
			start=NULL;
			end=NULL;
		}
	}
	else
			depart();
	}
	int successful_departures=0;
	void depart()
	{
		if(st==NULL)
		{
			printf("No flights to depart \n");
			return;
		}
		if(st!=en)
		{
		printf("Departing Information \n");
		printf("Flight Departed : %d \n",st->flight_no);
		successful_departures++;
		cr=st;
		st=st->add;
		free(cr);
	}
	else
	{
		printf("Departing Information \n");
		printf("Flight Departed : %d \n",st->flight_no);
		successful_departures++;
		free(st);
		st=NULL;
		en=NULL;
	}

	}

void	show_stats(int interval,int calc_landing_q_size,int calc_departing_q_size,int successful_departures,int successful_arrivals)
{
	printf("Interval Stats \n");
	printf("Landing Queue Size : %d",calc_landing_q_size);
	printf("Landing Departing Queue Size : %d",calc_departing_q_size);
	printf("Successful Departures : %d",successful_departures);
	printf("Successful Arrivals : %d",successful_arrivals);
}


int main()
{
    start=NULL;
    end=NULL;
    st=NULL;
    en=NULL;
	clock_t time=clock();
	clock_t after,diff;
	int interval=0;

printf("%d",(int)rand%26);

	do
	{


        int pass=1;
        int flag=1;

            
	 	do
	 	{
	 		after=clock();
		    diff=(int)(after-time)/CLOCKS_PER_SEC;

		   	
			
            if(pass==1)
            {
				int i=5;
				int j=1;
				printf("%d",i);	
				
			while(j<=i)
			{
				printf("Hello");
			if(start==NULL)
			{

				struct flight_arrival *node;
				node=malloc(sizeof(struct flight_arrival));
				node->flight_no=(int)rand();
				node->add=NULL;
				node->fuel_time=(int)rand()%26;
				start=node;
				end=node;
				printf("Node created");
			}
			else
			{
				struct flight_arrival *node=malloc(sizeof(struct flight_arrival));
				node->flight_no=(int)rand();
				node->add=NULL;
				node->fuel_time=(int)rand()%26;
				end=node;
				printf("Node created");
			
			}
                    j++;
			}
			
			
            }


            pass++;
			

			if(diff%1==0&&flag==1)
            {
                update_fuel();
                flag=0;
			}

			if(diff%2==0&&flag==1)
			{
				
				flight_land();
				flag=0;
					
				}

				////count_flight_land++;
			

	}while(diff<=12);
	
	
		pass=1;
  		show_stats(interval,calc_landing_q_size(),calc_departing_q_size(),successful_departures,successful_arrivals);
  		landing_q_trav();
  		interval++;


  		departing_q_trav();
  		successful_departures=0;
  		successful_arrivals=0;


  	}while(diff<1000);
  	return 0;
}
//show stats() defination not available
