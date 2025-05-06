#include<iostream>
#include<bits/stdc++.h>
#include<omp.h>
using namespace std;
using namespace std::chrono;

void bubblesort_parallel(vector<int> &arr)
{
    for(int i=0;i<arr.size();i++)
    {
        for(int j=0;j<arr.size()-i-1;j++)
        {
            
            if(arr[j]>arr[j+1])
            {
                #pragma omp critical
                swap(arr[j],arr[j+1]);
            }
        }
    }
}

void bubblesort(vector<int> &arr)
{
    for(int i=0;i<arr.size();i++)
    {
        for(int j=0;j<arr.size()-i-1;j++)
        {
            if(arr[j]>arr[j+1])
            {
                swap(arr[j],arr[j+1]);
            }
        }
    }
}

void merge(vector<int> &arr, int start, int end,int mid)
{
    vector<int> L;
    vector<int> R;
    for(int i=start;i<=mid;i++)
    {
        L.push_back(arr[i]);
    }
    for(int i=mid+1;i<=end;i++)
    {
        R.push_back(arr[i]);
    }
    int i=0;
    int j=0;
    int k=start;
    while(i<L.size() && j<R.size())
    {
        if(L[i]<=R[j])
        {
            arr[k]=L[i++];
        }
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
void mergesort(vector<int> &arr,int start, int end)
{
    if(start<end)
    {
        int mid=(start+end)/2;
        #pragma omp parallel sections
        {
            #pragma omp section
        mergesort(arr,start,mid);
        #pragma omp section
        mergesort(arr,mid+1,end);
        merge(arr,start,end,mid);
        }
    }
}
int main()
{
    vector<int> n={100,200,3,-9};
    auto start=high_resolution_clock::now();
    mergesort(n,0,n.size()-1);
    auto end=high_resolution_clock::now();
    auto dur=duration_cast<microseconds>(end-start);
    cout<<dur.count();
    for(auto i:n)
        cout<<"--"<<i;
}



// vector<int> arr1 = original;
// auto start1 = high_resolution_clock::now();
// bubblesort(arr1);
// auto end1 = high_resolution_clock::now();
// cout << "Bubble Sort (Sequential): " << duration_cast<microseconds>(end1 - start1).count() << " microseconds\n";

// // Bubble Sort Parallel
// vector<int> arr2 = original;
// auto start2 = high_resolution_clock::now();
// bubblesort_parallel(arr2);
// auto end2 = high_resolution_clock::now();
// cout << "Bubble Sort (Parallel): " << duration_cast<microseconds>(end2 - start2).count() << " microseconds\n";

// // Merge Sort
// vector<int> arr3 = original;
// auto start3 = high_resolution_clock::now();
// mergesort(arr3, 0, arr3.size() - 1);
// auto end3 = high_resolution_clock::now();
// cout << "Merge Sort (Sequential): " << duration_cast<microseconds>(end3 - start3).count() << " microseconds\n";

// // Merge Sort Parallel
// vector<int> arr4 = original;
// auto start4 = high_resolution_clock::now();
// mergesort_parallel(arr4, 0, arr4.size() - 1);
// auto end4 = high_resolution_clock::now();
// cout << "Merge Sort (Parallel): " << duration_cast<microseconds>(end4 - start4).count() << " microseconds\n";

// return 0;
