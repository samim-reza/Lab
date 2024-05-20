#include <bits/stdc++.h>
using namespace std;

int main()
{
    int n, m;
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

    unordered_map<int, int> frame_frequency;
    unordered_map<int, int> last_used;
    vector<int> frames(n, -1);
    int page_faults = 0;

    cout << "The Page Replacement Process is -->" << endl;
    for (int i = 0; i < m; i++)
    {
        int page = pages[i];
        bool found = false;

        for (int j = 0; j < n; j++)
        {
            if (frames[j] == page)
            {
                found = true;
                frame_frequency[page]++;
                last_used[page] = i;
                break;
            }
        }

        if (!found)
        {
            page_faults++;

            int lfu_index = -1, min_frequency = INT_MAX, earliest_time = INT_MAX;

            for (int j = 0; j < n; j++)
            {
                if (frames[j] == -1)
                {
                    lfu_index = j;
                    break;
                }
                else if (frame_frequency[frames[j]] < min_frequency)
                {
                    min_frequency = frame_frequency[frames[j]];
                    earliest_time = last_used[frames[j]];
                    lfu_index = j;
                }
                else if (frame_frequency[frames[j]] == min_frequency && last_used[frames[j]] < earliest_time)
                {
                    earliest_time = last_used[frames[j]];
                    lfu_index = j;
                }
            }

            if (frames[lfu_index] != -1)
            {
                frame_frequency.erase(frames[lfu_index]);
                last_used.erase(frames[lfu_index]);
            }
            frames[lfu_index] = page;
            frame_frequency[page] = 1;
            last_used[page] = i;

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
        }
    }

    cout << "Total no of page faults using LFU is: " << page_faults << endl;
    return 0;
}
