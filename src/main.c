#include <mpi.h>
#include <stdio.h>

#include "Program.h"
#include "Node.h"
#include "TweetParsing.h"

int main(int argc, char * argv[])
{
    MPI_Init(&argc, &argv);

    PROGRAM_CONTEXT ProgramContext; 
    int Status;

    //Init Program & Node -> ParseArguments and set NodeID
    Status = InitProgramContext(&ProgramContext, argc, argv);
    if(Status != 0)
    {
    	return Status;
    }
    
    //Parse File
    Status = ParseTweets(&ProgramContext);
    if(Status != 0)
    {
    	return Status;
    }
    
    if(IsMasterNode(&(ProgramContext.NodeContext)))
    {
        printf("File: %s\nTweetCount: %d\n", ProgramContext.Filename, ProgramContext.TweetCount);
    }

    //Start Bitonic Sort

    MPI_Finalize();
	return 0;
}