# The Sleeping Teaching Assistant
Room: 1 desk with a chair and computer
Hallway: 3 chairs
POSIX threads, mutex locks, and semaphores

-pthread is required when compiling this code.

![](https://i.imgur.com/AwLMeVu.png)

Initialy, the program generates n Students for TA(N = 5, from 0 to 4)

Whenever the student needs help, if TA is available, then the student goes to TA directly.

If TA is currently busy on helping one student, others that arrive need to sit on the chair and start waiting.

There are 3 Chairs on the Hallway for waiting. If all the chairs is being occupied, then the student who comes after will have to return and go back next time.

![](https://i.imgur.com/F3eOw7J.png)

This program will do infinate loop, so the screenshot is only taken arbitrarily.

## Infrastrucure graph


## Team Members
107440020 楊宗翰：討論架構及內容，協助製作document

110598111 謝成彥：協助實作code

110054817 黃慧：討論架構及內容

110054835 陳宥融：討論架構及內容
