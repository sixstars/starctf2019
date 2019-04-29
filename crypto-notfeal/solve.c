// this file has been modified and you can recover subkey for each round interactively

//Differential Cryptanalysis of FEAL-4
//Uses a chosen-plaintext attack to fully recover the key
//For use with tutorial at http://theamazingking.com/crypto-feal.php

#include <stdio.h>
#include <math.h>
#include <stdint.h>

#define MAX_CHOSEN_PAIRS 10000
#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

int winner = 0;
int loser = 0;

unsigned long subkey[6];
unsigned long statickey[6] = {0x6bb508a,0xd72eaec0,0x71d88eee,0xa5da0171,0x3139398f,0x2ccdf0f0};

unsigned char rotl2(unsigned char a) {return ((a << 2) | (a >> 6));}

unsigned long leftHalf(unsigned long long a) {return (a >> 32LL);}
unsigned long rightHalf(unsigned long long a) {return a;}
unsigned char sepByte(unsigned long a, unsigned char index) {return a >> (8 * index);}
unsigned long combineBytes(unsigned char b3, unsigned char b2, unsigned char b1, unsigned char b0)
{
 		 return b3 << 24L | (b2 << 16L) | (b1 << 8L) | b0;
}
unsigned long long combineHalves(unsigned long leftHalf, unsigned long rightHalf)
{
 		 return (((unsigned long long)(leftHalf)) << 32LL) | (((unsigned long long)(rightHalf)) & 0xFFFFFFFFLL);
}

unsigned char gBox(unsigned char a, unsigned char b, unsigned char mode)
{
    return rotl2(a + b + mode);
}

unsigned long fBox(unsigned long plain)
{
    unsigned char x0 = sepByte(plain, 0);
    unsigned char x1 = sepByte(plain, 1);
    unsigned char x2 = sepByte(plain, 2);
    unsigned char x3 = sepByte(plain, 3);

    unsigned char t0 = (x2 ^ x3);
    
    unsigned char y1 = gBox(x0 ^ x1, t0, 1);
    unsigned char y0 = gBox(x0, y1, 0);
    unsigned char y2 = gBox(t0, y1, 0);
    unsigned char y3 = gBox(x3, y2, 1);

    return combineBytes(y0, y1, y2, y3);
    //return combineBytes(y3, y2, y1, y0);
}

unsigned long long encrypt(unsigned long long plain)
{
    unsigned long left = leftHalf(plain);
    unsigned long right = rightHalf(plain);
    //printf("%x,%x\n",left,right);

    left = left ^ subkey[4];
    right = right ^ subkey[5];
    right = right ^ left;
    //printf("%x,%x\n",left,right);

    unsigned long round2Right = left;
    unsigned long round2Left = right ^ fBox(left ^ subkey[0]);
    //printf("after k0: %x,%x\n",round2Left,round2Right);

    unsigned long round3Right = round2Left;
    unsigned long round3Left = round2Right ^ fBox(round2Left ^ subkey[1]);
    //printf("after k1: %x,%x\n",round3Left,round3Right);

    unsigned long round4Right = round3Left;
    unsigned long round4Left = round3Right ^ fBox(round3Left ^ subkey[2]);
    //printf("after k2: %x,%x\n",round4Left,round4Right);

    left = round4Right ^ fBox(round4Left ^ subkey[3]);
    right = round4Left;
    //printf("after k3: %x,%x\n",left,right);
    unsigned long cipherRight = left ^ right;
    unsigned long cipherLeft = right;
    //printf("%x,%x\n",cipherLeft,cipherRight);

    return combineHalves(cipherLeft, cipherRight);
}

void generateSubkeys(int seed)
{
    srand(seed);

    int c;
    for(c = 0; c <  6; c++)
        //subkey[c] = (rand() << 16L) | (rand() & 0xFFFFL);
        subkey[c] = statickey[c];
}

