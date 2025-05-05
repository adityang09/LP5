#include<iostream>
#include<omp.h>
#include<bits/stdc++.h>
using namespace std;
using namespace std::chrono;

int minval_seq(int arr[],int n)
{
    int min=arr[0];
    for(int i=0;i<n;i++)
    {
        if(arr[i]<min)
            min=arr[i];
    }
    return min;
}
int maxval_seq(int arr[],int n)
{
    int max=arr[0];
    for(int i=0;i<n;i++)
    {
        if(arr[i]>max)
            max=arr[i];
    }
    return max;
}

int sum(int arr[],int n)
{
    int sum=0;
    for(int i=0;i<n;i++)
    {
       sum+=arr[i];
    }
    return sum;
}

double avg(int arr[],int n)
{
    return double(sum(arr,n)/n);
}

int parallel_minval_seq(int arr[],int n)
{
    int mina=arr[0];
    #pragma omp parallel for reduction (min:mina)
    for(int i=0;i<n;i++)
    {
        if(arr[i]<mina)
            mina=arr[i];
    }
    return mina;
}
int pmaxval_seq(int arr[],int n)
{
    int maxi=arr[0];
    #pragma parallel for reduction (max:maxi)
    for(int i=0;i<n;i++)
    {
        if(arr[i]>maxi)
            maxi=arr[i];
    }
    return maxi;
}

int psum(int arr[],int n)
{
    int sum=0;
#pragma omp parallel for reduction(+:sum)
    for(int i=0;i<n;i++)
    {
       sum+=arr[i];
    }
    return sum;
}
int main()
{
    int n;
    cin>>n;
    int arr[n];
    cout << "Enter the elements: ";
    for(int i = 0; i < n; i++) 
        cin >> arr[i];
    auto start=high_resolution_clock::now();
    cout<<psum(arr,n);
    auto end= high_resolution_clock::now();
    auto duration=duration_cast<microseconds>(end-start);
    cout<<duration.count();


}



