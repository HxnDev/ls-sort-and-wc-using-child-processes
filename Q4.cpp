#include<iostream>
#include<fstream>
#include<unistd.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<fcntl.h>
#include<string.h>
using namespace std;

int main(int argc, char *argv[])
{

	pid_t pid1,pid2,pid3;
	int pipe1[2],pipe2[2];			
	pipe(pipe1);
	pipe(pipe2);

	pid=fork();
	
	
	if (pid1 > 0)       //Parent 1
	{
		close(pipe1[1]);   //Writing end of pipe 1		
		
        wait(NULL);
		
		pid2 = fork();	
		
		if (pid2 > 0)	    //Parent 2
		{
			close(pipe1[0]);   //Reading end of pipe 1	 
			close(pipe2[1]);	//Writing end of pipe 2
			
            wait(NULL);

			pid3 = fork();	
			
			if(pid3 > 0)	//Parent 3
			{
				close(pipe2[0]);	    //Reading end of pipe 2	
				wait(NULL);
			
			}
			
			else if(pid3 == 0)	    //Child 3
			{
                close(pipe1[0]);	    //Reading end of pipe 1
                close(pipe1[1]);	    //Writing end of pipe 2
                close(pipe2[1]);	    //Writing end of pipe 2
        
                dup2(pipe2[0],0);	
                execl("/usr/bin/wc","wc",NULL,NULL);	
			}

            else
            {
                cout<<"Fork3 failed"<<endl;
            }
            
		}
		
		else if(pid2 == 0)	
		{
			
			close(pipe1[1]);       //Writing end of pipe 1
			close(pipe2[0]);      //Reading end of pipe 2
			
	
			dup2(pipe1[0],0);		
			dup2(pipe2[1],1);
			execl("/usr/bin/sort -r","sort",NULL); // sorting
		}

        else
        {
            cout<<"Fork3 failed"<<endl;
        }
        
	
	}
	
	else if (pid1 == 0)	
	{
		close(pipe1[0]);		   //Reading end of pipe 1
		close(pipe2[0]);         //Reading end of pipe 1
		close(pipe2[1]);         //Reading end of pipe 2
		dup2(pipe1[1],1);		
		
        execl("/bin/ls","ls",NULL,NULL);
	}

    else
    {
        cout<<"Fork1 Failed"<<endl;
    }	
}