int numPlain;
unsigned long long plain0[MAX_CHOSEN_PAIRS];
unsigned long long cipher0[MAX_CHOSEN_PAIRS];
unsigned long long plain1[MAX_CHOSEN_PAIRS];
unsigned long long cipher1[MAX_CHOSEN_PAIRS];
unsigned long long statsp0[3][6] = { { 7048188517316564675 , 12825066424297617397 , 13982275348835645359 , 7997352591980253550 , 9943703837341437757 , 5368176796867583232 }, { 9982887355749123894 , 12785352781874485157 , 18182536123490373230 , 566806643083139550 , 1137695181956960488 , 12891026720139082070 }, { 1032281012381723908 , 564680172018082774 , 15389916003603960434 , 546489500611450050 , 5548532938388242886 , 4542632487350202421 } };
unsigned long long statsc0[3][6] = { { 9533448304884396727 , 8340911223642448591 , 486015852071413092 , 14732768206283729966 , 12927250386057848896 , 3498771934970192171 }, { 8077475163787409928 , 36070376451496024 , 7002065340444720468 , 3836488262879817996 , 4116651873321838340 , 14724546609183381907 }, { 1491395147629028468 , 27986761417199792 , 13740334194824131372 , 18286444664320791649 , 10713566717792549958 , 16772546883815774487 } };
unsigned long long statsp1[3][6] = { { 7048188517316597315 , 12825066424297650037 , 13982275348835612463 , 7997352591980286446 , 9943703837341405117 , 5368176796867616128 }, { 9983028642993326006 , 12785211494630348581 , 18182677410734575342 , 566665355839002974 , 1137553894712758376 , 12891166907871656406 }, { 888165824272313604 , 420564983975781334 , 15534031191713370738 , 402374312569148610 , 5692648126497653190 , 4398517299307900981 } };
unsigned long long statsc1[3][6] = { { 11841643330673051197 , 1429900272615608838 , 1892929039183045184 , 12424629587526100290 , 6011876236192051371 , 5807051352085183119 }, { 10005886116650948 , 5955897681464251364 , 8361616289863700606 , 2815539139842138997 , 16646128912008935309 , 3222529086334277393 }, { 12518318839774872943 , 5153246371973425526 , 710059454831319824 , 5686169572215424911 , 8960951235428587375 , 5475852056143094730 } };

void undoFinalOperation()
{
        int c;
        for(c = 0; c < numPlain; c++)
        {
            unsigned long cipherLeft0 = leftHalf(cipher0[c]);
            unsigned long cipherRight0 = rightHalf(cipher0[c]) ^ cipherLeft0;
            unsigned long cipherLeft1 = leftHalf(cipher1[c]);
            unsigned long cipherRight1 = rightHalf(cipher1[c]) ^ cipherLeft1; 
			
			cipher0[c] = combineHalves(cipherLeft0, cipherRight0);   
			cipher1[c] = combineHalves(cipherLeft1, cipherRight1);
         }   
}

unsigned long crackLastRound(unsigned long outdiff)
{
    printf("  Using output differential of 0x%08x\n", outdiff);
    printf("  Cracking...");

    unsigned long fakeK;
    for(fakeK = 0x00000000L; fakeK < 0xFFFFFFFFL; fakeK++)
    {
        int score = 0;

        int c;
        for(c = 0; c < numPlain; c++)
        {
            unsigned long Y0 = leftHalf(cipher0[c]);
            unsigned long Y1 = leftHalf(cipher1[c]);

            unsigned long fakeInput0 = Y0 ^ fakeK;
            unsigned long fakeInput1 = Y1 ^ fakeK;
            unsigned long fakeOut0 = fBox(fakeInput0) ^ rightHalf(cipher0[c]);
            unsigned long fakeOut1 = fBox(fakeInput1) ^ rightHalf(cipher1[c]);
            uint32_t fakeDiff = fakeOut0 ^ fakeOut1;

            if (fakeDiff == outdiff) score++; else break;
        }

        if (score == numPlain)
        {
            printf("found subkey : 0x%08lx\n", fakeK);
            //return fakeK;
        }
    }
    
    printf("failed\n");
    return 0;
}

void chosenPlaintext(unsigned long long diff)
{
    int c = -1;
    if (diff == 0x0000000000008080LL) {
        c = 0;
    } else if (diff == 0x0000808000008080LL) {
        c = 1;
    } else if (diff == 0x0200000002000000LL) {
        c = 2;
    }
    for (int i=0; i < numPlain; i++) {
        plain0[i] = statsp0[c][i];
        plain1[i] = statsp1[c][i];
        cipher0[i] = statsc0[c][i];
        cipher1[i] = statsc1[c][i];
    }
}

