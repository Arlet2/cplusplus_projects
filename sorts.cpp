#include <iostream>
#include <ctime>
#include <windows.h>
#include <stack>
using namespace std;
template <typename T>
// сортировка простыми обменами (пузырёк)
void simple_changes(int *a,int size)
{
    for(int i=1;i<size;i++)
    {
        for(int j=0;j<size-i;j++)
        {
            if(a[j] > a[j+1])
                swap(a[j],a[j+1]);
        }
    }
}
// улучшенная сортировка пузырьком
// лучше по количеству сравнений, но количество перессылок не меняется
void simple_changes2(int *a,int size)
{
    bool changed;
    int i = 1;
    do{
        changed = false;
        for(int j=0;j<size-i;j++)
        {
            if(a[j] > a[j+1])
            {
                swap(a[j],a[j+1]);
                changed = true;
            }
        }
        i++;
    }while(changed);
}
// 3 вариант пузырька: выигрывает по количеству сравнений
// Запоминать последний индекс обмена (элементы с большим индексом уже готовы)
// (с меньшим, если слева направо)
void simple_changes3(int *a,int size)
{
    int last=1;
    int prlast=0;
    while(prlast < size)
    {
        prlast=size;
        for(int i=size-1;i>=last;i--)
        {
            if(a[i] < a[i-1])
            {
                swap(a[i],a[i-1]);
                prlast = i;
            }
        }
        last = prlast+1;
    }
}
void print(int *a,int size)
{
    for(int i=0;i<size;i++)
        cout << a[i] << " ";
    cout << endl;
}
void generate(int *a,int size,bool if_print)
{
    cout << "CREATE NEW ARRAY:\n";
    for(int i=0;i<size;i++)
    {
        a[i] = rand()%30;
        if(if_print)
            cout << a[i] << " ";
    }
    cout << endl;
}
// Быстрая сортировка
void split(int *a,int size,int left,int right)
{
    if(left >= right)
        return;
    // неплохой x - середина промежутка
    // но лучше всех подходит случайный x
    int x = a[(right-left)/2+left];
    int ileft = left;
    int iright = right;
    while(ileft <= iright)
    {
        while(a[ileft] < x) ileft++;
        while(a[iright] > x) iright--;
        if(ileft <= iright)
        {
            swap(a[ileft],a[iright]);
            ++ileft;
            --iright;
        }
    }
    // получили промежутки от left до ileft и от iright до right
    // работаем уже с ними
    if(right-ileft > iright - left)
    {
        split(a,size,left,iright);
        split(a,size,ileft,right);
    }
    else
    {
        split(a,size,ileft,right);
        split(a,size,left,iright);
    }
}
void quick_sort1(int *a,int size)
{
    // определяем элемент x, относительно которого сортируем
    split(a,size,0,size-1);
}
void quick_sort2(int *a,int size)
{
    stack<pair<int,int>> st;
    st.push(make_pair(0,size-1));
    int x;
    int left,right,ileft,iright;
    while(!st.empty())
    {
        if(st.top().first >= st.top().second)
        {
            st.pop();
            continue;
        }
        left = st.top().first;
        right = st.top().second;
        ileft = left;
        iright = right;
        
        x = a[(right-left)/2+left];
        while(ileft <= iright)
        {
            while(a[ileft] < x) ileft++;
            while(a[iright] > x) iright--;
            if(ileft <= iright)
            {
                swap(a[ileft],a[iright]);
                ++ileft;
                --iright;
            }
        }
        // получили промежутки от left до ileft и от iright до right
        // работаем уже с ними

        // ЛОГИКА! СНАЧАЛА УДАЛЯЕМ СТАРЫЙ ЭЛЕМЕНТ, ПОТОМ ДВА НОВЫХ ДОБАВЛЯЕМ
        if(right-ileft > iright - left)
        {
            st.pop();
            st.push(make_pair(left,iright));
            st.push(make_pair(ileft,right));
        }
        else
        {
            st.pop();
            st.push(make_pair(ileft,right));
            st.push(make_pair(left,iright));
        }
        
    }
}
// Сортировка простыми вставками
void simple_inserts(int *a,int size)
{
    int j;
    int x;
    for(int i=1;i<size;i++)
    {
        x = a[i];
        j = i - 1;
        while(j != -1 && x < a[j])
        {
            a[j+1] = a[j]; // сдвигаем отсортированные элементы вправо
            j--;
        }
        a[j+1] = x;
    }
}
void simple_inserts2(int *a,int size)
{
    int x,j;
    int left;
    int right;
    for(int i=1;i<size;i++)
    {
        x = a[i];
        left = 0;
        right = i-1;
        while(left <= right)
        {
            j = (left+right)/2;
            if(x < a[j]) right = j-1;
            else left = j + 1;
        }
        // здесь нашли место вставки
        // сдвигаем элементы
        for(int z=i-1;z >= left;z--)
            a[z+1] = a[z];
        a[left] = x;
    }
}
// Улучшение сортировки вставками, сортировка Шелла
void shell(int *a,int size)
{
    int distance = size/2;
    int x;
    int j;
    while(distance>0)
    {
        for(int i=distance;i<size;i++)
        {   // двигаемся по всему массиву с distance
            x = a[i];
            j = i -distance;
            while(j >= 0 && x < a[j])
            {
                // перемещение по элементам по distance, сортировка вставками по ним
                // перемещаем a[i] (правый элемент) по distance a[j] (слева)
                a[j+distance] = a[j];
                j-= distance;
            }
            a[j+distance] = x;
        }
        distance/=2;
    }
}

void msort(int *a,int size)
{
    for(int i=0;i<size;i++)
    {
        for(int j=i+1;j<size;j++)
        {
            if(a[i] > a[j])
                swap(a[i],a[j]);
        }
    }
}
// Улучшение пузырька// шейкерная сортировка
void cocktail(int *a,int size)
{
    int rlast = size-1;
    int llast = 0;
    while(rlast != -1 || llast != -1)
    {
        for(int i=rlast;i>=1;i--)
            if(a[i-1] > a[i])
            {
                swap(a[i-1],a[i]);
                llast = i-1;
            }
        rlast = -1;
        for(int i=llast;i<size-1;i++)
            if(a[i] > a[i+1])
            {
                swap(a[i],a[i+1]);
                rlast = i+1;
            }
        llast = -1;
    }
}
int main()
{
    srand(time(NULL));
    const int SIZE = 20;
    int a[SIZE];
    generate(a,SIZE,1);
    cocktail(a,SIZE);
    print(a,SIZE);
}