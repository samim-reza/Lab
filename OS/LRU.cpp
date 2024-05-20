#include <bits/stdc++.h>
using namespace std;

int main()
{
    int n, m, h = 0;
    cout << "Enter the number of frames: ";
    cin >> n;
    cout << "Enter the number of pages: ";
    cin >> m;
    vector<int> pages(m);
    cout << "Enter the pages: ";
    for (int i = 0; i < m; i++)
    {
        cin >> pages[i];
    }
    vector<int> frames(n, -1);
    vector<int> used(n, 0);
    int page_faults = 0;
    cout << "The Page Replacement Process is -->" << endl;
    for (int i = 0; i < m; i++)
    {
        bool flag = false;
        for (int j = 0; j < n; j++)
        {
            if (frames[j] == pages[i])
            {
                flag = true;
                used[j] = i + 1; // Update the usage count with the current step
                break;
            }
        }
        if (!flag)
        {
            int min = INT_MAX, index = -1;
            for (int j = 0; j < n; j++)
            {
                if (used[j] < min)
                {
                    min = used[j];
                    index = j;
                }
            }
            frames[index] = pages[i];
            used[index] = i + 1; // Update the usage count with the current step
            page_faults++;
            cout << "For " << pages[i] << " :";
            for (int j = 0; j < n; j++)
            {
                if (frames[j] != -1)
                    cout << " " << frames[j];
            }
            cout << endl;
        }
        else
        {
            cout << "For " << pages[i] << " :No page fault!" << endl;
            h++;
        }
    }
    cout << "Total no of page faults using LRU is :" << page_faults << endl;
    return 0;
}