void undoLastRound(unsigned long crackedSubkey)
{
 	 int c;
 	 for(c = 0; c < numPlain; c++)
 	 {
 	        unsigned long cipherLeft0 = leftHalf(cipher0[c]);
            unsigned long cipherRight0 = rightHalf(cipher0[c]);
            unsigned long cipherLeft1 = leftHalf(cipher1[c]);
            unsigned long cipherRight1 = rightHalf(cipher1[c]); 
			
			cipherRight0 = cipherLeft0;
			cipherRight1 = cipherLeft1;
			cipherLeft0 = fBox(cipherLeft0 ^ crackedSubkey) ^ rightHalf(cipher0[c]);
			cipherLeft1 = fBox(cipherLeft1 ^ crackedSubkey) ^ rightHalf(cipher1[c]);
			

			cipher0[c] = combineHalves(cipherLeft0, cipherRight0);
			cipher1[c] = combineHalves(cipherLeft1, cipherRight1);	   
   	 }
}

void prepForCrackingK0()
{
 	 int c;
	 for(c = 0; c < numPlain; c++)
	 {
  	  	    unsigned long cipherLeft0 = leftHalf(cipher0[c]);
            unsigned long cipherRight0 = rightHalf(cipher0[c]);
            unsigned long cipherLeft1 = leftHalf(cipher1[c]);
            unsigned long cipherRight1 = rightHalf(cipher1[c]); 
			
			unsigned long tempLeft0 = cipherLeft0;
			unsigned long tempLeft1 = cipherLeft1;
			cipherLeft0 = cipherRight0;
			cipherLeft1 = cipherRight1;
			cipherRight0 = tempLeft0;
			cipherRight1 = tempLeft1;

			cipher0[c] = combineHalves(cipherLeft0, cipherRight0);
			cipher1[c] = combineHalves(cipherLeft1, cipherRight1);    	   
     } 	 
}

