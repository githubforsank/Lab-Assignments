#include<iostream>
#include<cstdlib>

using namespace std;

struct LinkList{
	int data;
	LinkList* next;
}

*front=NULL,*f=NULL,*end=NULL,*e=NULL,*current,*current2,*T1,*T2;

void node_Insert(int a)
{
	current=new LinkList;
	current->data=a;
	current->next=NULL;
	if(front==NULL)
	{
		front=end=current;
		end->next=NULL;
	}
	
	else
	{
		end->next=current;
		end=current;
		end->next=NULL;
	}
}

void node_Insert2(int b)
{
	current2=new LinkList;
	current2->data=b;
	current2->next=NULL;
	if(f==NULL)
	{
		f=e=current2;
		e->next=NULL;
	}
	
	else
	{
		e->next=current2;
		e=current2;
		e->next=NULL;
	}
}

void merge()
{
	end->next=f;
}


void sort()
{
	int t=0;
	for(T1=front;T1!=NULL;T1=T1->next)
	{
		for(T2=T1->next;T2!=NULL;T2=T2->next)
		{
			if(T1->data>T2->data)
			{
				t=T1->data;
				T1->data=T2->data;
				T2->data=t;
			}
		}
	}
}

void sort2()
{
	int t=0;
	for(T1=f;T1!=NULL;T1=T1->next)
	{
		for(T2=T1->next;T2!=NULL;T2=T2->next)
		{	
			if(T1->data>T2->data)
			{
			t=T1->data;
			T1->data=T2->data;
			T2->data=t;
		}
		}
	}
}


void display()
{
	for(T1=front;T1!=NULL;T1=T1->next)
	{
		cout<<T1->data<<" >> ";
	}
	cout<<endl;
}

void display2()
{
	for(T2=f;T2!=NULL;T2=T2->next)
	{
		cout<<T2->data<<" >> ";
	}
	cout<<endl;
}
void display_merge()
{
	int c=0;
	int i=0;
	int a[100];
	
	for(T1=front;T1!=NULL;T1=T1->next)
	{
		a[i]=T1->data;
		c++;
		i++;
	}
	for(i=c-1;i>=0;i--)
	{
		cout<<a[i]<<" << ";
	}
	cout<<endl;
}
int main()
{
	void node_Insert(int);
	void node_Insert2(int);
	void display();
	void sort();
	void sort2();
	
	for(int i=0;i<10;i++)
	{
	
	int c= rand()%100;
	node_Insert(c);
}
	cout<<"The unsorted LINK LIST 1 :\n\n";
	display();
	cout<<endl;
	sort();
	cout<<"The sorted LINK LIST 1 :\n\n";
	display();
	cout<<endl;
	cout<<endl;
	
	
	for(int i=0;i<10;i++)
	{
		 int d=rand()%100;
		 node_Insert2(d);
	}
	
cout<<"The unsorted LINK LIST 2:\n\n";
	display2();
	cout<<endl;
	sort2();
	cout<<"The sorted LINK LIST 2 :\n\n";
	display2();
	cout<<endl;
	
	cout<<"The merged LinkList : \n\n";
	merge();
	display();
	cout<<endl<<endl;
	cout<<"The sorted merged list :\n\n";
	sort();
	display();
	cout<<endl<<endl;
	
	cout<<"The reverse merged list : \n\n";
	display_merge();
	
}
	
