#include<iostream>
#include<bits/stdc++.h>
#include<omp.h>
using namespace std;
using namespace std::chrono;

void bubblesort(vector<int> & arr)
{
    int n=arr.size();
 
    for(int i=0;i<arr.size();i++)
    {
        for(int j=0;j<n-i-1;j++)
        {
            
            if(arr[j]>arr[j+1])
                #pragma omp critical
                swap(arr[j],arr[j+1]);
        }
    }
   
}
void merge(vector<int> & arr,int start,int end,int mid)
{
    vector<int>L;
    vector<int> R;
    for(int i=start;i<=mid;i++)
    {
        L.push_back(arr[i]);
    }
    for(int i=mid+1;i<=end;i++)
    {
        R.push_back(arr[i]);
    }
    int i=0,j=0,k=start;
    while(i<L.size() && j<R.size())
    {
        if(L[i]<=R[j])
            arr[k]=L[i++];
        else
            arr[k]=R[j++];
        k++;      
    }
    while(i<L.size())
    {
        arr[k++]=L[i++];
    }
    while(j<R.size())
    {
        arr[k++]=R[j++];
    }
}
void mergesort(vector<int> & arr,int start,int end)
{
    if(start<end)
    {
       int mid=(start+end)/2;
       #pragma omp parallel  sections
       {
        #pragma omp section
       mergesort(arr,start,mid);
       #pragma omp section
       mergesort(arr,mid+1,end);
}
merge(arr,start,end,mid);
    }
}
int main()
{
    vector<int> a={100,200,9,4,5,-1,240,140,3,45,9,10};
    vector<int> a1=a;
    auto start=high_resolution_clock::now();
    bubblesort(a);
    auto s=high_resolution_clock::now();
    auto d=duration_cast<microseconds>(s-start).count();
    cout<<"---"<<d;
    for(int i=0;i<a.size();i++)
    {
        cout<<a[i]<<" ";
    }
    cout<<endl;
    
    auto stt=high_resolution_clock::now();
    mergesort(a1,0,a1.size()-1);
    auto stat=high_resolution_clock::now();
    auto l=duration_cast<microseconds>(stat-stt).count();
    cout<<"---"<<l;
    for(int i=0;i<a1.size();i++)
    {
        cout<<a1[i]<<" ";
    }

}