int main()
{
/*
    generateSubkeys(time(NULL));
    cipher0[0] = encrypt(0x3433323138373635);
    printf("---\n");
    cipher1[0] = encrypt(0x3433323138373635^0x8080);
    //cipher1[0] = encrypt(0x3433323138373635^0x808000008080);
    //cipher1[0] = encrypt(0x3433323138373635^0x200000002000000);
    printf("---\n");
    printf("%llx\n", cipher0[0]) ;
    printf("%llx\n", cipher1[0]) ;
    numPlain = 1;

 	undoFinalOperation();
 	undoLastRound(subkey[3]);
 	undoLastRound(subkey[2]);
 	undoLastRound(subkey[1]);
    printf("%llx\n", cipher0[0]) ;
    printf("%llx\n", cipher1[0]) ;
    return 0;
    */
    printf("JK'S FEAL-4 DIFFERENTIAL CRYPTANALYSIS DEMO\n");
    printf("-------------------------------------------\n");
 	printf("\n");
 
 
 int graphData[20];
 
 int c;

    generateSubkeys(time(NULL));
	numPlain = 6;
	unsigned long long inputDiff1 = 0x0000000000008080LL;
	unsigned long long inputDiff2 = 0x0000808000008080LL;
	unsigned long long inputDiff3 = 0x0200000002000000LL;
	unsigned long outDiff = 0x02000000L;
	
	unsigned long fullStartTime = time(NULL);
	
//CRACKING ROUND 4
    printf("ROUND 4\n");
 	chosenPlaintext(inputDiff1);
 	undoFinalOperation();
	unsigned long startTime = time(NULL);
	//unsigned long crackedSubkey3 = crackLastRound(outDiff);
	unsigned long crackedSubkey3 = subkey[3];
    printf("%x\n", crackedSubkey3);
    unsigned long endTime = time(NULL);
   	printf("  Time to crack round #4 = %i seconds\n", (endTime - startTime));

//CRACKING ROUND 3
    printf("ROUND 3\n");
 	chosenPlaintext(inputDiff2);
 	undoFinalOperation();
 	undoLastRound(crackedSubkey3);
	startTime = time(NULL);
	//unsigned long crackedSubkey2 = crackLastRound(outDiff);
	unsigned long crackedSubkey2 = subkey[2];
    endTime = time(NULL);
   	printf("  Time to crack round #3 = %i seconds\n", (endTime - startTime));

//CRACKING ROUND 2
    printf("ROUND 2\n");
 	chosenPlaintext(inputDiff3);
 	undoFinalOperation();
 	undoLastRound(crackedSubkey3);
 	undoLastRound(crackedSubkey2);
	startTime = time(NULL);
	//unsigned long crackedSubkey1 = crackLastRound(outDiff);
	unsigned long crackedSubkey1 = subkey[1];
    endTime = time(NULL);
    printf("  Time to crack round #2 = %i seconds\n", (endTime - startTime));

//CRACK ROUND 1
    printf("ROUND 1\n");
    undoLastRound(crackedSubkey1);
	unsigned long crackedSubkey0 = 0;
	unsigned long crackedSubkey4 = 0;
	unsigned long crackedSubkey5 = 0;
	
	printf("  Cracking...");
	
	startTime = time(NULL);
    uint32_t guessK0;
    for(guessK0 = 0; guessK0 < 0xFFFFFFFFL; guessK0++)
    {
	      uint32_t guessK4 = 0;
	      uint32_t guessK5 = 0;
 		  int c;
 		  for(c = 0; c < numPlain; c++)
 		  {
		   		uint32_t plainLeft0 = leftHalf(plain0[c]);
		   		uint32_t plainRight0 = rightHalf(plain0[c]);
		   		uint32_t cipherLeft0 = leftHalf(cipher0[c]);
		   		uint32_t cipherRight0 = rightHalf(cipher0[c]);
		   		
	 	   		uint32_t tempy0 = fBox(cipherLeft0 ^ guessK0) ^ cipherRight0;

	 	  		if (guessK4 == 0)
	 	  		{
				   guessK4 = cipherLeft0 ^ plainLeft0;
  		           guessK5 = tempy0 ^ cipherLeft0 ^ plainRight0;
			    }
			  	else if (((cipherLeft0 ^ plainLeft0) != guessK4) || ((tempy0 ^ cipherLeft0 ^ plainRight0) != guessK5))
  		        {
				 	 guessK4 = 0;
				 	 guessK5 = 0;
					  break; 	 
 		 		}
           }
 	  	   if (guessK4 != 0)
  		   {

		   	  crackedSubkey0 = guessK0;
		   	  crackedSubkey4 = guessK4;
		   	  crackedSubkey5 = guessK5;
		   	  endTime = time(NULL);
				 		   	  
		   	  printf("found subkeys : 0x%08lx  0x%08lx  0x%08lx\n", guessK0, guessK4, guessK5);
			  printf("  Time to crack round #1 = %i seconds\n", (endTime - startTime));
		   	  //break;
	       
		   }	  
    }

	printf("\n\n");
	printf("0x%08lx - ", crackedSubkey0); if (crackedSubkey0 == subkey[0]) printf("Subkey 0 : GOOD!\n"); else printf("Subkey 0 : BAD\n");
	printf("0x%08lx - ", crackedSubkey1); if (crackedSubkey1 == subkey[1]) printf("Subkey 1 : GOOD!\n"); else printf("Subkey 1 : BAD\n");
	printf("0x%08lx - ", crackedSubkey2); if (crackedSubkey2 == subkey[2]) printf("Subkey 2 : GOOD!\n"); else printf("Subkey 2 : BAD\n");
	printf("0x%08lx - ", crackedSubkey3); if (crackedSubkey3 == subkey[3]) printf("Subkey 3 : GOOD!\n"); else printf("Subkey 3 : BAD\n");
	printf("0x%08lx - ", crackedSubkey4); if (crackedSubkey4 == subkey[4]) printf("Subkey 4 : GOOD!\n"); else printf("Subkey 4 : BAD\n");
	printf("0x%08lx - ", crackedSubkey5); if (crackedSubkey5 == subkey[5]) printf("Subkey 5 : GOOD!\n"); else printf("Subkey 5 : BAD\n");
	printf("\n");

	unsigned long fullEndTime = time(NULL);
	printf("Total crack time = %i seconds\n", (fullEndTime - fullStartTime));


printf("FINISHED\n");

    return 0;
}
