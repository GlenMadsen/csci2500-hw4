/*******************************************************************************/
/* Assignment 4: 64 Bit Carry Lookahead Adder **********************************/
/* Name: Glen Madsen ***********************************************************/
/*******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
int main()
{
    long long int num1;	// variables used to take hex input
    long long int num2;
    long long int temp; // temp variable of same size
    int op; 			// Variable used to determine subtraction or addition
    char * n1 =  malloc(sizeof(int)*64);; 	// Array of size 64 for the binary num
    char * n2 =  malloc(sizeof(int)*64);;
    char * sum =  malloc(sizeof(int)*64);;
    int n1len = 1;	// length of the hexadecimal, used for formatting
    int n2len = 1;

    printf("Enter A (hex):\n");	// Takes hex input
    fflush(stdout);
    scanf("%llx", &num1);
    printf("Enter B (hex):\n");
    fflush(stdout);
    scanf("%llx", &num2);
    printf("Add (0) or subtract (1):\n");	// Addition or Subtraction
    fflush(stdout);
    scanf("%d", &op);

    if((int)ceil(log2(num1)/4) >= 1)		// Calculates the length of the hex based of 2^n etc.
    {										//So i can figure out how many leading zeros to print
    	n1len = (int)ceil(log2(num1)/4);	// Divides the power by 4 because hex is in bytes
    }										// Dont need to do this, printf can figure it out for
    if((int)ceil(log2(num2)/4) >= 1)		// me, but I did this before I learned that so this
    {										// is staying since it works.
    	n2len = (int)ceil(log2(num2)/4);
    }

    temp = num1;
    for (int i = 0;i<64;++i) //store hex into array as binary in an array
    {
    	n1[i] = temp&1;
    	temp >>=1;
    }

    temp = num2;
    for (int i = 0;i<64;++i)
    {
    	n2[i] = temp&1;
    	temp >>=1;
    }

    printf("\nA is ");	// Prints out A as hex with leading 0's and the actual number
    for(int i = 0; i < 16-n1len; i++)
    {
    	printf("0");
    }
    printf("%llx or %llu\n", num1, num1);

    printf("B is ");    // Prints out B as hex with leading 0's and the actual number
    for(int i = 0; i < 16-n2len; i++)
    {
    	printf("0");
    }
    printf("%llx or %llu\n", num2, num2); // Prints the hexidecimal and the decimal number

    if(op == 1)	// If doing subtraction, invert the binary string B
    {
    	printf("Inverting %llu\nB (bin) : ",num2);
    	for(int i = 63; i >=0; i--)
    	{
    		n2[i] = !n2[i];
    		printf("%d",n2[i]); // Prints it
    	}
    	printf("\n");
    }


    printf("\nCalculate sum, S:\n"); // Need to do inverse for subtraction, unimplemented
    printf("\nA (bin) : "); // Prints out A as binary
    for(int i = 63; i>=0;i--)
    {
    	printf("%d",n1[i]);
    }
    printf("\n");
    printf("B (bin) : "); // Prints out B as binary
    for(int i = 63; i>=0;i--)
    {
    	printf("%d",n2[i]);
    }

///////////////////////////////////////Doing Math////////////////////////////////////////////////
    // Create all my arrays for objects here.
    char g[64]; 	// Array of size 64 for the 1st level (Generates)
    char p[64];		// (Propagates)
    char c[64];		// (carries)
    char gg[16]; 	// Array of size 16 for the 2nd level
    char gp[16];
    char gc[16];
    char sg[4]; 	// Array of size 4 for the 3rd level
    char sp[4];
    char sc[4];

    for(int i = 0; i < 64; i++)				// Computes Generates and Propagates at the 1st level
    {
    	g[i] = n1[i] & n2[i];
    	p[i] = n1[i] | n2[i];
    }
    for(int j = 0; j < 16; j++)				// Computes Generates and Propagates at the 2nd level
    {
    	gg[j] = g[j*4+3] |
    			( p[j*4+3] & g[j*4+2] ) |
				( p[j*4+3] & p[j*4+2] & g[j*4+1] ) |
				( p[j*4+3] & p[j*4+2] & p[j*4+1] & g[j*4] );
    	gp[j] = ( p[j*4+3] & p[j*4+2] & p[j*4+1] & p[j*4] );
    }
    for(int k = 0; k < 4; k++)				// Computes Generates and Propagates at the 3rd level
    {
    	sg[k] = gg[k*4+3] |
    			( gp[k*4+3] & gg[k*4+2] ) |
				( gp[k*4+3] & gp[k*4+2] & gg[k*4+1] ) |
				( gp[k*4+3] & gp[k*4+2] & gp[k*4+1] & gg[k*4] );
    	sp[k] = ( gp[k*4+3] & gp[k*4+2] & gp[k*4+1] & gp[k*4] );
    }
    for(int k = 0; k < 4; k++) // Computes the carry at the third level
    {
    	if(k == 0)
    	{
    		sc[k] = sg[k] | (sp[k] & op); //Computes the carry Uses initial carry for the first i
    	}
    	else
    	{
    		sc[k] = sg[k] | (sp[k] & sc[k-1]); //Computes the carry
    	}
    }
    for(int j = 0; j < 16; j++) // Computes the carry at the second level
    {
    	if(j == 0)
    	{
    		gc[j] = gg[j] | (gp[j] & op); //Computes the carry Uses initial carry for the first i
    	}
    	else if(j%4 == 0)
    	{
    		gc[j] = gg[j] | (gp[j] & sc[j/4-1]); // Uses sc for j divisible by 4
    	}
    	else
    	{
    		gc[j] = gg[j] | (gp[j] & gc[j-1]); // Computes the carry
    	}
    }
    for(int i = 0; i < 64; i++) // Computes the carries at the first level
    {
    	if(i == 0) // Computes the carry, Uses initial carry for the first i
    	{
    		c[i] = g[i] | (p[i] & op);
    	}
    	else if(i%16 == 0)
    	{
    		c[i] = g[i] | (p[i] & sc[i/16-1]);
    		//printf("\nsc: %d, gc: %d\n",c[i], g[i] | (p[i] & gc[i/41]));
    	}
    	else if(i%4 == 0) // Computes the carry, Uses gc for i divisible by 4
    	{
    		c[i] = g[i] | (p[i] & gc[i/4-1]);
    	}
    	else	   // Computes the carry
    	{
    		c[i] = g[i] | (p[i] & c[i-1]);
    	}
    }

    printf("\nS (bin) : ");
    for(int i = 63; i >= 0; i--) // Calculating the sum using Xors and the calculated carries
    {
    	if(i == 0)
    	{   // Fills the sum array with all the calculated 1's and 0's
        	sum[i] = n1[i] ^ n2[i] ^ op; // Uses initial carry at the value least significant bit
    	}
    	else
    	{
    		sum[i] = n1[i] ^ n2[i] ^ c[i-1]; // Uses the calculated carry otherwise.
    	}
    	printf("%d", sum[i]); // Prints it as it calculates it
    }
    temp = 0; // Resets temp just in case so temp can hold the sum.
    for(int i = 63; i >= 0; i--)
    {
    	temp = (temp<<1)|sum[i];	// Converts from binary to decimal
    	//temp += sum[i]*pow(2,i);  // Unused conversion since shifting was better.
    }
    printf("\n\nS is %016llx or %lld",temp, temp);// Prints the answer in hexadecimal and decimal
    return 0;
}
