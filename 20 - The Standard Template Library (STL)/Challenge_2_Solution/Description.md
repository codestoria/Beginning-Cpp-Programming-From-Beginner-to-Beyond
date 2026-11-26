# Section 20: Challenge 2 - Using `std::list`

In this challenge, you will create a menu-driven application that will simulate a user playing songs from a playlist of songs.

We will use a list to simulate the user selecting the first song in the playlist and then selecting next and previous to play forward or backwards through the playlist.

Please refer to the video demo run.

We will also allow users to add a new song to the playlist, and it will be added prior to the currently playing song.

The menu looks as follows:
```text
F - Play First Song
N - Play Next song
P - Play Previous song
A - Add and play a new Song at current location
L - List the current playlist
===============================================
Enter a selection (Q to quit):
```

The available playlist is modeled as a `std::list<Song>`. I will provide the `Song` class. Following are the songs in the playlist. I grabbed these from the pop charts; you can use them or change them to any songs you wish:

| Song | Artist | Rating |
| :--- | :--- | :---: |
| God's Plan | Drake | 5 |
| Never Be The Same | Camila Cabello | 5 |
| Pray For Me | The Weeknd and K. Lamar | 4 |
| The Middle | Zedd, Maren Morris & Grey | 5 |
| Wait | Maroon 5 | 4 |
| Whatever It Takes | Imagine Dragons | 3 |

I have provided the starting project.
Have fun!

---

**Note:**
If you have trouble reading input, you can clear `std::cin` and ignore everything in the buffer with:
```cpp
std::cin.clear();
std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
```
Make sure you `#include <limits>` to use it.
