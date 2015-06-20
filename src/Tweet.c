#include "Tweet.h"

#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>

#include "Program.h"

void PrintTweet(PPROGRAM_CONTEXT ProgramContext, PTWEET Tweet)
{
	PrintTweetDebugInfoToStream(stdout, Tweet);
	wprintf(L"%S\n", ProgramContext->TweetStrings + Tweet->TweetStringOffset);
}

void PrintTweetDebugInfoToStream(FILE * Stream, PTWEET Tweet)
{
	fwprintf(Stream, L"%3u\t%2u\t", Tweet->Size, Tweet->SearchTermValue);
	
	for(PUNICODE_APPEARANCE UnicodeAppearance = Tweet->UnicodeAppearance; UnicodeAppearance != (Tweet->UnicodeAppearance + Tweet->NumberOfDifferentUnicodes); UnicodeAppearance++)
	{
		fwprintf(Stream, L"%4x: %3u\t", UnicodeAppearance->UnicodeCharacter, UnicodeAppearance->NumberOfAppearance);
	}
}

void CleanUpTweet(PTWEET Tweet)
{
	if(Tweet->UnicodeAppearance != NULL)
	{
		free(Tweet->UnicodeAppearance);
		Tweet->UnicodeAppearance = NULL;
	}
}

int CompareTweets(const void * a, const void * b)
{	
	const TWEET * A = a;
	const TWEET * B = b;
	
	if(A->SearchTermValue != B->SearchTermValue)
	{
		return B->SearchTermValue - A->SearchTermValue;
	}
	else
	{
		uint8_t MinIndex = A->NumberOfDifferentUnicodes < B->NumberOfDifferentUnicodes ? A->NumberOfDifferentUnicodes : B->NumberOfDifferentUnicodes;	
		
		PUNICODE_APPEARANCE UnicodeAppearanceA, UnicodeAppearanceB;
		
		for(UnicodeAppearanceA = A->UnicodeAppearance, UnicodeAppearanceB = B->UnicodeAppearance; 
			UnicodeAppearanceA != (A->UnicodeAppearance + MinIndex);
			UnicodeAppearanceA++, UnicodeAppearanceB++)
		{
			if(UnicodeAppearanceA->UnicodeCharacter != UnicodeAppearanceB->UnicodeCharacter)
			{
				return UnicodeAppearanceA->UnicodeCharacter - UnicodeAppearanceB->UnicodeCharacter;
			}
			else if(UnicodeAppearanceA->NumberOfAppearance != UnicodeAppearanceB->NumberOfAppearance)
			{
				return UnicodeAppearanceB->NumberOfAppearance - UnicodeAppearanceA->NumberOfAppearance;
			}
		}
		
		return 	B->NumberOfDifferentUnicodes - A->NumberOfDifferentUnicodes;
	}
}