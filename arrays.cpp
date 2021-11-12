#include <iostream>
#include <ctime>
using namespace std;
void fill_array(int* const arr,const int size)
{
    for(int i=0;i<size;i++)
        arr[i] = rand()%20;
}
void show_array(const int* const arr,const int size)
{
    for(int i=0;i<size;i++)
        cout << arr[i] << " ";
    cout << endl;
}
void push_back(int *&arr,int &size,const int value)
{
    // * &arr - ссылка на указатель первого элемента массива
    // ПОЗВОЛЯЕТ МЕНЯТЬ УКАЗАТЕЛЬ
    int *new_array = new int[size+1];
    for(int i=0;i<size;i++)
    {
        *(new_array+i) = arr[i]; 
    }
    delete[] arr;
    new_array[size++] = value;
    arr = new_array; // МЫ ПРИСВАИВАЕМ АДРЕС НА ПЕРВЫМ ЭЛЕМЕНТ МАССИВА
    new_array = nullptr;
}
void pop_back(int *&arr,int &size)
{
    int *new_array = new int[--size];
    for(int i=0;i<size;i++)
        new_array[i] = arr[i];
    delete[] arr;
    arr = new_array;
    new_array = nullptr;
}
void erase(int *&arr,int &size,const int index)
{
    int *new_array = new int[size-1];
    for(int i=0;i<size;i++)
    {
        if(i == index)
            break;
        new_array[i] = arr[i];
    }
    for(int i=index;i<size;i++)
        new_array[i] = arr[i+1];
    delete[] arr;
    arr = new_array;
    size--;
    new_array = nullptr;
}
void erase(int *&arr,int &size,const int bindex,const int eindex)
{
    int t = eindex - bindex + 1;
    int *new_array = new int[size-t];
    for(int i=0;i<size;i++)
    {
        if(i >= bindex)
            break;
        new_array[i] = arr[i];
    }
    for(int i=bindex;i<size-t;i++)
        new_array[i] = arr[i+t];
    delete[] arr;
    arr = new_array;
    size-=t;
    new_array = nullptr;
}
int main()
{
    srand(time(NULL));
    int size = 15;
    int *arr = new int[size];
    fill_array(arr,size);
    show_array(arr,size);

    push_back(arr,size,100);
    show_array(arr,size);

    pop_back(arr,size);
    show_array(arr,size);

    erase(arr,size,3);
    show_array(arr,size);

    erase(arr,size,0,5);
    show_array(arr,size);

    delete[] arr;
    arr = nullptr;
}