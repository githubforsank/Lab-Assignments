 #include<stdio.h>
#include<stdlib.h>
struct Student{
    char name[30];
    int roll_no;
    float sgpa;
    float cgpa;
    struct Student *add;
}*home,*current,*new_ptr,*temp;

void main()
{
    home=NULL;
    current=NULL;
    int choice;
    printf("<<<........Welcome to the Student Database ........>>> \n");
    while(1)
    {
    	printf("...................................\n");
        printf("..	Enter Choice             .. \n"
               "..	1- Append                ..\n"
               "..	2- Search                ..\n"
               "..      3- Delete                ..\n"
               "..      4- Display               ..\n" );
        printf("...................................\n");
               scanf("%d",&choice);
               switch(choice)
               {

               case 1:
                    append();
                    break;
               case 2:
                    search();
                    break;
               case 3:
                    delete_node();
                    break;
               case 4:
                    display();
                    break;
               default:
                printf(" Oooops ! Your finger just touched the wrong button Re-Enter \n");
               }
    }

}
void append()
{
    current=home;
    printf("...................................\n"
			"..     Append Query              ..\n"
		    "...................................\n\n\n");
    
    if(current==NULL)
    {
        current=malloc(sizeof(struct Student));
        printf("Enter name : ");
        scanf("%s",current->name);
        printf("Enter roll no: ");
        scanf("%d",&current->roll_no);
        printf("Enter sgpa : ");
        scanf("%f",&current->sgpa);
        printf("Enter cgpa : ");
        scanf("%f",&current->cgpa);
        home=current;
        current->add=NULL;
        printf("Node creation successful !!! \n");
        printf("...................................\n");
	printf("..	Node Details	         ..\n");
	printf("...................................\n\n");
	printf("Name : %s\n",current->name);
	printf("Roll No.: %d\n",current->roll_no);
	printf("SGPA : %f\n",current->sgpa);
    printf("CGPA : %f\n",current->cgpa);    
    }
    else{

         	new_ptr=malloc(sizeof(struct Student));
            printf("Enter name : ");
            scanf("%s",new_ptr->name);
            printf("Enter roll no: ");
            scanf("%d",&new_ptr->roll_no);
            printf("Enter sgpa : ");
            scanf("%f",&new_ptr->sgpa);
            printf("Enter cgpa");
            scanf("%f",&new_ptr->cgpa);
            new_ptr->add=NULL;
            printf("\nNode creation successful !!! \n");
            printf("..	Node Details	         ..\n");
			printf("...................................\n\n");
			printf("Name : %s\n",new_ptr->name);
			printf("Roll No.: %d\n",new_ptr->roll_no);
			printf("SGPA : %f\n",new_ptr->sgpa);
   			printf("CGPA : %f\n",new_ptr->cgpa); 
            while(current->add!=NULL)
        {
         	  current=current->add;

        }
        current->add=new_ptr;
    }
}
void search()
{
	printf("...................................\n");
	printf("..	Search Query	         ..\n");
	printf("...................................\n\n");
    int roll_no;
    int flag=0;
    current=home;
    printf("Enter roll no. to search \n");
    scanf("%d",&roll_no);
    while(current!=NULL)
    {
        if(current->roll_no==roll_no)
        {
            printf("Record found !!!!\n");
            printf("Student Name : %s \n",current->name);
            printf("Roll No.: %d \n",current->roll_no);
            printf("SGPA : %f \n",current->sgpa);
            printf("CGPA : %f \n",current->cgpa);
            flag=1;
        }
        if(flag==1)
            break;
            current=current->add;
    }
    if(flag==0)
    {
        printf("No such record exists\n");
    }

}
void display()
{
	printf("...................................\n");
	printf("..	Display Query	         ..\n");
	printf("...................................\n\n");
    int i=1;
    current=home;
    if(current==NULL)
    	printf("No records present !!!! \n");
    while(current!=NULL)
    {
		printf(".......................................\n");
        printf("Record No : %d\n",i);
        printf("Student Name : %s\n",current->name);
        printf("Roll No.: %d\n",current->roll_no);
        printf("SGPA : %f\n",current->sgpa);
        printf("CGPA : %f\n",current->cgpa);
        i++;
        printf(".......................................\n\n");
        current=current->add;

    }
}

void delete_node()
{
	printf("...................................\n");
	printf("..	Delete Query	         ..\n");
	printf("...................................\n\n");
    current=home;
    int flag=0;
    int roll_no;

    if(current==NULL)
    {
        printf("No records to be deleted \n");
        return;
    }
    printf("Enter -roll no- to be deleted \n ");
    scanf("%d",&roll_no);
    if(current->roll_no==roll_no){
        printf("Records deleted \n");
        printf("Student Name : %s \n",current->name);
        printf("Roll No.: %d \n",current->roll_no);
        printf("SGPA : %f \n",current->sgpa);
        printf("CGPA : %f \n",current->cgpa);
        home=NULL;
        current=NULL;
        printf("Node deleted successfully !!!!! \n");
        return;
        }
        while(current->add!=NULL)
        {
            if(current->add->roll_no==roll_no)
            {
            temp=current->add;
            printf("Student Name : %s \n",current->add->name);
            printf("Roll No.: %d \n",current->add->roll_no);
            printf("SGPA : %f \n",current->add->sgpa);
            printf("CGPA : %f \n",current->add->cgpa);
            current->add=current->add->add;
            free(temp);
            printf("Node deleted successfully !!!!! \n");
            flag=1;
            }
            if(flag==1)
                break;
        }
        printf("No such records found \n");

}
