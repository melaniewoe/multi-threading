//
//  main.cpp
//  multithreading_pthread
//
//  Created by Melanie W. on 9/22/17.
//  Copyright © 2017 Melanie Woe. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <pthread.h>
#include <ctime>
#include <regex>
using namespace std;

//matrix A and B, result will be store as matrix C
long A[10000][10000], B[10000][10000], C[10000][10000];
long rowFirst, columnFirst, rowSecond, columnSecond;
int numThread;

//function to multiply matrix A and B
void* matrix (void* lineNumber)
{
    //struct threadLine *line;
    //line = (struct threadLine *) lineNumber;
    int i, j, k;
    long line = (long)lineNumber;
    long from = line;
    long to = line + 1;
    
    //Multiplying matrix A and B and store it in matrix C per row
    for(i = from; i < to; ++i)
    {
        for(j = 0; j < columnSecond; ++j)
        {
            C[i][j] = 0;
            for(k = 0; k < columnFirst; ++k)
            {
                C[i][j] += A[line][k] * B[k][j];
            }
        }
    }
    
    return 0;
}

int main()
{
    
    //read the file called matrix
    ifstream read("matrix.txt");
    
    // if it can read
    if (read.is_open())
    {
        read >> rowFirst >> columnFirst;                //read the first matrix
        if(read.fail())
        {
            cout << "invalid value in file." << endl;
            return -1;
        }
        
        //thread ids
        pthread_t tid[rowFirst];
        
        for (int i = 0; i < rowFirst; i++)
        {
            for(int j = 0; j < columnFirst; j++)
            {
                read >>A[i][j];                         //store it in array A
                if(read.fail())
                {
                    cout << "invalid value in file" << endl;
                    return -1;
                }
            }
        }
        read >> rowSecond >> columnSecond;              //read the second matrix
        if(read.fail())
        {
            cout << "invalid value in file" << endl;
            return -1;
        }
        
        else if (columnFirst != rowSecond)                   //column of first matrix must be the same as the second one
        {
            cout << "column of first matrix must have the same value as row of second matrix" << endl;
            return -1;
        }
        
        else
        {
            for (int i = 0; i < rowSecond; i++)
            {
                for(int j = 0; j < columnSecond; j++)
                {
                    read >>B[i][j];                         //store it in array B
                    if(read.fail())
                    {
                        cout << "invalid value in file" << endl;
                        return -1;
                    }
                }
            }
            
            //print input data
            cout << "Matrix A: ";                           //print array A
            for (int i = 0; i < rowFirst; i++)
            {
                for(int j = 0; j < columnFirst; j++)
                {
                    cout << A[i][j] << ' ';
                }
                cout << endl;
            }
            
            cout << "Matrix B: ";                           //print array B
            for (int i = 0; i < rowSecond; i++)
            {
                for(int j = 0; j < columnSecond; j++)
                {
                    cout << B[i][j] << ' ';
                }
                cout << endl;
            }
            cout << endl;
            
            int timeStart = clock();                        //start time counting
            
            //loop for creating threads based on number of row (calculate per row)
            for (int x = 0; x < rowFirst; x++)
            {
                
                int idSuccess = pthread_create(&tid[x], NULL, matrix, (void *) x);
                cout << "Created worker thread " << tid[x] << " for row " << x << endl;
                
                //check to see if thread is created sucessfully
                if (idSuccess)
                {
                    cout << "Thread creation failed : " << strerror(idSuccess);
                    return idSuccess;
                }
            }
            cout << endl;
            
            //wait for every threads complete
            for (int x = 0; x < rowFirst; x++)
            {
                pthread_join(tid[x], NULL);
            }
            
            int timeStop = clock();                         //stop time counting
            
            cout << "Matrix C = A x B: ";                   //display result
            for (int i = 0; i < rowFirst; i++)
            {
                for(int j = 0; j < columnSecond; j++)
                {
                    cout << C[i][j] << " ";
                    if(j == columnSecond - 1)
                    {
                        cout << endl;
                    }
                }
            }
            
            cout << endl;
            
            //print out time
            double timeVal = (timeStop - timeStart)/double(CLOCKS_PER_SEC)*1000;
            cout << "Total execution time using " << rowFirst + 1 << " threads is " << timeVal << " ms."<< endl;

        }
        
        
    }
    else if(!read)
    {
        cout << "Unable to read file" << endl;
    }
    
    
}
